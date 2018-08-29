// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Aug 29 16:04:36 2018
//  Last Modified : <180829.1836>
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

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"

#include "openlcb/EventHandler.hxx"
#include "openlcb/WriteHelper.hxx"
#include "os/Gpio.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
//#include "utils/Debouncer.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "ThreeStateDebouncer.hxx"

#include "ConfiguredPointSense.hxx"

void PolledPointSense::handle_identify_producer(const openlcb::EventRegistryEntry &entry,
                                                openlcb::EventReport *event,
                                                BarrierNotifiable *done)
{
    SendProducerIdentified(event,done);
    done->maybe_done();
}

void PolledPointSense::handle_identify_global(const openlcb::EventRegistryEntry &entry,
                                              openlcb::EventReport *event,
                                              BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node_)
    {
        done->notify();
    }
    SendAllProducersIdentified(done);
    done->maybe_done();
}

void PolledPointSense::SendEventReport(openlcb::WriteHelper *writer, Notifiable *done)
{
    switch (get_current_state())
    {
    case openlcb::EventState::VALID:
        writer->WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(normal_event_),
                           done);
        break;
    case openlcb::EventState::INVALID:
        writer->WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(reverse_event_),
                           done);
        break;
    default:
        break;
    }
}

void PolledPointSense::register_handler()
{
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this, normal_event_), 0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this, reverse_event_), 0);
}

void PolledPointSense::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}

void PolledPointSense::SendProducerIdentified(EventReport *event, BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
    switch (get_current_state()) {
    case openlcb::EventState::VALID:
        if (event->event == normal_event_)
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        else if (event->event == reverse_event_)
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
        break;
    case openlcb::EventState::INVALID:
        if (event->event == normal_event_)
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
        else if (event->event == reverse_event_)
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    default:
        break;
    }
    openlcb::event_write_helper1.WriteAsync(node_, mti, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(event->event),
                                            done->new_child());

}

void PolledPointSense::SendAllProducersIdentified(BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_n, mti_r;
    switch (get_current_state()) {
    case openlcb::EventState::VALID:
        mti_n = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        mti_r = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
        break;
    case openlcb::EventState::INVALID:
        mti_n = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
        mti_r = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    default:
        mti_r = mti_n = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
        break;
    }
    openlcb::event_write_helper1.WriteAsync(node_, mti_n, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(normal_event_),
                                   done->new_child());
    openlcb::event_write_helper2.WriteAsync(node_, mti_r, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(reverse_event_),
                                   done->new_child());
}


ConfigUpdateListener::UpdateAction PointSense::apply_configuration(int fd, bool initial_load,
                                                                   BarrierNotifiable *done)
{
    AutoNotify n(done);
    uint8_t debounce_arg = cfg_.debounce().read(fd);
    openlcb::EventId cfg_normal_event = cfg_.normal_event().read(fd);
    openlcb::EventId cfg_reverse_event = cfg_.reverse_event().read(fd);
    if (cfg_normal_event != producer_.normal_event() ||
        cfg_reverse_event != producer_.reverse_event())
    {
        auto saved_normal = producer_.normal();
        auto saved_reverse = producer_.reverse();
        auto saved_node = producer_.node();
        signed char saved_state;
        switch (producer_.get_current_state()) {
        case openlcb::EventState::VALID:
            saved_state = 0; break;
        case openlcb::EventState::INVALID:
            saved_state = 1; break;
        default:
            saved_state = -1; break;
        }        
        producer_.PolledPointSense::~PolledPointSense();
        new (&producer_) PolledPointSense(saved_node,
                                          debounce_arg,
                                          cfg_normal_event,
                                          cfg_reverse_event,
                                          saved_normal,
                                          saved_reverse,
                                          saved_state);
        return REINIT_NEEDED; // Causes events identify.
    }
    return UPDATED;
}


