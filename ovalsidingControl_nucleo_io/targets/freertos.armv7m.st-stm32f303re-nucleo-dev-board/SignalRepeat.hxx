// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Aug 26 19:27:39 2018
//  Last Modified : <180826.2107>
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

#ifndef __SIGNALREPEAT_HXX
#define __SIGNALREPEAT_HXX

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"

CDI_GROUP(Signal3Config);
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<8>, //
                Name("Description"), Description("User name of this signal"));
CDI_GROUP_ENTRY(stop_event, openlcb::EventConfigEntry, //
                Name("Stop Aspect Event"),
                Description("Receiving this event ID will cause the signal to display stop (red)."));
CDI_GROUP_ENTRY(approach_event, openlcb::EventConfigEntry, //
                Name("Approach Aspect Event"),
                Description("Receiving this event ID will cause the signal to display approach (yellow)."));
CDI_GROUP_ENTRY(clear_event, openlcb::EventConfigEntry, //
                Name("Clear Aspect Event"),
                Description("Receiving this event ID will cause the signal to display clear (green)."));
CDI_GROUP_END();

CDI_GROUP(Signal3over2Config);
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<8>, //
                Name("Description"), Description("User name of this signal"));
CDI_GROUP_ENTRY(stop_event, openlcb::EventConfigEntry, //
                Name("Stop Aspect Event"),
                Description("Receiving this event ID will cause the signal to display stop (red over red)."));
CDI_GROUP_ENTRY(approachLimited_event, openlcb::EventConfigEntry, //
                Name("Approach Limited"),
                Description("Receiving this event ID will cause the signal to display approach limited (red over yellow)."));
CDI_GROUP_ENTRY(approach_event, openlcb::EventConfigEntry, //
                Name("Approach Aspect Event"),
                Description("Receiving this event ID will cause the signal to display approach (yellow over red)."));
CDI_GROUP_ENTRY(clear_event, openlcb::EventConfigEntry, //
                Name("Clear Aspect Event"),
                Description("Receiving this event ID will cause the signal to display clear (green over red)."));
CDI_GROUP_END();

class Signal3 : public openlcb::SimpleEventHandler, public ConfigUpdateListener
{
public:
    Signal3(openlcb::Node *node, const Signal3Config &cfg, 
            const Gpio *red, const Gpio *yellow, const Gpio *green)
                : node_(node)
          , red_(red)
          , yellow_(yellow)
          , green_(green)
          , cfg_(cfg)
          , stop_(0)
          , approach_(0)
          , clear_(0)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    template <class RED, class YELLOW, class GREEN>
    Signal3(openlcb::Node *node, const Signal3Config &cfg,
            const RED &, const YELLOW &, const GREEN &,
            const Gpio* red = RED::instance(), 
            const Gpio* yellow = YELLOW::instance(),
            const Gpio* green = GREEN::instance())
                : node_(node)
          , red_(red)
          , yellow_(yellow)
          , green_(green)
          , cfg_(cfg)
          , stop_(0)
          , approach_(0)
          , clear_(0)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE;
    void factory_reset(int fd) OVERRIDE
    {
        cfg_.description().write(fd, "");
    }
    void handle_event_report(const EventRegistryEntry &entry, 
                             EventReport *event,
                             BarrierNotifiable *done) override;
    void handle_identify_global(const EventRegistryEntry &registry_entry, 
                                EventReport *event, BarrierNotifiable *done)
          OVERRIDE;
    void handle_identify_consumer(const EventRegistryEntry &registry_entry,
                                  EventReport *event,
                                  BarrierNotifiable *done) override;
private:
    openlcb::Node *node_;
    const Gpio* red_;
    const Gpio* yellow_;
    const Gpio* green_;
    const Signal3Config cfg_;
    uint64_t stop_, approach_, clear_;
    void SendAllConsumersIdentified(BarrierNotifiable *done);
    void SendConsumerIdentified(EventReport *event,BarrierNotifiable *done);
};
                
class Signal3over2 : public openlcb::SimpleEventHandler, public ConfigUpdateListener
{
public:
    Signal3over2(openlcb::Node *node, const Signal3over2Config &cfg, 
            const Gpio *redU, const Gpio *yellowU, const Gpio *greenU,
            const Gpio *redL, const Gpio *yellowL)
                : node_(node)
          , redU_(redU)
          , yellowU_(yellowU)
          , greenU_(greenU)
          , redL_(redL)
          , yellowL_(yellowL)
          , cfg_(cfg)
          , stop_(0)
          , approach_(0)
          , approachLimited_(0)
          , clear_(0)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    template <class REDU, class YELLOWU, class GREENU, class REDL, class YELLOWL>
    Signal3over2(openlcb::Node *node, const Signal3Config &cfg,
                 const REDU &, const YELLOWU &, const GREENU &,
                 const Gpio* redU = REDU::instance(), 
                 const Gpio* yellowU = YELLOWU::instance(),
                 const Gpio* greenU = GREENU::instance(),
                 const Gpio* redL = REDL::instance(), 
                 const Gpio* yellowL = YELLOWL::instance())
                : node_(node)
          , redU_(redU)
          , yellowU_(yellowU)
          , greenU_(greenU)
          , redL_(redL)
          , yellowL_(yellowL)
          , cfg_(cfg)
          , stop_(0)
          , approach_(0)
          , approachLimited_(0)
          , clear_(0)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE;
    void factory_reset(int fd) OVERRIDE
    {
        cfg_.description().write(fd, "");
    }
    
    void handle_event_report(const EventRegistryEntry &entry, 
                             EventReport *event,
                             BarrierNotifiable *done) override;
    void handle_identify_global(const EventRegistryEntry &registry_entry, 
                                EventReport *event, BarrierNotifiable *done)
          OVERRIDE;
    void handle_identify_consumer(const EventRegistryEntry &registry_entry,
                                  EventReport *event,
                                  BarrierNotifiable *done) override;
private:
    openlcb::Node *node_;
    const Gpio* redU_;
    const Gpio* yellowU_;
    const Gpio* greenU_;
    const Gpio* redL_;
    const Gpio* yellowL_;
    const Signal3over2Config cfg_;
    uint64_t stop_, approach_, approachLimited_, clear_;
    void SendAllConsumersIdentified(BarrierNotifiable *done);
    void SendConsumerIdentified(EventReport *event,BarrierNotifiable *done);
};
                
#endif // __SIGNALREPEAT_HXX

