// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Dec 17 09:17:14 2022
//  Last Modified : <221217.0918>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2022  Robert Heller D/B/A Deepwoods Software
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

#ifndef __TRACKCIRCUITCONFIG_HXX
#define __TRACKCIRCUITCONFIG_HXX

#include "openlcb/ConfigRepresentation.hxx"

static const char TrackSpeedMap[] = 
"<relation><property>0</property><value>Stop</value></relation>"
"<relation><property>1</property><value>Restricting/Tumble Down</value></relation>"
"<relation><property>2</property><value>Slow</value></relation>"
"<relation><property>3</property><value>Medium</value></relation>"
"<relation><property>4</property><value>Limited</value></relation>"
"<relation><property>5</property><value>Approach</value></relation>"
"<relation><property>6</property><value>Approach-Medium</value></relation>"
"<relation><property>7</property><value>Clear/Procede</value></relation>";

#define TRACKCIRCUITCOUNT 8

/// CDI Configuration for a @ref TrackCircuit
CDI_GROUP(TrackCircuitConfig);
CDI_GROUP_ENTRY(description,openlcb::StringConfigEntry<16>,
                Name("Remote Mast Description"));
CDI_GROUP_ENTRY(remotemastlink,openlcb::EventConfigEntry,
                Name("(C) Remote Mast Link Address."
                     "Copy from 'Next' Mast and Paste here."));
CDI_GROUP_END();




#endif // __TRACKCIRCUITCONFIG_HXX

