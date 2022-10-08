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
//  Last Modified : <221008.1326>
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

#include <freertos_drivers/esp32/Esp32WiFiConfiguration.hxx>
#include <openlcb/ConfigRepresentation.hxx>
#include "openlcb/ConfiguredConsumer.hxx"
#include "openlcb/ConfiguredProducer.hxx"
#include "openlcb/MemoryConfig.hxx"
#include "openlcb/MultiConfiguredConsumer.hxx"
#include "MultiConfiguredProducer.hxx" // Not in OpenMRN?

#include "hardware.hxx"

#include "Logic.hxx"
#include "TrackCircuit.hxx"

namespace esp32multiio
{

using LogicGroup = openlcb::RepeatedGroup<LogicConfig, LOGICCOUNT>;
using TrackCircuitGroup = openlcb::RepeatedGroup<TrackCircuitConfig, TRACKCIRCUITCOUNT>;
using PortConsumers = openlcb::RepeatedGroup<openlcb::ConsumerConfig, 8>;
using PortProducers = openlcb::RepeatedGroup<openlcb::ProducerConfig, 8>;


CDI_GROUP(CARD0)
CDI_GROUP_ENTRY(PortA,PortConsumers,Name("A"), RepName("A"));
#ifdef PRODUCITION
CDI_GROUP_ENTRY(PortB,PortConsumers,Name("B"), RepName("B"));
CDI_GROUP_ENTRY(PortC,PortConsumers,Name("C"), RepName("C"));
#endif
CDI_GROUP_END();

#ifdef PRODUCITION
CDI_GROUP(CARD1)
CDI_GROUP_ENTRY(PortA,PortConsumers,Name("A"), RepName("A"));
CDI_GROUP_ENTRY(PortB,PortConsumers,Name("B"), RepName("B"));
CDI_GROUP_ENTRY(PortC,PortConsumers,Name("C"), RepName("C"));
CDI_GROUP_END();
#endif

CDI_GROUP(CARD2)
CDI_GROUP_ENTRY(PortA,PortProducers,Name("A"), RepName("A"));
#ifdef PRODUCITION
CDI_GROUP_ENTRY(PortB,PortProducers,Name("B"), RepName("B"));
CDI_GROUP_ENTRY(PortC,PortProducers,Name("C"), RepName("C"));
#endif
CDI_GROUP_END();


/// Defines the main segment in the configuration CDI. This is laid out at
/// origin 128 to give space for the ACDI user data at the beginning.
CDI_GROUP(IoBoard, Segment(openlcb::MemoryConfigDefs::SPACE_CONFIG),
          Offset(128));
/// Each entry declares the name of the current entry, then the type and then
/// optional arguments list.
CDI_GROUP_ENTRY(internal_config, openlcb::InternalConfigData);
CDI_GROUP_ENTRY(Card0,CARD0,Name("Card0"));
#ifdef PRODUCITION
CDI_GROUP_ENTRY(Card1,CARD1,Name("Card1"));
#endif
CDI_GROUP_ENTRY(Card2,CARD2,Name("Card2"));
CDI_GROUP_ENTRY(logics, LogicGroup, Name("LOGIC"), RepName("Logic"));
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
CDI_GROUP_ENTRY(userinfo, openlcb::UserInfoSegment);
/// Adds the main configuration segment.
CDI_GROUP_ENTRY(seg, IoBoard);
/// Adds the versioning segment.
CDI_GROUP_ENTRY(version, VersionSeg);
CDI_GROUP_END();



}

#endif // __CDI_HXX

