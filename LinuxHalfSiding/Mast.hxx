// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Jun 11 17:23:44 2018
//  Last Modified : <181124.1323>
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

#ifndef __MAST_HXX
#define __MAST_HXX

#include "os/Gpio.hxx"
#include "openlcb/MemoryConfig.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"

#include "Points.hxx"
#include "OccDetector.hxx"
#include "NoProducerOccDetector.hxx"

CDI_GROUP(MastPointsConfiguration);
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<15>, //
                Name("Description"), Description("User name of this mast"));
CDI_GROUP_ENTRY(stop_event, openlcb::EventConfigEntry, //
                Name("Stop Aspect"),
                Description("This event will be produced when the aspect becomes Stop"));
CDI_GROUP_ENTRY(approach_limited_event, openlcb::EventConfigEntry, //
                Name("Approach Limited Aspect"),
                Description("This event will be produced when the aspect becomes Approach Limited"));
CDI_GROUP_ENTRY(approach_event, openlcb::EventConfigEntry, //
                Name("Approach Aspect"),
                Description("This event will be produced when the aspect becomes Approach"));
CDI_GROUP_ENTRY(clear_event, openlcb::EventConfigEntry, //
                Name("Clear Aspect"),
                Description("This event will be produced when the aspect becomes Clear"));
CDI_GROUP_END();

CDI_GROUP(MastFrogConfiguration);
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<15>, //
                Name("Description"), Description("User name of this mast"));
CDI_GROUP_ENTRY(stop_event, openlcb::EventConfigEntry, //
                Name("Stop Aspect"),
                Description("This event will be produced when the aspect becomes Stop"));
CDI_GROUP_ENTRY(approach_event, openlcb::EventConfigEntry, //
                Name("Approach Aspect"),
                Description("This event will be produced when the aspect becomes Approach"));
CDI_GROUP_ENTRY(clear_event, openlcb::EventConfigEntry, //
                Name("Clear Aspect"),
                Description("This event will be produced when the aspect becomes Clear"));
CDI_GROUP_END();



class MastPoints : public openlcb::Polling , public ConfigUpdateListener, public openlcb::SimpleEventHandler {
public:
    MastPoints(openlcb::Node *_node, const MastPointsConfiguration &cfg,
               OccupancyDetector *_occ, Points *_points, 
               openlcb::EventState _pointseventstate,
               NoProducerOccDetector *_next,
               const Gpio *_maingreen, const Gpio *_mainyellow, 
               const Gpio *_mainred,
               const Gpio *_divyellow, const Gpio *_divred) 
                : node(_node)
          , occ(_occ)
          , points(_points)
          , pointseventstate(_pointseventstate)
          , next(_next)
          , maingreen(_maingreen)
          , mainyellow(_mainyellow)
          , mainred(_mainred)
          , divyellow(_divyellow)
          , divred(_divred)
          , config(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    openlcb::Polling *polling()
    {
        return this;
    }
    virtual void poll_33hz(openlcb::WriteHelper *helper, Notifiable *done) {
        if (eval()) {
            SendEventReport(helper, done);
        } else {
            done->notify();
        }
    }
    virtual UpdateAction apply_configuration(int fd, 
                                             bool initial_load,
                                             BarrierNotifiable *done) override
    {
        AutoNotify n(done);
        openlcb::EventId cfg_event_stop = config.stop_event().read(fd);
        openlcb::EventId cfg_event_approach_limited = config.approach_limited_event().read(fd);
        openlcb::EventId cfg_event_approach = config.approach_event().read(fd);
        openlcb::EventId cfg_event_clear = config.clear_event().read(fd);
        if (cfg_event_stop != event_stop ||
            cfg_event_approach_limited != event_approach_limited ||
            cfg_event_approach != event_approach ||
            cfg_event_clear != event_clear) {
            if (!initial_load) unregister_handler();
            event_stop = cfg_event_stop;
            event_approach_limited = cfg_event_approach_limited;
            event_approach = cfg_event_approach;
            event_clear = cfg_event_clear;
            register_handler();
            return REINIT_NEEDED; // Causes events identify.
        }
        return UPDATED;
    }
    virtual void factory_reset(int fd);
    bool eval();
    void handle_identify_global(const EventRegistryEntry &registry_entry, 
                                EventReport *event, 
                                BarrierNotifiable *done) OVERRIDE;
    void handle_identify_producer(const EventRegistryEntry &registry_entry,
                                  EventReport *event, 
                                  BarrierNotifiable *done) override;
private:
    openlcb::Node *node;
    OccupancyDetector *occ;
    Points *points;
    openlcb::EventState pointseventstate;
    NoProducerOccDetector *next;
    const Gpio *maingreen;
    const Gpio *mainyellow;
    const Gpio *mainred;
    const Gpio *divyellow;
    const Gpio *divred;
    enum {stop, approach_limited, approach, clear} aspect;
    const MastPointsConfiguration config;
    openlcb::EventId event_stop, event_approach_limited, event_approach, event_clear;
    void SendEventReport(openlcb::WriteHelper *writer, Notifiable *done);
    void SendAllProducersIdentified(EventReport *event,BarrierNotifiable *done);
    void SendProducerIdentified(EventReport *event,BarrierNotifiable *done);
    void register_handler();
    void unregister_handler();
};

class MastFrog : public openlcb::Polling , public ConfigUpdateListener, public openlcb::SimpleEventHandler {
public:
    MastFrog(openlcb::Node *_node, const MastFrogConfiguration &cfg, 
             OccupancyDetector *_occ,  Points *_points, 
             openlcb::EventState _pointseventstate,
             NoProducerOccDetector *_next,
             const Gpio *_green, const Gpio *_yellow, 
             const Gpio *_red) 
                : node(_node)
          , occ(_occ)
          , points(_points)
          , pointseventstate(_pointseventstate)
          , next(_next)
          , green(_green)
          , yellow(_yellow)
          , red(_red)
          , config(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    bool eval();
    openlcb::Polling *polling()
    {
        return this;
    }
    virtual void poll_33hz(openlcb::WriteHelper *helper, Notifiable *done) {
        if (eval()) {
            SendEventReport(helper, done);
        } else {
            done->notify();
        }
    }
    virtual UpdateAction apply_configuration(int fd, bool initial_load,
                                             BarrierNotifiable *done) override
    {
        AutoNotify n(done);
        openlcb::EventId cfg_event_stop = config.stop_event().read(fd);
        openlcb::EventId cfg_event_approach = config.approach_event().read(fd);
        openlcb::EventId cfg_event_clear = config.clear_event().read(fd);
        if (cfg_event_stop != event_stop ||
            cfg_event_approach != event_approach ||
            cfg_event_clear != event_clear) {
            if (!initial_load) unregister_handler();
            event_stop = cfg_event_stop;
            event_approach = cfg_event_approach;
            event_clear = cfg_event_clear;
            register_handler();
            return REINIT_NEEDED; // Causes events identify.
        }
        return UPDATED;
    }
    virtual void factory_reset(int fd);
    void handle_identify_global(const EventRegistryEntry &registry_entry, 
                                EventReport *event, 
                                BarrierNotifiable *done) OVERRIDE;
    void handle_identify_producer(const EventRegistryEntry &registry_entry,
                                  EventReport *event, 
                                  BarrierNotifiable *done) override;
private:
    openlcb::Node *node;
    OccupancyDetector *occ;
    Points *points;
    openlcb::EventState pointseventstate;
    NoProducerOccDetector *next;
    const Gpio *green;
    const Gpio *yellow;
    const Gpio *red;
    enum {stop, approach, clear} aspect;
    openlcb::EventId event_stop, event_approach, event_clear;
    const MastFrogConfiguration config;
    void SendEventReport(openlcb::WriteHelper *writer, Notifiable *done);
    void SendAllProducersIdentified(EventReport *event,BarrierNotifiable *done);
    void SendProducerIdentified(EventReport *event,BarrierNotifiable *done);
    void register_handler();
    void unregister_handler();
};

#endif // __MAST_HXX

