// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Oct 23 17:16:59 2019
//  Last Modified : <191023.1728>
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

static const char rcsid[] = "@(#) : $Id$";

#include "dcc/Packet.hxx"
#include "dcc/PacketSource.hxx"
#include "DuplexUpdateLoop.hxx"

DuplexUpdateLoop::DuplexUpdateLoop(Service *service, 
                                   dcc::PacketFlowInterface *mainTrack_send, 
                                   dcc::PacketFlowInterface *progTrack_send)
      : StateFlow(service)
, mainTrackSend_(mainTrack_send)
, progTrackSend_(progTrack_send)
, nextRefreshIndexMain_(0)
, nextRefreshIndexProg_(0)
, lastCycleStartMain_(os_get_time_monotonic())
, lastCycleStartProg_(os_get_time_monotonic())
{
}

DuplexUpdateLoop::~DuplexUpdateLoop()
{
}

StateFlowBase::Action DuplexUpdateLoop::entry()
{
    long long current_time = os_get_time_monotonic();
    long long prev_cycle_start = lastCycleStartMain_;
    if (nextRefreshIndexMain_ >= refreshSourcesMain_.size())
    {
        nextRefreshIndexMain_ = 0;
        lastCycleStartMain_ = current_time;
    }
    if (nextRefreshIndexMain_ == 0 &&
        (current_time - prev_cycle_start < MSEC_TO_NSEC(5) ||
         refreshSourcesMain_.empty()))
    {
        // We do not want to send another packet to the same locomotive too
        // quick. We send an idle packet instead. OR: We do not have any
        // locomotives at all. We will keep sending idle packets.
        message()->data()->set_dcc_idle();
    }
    else
    {
        // Send an update to the current loco.
        refreshSourcesMain_[nextRefreshIndexMain_]
            ->get_next_packet(0, message()->data());
        nextRefreshIndexMain_++;
    }
    // We pass on the filled packet to the track processor.
    mainTrackSend_->send(transfer_message());
    
    current_time = os_get_time_monotonic();
    prev_cycle_start = lastCycleStartProg_;
    if (nextRefreshIndexProg_ >= refreshSourcesProg_.size())
    {
        nextRefreshIndexProg_ = 0;
        lastCycleStartProg_ = current_time;
    }
    if (nextRefreshIndexProg_ == 0 &&
        (current_time - prev_cycle_start < MSEC_TO_NSEC(5) ||
         refreshSourcesProg_.empty()))
    {
        // We do not want to send another packet to the same locomotive too
        // quick. We send an idle packet instead. OR: We do not have any
        // locomotives at all. We will keep sending idle packets.
        message()->data()->set_dcc_idle();
    }
    else
    {
        // Send an update to the current loco.
        refreshSourcesProg_[nextRefreshIndexProg_]
            ->get_next_packet(0, message()->data());
        nextRefreshIndexProg_++;
    }
    // We pass on the filled packet to the track processor.
    progTrackSend_->send(transfer_message());
    return exit();
}
    

