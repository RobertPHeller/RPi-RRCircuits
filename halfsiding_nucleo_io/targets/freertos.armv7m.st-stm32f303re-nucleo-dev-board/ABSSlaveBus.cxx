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
//  Last Modified : <180731.0952>
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

static openlcb::WriteHelper event_write_helper5;
static openlcb::WriteHelper event_write_helper6;
static openlcb::WriteHelper event_write_helper7;
static openlcb::WriteHelper event_write_helper8;



ConfigUpdateListener::UpdateAction ABSSlaveNode::apply_configuration(int fd, 
                                               bool initial_load,
                                               BarrierNotifiable *done)
{
    OSMutexLock ConfigureUpdateLock(&mutex_);
    AutoNotify n(done);
    //LOG(INFO,"ABSSlaveNode::apply_configuration(%d,%d,%p) called\n",fd,initial_load,done);
    nodeid = config.nodeid().read(fd);
    enabled = config.enabled().read(fd);
    if (nodeid < 0 || nodeid > 63) enabled = false;
    //LOG(INFO,"%d, %d ",nodeid,enabled);
    if (enabled) {
        LOG(INFO,"ABSSlaveNode::apply_configuration(): enabled: nodeid = %d\n",nodeid);
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
        cfg_west_clear_event != west_clear_event) {
        unregister_handler();
        occupied_event = cfg_occupied_event;
        unoccupied_event = cfg_unoccupied_event;
        east_stop_event = cfg_east_stop_event;
        east_approach_event = cfg_east_approach_event;
        east_clear_event = cfg_east_clear_event;
        west_stop_event = cfg_west_stop_event;
        west_approach_event = cfg_west_approach_event;
        west_clear_event = cfg_west_clear_event;
        if (enabled) {
            LOG(INFO,"ABSSlaveNode::apply_configuration(): registering event handler\n");
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
    //LOG(INFO,"ABSSlaveNode::UpdateState(%s,%p)",message,done);
    if (sscanf(message,":R%d%cE%cW%c;",&id,&o,&e,&w) != 4) return;
    if (id != nodeid) return;
    if (o != occ) {
        occ = o;
        if (occ == 'O' ) {
            if (occupied_event != 0LL) 
                openlcb::event_write_helper1.WriteAsync(
                     node,
                     openlcb::Defs::MTI_EVENT_REPORT,
                     openlcb::WriteHelper::global(),
                     openlcb::eventid_to_buffer(occupied_event),
                     done);
        } else if (occ == 'C') {
            if (unoccupied_event != 0LL)
                openlcb::event_write_helper1.WriteAsync(
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
                openlcb::event_write_helper2.WriteAsync(node,
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
                openlcb::event_write_helper2.WriteAsync(node,
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
                openlcb::event_write_helper2.WriteAsync(node,
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
                openlcb::event_write_helper3.WriteAsync(node,
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
                openlcb::event_write_helper3.WriteAsync(node,
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
                openlcb::event_write_helper3.WriteAsync(node,
                                           openlcb::Defs::MTI_EVENT_REPORT,
                                           openlcb::WriteHelper::global(),
                                           openlcb::eventid_to_buffer(west_clear_event),
                                           done);
        }
        break;
    }
}



ABSSlaveBus::ABSSlaveBus(openlcb::Node *n,const ABSSlaveList &_slaves) : slaveconfiglist(_slaves) 
{
    node = n;
    slaveIndex = MAXSLAVES;
}

void ABSSlaveBus::begin(const char *serialport) {
    //LOG(INFO,"ABSSlaveBus::begin(\"%s\") entered\n",serialport);
    slaves[0].begin(node,slaveconfiglist.entry<0>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 0 begun\n");
    slaves[1].begin(node,slaveconfiglist.entry<1>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 1 begun\n");
    slaves[2].begin(node,slaveconfiglist.entry<2>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 2 begun\n");
    slaves[3].begin(node,slaveconfiglist.entry<3>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 3 begun\n");
    slaves[4].begin(node,slaveconfiglist.entry<4>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 4 begun\n");
    slaves[5].begin(node,slaveconfiglist.entry<5>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 5 begun\n");
    slaves[6].begin(node,slaveconfiglist.entry<6>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 6 begun\n");
    slaves[7].begin(node,slaveconfiglist.entry<7>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 7 begun\n");
    slaves[8].begin(node,slaveconfiglist.entry<8>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 8 begun\n");
    slaves[9].begin(node,slaveconfiglist.entry<9>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 9 begun\n");
    slaves[10].begin(node,slaveconfiglist.entry<10>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 10 begun\n");
    slaves[11].begin(node,slaveconfiglist.entry<11>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 11 begun\n");
    slaves[12].begin(node,slaveconfiglist.entry<12>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 12 begun\n");
    slaves[13].begin(node,slaveconfiglist.entry<13>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 13 begun\n");
    slaves[14].begin(node,slaveconfiglist.entry<14>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 14 begun\n");
    slaves[15].begin(node,slaveconfiglist.entry<15>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 15 begun\n");
    slaves[16].begin(node,slaveconfiglist.entry<16>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 16 begun\n");
    slaves[17].begin(node,slaveconfiglist.entry<17>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 17 begun\n");
    slaves[18].begin(node,slaveconfiglist.entry<18>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 18 begun\n");
    slaves[19].begin(node,slaveconfiglist.entry<19>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 19 begun\n");
    slaves[20].begin(node,slaveconfiglist.entry<20>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 20 begun\n");
    slaves[21].begin(node,slaveconfiglist.entry<21>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 21 begun\n");
    slaves[22].begin(node,slaveconfiglist.entry<22>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 22 begun\n");
    slaves[23].begin(node,slaveconfiglist.entry<23>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 23 begun\n");
    slaves[24].begin(node,slaveconfiglist.entry<24>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 24 begun\n");
    slaves[25].begin(node,slaveconfiglist.entry<25>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 25 begun\n");
    slaves[26].begin(node,slaveconfiglist.entry<26>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 26 begun\n");
    slaves[27].begin(node,slaveconfiglist.entry<27>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 27 begun\n");
    slaves[28].begin(node,slaveconfiglist.entry<28>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 28 begun\n");
    slaves[29].begin(node,slaveconfiglist.entry<29>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 29 begun\n");
    slaves[30].begin(node,slaveconfiglist.entry<30>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 30 begun\n");
    slaves[31].begin(node,slaveconfiglist.entry<31>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 31 begun\n");
    slaves[32].begin(node,slaveconfiglist.entry<32>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 32 begun\n");
    slaves[33].begin(node,slaveconfiglist.entry<33>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 33 begun\n");
    slaves[34].begin(node,slaveconfiglist.entry<34>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 34 begun\n");
    slaves[35].begin(node,slaveconfiglist.entry<35>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 35 begun\n");
    slaves[36].begin(node,slaveconfiglist.entry<36>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 36 begun\n");
    slaves[37].begin(node,slaveconfiglist.entry<37>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 37 begun\n");
    slaves[38].begin(node,slaveconfiglist.entry<38>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 38 begun\n");
    slaves[39].begin(node,slaveconfiglist.entry<39>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 39 begun\n");
    slaves[40].begin(node,slaveconfiglist.entry<40>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 40 begun\n");
    slaves[41].begin(node,slaveconfiglist.entry<41>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 41 begun\n");
    slaves[42].begin(node,slaveconfiglist.entry<42>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 42 begun\n");
    slaves[43].begin(node,slaveconfiglist.entry<43>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 43 begun\n");
    slaves[44].begin(node,slaveconfiglist.entry<44>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 44 begun\n");
    slaves[45].begin(node,slaveconfiglist.entry<45>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 45 begun\n");
    slaves[46].begin(node,slaveconfiglist.entry<46>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 46 begun\n");
    slaves[47].begin(node,slaveconfiglist.entry<47>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 47 begun\n");
    slaves[48].begin(node,slaveconfiglist.entry<48>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 48 begun\n");
    slaves[49].begin(node,slaveconfiglist.entry<49>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 49 begun\n");
    slaves[50].begin(node,slaveconfiglist.entry<50>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 50 begun\n");
    slaves[51].begin(node,slaveconfiglist.entry<51>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 51 begun\n");
    slaves[52].begin(node,slaveconfiglist.entry<52>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 52 begun\n");
    slaves[53].begin(node,slaveconfiglist.entry<53>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 53 begun\n");
    slaves[54].begin(node,slaveconfiglist.entry<54>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 54 begun\n");
    slaves[55].begin(node,slaveconfiglist.entry<55>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 55 begun\n");
    slaves[56].begin(node,slaveconfiglist.entry<56>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 56 begun\n");
    slaves[57].begin(node,slaveconfiglist.entry<57>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 57 begun\n");
    slaves[58].begin(node,slaveconfiglist.entry<58>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 58 begun\n");
    slaves[59].begin(node,slaveconfiglist.entry<59>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 59 begun\n");
    slaves[60].begin(node,slaveconfiglist.entry<60>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 60 begun\n");
    slaves[61].begin(node,slaveconfiglist.entry<61>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 61 begun\n");
    slaves[62].begin(node,slaveconfiglist.entry<62>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 62 begun\n");
    slaves[63].begin(node,slaveconfiglist.entry<63>());
    //LOG(INFO,"ABSSlaveBus::begin(): slave 63 begun\n");
    fd = ::open(serialport, O_RDWR);
    //LOG(INFO,"ABSSlaveBus::begin(): port opened, fd = %d\n",fd);
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
        openlcb::event_write_helper1.WriteAsync(node, mti_o,  openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(occupied_event),
                                            done->new_child());
    if (unoccupied_event != 0LL)
        openlcb::event_write_helper2.WriteAsync(node, mti_c,  openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(unoccupied_event),
                                            done->new_child());
    if (west_stop_event != 0LL)
        openlcb::event_write_helper3.WriteAsync(node, mti_ws, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(west_stop_event),
                                            done->new_child());
    if (west_approach_event != 0LL)
        openlcb::event_write_helper4.WriteAsync(node, mti_wa, openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(west_approach_event),
                                            done->new_child());
    if (west_clear_event != 0LL)
        event_write_helper5.WriteAsync(node, mti_wc, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(west_clear_event),
                                   done->new_child());
    if (east_stop_event != 0LL)
        event_write_helper6.WriteAsync(node, mti_es, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(east_stop_event),
                                   done->new_child());
    if (east_approach_event != 0LL)
        event_write_helper7.WriteAsync(node, mti_ea, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(east_approach_event),
                                   done->new_child());
    if (east_clear_event != 0LL)
        event_write_helper8.WriteAsync(node, mti_ec, openlcb::WriteHelper::global(),
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
    openlcb::event_write_helper1.WriteAsync(node, mti,  openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(event->event),
                                            done->new_child());
}

void ABSSlaveNode::unregister_handler()
{
    //LOG(INFO,"ABSSlaveNode::unregister_handler(): registeredCount = %d\n",registeredCount);
    if (registeredCount > 0) {
        openlcb::EventRegistry::instance()->unregister_handler(this);
    }
}

void ABSSlaveNode::register_handler()
{
    LOG(INFO,"ABSSlaveNode::register_handler(): [enter] registeredCount = %d\n",registeredCount);
    if (occupied_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, occupied_event), 0);
        registeredCount++;
    }
    if (unoccupied_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, unoccupied_event), 0);
        registeredCount++;
    }
    if (west_stop_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, west_stop_event), 0);
        registeredCount++;
    }
    if (west_approach_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, west_approach_event), 0);
        registeredCount++;
    }
    if (west_clear_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, west_clear_event), 0);
        registeredCount++;
    }
    if (east_stop_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, east_stop_event), 0);
        registeredCount++;
    }
    if (east_approach_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, east_approach_event), 0);
        registeredCount++;
    }
    if (east_clear_event != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, east_clear_event), 0);
        registeredCount++;
    }
    LOG(INFO,"ABSSlaveNode::register_handler(): [exit] registeredCount = %d\n",registeredCount);
}



void *ABSSlaveBus::entry()
{
    while (true) {
        int islave;
        for (islave = 0; islave < MAXSLAVES; islave++) {
            if (slaveIndex >= MAXSLAVES) {
                slaveIndex = 0;
            }
            if (slaves[slaveIndex++].Process(fd,this)) {
                LOG(INFO,"ABSSlaveBus::entry(): slaveIndex = %d processed\n",slaveIndex);
                break;
            }
        }
        usleep(30000); // 30ms
    }
    return NULL;
}
