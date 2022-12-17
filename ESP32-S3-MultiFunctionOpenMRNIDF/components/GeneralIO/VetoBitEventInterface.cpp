// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Feb 27 10:37:29 2022
//  Last Modified : <220227.1434>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2022  Robert Heller D/B/A Deepwoods Software
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

#include <unistd.h>

#include "utils/logging.h"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/EventService.hxx"
#include "VetoBitEventInterface.hxx"

void VetoBitEventConsumer::register_handler(uint64_t event_on, 
                                            uint64_t event_off,
                                            uint64_t veto_on, 
                                            uint64_t veto_off)
{
    openlcb::EventRegistry::instance()->register_handler(
         openlcb::EventRegistryEntry(this, event_on), 1);
    openlcb::EventRegistry::instance()->register_handler(
         openlcb::EventRegistryEntry(this, event_off), 1);
    openlcb::EventRegistry::instance()->register_handler(
         openlcb::EventRegistryEntry(this, veto_on), 1);
    openlcb::EventRegistry::instance()->register_handler(
         openlcb::EventRegistryEntry(this, veto_off), 1);
}

void VetoBitEventConsumer::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}

void VetoBitEventConsumer::SendConsumerIdentified(openlcb::EventReport *event, 
                                                  BarrierNotifiable *done)
{
    openlcb::EventState vetostate = bit_->get_veto_state();
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID + vetostate;
    event->event_write_helper<1>()->WriteAsync(bit_->node(), mti,
        openlcb::WriteHelper::global(), 
        openlcb::eventid_to_buffer(bit_->veto_on()),
        done->new_child());
    mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID + openlcb::invert_event_state(vetostate);
    event->event_write_helper<2>()->WriteAsync(bit_->node(), mti,
        openlcb::WriteHelper::global(), 
        openlcb::eventid_to_buffer(bit_->veto_off()),
        done->new_child());
    openlcb::EventState state = bit_->get_current_state();
    mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID + state;
    event->event_write_helper<3>()->WriteAsync(bit_->node(), mti,
        openlcb::WriteHelper::global(), openlcb::eventid_to_buffer(bit_->event_on()),
        done->new_child());
    mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID + openlcb::invert_event_state(state);
    event->event_write_helper<4>()->WriteAsync(bit_->node(), mti,
        openlcb::WriteHelper::global(), openlcb::eventid_to_buffer(bit_->event_off()),
        done->new_child());
}


void VetoBitEventConsumer::HandlePCIdentify(openlcb::Defs::MTI mti, 
                                            openlcb::EventReport *event,
                                            BarrierNotifiable *done)
{
    if (event->src_node.id == bit_->node()->node_id())
    {
        // We don't respond to queries from our own node. This is not nice, but
        // we
        // want to avoid to answering our own Query command.
        done->notify();
        return;
    }
    openlcb::EventState active;
    if (event->event == bit_->event_on())
    {
        active = bit_->get_current_state();
    }
    else if (event->event == bit_->event_off())
    {
        active = openlcb::invert_event_state(bit_->get_current_state());
    }
    else if (event->event == bit_->veto_on())
    {
        active = bit_->get_veto_state();
    }
    else if (event->event == bit_->veto_off())
    {
        active = openlcb::invert_event_state(bit_->get_veto_state());
    }
    else
    {
        done->notify();
        return;
    }
    mti = mti + active;
    event->event_write_helper<1>()->WriteAsync(bit_->node(), mti,
        openlcb::WriteHelper::global(), openlcb::eventid_to_buffer(event->event), done);
}

void VetoBitEventConsumer::SendQuery(openlcb::WriteHelper *writer, 
                                     BarrierNotifiable *done)
{
    write_helpers[0].WriteAsync(bit_->node(), openlcb::Defs::MTI_PRODUCER_IDENTIFY,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(bit_->event_on()), done->new_child());
    write_helpers[1].WriteAsync(bit_->node(), openlcb::Defs::MTI_PRODUCER_IDENTIFY,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(bit_->veto_on()), done->new_child());
}


void VetoBitEventConsumer::handle_event_report(const openlcb::EventRegistryEntry &entry,
                                               openlcb::EventReport *event,
                                               BarrierNotifiable *done)
{
    if (event->event == bit_->event_on())
    {
        bit_->set_state(true);
    }
    else if (event->event == bit_->event_off())
    {
        bit_->set_state(false);
    }
    else if (event->event == bit_->veto_on())
    {
        bit_->set_veto_state(true);
    }
    else if (event->event == bit_->veto_off())
    {
        bit_->set_veto_state(false);
    }
    done->notify();
}

void VetoBitEventConsumer::handle_identify_global(const openlcb::EventRegistryEntry &entry,
                                                  openlcb::EventReport *event,
                                                  BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != bit_->node())
    {
        return done->notify();
    }
    SendConsumerIdentified(event, done);
    done->maybe_done();
}

void VetoBitEventConsumer::handle_identify_consumer(const openlcb::EventRegistryEntry &entry,
                                                    openlcb::EventReport *event,
                                                    BarrierNotifiable *done)
{
    HandlePCIdentify(openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID, event, done);
}

void VetoBitEventConsumer::handle_producer_identified(const openlcb::EventRegistryEntry &entry,
                                                      openlcb::EventReport *event,
                                                      BarrierNotifiable *done)
{
    done->notify();
    bool value;
    if (event->state == openlcb::EventState::VALID)
    {
        value = true;
    }
    else if (event->state == openlcb::EventState::INVALID)
    {
        value = false;
    }
    else
    {
        return; // nothing to learn from this message.
    }
    if (event->event == bit_->event_on())
    {
        bit_->set_state(value);
    }
    else if (event->event == bit_->event_off())
    {
        bit_->set_state(!value);
    }
    else if (event->event == bit_->veto_on())
    {
        bit_->set_veto_state(value);
    }
    else if (event->event == bit_->veto_off())
    {
        bit_->set_veto_state(!value);
    }
    else
    {
        return; // uninteresting event id.
    }
}



