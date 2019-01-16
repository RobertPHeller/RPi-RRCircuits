// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Oct 15 21:27:11 2018
//  Last Modified : <181124.1416>
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

#include "MadHatter.hxx"
#include "openlcb/EventService.hxx"

#ifdef LOGLEVEL
#undef LOGLEVEL
#define LOGLEVEL VERBOSE
#endif

bool MadHatter::eval()
{
    bool result = false;
    // Handle off:
    if (siding->OccupiedP()) {
        if_off = true;
    } else {
        if_off = false;
        result = then_off;
        then_off = false;
    }
    if (s31459->OccupiedP()) {
        if (if_off) {
            result = !then_off;
            then_off = true;
            lightGpio->clr();
        }
    }
    //if (result) return result;
    // Handle on:
    if (s31459->OccupiedP()) {
        if_on = true;
    } else {
        if_on = false;
        result = then_on;
        then_on = false;
    }
    if (siding->OccupiedP()) {
        if (if_on) {
            result = !then_on;
            then_on = true;
            lightGpio->set();
        }
    }
    return result;
}

void MadHatter::handle_identify_global(const EventRegistryEntry &registry_entry, 
                                       EventReport *event, 
                                       BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node)
    {
        done->notify();
    }
    SendAllProducersIdentified(event,done);
    done->maybe_done();
}

void MadHatter::handle_identify_producer(const EventRegistryEntry &registry_entry,
                                         EventReport *event, 
                                         BarrierNotifiable *done)
{
    SendProducerIdentified(event,done);
    done->maybe_done();
}

void MadHatter::SendEventReport(openlcb::WriteHelper *writer, Notifiable *done)
{
    if (then_on) {
        writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_on),done);
    } else if (then_off) {
        writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_off),done);
    }
}

void MadHatter::SendAllProducersIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_on = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID, 
          mti_off = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    
    if (then_on) {
        mti_on = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }
    if (then_off) {
        mti_off = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }
    if (event_on != 0LL)
        event->event_write_helper<1>()->WriteAsync(node, mti_on, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_on),
                                   done->new_child());
    if (event_off != 0LL)
        event->event_write_helper<2>()->WriteAsync(node, mti_off, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_off),
                                   done->new_child());
}

void MadHatter::SendProducerIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    if (event->event == event_on && then_on) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else if (event->event == event_off && then_off) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }
    event->event_write_helper<1>()->WriteAsync(node, mti, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event->event),
                                   done->new_child());
}

void MadHatter::register_handler()
{
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this, event_on), 0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this, event_off), 0);
}

void MadHatter::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}



