// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Aug 26 20:36:02 2018
//  Last Modified : <181124.1406>
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
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "SignalRepeat.hxx"

ConfigUpdateListener::UpdateAction Signal3::apply_configuration(int fd, 
                                                                bool initial_load,
                                                                BarrierNotifiable *done)
{
    AutoNotify n(done);
    openlcb::EventId cfg_stop_event = cfg_.stop_event().read(fd);
    openlcb::EventId cfg_approach_event = cfg_.approach_event().read(fd);
    openlcb::EventId cfg_clear_event = cfg_.clear_event().read(fd);
    if (cfg_stop_event != stop_ ||
        cfg_approach_event != approach_ ||
        cfg_clear_event != clear_) {
        if (!initial_load) {
            openlcb::EventRegistry::instance()->unregister_handler(this);
        }
        stop_ = cfg_stop_event;
        approach_ = cfg_approach_event;
        clear_ = cfg_clear_event;
        openlcb::EventRegistry::instance()->register_handler(
                 openlcb::EventRegistryEntry(this, stop_), 0);
        openlcb::EventRegistry::instance()->register_handler(
                 openlcb::EventRegistryEntry(this, approach_), 0);
        openlcb::EventRegistry::instance()->register_handler(
                 openlcb::EventRegistryEntry(this, clear_), 0);
        return REINIT_NEEDED; // Causes events identify.
    }
    return UPDATED;
}

void Signal3::handle_event_report(const EventRegistryEntry &entry, 
                                  EventReport *event,
                                  BarrierNotifiable *done)
{
    if (event->event == stop_) {
        red_->set();
        yellow_->clr();
        green_->clr();
    } else if (event->event == approach_) {
        red_->clr();
        yellow_->set();
        green_->clr();
    } else if (event->event == clear_) {
        red_->clr();
        yellow_->clr();
        green_->set();
    }
    done->notify();
}

void Signal3::handle_identify_global(const EventRegistryEntry &registry_entry, 
                                     EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node_)
    {
        done->notify();
    }
    SendAllConsumersIdentified(event,done);
    done->maybe_done();
}

void Signal3::handle_identify_consumer(const EventRegistryEntry &registry_entry,
                                       EventReport *event,
                                       BarrierNotifiable *done)
{
    SendConsumerIdentified(event,done);
    done->maybe_done();
}

void Signal3::SendAllConsumersIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_s, mti_a, mti_c;
    mti_s = mti_a = mti_c = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN;
    event->event_write_helper<1>()->WriteAsync(node_, mti_s, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(stop_),
                                            done->new_child());
    event->event_write_helper<2>()->WriteAsync(node_, mti_a, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(approach_),
                                            done->new_child());
    event->event_write_helper<3>()->WriteAsync(node_, mti_c, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(clear_),
                                            done->new_child());
}

void Signal3::SendConsumerIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN;
    if (event->event == stop_ ||
        event->event == approach_ ||
        event->event == clear_)
        event->event_write_helper<1>()->WriteAsync(node_, mti, openlcb::WriteHelper::global(),
                                                openlcb::eventid_to_buffer(event->event),
                                                done->new_child());
}



ConfigUpdateListener::UpdateAction Signal3over2::apply_configuration(int fd, 
                                                                     bool initial_load,
                                                                     BarrierNotifiable *done) 
{
    AutoNotify n(done);
    openlcb::EventId cfg_stop_event = cfg_.stop_event().read(fd);
    openlcb::EventId cfg_approach_event = cfg_.approach_event().read(fd);
    openlcb::EventId cfg_approachLimited_event = cfg_.approachLimited_event().read(fd);
    openlcb::EventId cfg_clear_event = cfg_.clear_event().read(fd);
    if (cfg_stop_event != stop_ ||
        cfg_approach_event != approach_ ||
        cfg_approachLimited_event != approachLimited_ ||
        cfg_clear_event != clear_) {
        if (!initial_load) {
            openlcb::EventRegistry::instance()->unregister_handler(this);
        }
        stop_ = cfg_stop_event;
        approach_ = cfg_approach_event;
        clear_ = cfg_clear_event;
        openlcb::EventRegistry::instance()->register_handler(
                 openlcb::EventRegistryEntry(this, stop_), 0);
        openlcb::EventRegistry::instance()->register_handler(
                 openlcb::EventRegistryEntry(this, approach_), 0);
        openlcb::EventRegistry::instance()->register_handler(
                 openlcb::EventRegistryEntry(this, approachLimited_), 0);
        openlcb::EventRegistry::instance()->register_handler(
                 openlcb::EventRegistryEntry(this, clear_), 0);
        return REINIT_NEEDED; // Causes events identify.
    }
    return UPDATED;
}

void Signal3over2::handle_event_report(const EventRegistryEntry &entry, 
                                       EventReport *event,
                                       BarrierNotifiable *done)
{
    if (event->event == stop_) {
        redU_->set();
        yellowU_->clr();
        greenU_->clr();
        redL_->set();
        yellowL_->clr();
    } else if (event->event == approach_) {
        redU_->clr();
        yellowU_->set();
        greenU_->clr();
        redL_->set();
        yellowL_->clr();
    } else if (event->event == approachLimited_) {
        redU_->set();
        yellowU_->clr();
        greenU_->clr();
        redL_->clr();
        yellowL_->set();
    } else if (event->event == clear_) {
        redU_->clr();
        yellowU_->clr();
        greenU_->set();
        redL_->set();
        yellowL_->clr();
    }
    done->notify();
}

void Signal3over2::handle_identify_global(const EventRegistryEntry &registry_entry, 
                                          EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node_)
    {
        done->notify();
    }
    SendAllConsumersIdentified(event,done);
    done->maybe_done();
}

void Signal3over2::handle_identify_consumer(const EventRegistryEntry &registry_entry,
                                            EventReport *event,
                                            BarrierNotifiable *done)
{
    SendConsumerIdentified(event,done);
    done->maybe_done();
}

void Signal3over2::SendAllConsumersIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_s, mti_a, mti_aL, mti_c;
    mti_s = mti_a = mti_aL = mti_c = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN;
    event->event_write_helper<1>()->WriteAsync(node_, mti_s, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(stop_),
                                            done->new_child());
    event->event_write_helper<2>()->WriteAsync(node_, mti_a, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(approach_),
                                            done->new_child());
    event->event_write_helper<3>()->WriteAsync(node_, mti_aL, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(approachLimited_),
                                            done->new_child());
    event->event_write_helper<4>()->WriteAsync(node_, mti_c, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(clear_),
                                            done->new_child());
}

void Signal3over2::SendConsumerIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN;
    if (event->event == stop_ ||
        event->event == approach_ ||
        event->event == approachLimited_ ||
        event->event == clear_)
        event->event_write_helper<1>()->WriteAsync(node_, mti, openlcb::WriteHelper::global(),
                                                openlcb::eventid_to_buffer(event->event),
                                                done->new_child());
}

