// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Jun 25 09:33:40 2022
//  Last Modified : <221217.1129>
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

#ifndef __CDI_HXX
#define __CDI_HXX

#include <openlcb/ConfigRepresentation.hxx>
#include "openlcb/ConfiguredConsumer.hxx"
#include "openlcb/ConfiguredProducer.hxx"
#include "openlcb/MemoryConfig.hxx"

#include "LogicConfig.hxx"
#include "MastConfig.hxx"
#include "TrackCircuitConfig.hxx"

#include "TurnoutConfig.hxx"
#include "PointsConfig.hxx"

#include "OccupancyDetectorConfig.hxx" 

#include "ButtonConfig.hxx"

#include "LEDConfig.hxx"

namespace esp32multifunction
{

constexpr uint8_t NUM_TURNOUTS = 4;
constexpr uint8_t NUM_POINTSS = 4;
constexpr uint8_t NUM_OCS = 4;
constexpr uint8_t NUM_BUTTONS = 4;
constexpr uint8_t NUM_LEDS = 4;
using LogicGroup = openlcb::RepeatedGroup<LogicConfig, LOGICCOUNT>;
using MastGroup = openlcb::RepeatedGroup<MastConfig, MASTCOUNT>;
using TrackCircuitGroup = openlcb::RepeatedGroup<TrackCircuitConfig, TRACKCIRCUITCOUNT>;

using TurnoutGroup = openlcb::RepeatedGroup<TurnoutConfig, NUM_TURNOUTS>;
using PointsGroup = openlcb::RepeatedGroup<PointsConfig, NUM_POINTSS>;
using OCGroup = openlcb::RepeatedGroup<OccupancyDetectorConfig, NUM_OCS>;
using ButtonGroup = openlcb::RepeatedGroup<ButtonConfig, NUM_BUTTONS>;
using LEDGroup = openlcb::RepeatedGroup<LEDConfig, NUM_LEDS>;

/// Defines the main segment in the configuration CDI. This is laid out at
/// origin 128 to give space for the ACDI user data at the beginning.
CDI_GROUP(IoBoard, Segment(openlcb::MemoryConfigDefs::SPACE_CONFIG),
          Offset(128));
/// Each entry declares the name of the current entry, then the type and then
/// optional arguments list.
CDI_GROUP_ENTRY(internal_config, openlcb::InternalConfigData);
CDI_GROUP_ENTRY(ocs, OCGroup, Name("OccupancyDetectors"), RepName("OC"));
CDI_GROUP_ENTRY(turnouts, TurnoutGroup, Name("Turnouts"), RepName("Turnout"));
CDI_GROUP_ENTRY(points, PointsGroup, Name("Points"), RepName("Points"));
CDI_GROUP_ENTRY(buttons, ButtonGroup, Name("Buttons"), RepName("Button"));
CDI_GROUP_ENTRY(leds,LEDGroup, Name("LEDS"), RepName("LED"));
CDI_GROUP_ENTRY(logics, LogicGroup, Name("LOGIC"), RepName("Logic"));
CDI_GROUP_ENTRY(masts, MastGroup, Name("Rule to aspect"),RepName("Mast"));
CDI_GROUP_ENTRY(circuits, TrackCircuitGroup, Name("TRACK CIRCUITS"), RepName("Circuit"));
CDI_GROUP_END();

/// This segment is only needed temporarily until there is program code to set
/// the ACDI user data version byte.
CDI_GROUP(VersionSeg, Segment(openlcb::MemoryConfigDefs::SPACE_CONFIG),
    Name("Version information"));
CDI_GROUP_ENTRY(acdi_user_version, openlcb::Uint8ConfigEntry,
    Name("ACDI User Data version"), Description("Set to 2 and do not change."));
CDI_GROUP_END();

/// The main structure of the CDI. ConfigDef is the symbol we use in main.cxx
/// to refer to the configuration defined here.
CDI_GROUP(ConfigDef, MainCdi());
/// Adds the <identification> tag with the values from SNIP_STATIC_DATA above.
CDI_GROUP_ENTRY(ident, openlcb::Identification);
/// Adds an <acdi> tag.
CDI_GROUP_ENTRY(acdi, openlcb::Acdi);
/// Adds a segment for changing the values in the ACDI user-defined
/// space. UserInfoSegment is defined in the system header.
CDI_GROUP_ENTRY(userinfo, openlcb::UserInfoSegment, Name("User Info"));
/// Adds the main configuration segment.
CDI_GROUP_ENTRY(seg, IoBoard, Name("Board Configuration"));
/// Adds the versioning segment.
CDI_GROUP_ENTRY(version, VersionSeg);
CDI_GROUP_END();



}

#endif // __CDI_HXX

