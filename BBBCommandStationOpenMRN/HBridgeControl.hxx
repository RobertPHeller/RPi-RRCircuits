// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Oct 28 13:33:15 2019
//  Last Modified : <221118.0819>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//! @copyright
//!    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
//!			51 Locke Hill Road
//!			Wendell, MA 01379-9728
//!
//!    This program is free software; you can redistribute it and/or modify
//!    it under the terms of the GNU General Public License as published by
//!    the Free Software Foundation; either version 2 of the License, or
//!    (at your option) any later version.
//!
//!    This program is distributed in the hope that it will be useful,
//!    but WITHOUT ANY WARRANTY; without even the implied warranty of
//!    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//!    GNU General Public License for more details.
//!
//!    You should have received a copy of the GNU General Public License
//!    along with this program; if not, write to the Free Software
//!    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//! @file HBridgeControl.hxx
//! Manage a H-Bridge.
//!
//////////////////////////////////////////////////////////////////////////////

#ifndef __HBRIDGECONTROL_HXX
#define __HBRIDGECONTROL_HXX

#include <executor/StateFlow.hxx>
#include "openlcb/PolledProducer.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include <openlcb/Node.hxx>
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"
#include <os/Gpio.hxx>
#include <os/OS.hxx>
#include <utils/ConfigUpdateListener.hxx>
#include <utils/Debouncer.hxx>

#define BIT(n) (1 << n)

/// CDI Configuration for a @ref HBridgeControl.
CDI_GROUP(HBridgeControlConfig);
CDI_GROUP_ENTRY(event_short,
                openlcb::EventConfigEntry,
                Name("Short Detected"),
                Description("This event will be produced when a short has "
                            "been detected on the track output."));
CDI_GROUP_ENTRY(event_short_cleared,
                openlcb::EventConfigEntry,
                Name("Short Cleared"),
                Description("This event will be produced when a short has "
                            "been cleared on the track output."));
CDI_GROUP_ENTRY(event_shutdown,
                openlcb::EventConfigEntry,
                Name("H-Bridge Shutdown"),
                Description("This event will be produced when the track "
                            "output power has exceeded the safety threshold "
                            "of the H-Bridge."));
CDI_GROUP_ENTRY(event_shutdown_cleared,
                openlcb::EventConfigEntry,
                Name("H-Bridge Shutdown Cleared"),
                Description("This event will be produced when the track "
                            "output power has returned to safe levels."));
CDI_GROUP_ENTRY(event_thermflagon,
                openlcb::EventConfigEntry,
                Name("Thermal Flag on"));
CDI_GROUP_ENTRY(event_thermflagoff,
                openlcb::EventConfigEntry,
                Name("Thermal Flag off"));
CDI_GROUP_END();

class HBridgeControl : public ConfigUpdateListener, public openlcb::Polling {
public:
    HBridgeControl(openlcb::Node *node, 
                   const HBridgeControlConfig &cfg, 
                   uint8_t currentAIN, 
                   const uint32_t limitMilliAmps,
                   const uint32_t maxMilliAmps,
                   const Gpio *enableGpio, 
                   const Gpio *thermFlagGpio = NULL);
    HBridgeControl(openlcb::Node *node, 
                   const HBridgeControlConfig &cfg, 
                   uint8_t currentAIN, 
                   const uint32_t maxMilliAmps,
                   const Gpio *enableGpio, 
                   const Gpio *thermFlagGpio = NULL);
    enum STATE : uint8_t
    {
        STATE_OVERCURRENT = BIT(0),
        STATE_SHUTDOWN    = BIT(1),
        STATE_ON          = BIT(2),
        STATE_OFF         = BIT(3)
    };
    ~HBridgeControl();
    virtual void poll_33hz(openlcb::WriteHelper *helper, Notifiable *done);
    virtual UpdateAction apply_configuration(int fd, bool initial_load,
                                             BarrierNotifiable *done);

    virtual void factory_reset(int fd);
    bool EnabledP() const {return state_ != STATE_OFF;}
    bool ThermalFlagP() const {return thermalFlag_ == 1;}
    bool OverCurrentP() const {return (state_ & STATE_OVERCURRENT) != 0;}
    openlcb::Polling *polling() {return this;}
    uint32_t getMaxMilliAmps() {return maxMilliAmps_;}
    uint32_t getLastReading() {return lastReading_;}
    bool isProgrammingTrack() {return isProgTrack_;}
    void enable_prog_response(bool enable)
    {
        progEnable_ = enable;
    }
private:
    openlcb::Node *node_;
    const HBridgeControlConfig cfg_;
    const uint8_t currentAIN_;
    const uint8_t adcSampleCount_{32};
    const uint8_t overCurrentRetryCount_{3};
    const Gpio *enableGpio_;
    const Gpio *thermFlagGpio_;
    const uint32_t maxMilliAmps_;
    const uint32_t overCurrentLimit_;
    const uint32_t shutdownLimit_;
    bool isProgTrack_;
    const uint32_t progAckLimit_;
    openlcb::MemoryBit<uint8_t> shortBit_;
    openlcb::MemoryBit<uint8_t> shutdownBit_;
    openlcb::MemoryBit<uint8_t> thermalFlagBit_;
    openlcb::BitEventProducer shortProducer_;
    openlcb::BitEventProducer shutdownProducer_;
    openlcb::BitEventProducer thermalFlagProducer_;
    bool progEnable_{false};
    uint8_t state_{STATE_OFF};
    uint8_t overCurrentCheckCount_{0};
    uint32_t lastReading_{0};
    uint8_t thermalFlag_{0};
};            

#endif // __HBRIDGECONTROL_HXX

