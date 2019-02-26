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
//  Last Modified : <190226.1135>
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
    processing_ = (MastProcessing) cfg_.processing().read(fd);
    fade_ = (LampFade) cfg_.fade().read(fd);
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
    cfg_.mastid().write(fd,"");
    CDI_FACTORY_RESET(cfg_.processing);
    CDI_FACTORY_RESET(cfg_.fade);
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

void Mast::handle_event_report(const EventRegistryEntry &entry, 
                               EventReport *event,
                               BarrierNotifiable *done)
{
}

void Mast::handle_identify_consumer(const EventRegistryEntry &registry_entry,
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

void Mast::SendAllConsumersIdentified(EventReport *event,BarrierNotifiable *done)
{
}

void Mast::SendConsumerIdentified(EventReport *event,BarrierNotifiable *done)
{
}




