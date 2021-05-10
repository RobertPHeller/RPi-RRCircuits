// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon May 3 08:56:43 2021
//  Last Modified : <210510.1526>
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

#ifndef __DUPLEXEDTRACKIF_HXX
#define __DUPLEXEDTRACKIF_HXX

#include <executor/Executor.hxx>
#include <executor/StateFlow.hxx>
#include <dcc/Packet.hxx>
#include <utils/Singleton.hxx>
#include <CommandStationDCCPRUTrack.hxx>

using CommandStationDCCMainTrack = CommandStationDCCPRUTrack<0>;
using CommandStationDCCProgTrack = CommandStationDCCPRUTrack<1>;

namespace BeagleCS
{
class DuplexedTrackIf : public StateFlow<Buffer<dcc::Packet>
, QList<1>>
, public Singleton<DuplexedTrackIf>
{
public:
    DuplexedTrackIf(Service *service, int pool_size, 
                    CommandStationDCCMainTrack *ops, 
                    CommandStationDCCProgTrack *prog);
    FixedPool *pool() override
    {
        return &pool_;
    }
protected:
    Action entry() override;
    Action finish()
    {
        return release_and_exit();
    }
    CommandStationDCCMainTrack *ops_;
    CommandStationDCCProgTrack *prog_;
    FixedPool pool_;
};

}
    
    

#endif // __DUPLEXEDTRACKIF_HXX

