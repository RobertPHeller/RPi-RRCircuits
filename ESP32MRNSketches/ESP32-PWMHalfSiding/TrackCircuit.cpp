// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Feb 28 20:24:32 2019
//  Last Modified : <211125.1733>
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

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"
#include <stdio.h>

#include <vector>

#include "TrackCircuit.h"

ConfigUpdateListener::UpdateAction TrackCircuit::apply_configuration(int fd, bool initial_load, BarrierNotifiable *done) 
{
    AutoNotify n(done);
    openlcb::EventId remotemastlink_cfg = cfg_.remotemastlink().read(fd);
    if (remotemastlink_cfg != remotemastlink_ || initial_load) {
        if (!initial_load) unregister_handler();
        remotemastlink_ = remotemastlink_cfg;
        register_handler();
        return REINIT_NEEDED; // Causes events identify.
    }
    return UPDATED;
}

void TrackCircuit::factory_reset(int fd)
{
    cfg_.description().write(fd,"");
}

void TrackCircuit::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node_)
    {
        done->notify();
        return;
    }
    SendAllConsumersIdentified(event,done);
    done->maybe_done();
}

void TrackCircuit::handle_event_report(const EventRegistryEntry &entry, EventReport *event, BarrierNotifiable *done)
{
    for (int i = 0; i < 8; i++) {
        if (event->event == (remotemastlink_+i)) {
            speed_ = (TrackSpeed) i;
            for (callback_type_iterator i = callbacks_.begin();
                 i != callbacks_.end(); i++) {
                (*i)->trigger(this,done);
            }
            break;
        }
    }
    done->maybe_done();
}

void TrackCircuit::handle_identify_consumer(const EventRegistryEntry &registry_entry, EventReport *event, BarrierNotifiable *done)
{
    SendConsumerIdentified(event,done);
    done->maybe_done();
}

void TrackCircuit::register_handler()
{
    for (int i = 0; i < 8; i++) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, remotemastlink_+i), 0);
    }
}

void TrackCircuit::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}

void TrackCircuit::SendAllConsumersIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti;
    for (int i=0; i < 8; i++) {
        if (i == (int) speed_) {
            mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
        } else {
            mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
        }
        write_helpers[i].WriteAsync(node_, mti, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(remotemastlink_+i),
                                   done->new_child());
    }
}

void TrackCircuit::SendConsumerIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN;
    if (event->event >= remotemastlink_ && event->event < (remotemastlink_+8)) {
        if ((event->event-speed_) == remotemastlink_) {
            mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
        } else {
            mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
        }
    }
    event->event_write_helper<1>()->WriteAsync(node_, mti, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event->event),
                                   done->new_child());
}
