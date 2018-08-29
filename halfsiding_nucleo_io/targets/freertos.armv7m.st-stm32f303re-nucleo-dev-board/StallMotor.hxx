// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Aug 29 15:01:16 2018
//  Last Modified : <180829.1854>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
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

#ifndef __STALLMOTOR_HXX
#define __STALLMOTOR_HXX

#include "os/Gpio.hxx"
#include "openlcb/MemoryConfig.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"

CDI_GROUP(StallMotorConfig);
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<15>, //
                Name("Description"), Description("User name of this turnout"));
CDI_GROUP_ENTRY(normal_event, openlcb::EventConfigEntry, //
                Name("Motor Normal"),
                Description("When this event is received, the motor will be set to align the turnout to its 'normal' position."));
CDI_GROUP_ENTRY(reverse_event, openlcb::EventConfigEntry, //
                Name("Motor Reversed"),
                Description("When this event is received, the motor will be set to align the turnout to its 'reversed' position."));
CDI_GROUP_END();


class StallMotor : public ConfigUpdateListener,
                            public openlcb::SimpleEventHandler
{
public:
    StallMotor(openlcb::Node *_node, const StallMotorConfig &cfg,
               const Gpio *_motorA, const Gpio *_motorB
               ) 
                : node(_node)
          , motorA(_motorA)
          , motorB(_motorB)
          , config(cfg)
          
    {
        motor_state = unknown;
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    virtual UpdateAction apply_configuration(int fd,
                                             bool initial_load,
                                             BarrierNotifiable *done) override;
    virtual void factory_reset(int fd);
    void handle_event_report(const EventRegistryEntry &entry, 
                             EventReport *event,
                             BarrierNotifiable *done) override;
    void handle_identify_global(const EventRegistryEntry &registry_entry, 
                                EventReport *event, BarrierNotifiable *done)
          OVERRIDE;
    void handle_identify_consumer(const EventRegistryEntry &registry_entry,
                                  EventReport *event,
                                  BarrierNotifiable *done) override;
    void register_handler();
    void unregister_handler();
private:
    openlcb::Node *node;
    const Gpio *motorA;
    const Gpio *motorB;
    enum MotorStates {normal,reversed,unknown} motor_state;
    openlcb::EventId normal_event;
    openlcb::EventId reverse_event;
    const StallMotorConfig config;
    void SendAllConsumersIdentified(BarrierNotifiable *done);
    void SendConsumerIdentified(EventReport *event,BarrierNotifiable *done);
};



#endif // __STALLMOTOR_HXX

