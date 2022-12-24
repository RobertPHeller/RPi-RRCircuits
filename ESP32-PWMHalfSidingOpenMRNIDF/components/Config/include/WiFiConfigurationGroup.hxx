// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri Dec 16 12:43:10 2022
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

#ifndef __WIFICONFIGURATIONGROUP_HXX
#define __WIFICONFIGURATIONGROUP_HXX
#include <openlcb/ConfigRepresentation.hxx>

#include "sdkconfig.h"

namespace esp32pwmhalfsiding
{

CDI_GROUP(WiFiStationConfiguration);
/// Allows setting the SSID that the Station will attempt to connect to.
CDI_GROUP_ENTRY(ssid, openlcb::StringConfigEntry<32>,
                Name("SSID"),
                Description("Configures the SSID that the ESP32 will connect to."));
/// Allows setting the password that the Station will use for connecting to the
/// configured SSID. Note: This value will be prefixed with [b64] and be base64
/// encoded when read via CDI.
CDI_GROUP_ENTRY(password, openlcb::StringConfigEntry<128>,
                Name("Password"),
                Description("Configures the password that the ESP32 will use for the station SSID."));
CDI_GROUP_END();

CDI_GROUP(WiFiConfiguration, Segment(CONFIG_OLCB_WIFI_MEMORY_SPACE_ID),
Offset(CONFIG_OLCB_WIFI_MEMORY_SPACE_OFFSET));

/// Allows configuring the WiFi operating mode of the node.
CDI_GROUP_ENTRY(wifi_mode, openlcb::Uint8ConfigEntry,
                Name("WiFi mode"),
                Description("Configures the WiFi operating mode."),
                Min(0), Max(3), Default(2), /* SoftAP */
                MapValues(
R"!^!(<relation><property>0</property><value>Off</value></relation>
<relation><property>1</property><value>Station Only</value></relation>)!^!"));
/// Allows configuration of the node's hostname.
CDI_GROUP_ENTRY(hostname_prefix, openlcb::StringConfigEntry<21>,
                Name("Hostname prefix"),
                Description(
R"!^!(Configures the hostname prefix used by the node.
Note: the node ID will be appended to this value.)!^!"));
/// Allows configuration of the node's hostname.
CDI_GROUP_ENTRY(station, WiFiStationConfiguration,
                Name("Station Configuration"),
                Description(
R"!^!(Configures the station WiFi interface on the ESP32 node.
This is used to have the ESP32 join an existing WiFi network.)!^!"));

CDI_GROUP_END();

}


#endif // __WIFICONFIGURATIONGROUP_HXX

