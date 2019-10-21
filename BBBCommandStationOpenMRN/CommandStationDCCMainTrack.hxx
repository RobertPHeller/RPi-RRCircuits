// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Oct 20 20:21:25 2019
//  Last Modified : <191020.2027>
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

#ifndef __COMMANDSTATIONDCCMAINTRACK_HXX
#define __COMMANDSTATIONDCCMAINTRACK_HXX

#include "utils/Singleton.hxx"
#include "dcc/PacketFlowInterface.hxx"
#include "dcc/UpdateLoop.hxx"

class CommandStationDCCMainTrack : private dcc::UpdateLoopBase {
public:
    CommandStationDCCMainTrack();
    virtual void notify_update(dcc::PacketSource *source, unsigned code);
    virtual bool add_refresh_source(dcc::PacketSource *source, unsigned priority = 0);
    virtual void remove_refresh_source(dcc::PacketSource *source);
};


#endif // __COMMANDSTATIONDCCMAINTRACK_HXX

