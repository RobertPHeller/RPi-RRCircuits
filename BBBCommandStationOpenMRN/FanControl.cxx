// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Oct 28 13:33:53 2019
//  Last Modified : <210318.1524>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
//			51 Locke Hill Road
//			Wendell, MA 01379-9728
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// 
//
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include <math.h>
#include "openlcb/PolledProducer.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "utils/logging.h"

#include "AnalogReadSysFS.h"
#include "FanControl.hxx"
#include "Hardware.hxx"

FanControl::FanControl(openlcb::Node *node,
                       const FanControlConfig &cfg,
                       uint8_t temperatureAIN,
                       const Gpio *fanGpio)
      : node_(node)
, cfg_(cfg)
, temperatureAIN_(temperatureAIN)
, fanGpio_(fanGpio)
, alarmon_event_(0)
, alarmoff_event_(0)
, fanon_event_(0)
, fanoff_event_(0)
, alarmthresh_(350)
, fanthresh_(250)
, registered_(false)
, fanon_(false)
, alarmon_(false)
{
    ConfigUpdateService::instance()->register_update_listener(this);
}

template <class FAN>
FanControl::FanControl(openlcb::Node *node,
                     const FanControlConfig &cfg,
                     uint8_t temperatureAIN,
                     const FAN&,
                     const Gpio *fanGpio)
      : node_(node)
, cfg_(cfg)
, temperatureAIN_(temperatureAIN)
, fanGpio_(fanGpio)
, alarmon_event_(0)
, alarmoff_event_(0)
, fanon_event_(0)
, fanoff_event_(0)
, alarmthresh_(350)
, fanthresh_(250)
, registered_(false)
, fanon_(false)
, alarmon_(false)
{
    ConfigUpdateService::instance()->register_update_listener(this);
}

FanControl::~FanControl()
{
    ConfigUpdateService::instance()->unregister_update_listener(this);
    if (registered_) unregister_handler();
}

void FanControl::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry,
                                        openlcb::EventReport *event,
                                        BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node_)
    {
        done->notify();
        return;
    }
    SendAllProducersIdentified(event,done);
    done->maybe_done();
}

void FanControl::handle_identify_producer(const openlcb::EventRegistryEntry &registry_entry,
                                          openlcb::EventReport *event, BarrierNotifiable *done)
{
    SendProducerIdentified(event,done);
    done->maybe_done();
}

void FanControl::poll_33hz(openlcb::WriteHelper *helper, Notifiable *done)
{
    BarrierNotifiable barrier(done);
    
    uint16_t hsTempTensC = (uint16_t)round(TempFromAIN(sysfs_adc_getvalue(temperatureAIN_))*10);
    //LOG(INFO,"*** FanControl::poll_33hz(): hsTempTensC = %d",hsTempTensC);
    //LOG(INFO,"*** -: alarmthresh_ = %d, alarmon_ = %d",alarmthresh_,alarmon_);
    if (hsTempTensC > alarmthresh_ && !alarmon_)
    {
        SendEventReport(5, alarmon_event_, &barrier);
        alarmon_ = true;
    } else if (hsTempTensC <= alarmthresh_ && alarmon_)
    {
        SendEventReport(5, alarmoff_event_, &barrier);
        alarmon_ = false;
    }
    //LOG(INFO,"*** -: fanthresh_ = %d, fanon_ = %d",fanthresh_,fanon_);
    if (hsTempTensC > fanthresh_ && !fanon_)
    {
        SendEventReport(6, fanon_event_, &barrier);
        fanon_ = true;
        fanGpio_->set();
    } else if (hsTempTensC <= fanthresh_ && fanon_)
    {
        SendEventReport(6, fanoff_event_, &barrier);
        fanon_ = false;
        fanGpio_->clr();
    }
    barrier.maybe_done();
    //LOG(INFO,"*** -: barrier.is_done() yields %d",barrier.is_done());
    if (!barrier.is_done()) {
        LOG(WARNING,"Opps, barrier in FanControl::poll_33hz() is not done!");
    }
}

void FanControl::SendEventReport(int helperIndex, openlcb::EventId event, BarrierNotifiable *done)
{
    write_helper[helperIndex].WriteAsync(node_, 
                                         openlcb::Defs::MTI_EVENT_REPORT,
                                         openlcb::WriteHelper::global(),
                                         openlcb::eventid_to_buffer(event),
                                         done->new_child());
                                               
}



ConfigUpdateListener::UpdateAction FanControl::apply_configuration(int fd, bool initial_load,
                                                                   BarrierNotifiable *done)
{
    AutoNotify n(done);
    alarmthresh_ = cfg_.alarmtemperaturethresh().read(fd);
    fanthresh_ = cfg_.fantemperaturethresh().read(fd);
    openlcb::EventId cfg_alarmon_event_ = cfg_.alarmon().read(fd);
    openlcb::EventId cfg_alarmoff_event_ = cfg_.alarmoff().read(fd);
    openlcb::EventId cfg_fanon_event_ = cfg_.fanon().read(fd);
    openlcb::EventId cfg_fanoff_event_ = cfg_.fanoff().read(fd);
    if (cfg_alarmon_event_ != alarmon_event_ ||
        cfg_alarmoff_event_ != alarmoff_event_ ||
        cfg_fanon_event_ != fanon_event_ ||
        cfg_fanoff_event_ != fanoff_event_) 
    {
        if (registered_) unregister_handler();
        alarmon_event_ = cfg_alarmon_event_;
        alarmoff_event_ = cfg_alarmoff_event_;
        fanon_event_ = cfg_fanon_event_;
        fanoff_event_ = cfg_fanoff_event_;
        register_handler();
        return REINIT_NEEDED;
    }
    return UPDATED;
}

void FanControl::factory_reset(int fd)
{
    CDI_FACTORY_RESET(cfg_.alarmtemperaturethresh);
    CDI_FACTORY_RESET(cfg_.fantemperaturethresh);
}

void FanControl::register_handler()
{
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,alarmon_event_), 0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,alarmoff_event_), 0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,fanon_event_), 0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,fanoff_event_), 0);
    registered_ = true;
}

void FanControl::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
    registered_ = false;
}


void FanControl::SendProducerIdentified(openlcb::EventReport *event, BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
    if (event->event == alarmon_event_) {
        if (alarmon_) mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        else mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    } else if (event->event == alarmoff_event_) {
        if (alarmon_) mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
        else mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else if (event->event == fanon_event_) {
        if (fanon_) 
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        else mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    } else if (event->event == fanoff_event_) {
        if (fanon_)
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
        else mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }

    write_helper[0].WriteAsync(node_, mti,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(event->event),
                                done->new_child());
}

void FanControl::SendAllProducersIdentified(openlcb::EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_alarmon = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID,
          mti_alarmoff = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID,
          mti_fanon = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID,
          mti_fanoff = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    if (alarmon_) {
        mti_alarmon = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else {
        mti_alarmoff = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }
    if (fanon_) {
        mti_fanon = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else {
        mti_fanoff = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }
    write_helper[1].WriteAsync(node_, mti_alarmon,
                               openlcb::WriteHelper::global(),
                               openlcb::eventid_to_buffer(alarmon_event_),
                               done->new_child());
    write_helper[2].WriteAsync(node_, mti_alarmoff,
                               openlcb::WriteHelper::global(),
                               openlcb::eventid_to_buffer(alarmoff_event_),
                               done->new_child());
    write_helper[3].WriteAsync(node_, mti_fanon,
                               openlcb::WriteHelper::global(),
                               openlcb::eventid_to_buffer(fanon_event_),
                               done->new_child());
    write_helper[4].WriteAsync(node_, mti_fanoff,
                               openlcb::WriteHelper::global(),
                               openlcb::eventid_to_buffer(fanoff_event_),
                               done->new_child());
}

