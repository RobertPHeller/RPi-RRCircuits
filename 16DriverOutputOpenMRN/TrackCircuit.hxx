// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Feb 27 14:11:23 2019
//  Last Modified : <190228.2036>
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

#ifndef __TRACKCIRCUIT_HXX
#define __TRACKCIRCUIT_HXX

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"
#include <stdio.h>

#include <vector>


static const char TrackSpeedMap[] = 
"<relation><property>0</property><value>Stop</value></relation>"
"<relation><property>1</property><value>Restricting/Tumble Down</value></relation>"
"<relation><property>2</property><value>Slow</value></relation>"
"<relation><property>3</property><value>Medium</value></relation>"
"<relation><property>4</property><value>Limited</value></relation>"
"<relation><property>5</property><value>Approach</value></relation>"
"<relation><property>6</property><value>Approach-Medium</value></relation>"
"<relation><property>7</property><value>Clear/Procede</value></relation>";

#define TRACKCIRCUITCOUNT 8

/// CDI Configuration for a @ref TrackCircuit
CDI_GROUP(TrackCircuitConfig);
CDI_GROUP_ENTRY(description,openlcb::StringConfigEntry<16>,
                Name("Remote Mast Description"));
CDI_GROUP_ENTRY(remotemastlink,openlcb::EventConfigEntry,
                Name("(C) Remote Mast Link Address."
                     "Copy from 'Next' Mast and Paste here."));
CDI_GROUP_END();

class TrackCircuit;

class TrackCircuitCallback {
public:
    virtual void trigger(const TrackCircuit *caller) = 0;
};

class TrackCircuit : public ConfigUpdateListener, public openlcb::SimpleEventHandler {
public:
    enum TrackSpeed {Stop_,Restricting_,Slow_,Medium_,Limited_,
              Approach_,ApproachMedium_,Clear_,Unknown_};
    TrackCircuit(openlcb::Node *n,const TrackCircuitConfig &cfg)
                : node_(n), cfg_(cfg)
    {
        speed_ = Unknown_;
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    TrackSpeed CurrentSpeed() const {return speed_;}
    void RegisterCallback(TrackCircuitCallback * callback) {
        callbacks_.push_back(callback);
    }
    void UnregisterCallback(TrackCircuitCallback * callback) {
        for (callback_type_iterator i = callbacks_.begin();
             i != callbacks_.end(); i++) {
            if (*i == callback) {
                callbacks_.erase(i);
                return;
            }
        }
    }
    virtual UpdateAction apply_configuration(int fd, 
                                             bool initial_load,
                                             BarrierNotifiable *done) override;
    virtual void factory_reset(int fd);
    void handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                EventReport *event, BarrierNotifiable *done) override;
    void handle_event_report(const EventRegistryEntry &entry, 
                             EventReport *event,
                             BarrierNotifiable *done) override;
    void handle_identify_consumer(const EventRegistryEntry &registry_entry,
                                  EventReport *event,
                                  BarrierNotifiable *done) override;
private:
    openlcb::Node *node_;
    const TrackCircuitConfig cfg_;
    openlcb::EventId remotemastlink_;
    TrackSpeed speed_;
    typedef vector<TrackCircuitCallback *> callback_type;
    typedef callback_type::iterator callback_type_iterator;
    callback_type callbacks_;
    openlcb::WriteHelper write_helpers[8];
    void register_handler();
    void unregister_handler();
    void SendAllConsumersIdentified(EventReport *event,BarrierNotifiable *done);
    void SendConsumerIdentified(EventReport *event,BarrierNotifiable *done);
};

extern TrackCircuit *circuits[8];




#endif // __TRACKCIRCUIT_HXX

