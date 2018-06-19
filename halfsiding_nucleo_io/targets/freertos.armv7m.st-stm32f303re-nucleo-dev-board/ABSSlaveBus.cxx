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
//  Last Modified : <180619.0811>
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


ConfigUpdateListener::UpdateAction ABSSlaveNode::apply_configuration(int fd, 
                                               bool initial_load,
                                               BarrierNotifiable *done)
{
    OSMutexLock ConfigureUpdateLock(&mutex_);
    AutoNotify n(done);
    enabled = config->enabled().read(fd);
    nodeid = config->nodeid().read(fd);
    openlcb::EventId cfg_occupied_event = config->occupied_event().read(fd);
    openlcb::EventId cfg_unoccupied_event = config->unoccupied_event().read(fd);

    openlcb::EventId cfg_east_stop_event = config->east_stop_event().read(fd);
    openlcb::EventId cfg_east_approach_event = config->east_approach_event().read(fd);
    openlcb::EventId cfg_east_clear_event = config->east_clear_event().read(fd);

    openlcb::EventId cfg_west_stop_event = config->west_stop_event().read(fd);
    openlcb::EventId cfg_west_approach_event = config->west_approach_event().read(fd);
    openlcb::EventId cfg_west_clear_event = config->west_clear_event().read(fd);
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
        register_handler();
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
    
    if (sscanf(message,":R%d%cE%cW%c;",&id,&o,&e,&w) != 4) return;
    if (id != nodeid) return;
    if (o != occ) {
        occ = o;
        if (occ == 'O' ) {
            openlcb::event_write_helper1.WriteAsync(node,
                                           openlcb::Defs::MTI_EVENT_REPORT,
                                           openlcb::WriteHelper::global(),
                                           openlcb::eventid_to_buffer(occupied_event),
                                           done);
        } else if (occ == 'C') {
            openlcb::event_write_helper1.WriteAsync(node,
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
    slaves[0].begin(n,slaveconfiglist.entry<0>());
    slaves[1].begin(n,slaveconfiglist.entry<1>());
    slaves[2].begin(n,slaveconfiglist.entry<2>());
    slaves[3].begin(n,slaveconfiglist.entry<3>());
    slaves[4].begin(n,slaveconfiglist.entry<4>());
    slaves[5].begin(n,slaveconfiglist.entry<5>());
    slaves[6].begin(n,slaveconfiglist.entry<6>());
    slaves[7].begin(n,slaveconfiglist.entry<7>());
    slaves[8].begin(n,slaveconfiglist.entry<8>());
    slaves[9].begin(n,slaveconfiglist.entry<9>());
    slaves[10].begin(n,slaveconfiglist.entry<10>());
    slaves[11].begin(n,slaveconfiglist.entry<11>());
    slaves[12].begin(n,slaveconfiglist.entry<12>());
    slaves[13].begin(n,slaveconfiglist.entry<13>());
    slaves[14].begin(n,slaveconfiglist.entry<14>());
    slaves[15].begin(n,slaveconfiglist.entry<15>());
    slaves[16].begin(n,slaveconfiglist.entry<16>());
    slaves[17].begin(n,slaveconfiglist.entry<17>());
    slaves[18].begin(n,slaveconfiglist.entry<18>());
    slaves[19].begin(n,slaveconfiglist.entry<19>());
    slaves[20].begin(n,slaveconfiglist.entry<20>());
    slaves[21].begin(n,slaveconfiglist.entry<21>());
    slaves[22].begin(n,slaveconfiglist.entry<22>());
    slaves[23].begin(n,slaveconfiglist.entry<23>());
    slaves[24].begin(n,slaveconfiglist.entry<24>());
    slaves[25].begin(n,slaveconfiglist.entry<25>());
    slaves[26].begin(n,slaveconfiglist.entry<26>());
    slaves[27].begin(n,slaveconfiglist.entry<27>());
    slaves[28].begin(n,slaveconfiglist.entry<28>());
    slaves[29].begin(n,slaveconfiglist.entry<29>());
    slaves[30].begin(n,slaveconfiglist.entry<30>());
    slaves[31].begin(n,slaveconfiglist.entry<31>());
    slaves[32].begin(n,slaveconfiglist.entry<32>());
    slaves[33].begin(n,slaveconfiglist.entry<33>());
    slaves[34].begin(n,slaveconfiglist.entry<34>());
    slaves[35].begin(n,slaveconfiglist.entry<35>());
    slaves[36].begin(n,slaveconfiglist.entry<36>());
    slaves[37].begin(n,slaveconfiglist.entry<37>());
    slaves[38].begin(n,slaveconfiglist.entry<38>());
    slaves[39].begin(n,slaveconfiglist.entry<39>());
    slaves[40].begin(n,slaveconfiglist.entry<40>());
    slaves[41].begin(n,slaveconfiglist.entry<41>());
    slaves[42].begin(n,slaveconfiglist.entry<42>());
    slaves[43].begin(n,slaveconfiglist.entry<43>());
    slaves[44].begin(n,slaveconfiglist.entry<44>());
    slaves[45].begin(n,slaveconfiglist.entry<45>());
    slaves[46].begin(n,slaveconfiglist.entry<46>());
    slaves[47].begin(n,slaveconfiglist.entry<47>());
    slaves[48].begin(n,slaveconfiglist.entry<48>());
    slaves[49].begin(n,slaveconfiglist.entry<49>());
    slaves[50].begin(n,slaveconfiglist.entry<50>());
    slaves[51].begin(n,slaveconfiglist.entry<51>());
    slaves[52].begin(n,slaveconfiglist.entry<52>());
    slaves[53].begin(n,slaveconfiglist.entry<53>());
    slaves[54].begin(n,slaveconfiglist.entry<54>());
    slaves[55].begin(n,slaveconfiglist.entry<55>());
    slaves[56].begin(n,slaveconfiglist.entry<56>());
    slaves[57].begin(n,slaveconfiglist.entry<57>());
    slaves[58].begin(n,slaveconfiglist.entry<58>());
    slaves[59].begin(n,slaveconfiglist.entry<59>());
    slaves[60].begin(n,slaveconfiglist.entry<60>());
    slaves[61].begin(n,slaveconfiglist.entry<61>());
    slaves[62].begin(n,slaveconfiglist.entry<62>());
    slaves[63].begin(n,slaveconfiglist.entry<63>());
    fd = ::open("/dev/ser1", O_RDWR);
    node = n;
    slaveIndex = MAXSLAVES;
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
    SendProducerIdentified(done);
    done->maybe_done();
}

void ABSSlaveNode::SendProducerIdentified(BarrierNotifiable *done)
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
    openlcb::event_write_helper1.WriteAsync(node, mti_o,  openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(occupied_event),
                                            done->new_child());
    openlcb::event_write_helper2.WriteAsync(node, mti_c,  openlcb::WriteHelper::global(),
                                            openlcb::eventid_to_buffer(unoccupied_event),
                                            done->new_child());
    openlcb::event_write_helper3.WriteAsync(node, mti_ws, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(west_stop_event),
                                   done->new_child());
    openlcb::event_write_helper4.WriteAsync(node, mti_wa, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(west_approach_event),
                                   done->new_child());
    /// Hmmm do I need to wait?  Or do something else?
    openlcb::event_write_helper1.WriteAsync(node, mti_wc, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(west_clear_event),
                                   done->new_child());
    openlcb::event_write_helper2.WriteAsync(node, mti_es, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(east_stop_event),
                                   done->new_child());
    openlcb::event_write_helper3.WriteAsync(node, mti_ea, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(east_approach_event),
                                   done->new_child());
    openlcb::event_write_helper4.WriteAsync(node, mti_ec, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(east_clear_event),
                                   done->new_child());
}

void ABSSlaveNode::unregister_handler()
{
}

void ABSSlaveNode::register_handler()
{
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
                break;
            }
        }
        usleep(30000); // 30ms
    }
    return NULL;
}
