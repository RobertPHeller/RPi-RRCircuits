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
//  Last Modified : <220927.0950>
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
#include "utils/logging.h"
#include <os/Gpio.hxx>
#include <stdio.h>

#include "Lamp.hxx"
#include "Rule.hxx"
#include "Mast.hxx"
#include "TrackCircuit.hxx"

Mast * Mast::masts[MASTCOUNT];

void Mast::Init(openlcb::Node *node,
                const openlcb::RepeatedGroup<MastConfig, MASTCOUNT> &config)
{
    openlcb::ConfigReference offset_(config);
    openlcb::RepeatedGroup<MastConfig, UINT_MAX> grp_ref(offset_.offset());
    Mast *prevMast = nullptr;
    for (unsigned i = 0; i < MASTCOUNT; i++) 
    {
        masts[i] = new Mast(node,grp_ref.entry(i),prevMast);
        prevMast = masts[i];
    }
}



ConfigUpdateListener::UpdateAction Mast::apply_configuration(int fd, 
                                                       bool initial_load,
                                                       BarrierNotifiable *done)
{
    AutoNotify n(done);
    MastProcessing processing_cfg = (MastProcessing) cfg_.processing().read(fd);
    if (processing_cfg != processing_ || initial_load) {
        if (previous_ == nullptr && processing_cfg == Linked) {
            processing_cfg = processing_;
            cfg_.processing().write(fd,(uint8_t)processing_);
        }
        if (processing_ == Normal && 
            !initial_load && 
            processing_cfg != Normal) unregister_handler();
        processing_ = processing_cfg;
    }
#ifdef HAVEPWM
    fade_ = (LampFade) cfg_.fade().read(fd);
#endif
    openlcb::EventId linkevent_cfg = cfg_.linkevent().read(fd);
    if ((linkevent_cfg != linkevent_) && (processing_ == Normal || initial_load)) {
        if (!initial_load) unregister_handler();
        linkevent_ = linkevent_cfg;
        register_handler();
        return REINIT_NEEDED; // Causes events identify.
    }
    mastid_ = cfg_.mastid().read(fd);
    return UPDATED;
}

uint16_t Mast::baseLinkEvent_(TRACKCIRCUITBASE);

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
    EventId id = node_->node_id();
    id <<= 16;
    id |= baseLinkEvent_;
    baseLinkEvent_ += 8;
    cfg_.linkevent().write(fd,id);
}

void Mast::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                  EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node_)
    {
        done->notify();
        return;
    }
    SendAllProducersIdentified(event,done);
    done->maybe_done();
}

void Mast::handle_identify_producer(const EventRegistryEntry &registry_entry,
                                    EventReport *event, 
                                    BarrierNotifiable *done)
{
    SendProducerIdentified(event,done);
    done->maybe_done();
}

void Mast::register_handler()
{
    for (int i = 0; i < 8; i++) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, linkevent_+i), 0);
    }
}

void Mast::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}

void Mast::SendAllProducersIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti;
    for (int i=0; i < 8; i++) {
        if (i == (int) currentSpeed_) {
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        } else {
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
        }
        write_helper[i].WriteAsync(node_, mti, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(linkevent_+i),
                                   done->new_child());
    }
}

void Mast::SendProducerIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
    if (event->event >= linkevent_ && event->event < (linkevent_+8)) {
        if ((event->event-currentSpeed_) == linkevent_) {
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        } else {
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
        }
    }
    event->event_write_helper<1>()->WriteAsync(node_, mti, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event->event),
                                   done->new_child());
}

void Mast::ClearCurrentRule(BarrierNotifiable *done)
{
    //LOG(ALWAYS, "*** Mast[%s]::ClearCurrentRule(): currentRule_ = %p", Mastid().c_str(),currentRule_);
    if (currentRule_ != nullptr)
    {
        currentRule_->ClearRule(done);
        currentRule_ = nullptr;
        return;
    } else if (processing_ == Linked && previous_ != nullptr) {
        previous_->ClearCurrentRule(done);
    }
}

void Mast::SetCurrentRuleAndSpeed(Rule *r, TrackCircuit::TrackSpeed s, 
                                  BarrierNotifiable *done)
{
    //LOG(ALWAYS, "*** Mast[%s]::SetCurrentRuleAndSpeed(): previous_ = %p",Mastid().c_str(),previous_);
    //LOG(ALWAYS, "*** Mast::SetCurrentRuleAndSpeed(): r = %p", r);
    if (processing_ == Linked && previous_ != nullptr) {
        previous_->SetCurrentRuleAndSpeed(r,s,done);
    } else {
        currentRule_ = r;
        currentSpeed_ = s;
        write_helper[(int)s].WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(linkevent_+(int)s),
                                done->new_child());
    }
    //done->maybe_done();
}





