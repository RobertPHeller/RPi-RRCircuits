// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Mar 13 10:34:17 2019
//  Last Modified : <210622.1016>
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

#ifndef __CONFIG_H
#define __CONFIG_H

#include "openlcb/ConfiguredConsumer.hxx"
#include "openlcb/ConfiguredProducer.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "openlcb/MemoryConfig.hxx"

#include "Logic.h"
#include "Mast.h"
#include "TrackCircuit.h"

#include "Turnout.h"
#include "Points.h"

#include "OccupancyDetector.h" 

#include "Button.h"

#include "LED.h"

namespace openlcb
{

/// Defines the identification information for the node. The arguments are:
///
/// - 4 (version info, always 4 by the standard
/// - Manufacturer name
/// - Model name
/// - Hardware version
/// - Software version
///
/// This data will be used for all purposes of the identification:
///
/// - the generated cdi.xml will include this data
/// - the Simple Node Ident Info Protocol will return this data
/// - the ACDI memory space will contain this data.
extern const SimpleNodeStaticValues SNIP_STATIC_DATA = {
    4,
    "OpenMRN",
    "DWS ESP32 PWM Multifuncion Board",
    ARDUINO_VARIANT,
    "1.00"};

constexpr uint8_t NUM_TURNOUTS = 4;
constexpr uint8_t NUM_POINTSS = 4;
constexpr uint8_t NUM_OCS = 4;
constexpr uint8_t NUM_BUTTONS = 4;
constexpr uint8_t NUM_LEDS = 4;

//constexpr uint8_t NUM_OUTPUTS = 8;
//constexpr uint8_t NUM_INPUTS = 8;

/// Declares a repeated group of a given base group and number of repeats. The
/// ProducerConfig and ConsumerConfig groups represent the configuration layout
/// needed by the ConfiguredProducer and ConfiguredConsumer classes, and come
/// from their respective hxx file.
using LogicGroup = openlcb::RepeatedGroup<LogicConfig, LOGICCOUNT>;
using MastGroup = openlcb::RepeatedGroup<MastConfig, MASTCOUNT>;
using TrackCircuitGroup = openlcb::RepeatedGroup<TrackCircuitConfig, TRACKCIRCUITCOUNT>;

using TurnoutGroup = RepeatedGroup<TurnoutConfig, NUM_TURNOUTS>;
using PointsGroup = RepeatedGroup<PointsConfig, NUM_POINTSS>;
using OCGroup = RepeatedGroup<OccupancyDetectorConfig, NUM_OCS>;
using ButtonGroup = RepeatedGroup<ButtonConfig, NUM_BUTTONS>;
using LEDGroup = RepeatedGroup<LEDConfig, NUM_LEDS>;

/// Modify this value every time the EEPROM needs to be cleared on the node
/// after an update.
static constexpr uint16_t CANONICAL_VERSION = 0x100;

/// Defines the main segment in the configuration CDI. This is laid out at
/// origin 128 to give space for the ACDI user data at the beginning.
CDI_GROUP(IoBoardSegment, Segment(MemoryConfigDefs::SPACE_CONFIG), Offset(128));
/// Each entry declares the name of the current entry, then the type and then
/// optional arguments list.
CDI_GROUP_ENTRY(internal_config, InternalConfigData);
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
CDI_GROUP(VersionSeg, Segment(MemoryConfigDefs::SPACE_CONFIG),
    Name("Version information"));
CDI_GROUP_ENTRY(acdi_user_version, Uint8ConfigEntry,
    Name("ACDI User Data version"), Description("Set to 2 and do not change."));
CDI_GROUP_END();

/// The main structure of the CDI. ConfigDef is the symbol we use in main.cxx
/// to refer to the configuration defined here.
CDI_GROUP(ConfigDef, MainCdi());
/// Adds the <identification> tag with the values from SNIP_STATIC_DATA above.
CDI_GROUP_ENTRY(ident, Identification);
/// Adds an <acdi> tag.
CDI_GROUP_ENTRY(acdi, Acdi);
/// Adds a segment for changing the values in the ACDI user-defined
/// space. UserInfoSegment is defined in the system header.
CDI_GROUP_ENTRY(userinfo, UserInfoSegment);
/// Adds the main configuration segment.
CDI_GROUP_ENTRY(seg, IoBoardSegment);
/// Adds the versioning segment.
CDI_GROUP_ENTRY(version, VersionSeg);
CDI_GROUP_END();

} // namespace openlcb



#endif // __CONFIG_H

