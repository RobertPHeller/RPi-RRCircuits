// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Dec 17 09:23:01 2022
//  Last Modified : <221217.0923>
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

#ifndef __LEDCONFIG_HXX
#define __LEDCONFIG_HXX

#include "openlcb/ConfigRepresentation.hxx"

static const char LEDPhaseMap[] =
    "<relation><property>0</property><value>Steady Highside</value></relation>"
    "<relation><property>1</property><value>Steady Lowside</value></relation>"

    "<relation><property>2</property><value>Pulse Highside</value></relation>"
    "<relation><property>3</property><value>Pulse Lowside</value></relation>"

    "<relation><property>4</property><value>A - Slow</value></relation>"
    "<relation><property>5</property><value>A - Medium</value></relation>"
    "<relation><property>6</property><value>A - Fast</value></relation>"

    "<relation><property>7</property><value>B - Slow</value></relation>"
    "<relation><property>8</property><value>B - Medium</value></relation>"
    "<relation><property>9</property><value>B - Fast</value></relation>";

/// CDI Configuration for a @ref LED
CDI_GROUP(LEDConfig);
CDI_GROUP_ENTRY(phase, openlcb::Uint8ConfigEntry,
                Name("LED Steady, Pulse, or Blink Phase (A-B)"),
                MapValues(LEDPhaseMap),Default(0));
CDI_GROUP_ENTRY(pulsewidth, openlcb::Uint8ConfigEntry,
                Name("Pulse width in seconds, 1 to 127"),
                Min(1), Max(127), Default(1));
/// This event will be consumed to turn the output on.
CDI_GROUP_ENTRY(
    event_on, openlcb::EventConfigEntry, //
    Name("LED on"),
    Description("This event will be consumed to turn the output on."));
/// This event will be consumed to turn the output off.
CDI_GROUP_ENTRY(
    event_off, openlcb::EventConfigEntry, //
    Name("LED off"),
    Description("This event will be consumed to turn the output off."));
CDI_GROUP_END();



#endif // __LEDCONFIG_HXX

