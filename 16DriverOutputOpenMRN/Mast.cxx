// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Feb 25 20:26:38 2019
//  Last Modified : <190226.2021>
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
#include <os/Gpio.hxx>
#include <stdio.h>

#include "Lamp.hxx"
#include "Rule.hxx"
#include "Mast.hxx"

ConfigUpdateListener::UpdateAction Mast::apply_configuration(int fd, 
                                                       bool initial_load,
                                                       BarrierNotifiable *done)
{
    AutoNotify n(done);
    MastProcessing processing_cfg = (MastProcessing) cfg_.processing().read(fd);
    if (processing_cfg != processing_) {
        processing_ = processing_cfg;
        switch (processing_) {
        case Unused:
        case Normal:
            UpdateMastHead(previous_);
            break;
        case Linked:
            UpdateMastHead(this);
            break;
        }
    }
#ifdef HAVEPWM
    fade_ = (LampFade) cfg_.fade().read(fd);
#endif
    openlcb::EventId linkevent_cfg = cfg_.linkevent().read(fd);
    if (linkevent_cfg != linkevent_) {
        if (!initial_load) unregister_handler();
        linkevent_ = linkevent_cfg;
        register_handler();
        return REINIT_NEEDED; // Causes events identify.
    }
    return UPDATED;
}

void Mast::factory_reset(int fd)
{
//    fprintf(stderr,"*** Mast::factory_reset(), &cfg_ is %p\n", &cfg_);
//    fprintf(stderr,"*** Mast::factory_reset(), cfg_.offset() = %d\n",cfg_.offset());
//    fprintf(stderr,"*** Mast::factory_reset(), cfg_.size() = %d\n",cfg_.size());
//    fprintf(stderr,"*** Mast::factory_reset(), cfg_.end_offset() = %d\n",cfg_.end_offset());
    cfg_.mastid().write(fd,"");
    CDI_FACTORY_RESET(cfg_.processing);
#ifdef HAVEPWM
    CDI_FACTORY_RESET(cfg_.fade);
#endif
}

void Mast::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                  EventReport *event, BarrierNotifiable *done)
{
}

void Mast::handle_identify_producer(const EventRegistryEntry &registry_entry,
                                    EventReport *event, 
                                    BarrierNotifiable *done)
{
}

void Mast::register_handler()
{
}

void Mast::unregister_handler()
{
}

void Mast::SendAllProducersIdentified(EventReport *event,BarrierNotifiable *done)
{
}

void Mast::SendProducerIdentified(EventReport *event,BarrierNotifiable *done)
{
}

void Mast::SetRule(Rule *newRule,Rule::TrackSpeed speed,
                   BarrierNotifiable *done)
{
    if (current_ != nullptr && current_ != newRule) {
        current_->ClearRule(done);
        current_ = nullptr;
    }
    for (int i = 0; i < RULESCOUNT; i++) {
        if (rules_[i] == newRule) {
            current_ = newRule;
            current_->SetRule(done);
            break;
        }
    }
    if (current_ == nullptr && processing_ == Linked) {
        previous_->SetRule(newRule,Rule::None_,done);
    }
    if (speed != Rule::None_) {
        write_helper.WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(linkevent_+(int)speed),
                                done);
    }
    done->maybe_done();
}

void Mast::UpdateMastHead(Mast *head)
{
    for (int i=0; i < RULESCOUNT; i++) {
        rules_[i]->SetParent(head);
    }
}



