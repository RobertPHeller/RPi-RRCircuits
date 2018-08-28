// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Tue Aug 28 09:34:15 2018
//  Last Modified : <180828.1212>
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
#include "utils/Debouncer.hxx"
#include "openlcb/RefreshLoop.hxx"

#include "ControlPoint.hxx"

void SwitchLeverBits::handle_identify_producer(const openlcb::EventRegistryEntry &entry,
                                               openlcb::EventReport *event,
                                               BarrierNotifiable *done)
{
    if (event->src_node.id == node_->node_id())
    {
        done->notify();
        return;
    }
    if (event->event == normal_event_ ||
        event->event == reverse_event_) {
        openlcb::event_write_helper1.WriteAsync(node_,
                                                openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN,
                                                openlcb::WriteHelper::global(),
                                                openlcb::eventid_to_buffer(event->event), 
                                                done);
    } else {
        done->notify();
        return;
    }
}

void SwitchLeverBits::handle_identify_global(const openlcb::EventRegistryEntry &entry,
                                             openlcb::EventReport *event,
                                             BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
    if (event->dst_node && event->dst_node != node_)
    {
        done->notify();
        return;
    }
    openlcb::event_write_helper1.WriteAsync(node_,mti,
                                            openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(normal_event_),
                                            done->new_child());
    openlcb::event_write_helper2.WriteAsync(node_,mti,
                                            openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(reverse_event_),
                                            done->new_child());
    done->maybe_done();
}

void SwitchLeverBits::SendEventReport(openlcb::WriteHelper *writer, Notifiable *done)
{
    uint64_t event;
    if (normal_->is_clr()) {
        event = normal_event_;
    } else if (reverse_->is_clr()) {
        event = reverse_event_;
    } else {
        done->notify();
        return;
    }
    writer->WriteAsync(node_, openlcb::Defs::MTI_EVENT_REPORT,
                       openlcb::WriteHelper::global(), 
                       openlcb::eventid_to_buffer(event),done);
              
}
void SwitchLeverBits::register_handler()
{
    openlcb::EventRegistry::instance()->register_handler(
          openlcb::EventRegistryEntry(this, normal_event_), 0);
    openlcb::EventRegistry::instance()->register_handler(
          openlcb::EventRegistryEntry(this, reverse_event_), 0);
}
void SwitchLeverBits::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}

void ControlPoint::poll_33hz(openlcb::WriteHelper *helper, Notifiable *done)
{
    if (debouncer_.update_state(codeButton_->is_clr())) {
        producer_.SendEventReport(helper, done);
    } else {
        done->notify();
    } 
}

ConfigUpdateListener::UpdateAction ControlPoint::apply_configuration(int fd, bool initial_load,
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
        producer_.SwitchLeverBits::~SwitchLeverBits();
        new (&producer_) SwitchLeverBits(saved_node,cfg_normal_event,cfg_reverse_event,saved_normal,saved_reverse);
        new (&debouncer_) QuiesceDebouncer::Options(debounce_arg);
        debouncer_.initialize(codeButton_->is_set());
        return REINIT_NEEDED; // Causes events identify.
    }
    return UPDATED;
}
