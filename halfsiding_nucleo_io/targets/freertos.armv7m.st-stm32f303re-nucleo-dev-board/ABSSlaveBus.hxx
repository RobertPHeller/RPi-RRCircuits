// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Jun 14 21:40:10 2018
//  Last Modified : <180616.2311>
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

#ifndef __ABSSLAVEBUS_HXX
#define __ABSSLAVEBUS_HXX

#include "os/OS.hxx"
#include "openlcb/MemoryConfig.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"

CDI_GROUP(ABSSlaveNodeConfiguration);
CDI_GROUP_ENTRY(enabled, openlcb::Uint8ConfigEntry, //
                Name("Node Enabled"), MapValues("<relation><property>1</property><value>Yes</value></relation><relation><property>0</property><value>No</value></relation>"));
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<15>, //
                Name("Description"), Description("User name of this node"));
CDI_GROUP_ENTRY(nodeid, openlcb::Uint8ConfigEntry, //
                Name("Node ID"), Min(0), Max(63));
CDI_GROUP_ENTRY(occupied_event, openlcb::EventConfigEntry, //
                Name("Occupied Event"),
                Description("This event will be produced when the block the node covers becomes occupied."));
CDI_GROUP_ENTRY(unoccupied_event, openlcb::EventConfigEntry, //
                Name("Unoccupied Event"),
                Description("This event will be produced when the block the node covers becomes unoccupied."));
CDI_GROUP_ENTRY(east_stop_event, openlcb::EventConfigEntry, //
                Name("East Stop Aspect"),
                Description("This event will be produced when the eastbound signal displays a Stop Aspect."));
CDI_GROUP_ENTRY(east_approach_event, openlcb::EventConfigEntry, //
                Name("East Approach Aspect"),
                Description("This event will be produced when the eastbound signal displays a Approach Aspect."));
CDI_GROUP_ENTRY(east_clear_event, openlcb::EventConfigEntry, //
                Name("East Clear Aspect"),
                Description("This event will be produced when the eastbound signal displays a Clear Aspect."));
CDI_GROUP_ENTRY(west_stop_event, openlcb::EventConfigEntry, //
                Name("West Stop Aspect"),
                Description("This event will be produced when the westbound signal displays a Stop Aspect."));
CDI_GROUP_ENTRY(west_approach_event, openlcb::EventConfigEntry, //
                Name("West Approach Aspect"),
                Description("This event will be produced when the westbound signal displays a Approach Aspect."));
CDI_GROUP_ENTRY(west_clear_event, openlcb::EventConfigEntry, //
                Name("West Clear Aspect"),
                Description("This event will be produced when the westbound signal displays a Clear Aspect."));
CDI_GROUP_END();

#define MAXSLAVES 64

using ABSSlaveList = openlcb::RepeatedGroup<ABSSlaveNodeConfiguration, MAXSLAVES>;

class ABSSlaveNode : public ConfigUpdateListener {
public:
    ABSSlaveNode() {
        enabled = false;
        nodeid = 255;
        occ    = 255;
        west_aspect = INVALID;
        east_aspect = INVALID;
    }
    virtual UpdateAction apply_configuration(int fd, 
                                             bool initial_load,
                                             BarrierNotifiable *done) override;
    virtual void factory_reset(int fd) {
        enabled = false;
    }
    void begin(const ABSSlaveNodeConfiguration &_config) {
        config = &_config;
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    uint8_t NodeID() const {return nodeid;}
    bool Enabled() const {return enabled;}
    void UpdateState(openlcb::Node *node,
                     //openlcb::WriteHelper *writer,
                     //Notifiable *done,
                     const char *message);
private:
    uint8_t nodeid;
    uint8_t occ;
    enum {stop, approach, clear, INVALID=255} west_aspect, east_aspect;
    openlcb::EventId occupied_event, unoccupied_event, 
          west_stop_event, west_approach_event, west_clear_event,
          east_stop_event, east_approach_event, east_clear_event;
    const ABSSlaveNodeConfiguration *config;
    bool enabled;
};

class ABSSlaveBus : public OSThread {
public:
    ABSSlaveBus(openlcb::Node *n,const ABSSlaveList &_slaves);
    //openlcb::Polling *polling() { return this; }
    //virtual void poll_33hz(openlcb::WriteHelper *writer, Notifiable *done);
protected:
    virtual void *entry();
private:
    int fd;
    openlcb::Node *node;
    const ABSSlaveList slaveconfiglist;
    ABSSlaveNode slaves[MAXSLAVES];
    uint8_t slaveIndex;
};
    
    

#endif // __ABSSLAVEBUS_HXX

