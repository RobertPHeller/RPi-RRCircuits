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
//  Last Modified : <191029.0057>
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

#ifndef __HBRIDGECONTROL_HXX
#define __HBRIDGECONTROL_HXX

#include "openlcb/PolledProducer.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"

/// CDI Configuration for a @ref HBridgeControl.
CDI_GROUP(HBridgeControlConfig);
CDI_GROUP_ENTRY(currentthresh,
                openlcb::Uint16ConfigEntry,
                Name("Current threshold, in milliamps."),
                Default(3000),Min(250),Max(3000),
                Description("This is the current level to issue an event."));
CDI_GROUP_ENTRY(overcurrent,
                openlcb::EventConfigEntry,
                Name("Over Current Event"));
CDI_GROUP_ENTRY(disable,
                openlcb::EventConfigEntry,
                Name("Disable DCC"));
CDI_GROUP_ENTRY(enable,
                openlcb::EventConfigEntry,
                Name("Enable DCC"));
CDI_GROUP_ENTRY(thermflagon,
                openlcb::EventConfigEntry,
                Name("Thermal Flag on"));
CDI_GROUP_ENTRY(thermflagoff,
                openlcb::EventConfigEntry,
                Name("Thermal Flag off"));
CDI_GROUP_END();

class HBridgeControl : public ConfigUpdateListener, public openlcb::SimpleEventHandler, public openlcb::Polling {
public:
    HBridgeControl(openlcb::Node *node, 
                   const HBridgeControlConfig &cfg, 
                   uint8_t currentAIN, 
                   const Gpio *enableGpio, 
                   const Gpio *thermFlagGpio);
    template <class ENABLE, class THERMFLAG>
          HBridgeControl(openlcb::Node *node, 
                         const HBridgeControlConfig &cfg, 
                         uint8_t currentAIN, 
                         const ENABLE&, 
                         const THERMFLAG&, 
                         const Gpio *enableGpio = ENABLE::instance(), 
                         const Gpio *thermFlagGpio = THERMFLAG::instance());
    ~HBridgeControl();
    virtual void handle_identify_global(const openlcb::EventRegistryEntry &registry_entry,
                                        openlcb::EventReport *event,
                                        BarrierNotifiable *done);
    virtual void handle_identify_consumer(const openlcb::EventRegistryEntry &registry_entry,
                                          openlcb::EventReport *event, BarrierNotifiable *done);
    virtual void handle_identify_producer(const openlcb::EventRegistryEntry &registry_entry,
                                          openlcb::EventReport *event, BarrierNotifiable *done);
    virtual void handle_event_report(const openlcb::EventRegistryEntry &registry_entry,
                                     openlcb::EventReport *event,
                                     BarrierNotifiable *done);
    virtual void poll_33hz(openlcb::WriteHelper *helper, Notifiable *done);
    virtual UpdateAction apply_configuration(int fd, bool initial_load,
                                             BarrierNotifiable *done);

    virtual void factory_reset(int fd);
    bool EnabledP() const {return isEnabled_;}
    bool ThermalFlagP() const {return thermflagState_;}
    bool OverCurrentP() const {return isOverCurrent_;}
    openlcb::Polling *polling() {return this;}
private:
    openlcb::Node *node_;
    const HBridgeControlConfig cfg_;
    uint8_t currentAIN_;
    const Gpio *enableGpio_;
    const Gpio *thermFlagGpio_;
    openlcb::EventId overcurrent_event_;
    openlcb::EventId disable_event_;
    openlcb::EventId enable_event_;
    openlcb::EventId thermflagon_event_;
    openlcb::EventId thermflagoff_event_;
    uint16_t currentthresh_;
    bool registered_;
    bool isEnabled_;
    bool thermflagState_;
    bool isOverCurrent_;
    /// Requests the event associated with the current value of the bit to be
    /// produced (unconditionally): sends an event report packet ot the bus.
    ///
    /// @param writer is the output flow to be used.
    ///
    /// @param done is the notification callback. If it is NULL, the writer will
    /// be invoked inline and potentially block the calling thread.
    void SendEventReport(int helperIndex, openlcb::EventId event, BarrierNotifiable *done);
    /// Registers this event handler with the global event manager. Call this
    /// from the constructor of the derived class.
    void register_handler();
    /// Removes this event handler from the global event manager. Call this
    /// from the destructor of the derived class.
    void unregister_handler();
    void SendProducerIdentified(openlcb::EventReport *event, BarrierNotifiable *done);
    void SendAllProducersIdentified(openlcb::EventReport *event,BarrierNotifiable *done);
    void SendConsumerIdentified(openlcb::EventReport *event, BarrierNotifiable *done);
    void SendAllConsumersIdentified(openlcb::EventReport *event,BarrierNotifiable *done);
    openlcb::WriteHelper write_helper[8];                              
};            

#endif // __HBRIDGECONTROL_HXX

