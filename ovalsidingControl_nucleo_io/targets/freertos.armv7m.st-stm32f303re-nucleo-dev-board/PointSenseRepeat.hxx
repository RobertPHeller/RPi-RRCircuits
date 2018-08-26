// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Aug 26 15:39:27 2018
//  Last Modified : <180826.1631>
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

#ifndef __POINTSENSEREPEAT_HXX
#define __POINTSENSEREPEAT_HXX

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"

CDI_GROUP(PointSenseRepeatConfig);
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<8>, //
                Name("Description"), Description("User name of this turnout."));
CDI_GROUP_ENTRY(
                normal_event, openlcb::EventConfigEntry, //
                Name("Normal"),
                Description("Receiving this event ID will indicate points are align for normal."));
CDI_GROUP_ENTRY(
                reverse_event, openlcb::EventConfigEntry, //
                Name("Reverse"),
                Description("Receiving this event ID will indicate points are align for reverse."));
CDI_GROUP_END();


class PointSenseRepeat : public ConfigUpdateListener
{
public:
    using Impl = openlcb::GPIOBit;
    
    PointSenseRepeat(openlcb::Node *node, const PointSenseRepeatConfig &cfg, 
                     const Gpio *normLED, const Gpio *revLED)
                : impl_normLED(node, 0, 0, normLED)
          , impl_revLED(node, 0, 0, revLED)
          , consumer_normLED(&impl_normLED)
          , consumer_revLED(&impl_revLED)
          , cfg_(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }
#if 0
    template <class normLED, class revLED>
    PointSenseRepeat(openlcb::Node *node, const PointSenseRepeatConfig &cfg, 
                     const normLED &, const revLED &,
                     const Gpio *normLEDg = normLED::instance(), 
                     const Gpio *revLEDg = revLED::instance())
                : impl_normLED(node, 0, 0, normLEDg)
          , impl_revLED(node, 0, 0, revLEDg)
          , consumer_normLED(&impl_normLED)
          , consumer_revLED(&impl_revLED)
          , cfg_(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }
#endif
    UpdateAction apply_configuration(int fd, bool initial_load,
                                              BarrierNotifiable *done) OVERRIDE
    {
        AutoNotify n(done);
        openlcb::EventId cfg_normal_event = cfg_.normal_event().read(fd);
        openlcb::EventId cfg_reverse_event = cfg_.reverse_event().read(fd);
        if (cfg_normal_event != impl_revLED.event_off() ||
            cfg_reverse_event != impl_revLED.event_on() ||
            cfg_normal_event != impl_normLED.event_on() ||
            cfg_reverse_event != impl_normLED.event_off())
        {
            auto saved_node = impl_normLED.node();
            auto saved_normLED = impl_normLED.gpio_;
            auto saved_revLED  = impl_revLED.gpio_;
            consumer_normLED.~BitEventConsumer();
            impl_normLED.Impl::~Impl();
            consumer_revLED.~BitEventConsumer();
            impl_revLED.Impl::~Impl();
            new (&impl_normLED)
                  Impl(saved_node, cfg_normal_event, cfg_reverse_event, saved_normLED);
            new (&consumer_normLED) openlcb::BitEventConsumer(&impl_normLED);
            new (&impl_revLED)
                  Impl(saved_node, cfg_reverse_event, cfg_normal_event, saved_revLED);
            new (&consumer_revLED) openlcb::BitEventConsumer(&impl_revLED);
            return REINIT_NEEDED; // Causes events identify.
        }
        return UPDATED;
    }
    void factory_reset(int fd) OVERRIDE
    {
        cfg_.description().write(fd, "");
    }
private:
    Impl impl_normLED;
    Impl impl_revLED;
    openlcb::BitEventConsumer consumer_normLED;
    openlcb::BitEventConsumer consumer_revLED;
    const PointSenseRepeatConfig cfg_;
};

#endif // __POINTSENSEREPEAT_HXX

