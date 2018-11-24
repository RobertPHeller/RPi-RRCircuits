// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Aug 29 18:37:03 2018
//  Last Modified : <181124.1343>
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

static const char rcsid[] = "@(#) : $Id$";

#include "os/Gpio.hxx"
#include "openlcb/MemoryConfig.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"

#include "StallMotor.hxx"

ConfigUpdateListener::UpdateAction StallMotor::apply_configuration(int fd,
                                                                   bool initial_load,
                                                                   BarrierNotifiable *done)
{
    AutoNotify n(done);
    openlcb::EventId cfg_normal_event = config.normal_event().read(fd);
    openlcb::EventId cfg_reverse_event = config.reverse_event().read(fd);
    if (cfg_normal_event != normal_event ||
        cfg_reverse_event != reverse_event) {
        if (!initial_load) unregister_handler();
        normal_event = cfg_normal_event;
        reverse_event = cfg_reverse_event;
        register_handler();
        return REINIT_NEEDED; // Causes events identify.
    }
    return UPDATED;
}

void StallMotor::factory_reset(int fd)
{
    config.description().write(fd,"");
}

void StallMotor::handle_event_report(const EventRegistryEntry &entry, 
                                     EventReport *event,
                                     BarrierNotifiable *done)
{
    if (event->event == normal_event) {
        motorA->set();
        motorB->clr();
        motor_state = normal;
    } else if (event->event == reverse_event) {
        motorB->set();
        motorA->clr();
        motor_state = reversed;
    }
    done->notify();
}

void StallMotor::handle_identify_global(const EventRegistryEntry &registry_entry, 
                                        EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node)
    {
        done->notify();
    }
    SendAllConsumersIdentified(event,done);
    done->maybe_done();
}

void StallMotor::handle_identify_consumer(const EventRegistryEntry &registry_entry,
                                          EventReport *event,
                                          BarrierNotifiable *done)
{
    SendConsumerIdentified(event,done);
    done->maybe_done();
}

void StallMotor::SendAllConsumersIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_n, mti_r;
    if (motor_state == normal) {
        mti_n = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
        mti_r = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
    } else if (motor_state == reversed) {
        mti_n = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
        mti_r = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else {
        mti_r = mti_n = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN;
    }
    event->event_write_helper<3>()->WriteAsync(node, mti_n, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(normal_event),
                                            done->new_child());
    event->event_write_helper<4>()->WriteAsync(node, mti_r, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(reverse_event),
                                            done->new_child());
}

void StallMotor::SendConsumerIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
    if (event->event == normal_event &&
        motor_state == normal) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (event->event == reverse_event &&
               motor_state == reversed) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN;
    }
    event->event_write_helper<3>()->WriteAsync(node, mti, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event->event),
                                            done->new_child());
}


void StallMotor::register_handler()
{
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this, normal_event), 0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this, reverse_event), 0);
}

void StallMotor::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}

