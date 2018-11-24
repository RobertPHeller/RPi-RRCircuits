// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri Jun 15 10:44:08 2018
//  Last Modified : <181124.1335>
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

static const char rcsid[] = "@(#) : $Id$";

#include "os/OS.hxx"
#include <stdio.h>
#include "ABSSlaveBus.hxx"
#include "openlcb/EventHandler.hxx"
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#ifdef LOGLEVEL
#undef LOGLEVEL
#define LOGLEVEL VERBOSE
#endif



ConfigUpdateListener::UpdateAction ABSSlaveNode::apply_configuration(int fd, 
                                               bool initial_load,
                                               BarrierNotifiable *done)
{
    OSMutexLock ConfigureUpdateLock(&mutex_);
    AutoNotify n(done);
    LOG(INFO,"ABSSlaveNode::apply_configuration(%d,%d,%p) called",fd,initial_load,done);
    nodeid = config.nodeid().read(fd);
    bool was_enabled = enabled;
    enabled = config.enabled().read(fd);
    //enabled = false;
    if (nodeid < 0 || nodeid > 63) enabled = false;
    LOG(INFO,"%d, %d ",nodeid,enabled);
    if (enabled) {
        LOG(INFO,"ABSSlaveNode::apply_configuration(): enabled: nodeid = %d",nodeid);
    }
    openlcb::EventId cfg_occupied_event = config.occupied_event().read(fd);
    openlcb::EventId cfg_unoccupied_event = config.unoccupied_event().read(fd);
    openlcb::EventId cfg_east_stop_event = config.east_stop_event().read(fd);
    openlcb::EventId cfg_east_approach_event = config.east_approach_event().read(fd);
    openlcb::EventId cfg_east_clear_event = config.east_clear_event().read(fd);
    openlcb::EventId cfg_west_stop_event = config.west_stop_event().read(fd);
    openlcb::EventId cfg_west_approach_event = config.west_approach_event().read(fd);
    openlcb::EventId cfg_west_clear_event = config.west_clear_event().read(fd);
    if (cfg_occupied_event != occupied_event ||
        cfg_unoccupied_event != unoccupied_event ||
        cfg_east_stop_event != east_stop_event ||
        cfg_east_approach_event != east_approach_event ||
        cfg_east_clear_event != east_clear_event ||
        cfg_west_stop_event != west_stop_event ||
        cfg_west_approach_event != west_approach_event ||
        cfg_west_clear_event != west_clear_event ||
        was_enabled != enabled) {
        if (!initial_load || was_enabled) unregister_handler();
        occupied_event = cfg_occupied_event;
        unoccupied_event = cfg_unoccupied_event;
        east_stop_event = cfg_east_stop_event;
        east_approach_event = cfg_east_approach_event;
        east_clear_event = cfg_east_clear_event;
        west_stop_event = cfg_west_stop_event;
        west_approach_event = cfg_west_approach_event;
        west_clear_event = cfg_west_clear_event;
        if (enabled) {
            LOG(INFO,"ABSSlaveNode::apply_configuration(): registering event handler");
            register_handler();
        }
        return REINIT_NEEDED; // Causes events identify.
    }
    return UPDATED;
}

void ABSSlaveNode::UpdateState(const char *message, 
                               Notifiable *done)
{
    int id;
    char o,e,w;
    AutoNotify n(done);
    LOG(INFO,"ABSSlaveNode::UpdateState(%s,%p)",message,done);
    if (sscanf(message,":R%d%cE%cW%c;",&id,&o,&e,&w) != 4) return;
    if (id != nodeid) return;
    if (o != occ) {
        occ = o;
        if (occ == 'O' ) {
            if (occupied_event != 0LL) 
                write_helpers[0].WriteAsync(
                     node,
                     openlcb::Defs::MTI_EVENT_REPORT,
                     openlcb::WriteHelper::global(),
                     openlcb::eventid_to_buffer(occupied_event),
                     done);
        } else if (occ == 'C') {
            if (unoccupied_event != 0LL)
                write_helpers[0].WriteAsync(
                     node,
                     openlcb::Defs::MTI_EVENT_REPORT,
                     openlcb::WriteHelper::global(),
                     openlcb::eventid_to_buffer(unoccupied_event),
                     done);
        }
    }
    switch (e) {
    case 'S': 
        if (east_aspect != stop) {
            east_aspect = stop;
            if (east_stop_event != 0LL)
                write_helpers[1].WriteAsync(node,
                                           openlcb::Defs::MTI_EVENT_REPORT,
                                           openlcb::WriteHelper::global(),
                                           openlcb::eventid_to_buffer(east_stop_event),
                                           done);
        }
        break;
    case 'A':
        if (east_aspect != approach) {
            east_aspect = approach;
            if (east_approach_event != 0LL)
                write_helpers[1].WriteAsync(node,
                                           openlcb::Defs::MTI_EVENT_REPORT,
                                           openlcb::WriteHelper::global(),
                                           openlcb::eventid_to_buffer(east_approach_event),
                                           done);
        }
        break;
    case 'C':
        if (east_aspect != clear) {
            east_aspect = clear;
            if (east_clear_event != 0LL)
                write_helpers[1].WriteAsync(node,
                                           openlcb::Defs::MTI_EVENT_REPORT,
                                           openlcb::WriteHelper::global(),
                                           openlcb::eventid_to_buffer(east_clear_event),
                                           done);
        }
        break;
    }
    switch (w) {
    case 'S': 
        if (west_aspect != stop) {
            west_aspect = stop;
            if (west_stop_event != 0LL)
                write_helpers[2].WriteAsync(node,
                                           openlcb::Defs::MTI_EVENT_REPORT,
                                           openlcb::WriteHelper::global(),
                                           openlcb::eventid_to_buffer(west_stop_event),
                                           done);
        }
        break;
    case 'A':
        if (west_aspect != approach) {
            west_aspect = approach;
            if (west_approach_event != 0LL)
                write_helpers[2].WriteAsync(node,
                                           openlcb::Defs::MTI_EVENT_REPORT,
                                           openlcb::WriteHelper::global(),
                                           openlcb::eventid_to_buffer(west_approach_event),
                                           done);
        }
        break;
    case 'C':
        if (west_aspect != clear) {
            west_aspect = clear;
            if (west_clear_event != 0LL)
                write_helpers[2].WriteAsync(node,
                                           openlcb::Defs::MTI_EVENT_REPORT,
                                           openlcb::WriteHelper::global(),
                                           openlcb::eventid_to_buffer(west_clear_event),
                                           done);
        }
        break;
    }
}

ABSSlaveNode::ABSSlaveNode(openlcb::Node *_node,const ABSSlaveNodeConfiguration &_config) {
    LOG(INFO,"ABSSlaveNode::ABSSlaveNode(...) entered");
    node = NULL;
    enabled = false;
    nodeid = 255;
    occ    = 255;
    west_aspect = INVALID;
    east_aspect = INVALID;
    config = _config;
    node = _node;
    ConfigUpdateService::instance()->register_update_listener(this);
}

ABSSlaveBus::ABSSlaveBus(openlcb::Node *n,const ABSSlaveList &_slaves) 
      : slaveconfiglist(_slaves) 
{
    node = n;
    LOG(INFO,"ABSSlaveBus::ABSSlaveBus(...) entered");
    for (unsigned i = 0; i < slaveconfiglist.num_repeats(); ++i) {
        LOG(INFO,"Allocating and constructing slave %d",i);
        slaves[i] = new ABSSlaveNode(n, slaveconfiglist.entry(i));
    }
    slaveIndex = MAXSLAVES;
}

void ABSSlaveBus::begin(const char *serialport) {
    LOG(INFO,"ABSSlaveBus::begin(\"%s\") entered",serialport);
    fd = ::open(serialport, O_RDWR);
    LOG(INFO,"ABSSlaveBus::begin(): port opened, fd = %d",fd);
}

bool ABSSlaveNode::Process(int fd, Notifiable *done)
{
    fd_set readfds, writefds, exceptfds;
    struct timeval timeout;
    
    OSMutexLock PollingLock(&mutex_);           
    if (enabled) {
        char buffer[64];
        int bytes = snprintf(buffer,64,":G%d;\r\n",nodeid);
        ::write(fd,buffer,bytes);
        usleep(1000); // 1ms
        FD_ZERO(&writefds);
        FD_ZERO(&exceptfds);
        FD_ZERO(&readfds);
        FD_SET(fd,&readfds);
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000; // 10ms timeout
        int status = select(fd+1,&readfds,&writefds,&exceptfds,
                            &timeout);
        if (status > 0) {
            bytes = ::read(fd,buffer,64);
            buffer[bytes] = '\0';
            UpdateState(buffer,done);
            return true;
        } else if (status == 0) {
            return false; // Timeout -- skip this node.
        } else {
            // error?
            return false;
        }
    }
    return false;
}

void ABSSlaveNode::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                      EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node)
    {
        done->notify();
    }
    SendAllProducersIdentified(done);
    done->maybe_done();
}

void ABSSlaveNode::SendAllProducersIdentified(BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_o = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID,
          mti_c = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID,
          mti_ws = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID, 
          mti_wa = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID, 
          mti_wc = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID,
          mti_es = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID, 
          mti_ea = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID, 
          mti_ec = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    if (occ == 'O') {
        mti_o = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else if (occ == 'C') {
        mti_c = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }
    switch (west_aspect) {
    case stop:
        mti_ws = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    case approach:
        mti_wa = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    case clear:
        mti_ws = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    default:
        break;
    }
    switch (east_aspect) {
    case stop:
        mti_es = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    case approach:
        mti_ea = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    case clear:
        mti_es = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    default:
        break;
    }
    if (occupied_event != 0LL)
        write_helpers[0].WriteAsync(node, mti_o,  openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(occupied_event),
                                            done->new_child());
    if (unoccupied_event != 0LL)
        write_helpers[1].WriteAsync(node, mti_c,  openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(unoccupied_event),
                                            done->new_child());
    if (west_stop_event != 0LL)
        write_helpers[2].WriteAsync(node, mti_ws, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(west_stop_event),
                                            done->new_child());
    if (west_approach_event != 0LL)
        write_helpers[3].WriteAsync(node, mti_wa, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(west_approach_event),
                                            done->new_child());
    if (west_clear_event != 0LL)
        write_helpers[4].WriteAsync(node, mti_wc, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(west_clear_event),
                                   done->new_child());
    if (east_stop_event != 0LL)
        write_helpers[5].WriteAsync(node, mti_es, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(east_stop_event),
                                   done->new_child());
    if (east_approach_event != 0LL)
        write_helpers[6].WriteAsync(node, mti_ea, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(east_approach_event),
                                   done->new_child());
    if (east_clear_event != 0LL)
        write_helpers[7].WriteAsync(node, mti_ec, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(east_clear_event),
                                   done->new_child());
}

void ABSSlaveNode::handle_identify_producer(const EventRegistryEntry &registry_entry,
                                            EventReport *event, 
                                            BarrierNotifiable *done)
{
    SendProducerIdentified(event,done);
    done->maybe_done();
}

void ABSSlaveNode::SendProducerIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
    if (event->event == occupied_event) {
        if (occ == 'O') {
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        } else if (occ == 'C') {
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
        }
    } else if (event->event == unoccupied_event) {
        if (occ == 'O') {
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
        } else if (occ == 'C') {
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        }
    } else if (event->event == west_stop_event) {
        switch (west_aspect) {
        case stop:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
            break;
        case approach:
        case clear:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
            break;
        case INVALID:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
        }
    } else if (event->event == west_approach_event) {
        switch (west_aspect) {
        case approach:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
            break;
        case stop:
        case clear:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
            break;
        case INVALID:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
        }
    } else if (event->event == west_clear_event) {
        switch (west_aspect) {
        case clear:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
            break;
        case approach:
        case stop:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
            break;
        case INVALID:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
        }
    } else if (event->event == east_stop_event) {
        switch (east_aspect) {
        case stop:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
            break;
        case approach:
        case clear:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
            break;
        case INVALID:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
        }
    } else if (event->event == east_approach_event) {
        switch (east_aspect) {
        case approach:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
            break;
        case stop:
        case clear:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
            break;
        case INVALID:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
        }
    } else if (event->event == east_clear_event) {
        switch (east_aspect) {
        case clear:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
            break;
        case stop:
        case approach:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
            break;
        case INVALID:
            mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
        }
    } else {
    }
    event->event_write_helper<1>()->WriteAsync(node, mti,  openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(event->event),
                                            done->new_child());
}

void ABSSlaveNode::unregister_handler()
{
    //LOG(INFO,"ABSSlaveNode::unregister_handler(): registeredCount = %d",registeredCount);
    openlcb::EventRegistry::instance()->unregister_handler(this);
}

void ABSSlaveNode::register_handler()
{
    //LOG(INFO,"ABSSlaveNode::register_handler(): [enter] registeredCount = %d",registeredCount);
    if (occupied_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, occupied_event), 0);
    }
    if (unoccupied_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, unoccupied_event), 0);
    }
    if (west_stop_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, west_stop_event), 0);
    }
    if (west_approach_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, west_approach_event), 0);
    }
    if (west_clear_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, west_clear_event), 0);
    }
    if (east_stop_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, east_stop_event), 0);
    }
    if (east_approach_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, east_approach_event), 0);
    }
    if (east_clear_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, east_clear_event), 0);
    }
    //LOG(INFO,"ABSSlaveNode::register_handler(): [exit] registeredCount = %d",registeredCount);
}

void ABSSlaveNode::factory_reset(int fd) {
    LOG(INFO,"ABSSlaveNode::factory_reset(%d)",fd);
    CDI_FACTORY_RESET(config.enabled);
    config.description().write(fd,"");
    CDI_FACTORY_RESET(config.nodeid);
}


void *ABSSlaveBus::entry()
{
    while (true) {
        int islave;
        for (islave = 0; islave < MAXSLAVES; islave++) {
            if (slaveIndex >= MAXSLAVES) {
                slaveIndex = 0;
            }
            if (slaves[slaveIndex++]->Process(fd,this)) {
                LOG(INFO,"ABSSlaveBus::entry(): slaveIndex = %d processed",slaveIndex);
                break;
            }
        }
        usleep(30000); // 30ms
    }
    return NULL;
}
