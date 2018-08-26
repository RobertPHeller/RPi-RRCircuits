// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Aug 26 17:44:20 2018
//  Last Modified : <180826.1801>
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

#ifndef __OCCUPENCYREPEAT_HXX
#define __OCCUPENCYREPEAT_HXX

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"

/// CDI Configuration for a @ref ConfiguredConsumer.
CDI_GROUP(OccupencyRepeatConfig);
/// Allows the user to assign a name for this output.
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<8>, //
                Name("Description"), Description("User name of this block."));
/// Specifies the event ID to set the output to ON.
CDI_GROUP_ENTRY(
    occupied_event, openlcb::EventConfigEntry, //
    Name("Occupied Event"),
    Description("Receiving this event ID will turn the block indication on."));
/// Specifies the event ID to set the output to OFF.
CDI_GROUP_ENTRY(
    unoccupied_event, openlcb::EventConfigEntry, //
    Name("Not Occupied Event"),
    Description("Receiving this event ID will turn the block indication off."));
CDI_GROUP_END();

/// OpenLCB Consumer class integrating a simple CDI-based configuration for two
/// event IDs, and an output GPIO object that will be turned on or off
/// depending on the incoming event notifications. This is usually the most
/// important object for a simple IO node.
class OccupencyRepeat : public ConfigUpdateListener
{
public:
    using Impl = openlcb::GPIOBit;

    OccupencyRepeat(openlcb::Node *node, const OccupencyRepeatConfig &cfg, const Gpio *gpio)
        : impl_(node, 0, 0, gpio)
        , consumer_(&impl_)
        , cfg_(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }

    template <class HW>
    OccupencyRepeat(openlcb::Node *node, const OccupencyRepeatConfig &cfg, const HW &, const Gpio* g = HW::instance())
        : impl_(node, 0, 0, g)
        , consumer_(&impl_)
        , cfg_(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }

    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE
    {
        AutoNotify n(done);
        openlcb::EventId cfg_event_on = cfg_.occupied_event().read(fd);
        openlcb::EventId cfg_event_off = cfg_.unoccupied_event().read(fd);
        if (cfg_event_off != impl_.event_off() ||
            cfg_event_on != impl_.event_on())
        {
            auto saved_gpio = impl_.gpio_;
            auto saved_node = impl_.node();
            // Need to reinitialize the consumer. We do this with in-place
            // destruction and construction.
            consumer_.~BitEventConsumer();
            impl_.Impl::~Impl();
            new (&impl_)
                Impl(saved_node, cfg_event_on, cfg_event_off, saved_gpio);
            new (&consumer_) openlcb::BitEventConsumer(&impl_);
            return REINIT_NEEDED; // Causes events identify.
        }
        return UPDATED;
    }

    void factory_reset(int fd) OVERRIDE
    {
        cfg_.description().write(fd, "");
    }

private:
    Impl impl_;
    openlcb::BitEventConsumer consumer_;
    const OccupencyRepeatConfig cfg_;
};



#endif // __OCCUPENCYREPEAT_HXX

