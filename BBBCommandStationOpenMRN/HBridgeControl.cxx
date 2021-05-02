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
//  Last Modified : <210502.1406>
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
#include "utils/Singleton.hxx"
#include <dcc/ProgrammingTrackBackend.hxx>

#include "AnalogReadSysFS.h"
#include "HBridgeControl.hxx"
#include "Hardware.hxx"

#include <vector>
#include <numeric>

HBridgeControl::HBridgeControl(openlcb::Node *node, 
                               const HBridgeControlConfig &cfg, 
                               uint8_t currentAIN, 
                               const uint32_t limitMilliAmps,
                               const uint32_t maxMilliAmps,
                               const Gpio *enableGpio, 
                               const Gpio *thermFlagGpio)
      : node_(node)
, cfg_(cfg)
, currentAIN_(currentAIN)
, enableGpio_(enableGpio)
, thermFlagGpio_(thermFlagGpio)
, maxMilliAmps_(maxMilliAmps)
, overCurrentLimit_((uint32_t)round(limitMilliAmps*.9)) // ~90% max value
, shutdownLimit_((uint32_t)round(limitMilliAmps*.99))
, isProgTrack_(false)
, progAckLimit_(0)
, shortBit_(node, 0, 0, &state_, STATE_OVERCURRENT)
, shutdownBit_(node, 0, 0, &state_, STATE_SHUTDOWN)
, thermalFlagBit_(node, 0, 0, &thermalFlag_, 1)
, shortProducer_(&shortBit_)
, shutdownProducer_(&shortBit_)
, thermalFlagProducer_(&thermalFlagBit_)
{
    ConfigUpdateService::instance()->register_update_listener(this);
}

HBridgeControl::HBridgeControl(openlcb::Node *node, 
                               const HBridgeControlConfig &cfg, 
                               uint8_t currentAIN, 
                               const uint32_t maxMilliAmps,
                               const Gpio *enableGpio, 
                               const Gpio *thermFlagGpio)
      : node_(node)
, cfg_(cfg)
, currentAIN_(currentAIN)
, enableGpio_(enableGpio)
, thermFlagGpio_(thermFlagGpio)
, maxMilliAmps_(maxMilliAmps)
, overCurrentLimit_(250) // ~250 mA
, shutdownLimit_(500)
, isProgTrack_(true)
, progAckLimit_(60) // ~60 mA
, shortBit_(node, 0, 0, &state_, STATE_OVERCURRENT)
, shutdownBit_(node, 0, 0, &state_, STATE_SHUTDOWN)
, thermalFlagBit_(node, 0, 0, &thermalFlag_, 1)
, shortProducer_(&shortBit_)
, shutdownProducer_(&shortBit_)
, thermalFlagProducer_(&thermalFlagBit_)
{
    ConfigUpdateService::instance()->register_update_listener(this);
}

HBridgeControl::~HBridgeControl()
{
    ConfigUpdateService::instance()->unregister_update_listener(this);
}

void HBridgeControl::poll_33hz(openlcb::WriteHelper *helper, Notifiable *done)
{
    vector<int> samples;
    
    while (samples.size() < adcSampleCount_)
    {
        samples.push_back(sysfs_adc_getvalue(currentAIN_));
        usleep(1);
    }
    
    lastReading_ = (uint32_t)round(CurrentFromAIN(std::accumulate(samples.begin(), samples.end(), 0)));
    
    if (isProgTrack_ && progEnable_)
    {
        auto backend = Singleton<ProgrammingTrackBackend>::instance();
        if (lastReading_ >= overCurrentLimit_)
        {
            backend->notify_service_mode_short();
        }
        else if (lastReading_ >= progAckLimit_)
        {
            backend->notify_service_mode_ack();
        }
    }
    
    uint8_t previous_state = state_;
    
    if (lastReading_ >= shutdownLimit_)
    {
        enableGpio_->clr();
        state_ = STATE_SHUTDOWN;
    }
    else if (lastReading_ >= overCurrentLimit_)
    {
        if (overCurrentCheckCount_++ >= overCurrentRetryCount_)
        {
            enableGpio_->clr();
            state_ = STATE_OVERCURRENT;
        }
    }
    else
    {
        if (enableGpio_->is_set())
        {
            overCurrentCheckCount_ = 0;
            state_ = STATE_ON;
        }
        else
        {
            state_ = STATE_OFF;
        }
    }
    bool async_event_req = false;
    if (previous_state != state_)
    {
        if (previous_state == STATE_SHUTDOWN || state_ == STATE_SHUTDOWN)
        {
            shutdownProducer_.SendEventReport(helper, done);
            async_event_req = true;
        }
        else if (previous_state == STATE_OVERCURRENT || state_ == STATE_OVERCURRENT)
        {
            shortProducer_.SendEventReport(helper, done);
            async_event_req = true;
        }
    }
    if (thermFlagGpio_ != NULL)
    {
        uint8_t previous_thermalFlag = thermalFlag_;
        if (thermFlagGpio_->is_set())
        {
            thermalFlag_ = 1;
        }
        else
        {
            thermalFlag_ = 0;
        }
        if (previous_thermalFlag != thermalFlag_) {
            thermalFlagProducer_.SendEventReport(helper, done);
            async_event_req = true;
        }
    }
    if (!async_event_req)
    {
        done->notify();
    }
}

ConfigUpdateListener::UpdateAction HBridgeControl::apply_configuration(int fd, bool initial_load,
                                                                       BarrierNotifiable *done)
{
    AutoNotify n(done);
    UpdateAction res = initial_load ? REINIT_NEEDED : UPDATED;
    openlcb::EventId short_detected = cfg_.event_short().read(fd);
    openlcb::EventId short_cleared = cfg_.event_short_cleared().read(fd);
    openlcb::EventId shutdown = cfg_.event_shutdown().read(fd);
    openlcb::EventId shutdown_cleared = cfg_.event_shutdown_cleared().read(fd);
    openlcb::EventId thermalflagon = cfg_.event_thermflagon().read(fd);
    openlcb::EventId thermalflagoff = cfg_.event_thermflagoff().read(fd);
    
    auto saved_node = shortBit_.node();
    if (short_detected != shortBit_.event_on() ||
        short_cleared != shortBit_.event_off())
    {
        shortBit_.openlcb::MemoryBit<uint8_t>::~MemoryBit();
        new (&shortBit_)openlcb::MemoryBit<uint8_t>(saved_node, short_detected, short_cleared, &state_, STATE_OVERCURRENT);
        shortProducer_.openlcb::BitEventProducer::~BitEventProducer();
        new (&shortProducer_)openlcb::BitEventProducer(&shortBit_);
        res = REINIT_NEEDED;
    }
    if (shutdown != shutdownBit_.event_on() ||
        shutdown_cleared != shutdownBit_.event_off())
    {
      saved_node = shutdownBit_.node();
      shutdownBit_.openlcb::MemoryBit<uint8_t>::~MemoryBit();
      new (&shutdownBit_)openlcb::MemoryBit<uint8_t>(saved_node, shutdown, shutdown_cleared, &state_, STATE_SHUTDOWN);
      shutdownProducer_.openlcb::BitEventProducer::~BitEventProducer();
      new (&shutdownProducer_)openlcb::BitEventProducer(&shutdownBit_);
      res = REINIT_NEEDED;
    }
    if (thermalflagon != thermalFlagBit_.event_on() ||
        thermalflagoff != thermalFlagBit_.event_off())
    {
      saved_node = thermalFlagBit_.node();
      thermalFlagBit_.openlcb::MemoryBit<uint8_t>::~MemoryBit();
      new (&thermalFlagBit_)openlcb::MemoryBit<uint8_t>(saved_node, thermalflagon, thermalflagoff, &thermalFlag_, 1);
      thermalFlagProducer_.openlcb::BitEventProducer::~BitEventProducer();
      new (&thermalFlagProducer_)openlcb::BitEventProducer(&thermalFlagBit_);
      res = REINIT_NEEDED;
    }
    
    return res;
}

void HBridgeControl::factory_reset(int fd)
{
}

