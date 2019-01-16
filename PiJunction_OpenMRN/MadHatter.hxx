// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Oct 15 20:58:22 2018
//  Last Modified : <181124.1417>
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

#ifndef __MADHATTER_HXX
#define __MADHATTER_HXX

#include "os/Gpio.hxx"
#include "openlcb/MemoryConfig.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"

#include "OccDetector.hxx"

CDI_GROUP(MadHatterConfiguration);
/// This event will be produced when the light is turned on.
CDI_GROUP_ENTRY(
    event_on, openlcb::EventConfigEntry, //
    Name("Event On"),
    Description("This event will be produced when the light is turned on."));
/// This event will be produced when the light is turned off.
CDI_GROUP_ENTRY(
    event_off, openlcb::EventConfigEntry, //
    Name("Event Off"),
    Description("This event will be produced when the light is turned off."));
CDI_GROUP_END();

class MadHatter : public openlcb::Polling , public ConfigUpdateListener, public openlcb::SimpleEventHandler {
public:
    MadHatter(openlcb::Node *_node, const MadHatterConfiguration  &cfg,
              OccupancyDetector *_siding, OccupancyDetector *_s31459,
              const Gpio *_lightGpio)
                : node(_node)
          , siding(_siding)
          , s31459(_s31459)
          , lightGpio(_lightGpio)
          , if_on(false)
          , then_on(false)
          , if_off(false)
          , then_off(false)
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
        openlcb::EventId cfg_event_on = config.event_on().read(fd);
        openlcb::EventId cfg_event_off = config.event_off().read(fd);
        if (cfg_event_on != event_on ||
            cfg_event_off != event_off) {
            if (!initial_load) unregister_handler();
            event_on = cfg_event_off;
            event_off = cfg_event_off;
            register_handler();
            return REINIT_NEEDED; // Causes events identify.
        }
        return UPDATED;
    }
    virtual void factory_reset(int fd) {}
    bool eval();
    void handle_identify_global(const EventRegistryEntry &registry_entry, 
                                EventReport *event, 
                                BarrierNotifiable *done) OVERRIDE;
    void handle_identify_producer(const EventRegistryEntry &registry_entry,
                                  EventReport *event, 
                                  BarrierNotifiable *done) override;
private:
    openlcb::Node *node;
    OccupancyDetector *siding, *s31459;
    const Gpio *lightGpio;
    bool if_on, then_on;
    bool if_off, then_off;
    const MadHatterConfiguration config;
    openlcb::EventId event_on, event_off;
    void SendEventReport(openlcb::WriteHelper *writer, Notifiable *done);
    void SendAllProducersIdentified(EventReport *event,BarrierNotifiable *done);
    void SendProducerIdentified(EventReport *event,BarrierNotifiable *done);
    void register_handler();
    void unregister_handler();
};

          


#endif // __MADHATTER_HXX

