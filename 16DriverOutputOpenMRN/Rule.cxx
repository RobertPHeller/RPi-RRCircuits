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
//  Last Modified : <190226.2046>
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


#include "Lamp.hxx"
#include "Rule.hxx"
#include "Mast.hxx"

ConfigUpdateListener::UpdateAction Rule::apply_configuration(int fd, 
                                       bool initial_load,
                                       BarrierNotifiable *done)
{
    AutoNotify n(done);
    name_ = (RuleName) cfg_.name().read(fd);
    speed_ = (TrackSpeed) cfg_.speed().read(fd);
    openlcb::EventId eventsets_cfg = cfg_.eventsets().read(fd);
    openlcb::EventId eventset_cfg = cfg_.eventset().read(fd);
    openlcb::EventId eventclear_cfg = cfg_.eventclear().read(fd);
    effects_ = (Effects) cfg_.effects().read(fd);
    effectsLamp_ = (Lamp::LampID) cfg_.effectslamp().read(fd);
    if (eventsets_cfg != eventsets_ ||
        eventset_cfg  != eventset_ ||
        eventclear_cfg != eventclear_) {
        if (!initial_load) unregister_handler();
        eventsets_ = eventsets_cfg;
        eventset_  = eventset_cfg;
        eventclear_ = eventclear_cfg;
        register_handler();
        return REINIT_NEEDED; // Causes events identify.
    }
    return UPDATED;
}

void Rule::factory_reset(int fd)
{
    CDI_FACTORY_RESET(cfg_.name);
    CDI_FACTORY_RESET(cfg_.speed);
    CDI_FACTORY_RESET(cfg_.effects);
    CDI_FACTORY_RESET(cfg_.effectslamp);
}

void Rule::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                  EventReport *event, BarrierNotifiable *done)
{
}

void Rule::handle_identify_producer(const EventRegistryEntry &registry_entry,
                                    EventReport *event, 
                                    BarrierNotifiable *done)
{
}

void Rule::handle_event_report(const EventRegistryEntry &entry, 
                               EventReport *event,
                               BarrierNotifiable *done)
{
    if (event->event == eventsets_) {
        if (parent_ != nullptr) {
            parent_->SetRule(this,speed_,done);
        } else {
            done->notify();
        }
    }
    done->maybe_done();
}

void Rule::handle_identify_consumer(const EventRegistryEntry &registry_entry,
                                    EventReport *event,
                                    BarrierNotifiable *done)
{
}

void Rule::register_handler()
{
}

void Rule::unregister_handler()
{
}

void Rule::SendAllConsumersIdentified(EventReport *event,BarrierNotifiable *done)
{
}

void Rule::SendConsumerIdentified(EventReport *event,BarrierNotifiable *done)
{
}



void Rule::ClearRule(BarrierNotifiable *done)
{
    for (int i=0; i < LAMPCOUNT; i++) {
        lamps_[i]->Off();
    }
    /* Effects: effects_, effectsLamp_ */
    write_helper.WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
                            openlcb::WriteHelper::global(),
                            openlcb::eventid_to_buffer(eventclear_),
                            done);
}

void Rule::SetRule(BarrierNotifiable *done)
{
    for (int i=0; i < LAMPCOUNT; i++) {
        lamps_[i]->On();
    }
    /* Effects: effects_, effectsLamp_ */
    write_helper.WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
                            openlcb::WriteHelper::global(),
                            openlcb::eventid_to_buffer(eventset_),
                            done);
}


