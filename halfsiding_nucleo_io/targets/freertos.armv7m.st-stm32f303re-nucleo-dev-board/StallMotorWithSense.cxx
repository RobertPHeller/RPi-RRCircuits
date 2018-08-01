// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri Jun 15 22:23:28 2018
//  Last Modified : <180801.1254>
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

#include "StallMotorWithSense.hxx"
#include "openlcb/EventService.hxx"

#ifdef LOGLEVEL
#undef LOGLEVEL
#define LOGLEVEL VERBOSE
#endif

void StallMotorWithSense::poll_33hz(openlcb::WriteHelper *helper, 
                                    Notifiable *done)
{
    PointStates newstate;
    if (pointsenseA->is_clr() && pointsenseB->is_set()) {
        newstate = normal;
    } else if (pointsenseB->is_clr() && pointsenseA->is_set()) {
        newstate = reversed;
    } else {
        newstate = unknown;
    }
    if (newstate == point_state) {
        debounceCount = 0;
        done->notify();
        return;
    }
    if (++debounceCount == 3) {
        point_state = newstate;
        debounceCount = 0;
        SendEventReport(helper, done);
    }
    done->notify();
}

void StallMotorWithSense::SendEventReport(openlcb::WriteHelper *writer, 
                                          Notifiable *done)
{
    switch (point_state) {
    case normal:
        if (points_normal_event != 0LL)
            writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(points_normal_event),
                           done);
        break;
    case reversed:
        if (points_reversed_event != 0LL)
            writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(points_reversed_event),
                           done);
        break;
    default:
        done->notify();
        return;
    }
    done->notify();
}

ConfigUpdateListener::UpdateAction 
StallMotorWithSense::apply_configuration(int fd,
                                         bool initial_load,
                                         BarrierNotifiable *done)
{
    AutoNotify n(done);
    openlcb::EventId cfg_motor_normal_event = config.motor_normal_event().read(fd);
    openlcb::EventId cfg_motor_reversed_event = config.motor_reversed_event().read(fd);
    openlcb::EventId cfg_points_normal_event = config.points_normal_event().read(fd);
    openlcb::EventId cfg_points_reversed_event = config.points_reversed_event().read(fd);
    if (cfg_motor_normal_event != motor_normal_event ||
        cfg_motor_reversed_event != motor_reversed_event ||
        cfg_points_normal_event != points_normal_event ||
        cfg_points_reversed_event != points_reversed_event) {
        if (!initial_load) unregister_handler();
        motor_normal_event = cfg_motor_normal_event;
        motor_reversed_event = cfg_motor_reversed_event;
        points_normal_event = cfg_points_normal_event;
        points_reversed_event = cfg_points_reversed_event;
        register_handler();
        return REINIT_NEEDED; // Causes events identify.
    }
    return UPDATED;
}

void StallMotorWithSense::factory_reset(int fd)
{
    LOG(INFO,"StallMotorWithSense::factory_reset(%d)",fd);
    config.description().write(fd,"");
}

void StallMotorWithSense::handle_event_report(const EventRegistryEntry &entry, 
                                              EventReport *event,
                                              BarrierNotifiable *done)
{
    if (event->event == motor_normal_event) {
        motorA->set();
        motorB->clr();
        motor_state = normal;
    } else if (event->event == motor_reversed_event) {
        motorB->set();
        motorA->clr();
        motor_state = reversed;
    }
    done->notify();
}

void StallMotorWithSense::handle_identify_global(const EventRegistryEntry &registry_entry, 
                                                 EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node)
    {
        done->notify();
    }
    SendAllProducersIdentified(done);
    SendAllConsumersIdentified(done);
    done->maybe_done();
}

void StallMotorWithSense::handle_identify_producer(const EventRegistryEntry &registry_entry,
                                                   EventReport *event,
                                                   BarrierNotifiable *done)
{
    SendProducerIdentified(event,done);
    done->maybe_done();
}
          
    
void StallMotorWithSense::SendAllProducersIdentified(BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_n, mti_r;
    if (point_state == normal) {
        mti_n = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        mti_r = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    } else if (point_state == reversed) {
        mti_n = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
        mti_r = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else {
        mti_r = mti_n = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
    }
    openlcb::event_write_helper1.WriteAsync(node, mti_n, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(points_normal_event),
                                   done->new_child());
    openlcb::event_write_helper2.WriteAsync(node, mti_r, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(points_reversed_event),
                                   done->new_child());
}

void StallMotorWithSense::SendProducerIdentified(EventReport *event,
                                                 BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    if (event->event == points_normal_event &&
        point_state == normal) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else if (event->event == points_reversed_event &&
               point_state == reversed) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }
    openlcb::event_write_helper1.WriteAsync(node, mti, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event->event),
                                   done->new_child());
}

void StallMotorWithSense::handle_identify_consumer(const EventRegistryEntry &registry_entry,
                                                   EventReport *event,
                                                   BarrierNotifiable *done)
{
    SendConsumerIdentified(event,done);
    done->maybe_done();
}
          
void StallMotorWithSense::SendAllConsumersIdentified(BarrierNotifiable *done)
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
    openlcb::event_write_helper3.WriteAsync(node, mti_n, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(motor_normal_event),
                                   done->new_child());
    openlcb::event_write_helper4.WriteAsync(node, mti_r, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(motor_reversed_event),
                                   done->new_child());
}

void StallMotorWithSense::SendConsumerIdentified(EventReport *event,
                                                 BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
    if (event->event == motor_normal_event &&
        motor_state == normal) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (event->event == motor_reversed_event &&
               motor_state == reversed) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN;
    }
    openlcb::event_write_helper3.WriteAsync(node, mti, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event->event),
                                            done->new_child());
}


void StallMotorWithSense::register_handler()
{
    if (motor_normal_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, motor_normal_event), 0);
    }
    if (motor_reversed_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, motor_reversed_event), 0);
    }
    if (points_normal_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, points_normal_event), 0);
    }
    if (points_reversed_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, points_reversed_event), 0);
    }
}

void StallMotorWithSense::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}

