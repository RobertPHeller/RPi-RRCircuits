// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Dec 17 09:15:46 2022
//  Last Modified : <221217.0930>
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

#ifndef __MASTCONFIG_HXX
#define __MASTCONFIG_HXX

#include "openlcb/ConfigRepresentation.hxx"
#include "RuleConfig.hxx"

#define MASTCOUNT 8

static const char MastProcessingMap[] = 
"<relation><property>0</property><value>Unused</value></relation>"
"<relation><property>1</property><value>Normal</value></relation>"
"<relation><property>2</property><value>Linked to previous</value></relation>";


static const char LampFadeMap[] =
"<relation><property>0</property><value>None</value></relation>"
"<relation><property>1</property><value>Incandescent</value></relation>";



/// CDI Configuration for a @ref Mast
CDI_GROUP(MastConfig);
CDI_GROUP_ENTRY(processing,openlcb::Uint8ConfigEntry,
                Name("Function"),Default(0),
                Description("Mast Processing"),
                MapValues(MastProcessingMap));
CDI_GROUP_ENTRY(mastid, openlcb::StringConfigEntry<8>,
                Name("Mast ID"));
CDI_GROUP_ENTRY(linkevent,openlcb::EventConfigEntry,
                Name("(P) Track Circuit Link Address. Copy and Paste into linked Track Circuit. (Read Only)"));
#define TRACKCIRCUITBASE 2048 // Leave lots of room
#ifdef HAVEPWM
CDI_GROUP_ENTRY(fade,openlcb::Uint8ConfigEntry,
                Name("Lamp Fade"),Default(0),
                MapValues(LampFadeMap));
#endif
CDI_GROUP_ENTRY(rules,RulesGroup,Name("Rules"),RepName("Rule"));
CDI_GROUP_END();




#endif // __MASTCONFIG_HXX

