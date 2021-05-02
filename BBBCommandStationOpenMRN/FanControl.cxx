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
//  Last Modified : <210502.1715>
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
#include <string.h>
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
#include <vector>
#include <numeric>

FanControl::FanControl(openlcb::Node *node,
                       const FanControlConfig &cfg,
                       uint8_t temperatureAIN,
                       const Gpio *fanGpio)
      : node_(node)
, cfg_(cfg)
, temperatureAIN_(temperatureAIN)
, fanGpio_(fanGpio)
, alarmBit_(node, 0, 0, &alarmon_, 1)
, fanBit_(node, 0, 0, &fanon_, 1)
, alarmProducer_(&alarmBit_)
, fanProducer_(&fanBit_)
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
, alarmBit_(node, 0, 0, &alarmon_, 1)
, fanBit_(node, 0, 0, &fanon_, 1)
, alarmProducer_(&alarmBit_)
, fanProducer_(&fanBit_)
{
    ConfigUpdateService::instance()->register_update_listener(this);
    //memset((void *)write_helper_,0,10*sizeof(openlcb::WriteHelper));
}

FanControl::~FanControl()
{
    ConfigUpdateService::instance()->unregister_update_listener(this);
}

void FanControl::poll_33hz(openlcb::WriteHelper *helper, Notifiable *done)
{
    vector<int> samples;
    
    while (samples.size() < 32)
    {
        samples.push_back(sysfs_adc_getvalue(temperatureAIN_));
        usleep(1);
    }
    
    uint16_t hsTempTensC = (uint16_t)round(TempFromAIN(std::accumulate(samples.begin(), samples.end(), 0))*10);
    //LOG(INFO,"*** FanControl::poll_33hz(): hsTempTensC = %d",hsTempTensC);
    //LOG(INFO,"*** -: alarmthresh_ = %d, alarmon_ = %d",alarmthresh_,alarmon_);
    bool async_event_req = false;
    if (hsTempTensC > alarmthresh_ && alarmon_ == 0)
    {
        alarmon_ = 1;
        alarmProducer_.SendEventReport(helper, done);
        async_event_req = true;
    } else if (hsTempTensC <= alarmthresh_ && alarmon_ == 1)
    {
        alarmon_ = 0;
        alarmProducer_.SendEventReport(helper, done);
        async_event_req = true;
    }
    //LOG(INFO,"*** -: fanthresh_ = %d, fanon_ = %d",fanthresh_,fanon_);
    if (hsTempTensC > fanthresh_ && fanon_ == 0)
    {
        fanon_ = 1;
        fanProducer_.SendEventReport(helper, done);
        async_event_req = true;
        fanon_ = true;
        fanGpio_->set();
    } else if (hsTempTensC <= fanthresh_ && fanon_ == 1)
    {
        fanon_ = 0;
        fanProducer_.SendEventReport(helper, done);
        async_event_req = true;
        fanGpio_->clr();
    }
    if (!async_event_req)
    {
        done->notify();
    }
}

ConfigUpdateListener::UpdateAction FanControl::apply_configuration(int fd, bool initial_load,
                                                                   BarrierNotifiable *done)
{
    AutoNotify n(done);
    UpdateAction res = initial_load ? REINIT_NEEDED : UPDATED;
    alarmthresh_ = cfg_.alarmtemperaturethresh().read(fd);
    fanthresh_ = cfg_.fantemperaturethresh().read(fd);
    openlcb::EventId alarmon = cfg_.alarmon().read(fd);
    openlcb::EventId alarmoff = cfg_.alarmoff().read(fd);
    openlcb::EventId fanon = cfg_.fanon().read(fd);
    openlcb::EventId fanoff = cfg_.fanoff().read(fd);
    auto saved_node = alarmBit_.node();
    if (alarmon != alarmBit_.event_on() ||
        alarmoff != alarmBit_.event_off())
    {
        alarmBit_.openlcb::MemoryBit<uint8_t>::~MemoryBit();
        new (&alarmBit_)openlcb::MemoryBit<uint8_t>(saved_node, alarmon, alarmoff, &alarmon_, 1);
        alarmProducer_.openlcb::BitEventProducer::~BitEventProducer();
        new (&alarmProducer_)openlcb::BitEventProducer(&alarmBit_);
        res = REINIT_NEEDED;
    }
    if (fanon != fanBit_.event_on() ||
        fanoff != fanBit_.event_off())
    {
        fanBit_.openlcb::MemoryBit<uint8_t>::~MemoryBit();
        new (&fanBit_)openlcb::MemoryBit<uint8_t>(saved_node, fanon, fanoff, &fanon_, 1);
        fanProducer_.openlcb::BitEventProducer::~BitEventProducer();
        new (&fanProducer_)openlcb::BitEventProducer(&fanBit_);
        res = REINIT_NEEDED;
    }
    
    return res;
}

void FanControl::factory_reset(int fd)
{
    CDI_FACTORY_RESET(cfg_.alarmtemperaturethresh);
    CDI_FACTORY_RESET(cfg_.fantemperaturethresh);
}

