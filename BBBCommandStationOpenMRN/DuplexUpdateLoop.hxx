// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Oct 23 16:57:31 2019
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

#ifndef __DUPLEXUPDATELOOP_HXX
#define __DUPLEXUPDATELOOP_HXX

#include <algorithm>

#include "dcc/UpdateLoop.hxx"
#include "executor/StateFlow.hxx"

class DuplexUpdateLoop : public StateFlow<Buffer<dcc::Packet>, QList<1>>,
                         private dcc::UpdateLoopBase
{
public:
    DuplexUpdateLoop(Service *service, dcc::PacketFlowInterface *mainTrack_send, dcc::PacketFlowInterface *progTrack_send);
    ~DuplexUpdateLoop();

    /** Adds a new refresh source to the background refresh packets. */
    bool add_refresh_source(
        dcc::PacketSource *source, unsigned priority) OVERRIDE
    {
        /// @todo implement priority refresh.
        AtomicHolder h(this);
        if (priority == dcc::UpdateLoopBase::PROGRAMMING_PRIORITY) {
            refreshSourcesProg_.push_back(source);
        } else {
            refreshSourcesMain_.push_back(source);
        }
        return true;
    }

    /** Deletes a packet refresh source. */
    void remove_refresh_source(dcc::PacketSource *source) OVERRIDE
    {
        AtomicHolder h(this);
        refreshSourcesProg_.erase(
            remove(refreshSourcesProg_.begin(), refreshSourcesProg_.end(), source),
            refreshSourcesProg_.end());
        refreshSourcesMain_.erase(
            remove(refreshSourcesMain_.begin(), refreshSourcesMain_.end(), source),
            refreshSourcesMain_.end());
    }

    /** We ignore notifications. The loop will get to them anyway. */
    void notify_update(dcc::PacketSource *source, unsigned code) OVERRIDE
    {
    }

    // Entry to the state flow -- when a new packet needs to be sent.
    Action entry() OVERRIDE;

private:
    // Place where we forward the packets filled in.
    dcc::PacketFlowInterface *mainTrackSend_;
    dcc::PacketFlowInterface *progTrackSend_;

    // Packet sources to ask about refreshing data periodically.
    vector<dcc::PacketSource *> refreshSourcesMain_;
    vector<dcc::PacketSource *> refreshSourcesProg_;

    /// Offset in the refreshSources_ vector for the next loco to send.
    size_t nextRefreshIndexMain_;
    size_t nextRefreshIndexProg_;
    /// os time for the last time we sent a packet for loco zero.
    long long lastCycleStartMain_;
    long long lastCycleStartProg_;
};


#endif // __DUPLEXUPDATELOOP_HXX

