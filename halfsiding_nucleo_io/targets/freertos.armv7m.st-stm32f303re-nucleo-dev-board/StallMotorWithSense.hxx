// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri Jun 15 19:02:56 2018
//  Last Modified : <180801.1221>
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

#ifndef __STALLMOTORWITHSENSE_HXX
#define __STALLMOTORWITHSENSE_HXX

#include "os/Gpio.hxx"
#include "openlcb/MemoryConfig.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"

CDI_GROUP(StallMotorWithSenseConfiguration);
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<15>, //
                Name("Description"), Description("User name of this turnout"));
CDI_GROUP_ENTRY(motor_normal_event, openlcb::EventConfigEntry, //
                Name("Motor Normal"),
                Description("When this event is received, the motor will be set to align the turnout to its 'normal' position."));
CDI_GROUP_ENTRY(motor_reversed_event, openlcb::EventConfigEntry, //
                Name("Motor Reversed"),
                Description("When this event is received, the motor will be set to align the turnout to its 'reversed' position."));
CDI_GROUP_ENTRY(points_normal_event, openlcb::EventConfigEntry, //
                Name("Points Normal"),
                Description("This event will be produced when the points are aligned to their 'normal' position."));
CDI_GROUP_ENTRY(points_reversed_event, openlcb::EventConfigEntry, //
                Name("Points Reversed"),
                Description("This event will be produced when the points are aligned to their 'reversed' position."));
CDI_GROUP_END();


class StallMotorWithSense : public openlcb::Polling, 
                public ConfigUpdateListener,
                public openlcb::SimpleEventHandler
{
public:
    StallMotorWithSense(openlcb::Node *_node, const StallMotorWithSenseConfiguration &cfg,
                        const Gpio *_motorA, const Gpio *_motorB,
                        const Gpio *_pointsenseA, const Gpio *_pointsenseB) 
          : config(cfg)
          
    {
        node = _node;
        motorA = _motorA;
        motorB = _motorB;
        pointsenseA = _pointsenseA;
        pointsenseB = _pointsenseB;
        motor_normal_event = 0;
        motor_reversed_event = 0;
        points_normal_event = 0;
        points_reversed_event = 0;
        point_state = unknown;
        motor_state = unknown;
        debounceCount = 0;
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    ~StallMotorWithSense() {
        unregister_handler();
    }
    openlcb::Polling *polling()
    {
        return this;
    }
    virtual void poll_33hz(openlcb::WriteHelper *helper, Notifiable *done);
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
    void handle_identify_producer(const EventRegistryEntry &registry_entry,
                                  EventReport *event,
                                  BarrierNotifiable *done) override;
    void handle_identify_consumer(const EventRegistryEntry &registry_entry,
                                  EventReport *event,
                                  BarrierNotifiable *done) override;
private:
    openlcb::Node *node;
    const Gpio *occ;
    const Gpio *motorA;
    const Gpio *motorB;
    const Gpio *pointsenseA;
    const Gpio *pointsenseB;
    openlcb::EventId motor_normal_event;
    openlcb::EventId motor_reversed_event;
    openlcb::EventId points_normal_event;
    openlcb::EventId points_reversed_event;
    typedef enum {normal, reversed, unknown} PointStates;
    PointStates point_state, motor_state;
    const StallMotorWithSenseConfiguration config;
    void SendAllProducersIdentified(BarrierNotifiable *done);
    void SendProducerIdentified(EventReport *event,BarrierNotifiable *done);
    void SendAllConsumersIdentified(BarrierNotifiable *done);
    void SendEventReport(openlcb::WriteHelper *writer, Notifiable *done);
    void SendConsumerIdentified(EventReport *event,BarrierNotifiable *done);
    uint8_t debounceCount; 
    void register_handler();
    void unregister_handler();
};


#endif // __STALLMOTORWITHSENSE_HXX

