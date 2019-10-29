// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Oct 28 13:33:31 2019
//  Last Modified : <191029.0107>
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
#include "HBridgeControl.hxx"
#include "Hardware.hxx"

HBridgeControl::HBridgeControl(openlcb::Node *node, 
                               const HBridgeControlConfig &cfg, 
                               uint8_t currentAIN, 
                               const Gpio *enableGpio, 
                               const Gpio *thermFlagGpio)
      : node_(node)
, cfg_(cfg)
, currentAIN_(currentAIN)
, enableGpio_(enableGpio)
, thermFlagGpio_(thermFlagGpio)
, overcurrent_event_(0)
, disable_event_(0)
, enable_event_(0)
, thermflagon_event_(0)
, thermflagoff_event_(0)
, currentthresh_(3000)
, registered_(false)
, isEnabled_(true)
, isOverCurrent_(false)
{
        ConfigUpdateService::instance()->register_update_listener(this);
}

template <class ENABLE, class THERMFLAG>
HBridgeControl::HBridgeControl(openlcb::Node *node, 
                               const HBridgeControlConfig &cfg, 
                               uint8_t currentAIN, 
                               const ENABLE&, 
                               const THERMFLAG&, 
                               const Gpio *enableGpio, 
                               const Gpio *thermFlagGpio)
      : node_(node)
, cfg_(cfg)
, currentAIN_(currentAIN)
, enableGpio_(enableGpio)
, thermFlagGpio_(thermFlagGpio)
, overcurrent_event_(0)
, disable_event_(0)
, enable_event_(0)
, thermflagon_event_(0)
, thermflagoff_event_(0)
, currentthresh_(3000)
, registered_(false)
, isEnabled_(true)
, isOverCurrent_(false)
{
        ConfigUpdateService::instance()->register_update_listener(this);
}
HBridgeControl::~HBridgeControl()
{
    ConfigUpdateService::instance()->unregister_update_listener(this);
    if (registered_) unregister_handler();
}

void HBridgeControl::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry,
                                            openlcb::EventReport *event,
                                            BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node_)
    {
        done->notify();
        return;
    }
    SendAllConsumersIdentified(event,done);
    SendAllProducersIdentified(event,done);
    done->maybe_done();
}

void HBridgeControl::handle_identify_consumer(const openlcb::EventRegistryEntry &registry_entry,
                                              openlcb::EventReport *event, BarrierNotifiable *done)
{
    SendConsumerIdentified(event,done);
    done->maybe_done();
}

void HBridgeControl::handle_identify_producer(const openlcb::EventRegistryEntry &registry_entry,
                                              openlcb::EventReport *event, BarrierNotifiable *done)
{
    SendProducerIdentified(event,done);
    done->maybe_done();
}

void HBridgeControl::handle_event_report(const openlcb::EventRegistryEntry &registry_entry,
                                         openlcb::EventReport *event,
                                         BarrierNotifiable *done)
{
    if (event->event == disable_event_)
    {
        enableGpio_->clr();
        isEnabled_ = false;
    } else if (event->event == enable_event_)
    {
        enableGpio_->set();
        isEnabled_ = true;
    } 
    done->maybe_done();
}

void HBridgeControl::poll_33hz(openlcb::WriteHelper *helper, Notifiable *done)
{
    BarrierNotifiable barrier(done);
    uint16_t currentMA = (uint16_t)round(CurrentFromAIN(sysfs_adc_getvalue(currentAIN_))*1000);
    //LOG(INFO, "*** HBridgeControl::poll_33hz(): currentMA = %d",currentMA);
    if (currentMA > currentthresh_ && !isOverCurrent_) {
        SendEventReport(0, overcurrent_event_, &barrier);
        isOverCurrent_ = true;
    } else if (currentMA <= currentthresh_) {
        isOverCurrent_ = false;
    }
    if (thermFlagGpio_->read() != thermflagState_) {
        thermflagState_ = thermFlagGpio_->read();
        if (thermflagState_) 
            SendEventReport(1, thermflagon_event_, &barrier);
        else
            SendEventReport(1, thermflagoff_event_, &barrier);
    }
    barrier.maybe_done();
}

ConfigUpdateListener::UpdateAction HBridgeControl::apply_configuration(int fd, bool initial_load,
                                                                       BarrierNotifiable *done)
{
    AutoNotify n(done);
    if (initial_load) thermflagState_ = thermFlagGpio_->read();
    currentthresh_ = cfg_.currentthresh().read(fd);
    openlcb::EventId cfg_overcurrent_event_  = cfg_.overcurrent().read(fd);
    openlcb::EventId cfg_disable_event_      = cfg_.disable().read(fd);
    openlcb::EventId cfg_enable_event_       = cfg_.enable().read(fd);
    openlcb::EventId cfg_thermflagon_event_  = cfg_.thermflagon().read(fd);
    openlcb::EventId cfg_thermflagoff_event_ = cfg_.thermflagoff().read(fd);
    if (cfg_overcurrent_event_  != overcurrent_event_ ||
        cfg_disable_event_      != disable_event_     ||
        cfg_enable_event_       != enable_event_      ||
        cfg_thermflagon_event_  != thermflagon_event_ ||
        cfg_thermflagoff_event_ != thermflagoff_event_)
    {
        if (registered_) unregister_handler();
        overcurrent_event_  = cfg_overcurrent_event_;
        disable_event_      = cfg_disable_event_;
        enable_event_       = cfg_enable_event_;
        thermflagon_event_  = cfg_thermflagon_event_;
        thermflagoff_event_ = cfg_thermflagoff_event_;
        register_handler();
        return REINIT_NEEDED;
    }
    return UPDATED;
}

void HBridgeControl::factory_reset(int fd)
{
    CDI_FACTORY_RESET(cfg_.currentthresh);
}

void HBridgeControl::register_handler()
{
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,overcurrent_event_), 0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,disable_event_), 0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,enable_event_), 0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,thermflagon_event_), 0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,thermflagoff_event_), 0);
    registered_ = true;
}

void HBridgeControl::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
    registered_ = false;
}

void HBridgeControl::SendProducerIdentified(openlcb::EventReport *event, BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
    if (event->event == thermflagon_event_) {
        if (thermflagState_) mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        else mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    } else if (event->event == thermflagoff_event_) {
        if (thermflagState_) mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
        else mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else if (event->event == overcurrent_event_) {
        if (isOverCurrent_) 
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        else mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    }

    write_helper[0].WriteAsync(node_, mti,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(event->event),
                                done->new_child());
}

void HBridgeControl::SendAllProducersIdentified(openlcb::EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_on = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID,
          mti_off = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID,
          mti_currover = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    if (thermflagState_) {
        mti_on = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else {
        mti_off = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }
    if (isOverCurrent_) {
        mti_currover = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }
    write_helper[1].WriteAsync(node_, mti_on,
                                               openlcb::WriteHelper::global(),
                                               openlcb::eventid_to_buffer(thermflagon_event_),
                                               done->new_child());
    write_helper[2].WriteAsync(node_, mti_off,
                                               openlcb::WriteHelper::global(),
                                               openlcb::eventid_to_buffer(thermflagoff_event_),
                                               done->new_child());
    write_helper[3].WriteAsync(node_, mti_currover,
                                               openlcb::WriteHelper::global(),
                                               openlcb::eventid_to_buffer(overcurrent_event_),
                                               done->new_child());
}

void HBridgeControl::SendConsumerIdentified(openlcb::EventReport *event, BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN;
    if (event->event == enable_event_) {
        if (isEnabled_) mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
        else mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
    } else if (event->event == disable_event_) {
        if (!isEnabled_) mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
        else mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
    }
    write_helper[4].WriteAsync(node_, mti,
                                               openlcb::WriteHelper::global(),
                                               openlcb::eventid_to_buffer(event->event),
                                               done->new_child());
}

void HBridgeControl::SendAllConsumersIdentified(openlcb::EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_enabled = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID,
          mti_disabled = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
    if (isEnabled_) mti_enabled = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    else mti_disabled = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    write_helper[5].WriteAsync(node_, mti_enabled,
                                               openlcb::WriteHelper::global(),
                                               openlcb::eventid_to_buffer(enable_event_),
                                               done->new_child());
    write_helper[6].WriteAsync(node_, mti_disabled,
                                               openlcb::WriteHelper::global(),
                                               openlcb::eventid_to_buffer(disable_event_),
                                               done->new_child());
}

void HBridgeControl::SendEventReport(int helperIndex, openlcb::EventId event, BarrierNotifiable *done)
{
    write_helper[helperIndex].WriteAsync(node_, 
                                         openlcb::Defs::MTI_EVENT_REPORT,
                                         openlcb::WriteHelper::global(),
                                         openlcb::eventid_to_buffer(event),
                                         done->new_child());
                                               
}
