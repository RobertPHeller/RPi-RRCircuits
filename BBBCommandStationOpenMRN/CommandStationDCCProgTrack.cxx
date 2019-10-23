// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Oct 23 17:30:57 2019
//  Last Modified : <191023.1813>
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

#include "CommandStationDCCProgTrack.hxx"

#include <prussdrv.h>
#include <pruss_intc_mapping.h>

CommandStationDCCProgTrack::CommandStationDCCProgTrack(Service *service, int pool_size)
      : StateFlow<Buffer<dcc::Packet>, QList<1>>(service)
, pool_(sizeof(Buffer<dcc::Packet>), pool_size)
{
}

StateFlowBase::Action CommandStationDCCProgTrack::entry()
{
    //auto *p = message()->data();
    // -- send p to the PRU to send to the track.
    return finish();
}

