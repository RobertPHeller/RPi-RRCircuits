// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Feb 25 20:06:13 2019
//  Last Modified : <221004.1657>
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


#include "Lamp.hxx"
#include "Rule.hxx"
#include "Mast.hxx"
#include "TrackCircuit.hxx"

bool Rule::UpdateConfig(const RuleConfig &cfg_,int fd, bool initial_load)
{
    name_ = (RuleName) cfg_.name().read(fd);
    speed_ = (TrackCircuit::TrackSpeed) cfg_.speed().read(fd);
    openlcb::EventId eventsets_cfg = cfg_.eventsets().read(fd);
    openlcb::EventId eventset_cfg = cfg_.eventset().read(fd);
    openlcb::EventId eventclear_cfg = cfg_.eventclear().read(fd);
#ifdef EFFECTS
    effects_ = (Effects) cfg_.effects().read(fd);
    effectsLamp_ = (Lamp::LampID) cfg_.effectslamp().read(fd);
#endif
    for (int i = 0; i < LAMPCOUNT; i++) {
        Lamp::LampID lampid = (Lamp::LampID)cfg_.lamps().entry(i).selection().read(fd);
        Lamp::LampPhase phase = (Lamp::LampPhase) cfg_.lamps().entry(i).phase().read(fd);
        uint16_t brightness = cfg_.lamps().entry(i).brightness().read(fd);
        uint32_t period = cfg_.lamps().entry(i).period().read(fd);
        lamps_[i].UpdateConfig(lampid,phase,brightness,period);
    }
#if 0
    LOG(ALWAYS,"*** Rule::apply_configuration(): "
        "eventsets_cfg is %llx, eventsets_ is %llx, "
        "eventset_cfg is %llx, eventset_ is %llx, "
        "eventclear_cfg is %llx, eventclear_ is %llx, "
        "initial_load is %d", eventsets_cfg, eventsets_,
        eventset_cfg, eventset_, eventclear_cfg, eventclear_, 
        initial_load);
#endif
    if (eventsets_cfg != eventsets_ ||
        eventset_cfg  != eventset_ ||
        eventclear_cfg != eventclear_ || initial_load) {
        if (!initial_load) unregister_handler();
        eventsets_ = eventsets_cfg;
        eventset_  = eventset_cfg;
        eventclear_ = eventclear_cfg;
        register_handler();
        return true; // Causes events identify.
    }
    return false;
}
void Rule::factory_reset(const RuleConfig &cfg_,int fd)
{
    CDI_FACTORY_RESET(cfg_.name);
    CDI_FACTORY_RESET(cfg_.speed);
#ifdef EFFECTS
    CDI_FACTORY_RESET(cfg_.effects);
    CDI_FACTORY_RESET(cfg_.effectslamp);
#endif
    for (int i = 0; i < LAMPCOUNT; i++) {
        CDI_FACTORY_RESET(cfg_.lamps().entry(i).selection);
        CDI_FACTORY_RESET(cfg_.lamps().entry(i).phase);
        CDI_FACTORY_RESET(cfg_.lamps().entry(i).brightness);
        CDI_FACTORY_RESET(cfg_.lamps().entry(i).period);
    }
}

void Rule::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                  EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node_)
    {
        done->notify();
        return;
    }
    SendAllConsumersIdentified(event,done);
    SendAllProducersIdentified(event,done);
    done->maybe_done();
}

void Rule::handle_identify_producer(const EventRegistryEntry &registry_entry,
                                    EventReport *event, 
                                    BarrierNotifiable *done)
{
    SendProducerIdentified(event,done);
    done->maybe_done();
}

void Rule::handle_identify_consumer(const EventRegistryEntry &registry_entry,
                                    EventReport *event,
                                    BarrierNotifiable *done)
{
    SendConsumerIdentified(event,done);
    done->maybe_done();
}

void Rule::handle_event_report(const EventRegistryEntry &entry, 
                               EventReport *event,
                               BarrierNotifiable *done)
{
    //LOG(ALWAYS, "*** Rule::handle_event_report(): event->event is %llx, eventsets_ is %llx, parent_ is %s",event->event,eventsets_,parent_->Mastid().c_str());
    if (event->event == eventsets_) {
        if (parent_ != nullptr) {
            parent_->ClearCurrentRule(done);
#ifdef EFFECTS
            /* Effects before set go here: effects_, effectsLamp_ */
#endif
            for (int i=0; i < LAMPCOUNT; i++) {
                //LOG(ALWAYS, "*** Rule::handle_event_report(): lamps_[%d] (%p) on",i,lamps_[i].Pin());
                lamps_[i].On();
            }
            write_helpers[1].WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
                                    openlcb::WriteHelper::global(),
                                    openlcb::eventid_to_buffer(eventset_),
                                    done->new_child());
            parent_->SetCurrentRuleAndSpeed(this,speed_,done);
            isSet_ = true;
        }
    }
    done->maybe_done();
}

void Rule::ClearRule(BarrierNotifiable *done)
{
    //LOG(ALWAYS, "*** Rule::ClearRule()");
    for (int i=0; i < LAMPCOUNT; i++) {
        lamps_[i].Off();
    }
#ifdef EFFECTS
    /* Effects after clear go here: effects_, effectsLamp_ */
#endif
    write_helpers[2].WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
                            openlcb::WriteHelper::global(),
                            openlcb::eventid_to_buffer(eventclear_),
                               done->new_child());
    isSet_ = false;
}

void Rule::register_handler()
{
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this, eventsets_), 0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this, eventset_), 0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this, eventclear_), 0);
}

void Rule::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}

void Rule::SendAllConsumersIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
    if (isSet_) mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    write_helpers[0].WriteAsync(node_,mti,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(eventsets_),
                                done->new_child());
}

void Rule::SendConsumerIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN;
    if (event->event == eventsets_) {
        //LOG(ALWAYS, "*** Rule::SendConsumerIdentified(): event->event is %llu",event->event);
        if (isSet_) mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
        else mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
    }
    event->event_write_helper<1>()->WriteAsync(node_, mti, 
                                               openlcb::WriteHelper::global(),
                                               openlcb::eventid_to_buffer(event->event),
                                               done->new_child());
}

void Rule::SendAllProducersIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_set = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID,
          mti_clear = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    if (isSet_) {
        mti_set = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else {
        mti_clear = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }
    write_helpers[1].WriteAsync(node_,mti_set,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(eventset_),
                                done->new_child());
    write_helpers[2].WriteAsync(node_,mti_clear,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(eventclear_),
                                done->new_child());
    
    
}

void Rule::SendProducerIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
    if (event->event == eventset_) {
        if (isSet_) mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        else mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    } else if (event->event == eventclear_) {
        if (!isSet_) mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        else mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    }
    event->event_write_helper<1>()->WriteAsync(node_, mti, 
                                               openlcb::WriteHelper::global(),
                                               openlcb::eventid_to_buffer(event->event),
                                               done->new_child());
    
}





