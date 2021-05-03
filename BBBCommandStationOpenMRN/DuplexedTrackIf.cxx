// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon May 3 09:02:58 2021
//  Last Modified : <210503.0939>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2021  Robert Heller D/B/A Deepwoods Software
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

#include "DuplexedTrackIf.hxx"

#include <dcc/Packet.hxx>
#include <executor/Executor.hxx>
#include <executor/StateFlow.hxx>
#include <sys/ioctl.h>
#include <utils/Buffer.hxx>
#include <utils/Queue.hxx>
#include <CommandStationDCCMainTrack.hxx>
#include <CommandStationDCCProgTrack.hxx>


namespace BeagleCS
{
DuplexedTrackIf::DuplexedTrackIf(Service *service, int pool_size, 
                                 CommandStationDCCMainTrack *ops,
                                 CommandStationDCCProgTrack *prog)
      : StateFlow<Buffer<dcc::Packet>, QList<1>>(service)
, ops_(ops)
, prog_(prog)
, pool_(sizeof(Buffer<dcc::Packet>), pool_size)
{
}

StateFlowBase::Action DuplexedTrackIf::entry()
{
    auto *p = message()->data();
    if (p->packet_header.send_long_preamble) {
        prog_->send(message());
    } else {
        ops_->send(message());
    }
    return finish();
}

}
