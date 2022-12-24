// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri Dec 16 12:40:48 2022
//  Last Modified : <221224.1141>
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

#ifndef __NODEIDCONFIGURATIONGROUP_HXX
#define __NODEIDCONFIGURATIONGROUP_HXX
#include <openlcb/ConfigRepresentation.hxx>

#include "sdkconfig.h"

namespace esp32pwmhalfsiding
{

CDI_GROUP(NodeIdConfig, Segment(CONFIG_OLCB_NODEID_MEMORY_SPACE_ID),
          Offset(CONFIG_OLCB_NODEID_MEMORY_SPACE_OFFSET));
CDI_GROUP_ENTRY(node_id, openlcb::StringConfigEntry<32>, Name("Node ID"),
                Description(
                            R"!^!(Identifier to use for this device.
NOTE: Changing this value will force a factory reset.)!^!"))
CDI_GROUP_END();

} // namespace esp32pwmhalfsiding


#endif // __NODEIDCONFIGURATIONGROUP_HXX

