// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Dec 17 09:32:27 2022
//  Last Modified : <221217.0933>
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

#ifndef __LAMPCONFIG_HXX
#define __LAMPCONFIG_HXX

#include "openlcb/ConfigRepresentation.hxx"

#define LAMPCOUNT 4

static const char LampSelectMap[] = 
    "<relation><property>0</property><value>Unused</value></relation>"

    "<relation><property>1</property><value>A0</value></relation>"
    "<relation><property>2</property><value>A1</value></relation>"
    "<relation><property>3</property><value>A2</value></relation>"
    "<relation><property>4</property><value>A3</value></relation>"
    "<relation><property>5</property><value>A4</value></relation>"
    "<relation><property>6</property><value>A5</value></relation>"
    "<relation><property>7</property><value>A6</value></relation>"
    "<relation><property>8</property><value>A7</value></relation>"

    "<relation><property>9</property><value>B0</value></relation>"
    "<relation><property>10</property><value>B1</value></relation>"
    "<relation><property>11</property><value>B2</value></relation>"
    "<relation><property>12</property><value>B3</value></relation>"
    "<relation><property>13</property><value>B4</value></relation>"
    "<relation><property>14</property><value>B5</value></relation>"
    "<relation><property>15</property><value>B6</value></relation>"
    "<relation><property>16</property><value>B7</value></relation>";

static const char LampPhaseMap[] = 
    "<relation><property>0</property><value>Steady</value></relation>"

    "<relation><property>1</property><value>A - Slow</value></relation>"
    "<relation><property>2</property><value>A - Medium</value></relation>"
    "<relation><property>3</property><value>A - Fast</value></relation>"

    "<relation><property>4</property><value>None (not used)</value></relation>"

    "<relation><property>5</property><value>B - Slow</value></relation>"
    "<relation><property>6</property><value>B - Medium</value></relation>"
    "<relation><property>7</property><value>B - Fast</value></relation>";


/// CDI Configuration for a @ref Lamp
CDI_GROUP(LampConfig);
/// Specifies the lamp selection for this lamp
CDI_GROUP_ENTRY(selection, openlcb::Uint8ConfigEntry, 
                Name("Lamp Selection"), MapValues(LampSelectMap),
                Default(0));
CDI_GROUP_ENTRY(phase, openlcb::Uint8ConfigEntry,
                Name("Lamp Phase (A-B) - Flash Rate"),
                MapValues(LampPhaseMap), Default(0));
CDI_GROUP_ENTRY(brightness, openlcb::Uint16ConfigEntry,
                Name("Lamp brightness, hundreths of a percent (0 to 10000)"),
                Min(0), Max(10000), Default(5000));
CDI_GROUP_ENTRY(period, openlcb::Uint32ConfigEntry,
                Name("PWM Period, in nanoseconds"),
                Default(1000000));
CDI_GROUP_END();

using LampGroup = openlcb::RepeatedGroup<LampConfig, LAMPCOUNT>;



#endif // __LAMPCONFIG_HXX

