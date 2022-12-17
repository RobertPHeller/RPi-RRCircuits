// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Dec 17 09:20:53 2022
//  Last Modified : <221217.0921>
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

#ifndef __OCCUPANCYDETECTORCONFIG_HXX
#define __OCCUPANCYDETECTORCONFIG_HXX

#include "openlcb/ConfigRepresentation.hxx"

/// CDI Configuration for a @ref ConfiguredProducer.
CDI_GROUP(OccupancyDetectorConfig);
/// Allows the user to assign a name for this input.
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<15>, //
                Name("Description"), Description("User name of this block."));
/// Configures the debounce parameter.
CDI_GROUP_ENTRY(
    debounce, openlcb::Uint8ConfigEntry, Name("Debounce parameter"),
    Default(3),
    Description("Amount of time to wait for the input to stabilize before "
                "producing the event. Unit is 30 msec of time. Usually a value "
                "of 2-3 works well in a non-noisy environment. In high noise "
                "(train wheels for example) a setting between 8 -- 15 makes "
                "for a slower response time but a more stable "
                "signal.\nFormally, the parameter tells how many times of "
                "tries, each 30 msec apart, the input must have the same value "
                "in order for that value to be accepted and the event "
                "transition produced."),
    Default(3));
/// This event will be produced when the input goes to HIGH.
CDI_GROUP_ENTRY(
    event_occupied, openlcb::EventConfigEntry, //
    Name("Block Occupied"),
    Description("This event will be produced when the block becomes occupied."));
/// This event will be produced when the input goes to LOW.
CDI_GROUP_ENTRY(
    event_clear, openlcb::EventConfigEntry, //
    Name("Block Clear"),
    Description("This event will be produced when the block becomes clear."));
CDI_GROUP_END();



#endif // __OCCUPANCYDETECTORCONFIG_HXX

