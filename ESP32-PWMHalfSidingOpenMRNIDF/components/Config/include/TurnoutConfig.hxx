// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Dec 17 09:18:58 2022
//  Last Modified : <221217.0919>
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

#ifndef __TURNOUTCONFIG_HXX
#define __TURNOUTCONFIG_HXX
#include "openlcb/ConfigRepresentation.hxx"
/// CDI Configuration for a @ref ConfiguredConsumer.
CDI_GROUP(TurnoutConfig);
/// Allows the user to assign a name for this output.
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<8>, //
                Name("Description"), Description("User name of this turnout."));
/// Specifies the event ID to set the output to ON.
CDI_GROUP_ENTRY(
    normal, openlcb::EventConfigEntry, //
    Name("Normal"),
    Description("Receiving this event ID will align the turnout to normal."));
/// Specifies the event ID to set the output to OFF.
CDI_GROUP_ENTRY(
    reversed, openlcb::EventConfigEntry, //
    Name("Reversed"),
    Description("Receiving this event ID will align the turnout to reversed."));
/// Specifies the event ID to set the veto to ON.
CDI_GROUP_ENTRY(
    veto_on, openlcb::EventConfigEntry, //
    Name("Veto On"),
    Description("Receiving this event ID will lock the turnout."));
/// Specifies the event ID to set the output to OFF.
CDI_GROUP_ENTRY(
    veto_off, openlcb::EventConfigEntry, //
    Name("Veto Off"),
    Description("Receiving this event ID will unlock the turnout."));
CDI_GROUP_END();



#endif // __TURNOUTCONFIG_HXX

