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
//  Last Modified : <221215.1605>
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

#include "Logic.hxx"
#include "Mast.hxx"
#include "TrackCircuit.hxx"

#include "Turnout.hxx"
#include "Points.hxx"

#include "OccupancyDetector.hxx" 

#include "Button.hxx"

#include "LED.hxx"

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

namespace openlcb {
extern const char CDI_DATA[];
const char CDI_DATA[] = R"xmlpayload(<?xml version="1.0" encoding="utf-8"?>
<cdi xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://openlcb.org/schema/cdi/1/1/cdi.xsd">
<identification>
<manufacturer>)xmlpayload" SNIP_PROJECT_PAGE R"xmlpayload(</manufacturer>
<model>)xmlpayload" SNIP_PROJECT_NAME R"xmlpayload(</model>
<hardwareVersion>)xmlpayload" SNIP_HW_VERSION R"xmlpayload(</hardwareVersion>
<softwareVersion>)xmlpayload" SNIP_SW_VERSION R"xmlpayload(</softwareVersion>
</identification>
<acdi/>
<segment space='251' origin='1'>
<name>User Info</name>
<string size='63'>
<name>User Name</name>
<description>This name will appear in network browsers for this device.</description>
</string>
<string size='64'>
<name>User Description</name>
<description>This description will appear in network browsers for this device.</description>
</string>
</segment>
<segment space='253' origin='128'>
<name>Board Configuration</name>
<group>
<name>Internal data</name>
<description>Do not change these settings.</description>
<int size='2'>
<name>Version</name>
</int>
<int size='2'>
<name>Next event ID</name>
</int>
</group>
<group replication='4'>
<name>OccupancyDetectors</name>
<repname>OC</repname>
<string size='15'>
<name>Description</name>
<description>User name of this block.</description>
</string>
<int size='1'>
<name>Debounce parameter</name>
<description>Amount of time to wait for the input to stabilize before producing the event. Unit is 30 msec of time. Usually a value of 2-3 works well in a non-noisy environment. In high noise (train wheels for example) a setting between 8 -- 15 makes for a slower response time but a more stable signal.
Formally, the parameter tells how many times of tries, each 30 msec apart, the input must have the same value in order for that value to be accepted and the event transition produced.</description>
<default>3</default>
</int>
<eventid>
<name>Block Occupied</name>
<description>This event will be produced when the block becomes occupied.</description>
</eventid>
<eventid>
<name>Block Clear</name>
<description>This event will be produced when the block becomes clear.</description>
</eventid>
</group>
<group replication='4'>
<name>Turnouts</name>
<repname>Turnout</repname>
<string size='8'>
<name>Description</name>
<description>User name of this turnout.</description>
</string>
<eventid>
<name>Normal</name>
<description>Receiving this event ID will align the turnout to normal.</description>
</eventid>
<eventid>
<name>Reversed</name>
<description>Receiving this event ID will align the turnout to reversed.</description>
</eventid>
<eventid>
<name>Veto On</name>
<description>Receiving this event ID will lock the turnout.</description>
</eventid>
<eventid>
<name>Veto Off</name>
<description>Receiving this event ID will unlock the turnout.</description>
</eventid>
</group>
<group replication='4'>
<name>Points</name>
<repname>Points</repname>
<string size='15'>
<name>Description</name>
<description>User name of this set of points.</description>
</string>
<eventid>
<name>Normal</name>
<description>This event will be produced when the points are aligned for normal.</description>
</eventid>
<eventid>
<name>Reversed</name>
<description>This event will be produced when the are aligned for reversed.</description>
</eventid>
</group>
<group replication='4'>
<name>Buttons</name>
<repname>Button</repname>
<string size='15'>
<name>Description</name>
<description>User name of this button.</description>
</string>
<eventid>
<name>Button Released</name>
<description>This event will be produced when the button is released.</description>
</eventid>
<eventid>
<name>Button Pushed</name>
<description>This event will be produced when the button is pushed.</description>
</eventid>
</group>
<group replication='4'>
<name>LEDS</name>
<repname>LED</repname>
<int size='1'>
<name>LED Steady, Pulse, or Blink Phase (A-B)</name>
<default>0</default>
<map><relation><property>0</property><value>Steady Highside</value></relation><relation><property>1</property><value>Steady Lowside</value></relation><relation><property>2</property><value>Pulse Highside</value></relation><relation><property>3</property><value>Pulse Lowside</value></relation><relation><property>4</property><value>A - Slow</value></relation><relation><property>5</property><value>A - Medium</value></relation><relation><property>6</property><value>A - Fast</value></relation><relation><property>7</property><value>B - Slow</value></relation><relation><property>8</property><value>B - Medium</value></relation><relation><property>9</property><value>B - Fast</value></relation></map>
</int>
<int size='1'>
<name>Pulse width in seconds, 1 to 127</name>
<min>1</min>
<max>127</max>
<default>1</default>
</int>
<eventid>
<name>LED on</name>
<description>This event will be consumed to turn the output on.</description>
</eventid>
<eventid>
<name>LED off</name>
<description>This event will be consumed to turn the output off.</description>
</eventid>
</group>
<group replication='32'>
<name>LOGIC</name>
<repname>Logic</repname>
<string size='32'>
<name>Logic description</name>
</string>
<int size='1'>
<name>Group Function</name>
<default>0</default>
<map><relation><property>0</property><value>Blocked</value></relation><relation><property>1</property><value>Group</value></relation><relation><property>2</property><value>Last (Single)</value></relation></map>
</int>
<group>
<name>Variable #1</name>
<int size='1'>
<name>Variable Trigger</name>
<default>0</default>
<map><relation><property>0</property><value>On Variable Change</value></relation><relation><property>1</property><value>On Matching Event</value></relation><relation><property>2</property><value>None</value></relation></map>
</int>
<int size='1'>
<name>Variable Source</name>
<default>0</default>
<map><relation><property>0</property><value>Use Variable's (C) Events</value></relation><relation><property>1</property><value>Track Circuit 1</value></relation><relation><property>2</property><value>Track Circuit 2</value></relation><relation><property>3</property><value>Track Circuit 3</value></relation><relation><property>4</property><value>Track Circuit 4</value></relation><relation><property>5</property><value>Track Circuit 5</value></relation><relation><property>6</property><value>Track Circuit 6</value></relation><relation><property>7</property><value>Track Circuit 7</value></relation><relation><property>8</property><value>Track Circuit 8</value></relation></map>
</int>
<int size='1'>
<name>Variable Track Speed</name>
<default>0</default>
<map><relation><property>0</property><value>Stop</value></relation><relation><property>1</property><value>Restricting/Tumble Down</value></relation><relation><property>2</property><value>Slow</value></relation><relation><property>3</property><value>Medium</value></relation><relation><property>4</property><value>Limited</value></relation><relation><property>5</property><value>Approach</value></relation><relation><property>6</property><value>Approach-Medium</value></relation><relation><property>7</property><value>Clear/Procede</value></relation></map>
</int>
<eventid>
<name>(C) Event to set variable true.</name>
</eventid>
<eventid>
<name>(C) Event to set variable false.</name>
</eventid>
</group>
<group>
<int size='1'>
<name>Logic function</name>
<default>0</default>
<map><relation><property>0</property><value>V1 AND V2</value></relation><relation><property>1</property><value>V1 OR V2</value></relation><relation><property>2</property><value>V1 XOR V2</value></relation><relation><property>3</property><value>V1 AND V2 => Change</value></relation><relation><property>4</property><value>V1 OR V2 => Change</value></relation><relation><property>5</property><value>V1 AND then V2 => true</value></relation><relation><property>6</property><value>V1 only</value></relation><relation><property>7</property><value>V2 only</value></relation><relation><property>8</property><value>null => true</value></relation></map>
</int>
</group>
<group>
<name>Variable #2</name>
<int size='1'>
<name>Variable Trigger</name>
<default>0</default>
<map><relation><property>0</property><value>On Variable Change</value></relation><relation><property>1</property><value>On Matching Event</value></relation><relation><property>2</property><value>None</value></relation></map>
</int>
<int size='1'>
<name>Variable Source</name>
<default>0</default>
<map><relation><property>0</property><value>Use Variable's (C) Events</value></relation><relation><property>1</property><value>Track Circuit 1</value></relation><relation><property>2</property><value>Track Circuit 2</value></relation><relation><property>3</property><value>Track Circuit 3</value></relation><relation><property>4</property><value>Track Circuit 4</value></relation><relation><property>5</property><value>Track Circuit 5</value></relation><relation><property>6</property><value>Track Circuit 6</value></relation><relation><property>7</property><value>Track Circuit 7</value></relation><relation><property>8</property><value>Track Circuit 8</value></relation></map>
</int>
<int size='1'>
<name>Variable Track Speed</name>
<default>0</default>
<map><relation><property>0</property><value>Stop</value></relation><relation><property>1</property><value>Restricting/Tumble Down</value></relation><relation><property>2</property><value>Slow</value></relation><relation><property>3</property><value>Medium</value></relation><relation><property>4</property><value>Limited</value></relation><relation><property>5</property><value>Approach</value></relation><relation><property>6</property><value>Approach-Medium</value></relation><relation><property>7</property><value>Clear/Procede</value></relation></map>
</int>
<eventid>
<name>(C) Event to set variable true.</name>
</eventid>
<eventid>
<name>(C) Event to set variable false.</name>
</eventid>
</group>
<int size='1'>
<name>Action when Conditional = True</name>
<default>0</default>
<map><relation><property>0</property><value>Send then Exit Group</value></relation><relation><property>1</property><value>Send then Evaluate Next</value></relation><relation><property>2</property><value>Exit Group</value></relation><relation><property>3</property><value>Evaluate Next</value></relation></map>
</int>
<int size='1'>
<name>Action when Conditional = False</name>
<default>3</default>
<map><relation><property>0</property><value>Send then Exit Group</value></relation><relation><property>1</property><value>Send then Evaluate Next</value></relation><relation><property>2</property><value>Exit Group</value></relation><relation><property>3</property><value>Evaluate Next</value></relation></map>
</int>
<group>
<int size='2'>
<description>Time Delay before action
Delay Time (1-60000).</description>
<min>0</min>
<max>60000</max>
<default>0</default>
</int>
<int size='1'>
<name>Interval</name>
<default>0</default>
<map><relation><property>0</property><value>Milliseconds</value></relation><relation><property>1</property><value>Seconds</value></relation><relation><property>2</property><value>Minutes</value></relation></map>
</int>
<int size='1'>
<name>Retriggerable</name>
<default>0</default>
<map><relation><property>0</property><value>No</value></relation><relation><property>1</property><value>Yes</value></relation></map>
</int>
</group>
<group replication='4'>
<name>A trigger or change will generate the following events.</name>
<repname>Action</repname>
<int size='1'>
<default>0</default>
<map><relation><property>0</property><value>None</value></relation><relation><property>1</property><value>Immediately</value></relation><relation><property>2</property><value>After delay</value></relation><relation><property>3</property><value>Immediate if True</value></relation><relation><property>4</property><value>Immediate if False</value></relation><relation><property>5</property><value>Delayed if True</value></relation><relation><property>6</property><value>Delayed if False</value></relation></map>
</int>
<eventid>
<name>(P) this event will be sent.</name>
</eventid>
</group>
</group>
<group replication='8'>
<name>Rule to aspect</name>
<repname>Mast</repname>
<int size='1'>
<name>Function</name>
<description>Mast Processing</description>
<default>0</default>
<map><relation><property>0</property><value>Unused</value></relation><relation><property>1</property><value>Normal</value></relation><relation><property>2</property><value>Linked to previous</value></relation></map>
</int>
<string size='8'>
<name>Mast ID</name>
</string>
<eventid>
<name>(P) Track Circuit Link Address. Copy and Paste into linked Track Circuit. (Read Only)</name>
</eventid>
<group replication='8'>
<name>Rules</name>
<repname>Rule</repname>
<int size='1'>
<name>Name</name>
<default>0</default>
<map><relation><property>0</property><value>0-Stop</value></relation><relation><property>1</property><value>1-Take Siding</value></relation><relation><property>2</property><value>2-Stop Orders</value></relation><relation><property>3</property><value>3-Stop Procede</value></relation><relation><property>4</property><value>4-Restricting</value></relation><relation><property>5</property><value>5-Permissive</value></relation><relation><property>6</property><value>6-Slow-Approach</value></relation><relation><property>7</property><value>7-Slow</value></relation><relation><property>8</property><value>8-Slow-Medium</value></relation><relation><property>9</property><value>9-Slow-Limited</value></relation><relation><property>10</property><value>10-Slow-Clear</value></relation><relation><property>11</property><value>11-Medium-Approach</value></relation><relation><property>12</property><value>12-Medium-Slow</value></relation><relation><property>13</property><value>13-Medium</value></relation><relation><property>14</property><value>14-Medium-Clear</value></relation><relation><property>15</property><value>15-Medium-Limited</value></relation><relation><property>16</property><value>16-Limited-Approach</value></relation><relation><property>17</property><value>17-Limited-Slow</value></relation><relation><property>18</property><value>18-Limited-Medium</value></relation><relation><property>19</property><value>19-Limited</value></relation><relation><property>20</property><value>20-Limited-Clear</value></relation><relation><property>21</property><value>21-Approach</value></relation><relation><property>22</property><value>22-Advance-Approach</value></relation><relation><property>23</property><value>23-Approach-Slow</value></relation><relation><property>24</property><value>24-Advance-Approach-Slow</value></relation><relation><property>25</property><value>25-Approach-Medium</value></relation><relation><property>26</property><value>26-Advance-Approach-Medium</value></relation><relation><property>27</property><value>27-Approach-Limited</value></relation><relation><property>28</property><value>28-Advance-Approach-Limited</value></relation><relation><property>29</property><value>29-Clear</value></relation><relation><property>30</property><value>30-Cab Speed</value></relation><relation><property>31</property><value>31-Dark</value></relation></map>
</int>
<int size='1'>
<name>Track Speed (on approach to signal)</name>
<default>0</default>
<map><relation><property>0</property><value>Stop</value></relation><relation><property>1</property><value>Restricting/Tumble Down</value></relation><relation><property>2</property><value>Slow</value></relation><relation><property>3</property><value>Medium</value></relation><relation><property>4</property><value>Limited</value></relation><relation><property>5</property><value>Approach</value></relation><relation><property>6</property><value>Approach-Medium</value></relation><relation><property>7</property><value>Clear/Procede</value></relation></map>
</int>
<eventid>
<name>(C) Event to Set Aspect. Note: Aspects are cleared automatically by the logic.</name>
</eventid>
<eventid>
<name>(P) Send this event when the Aspect is set.</name>
</eventid>
<eventid>
<name>(P) Send this event when the Aspect clears.</name>
</eventid>
<group replication='4'>
<name>Appearance</name>
<description>Individual Aspect Lamps</description>
<repname>Lamp</repname>
<int size='1'>
<name>Lamp Selection</name>
<default>0</default>
<map><relation><property>0</property><value>Unused</value></relation><relation><property>1</property><value>A0</value></relation><relation><property>2</property><value>A1</value></relation><relation><property>3</property><value>A2</value></relation><relation><property>4</property><value>A3</value></relation><relation><property>5</property><value>A4</value></relation><relation><property>6</property><value>A5</value></relation><relation><property>7</property><value>A6</value></relation><relation><property>8</property><value>A7</value></relation><relation><property>9</property><value>B0</value></relation><relation><property>10</property><value>B1</value></relation><relation><property>11</property><value>B2</value></relation><relation><property>12</property><value>B3</value></relation><relation><property>13</property><value>B4</value></relation><relation><property>14</property><value>B5</value></relation><relation><property>15</property><value>B6</value></relation><relation><property>16</property><value>B7</value></relation></map>
</int>
<int size='1'>
<name>Lamp Phase (A-B) - Flash Rate</name>
<default>0</default>
<map><relation><property>0</property><value>Steady</value></relation><relation><property>1</property><value>A - Slow</value></relation><relation><property>2</property><value>A - Medium</value></relation><relation><property>3</property><value>A - Fast</value></relation><relation><property>4</property><value>None (not used)</value></relation><relation><property>5</property><value>B - Slow</value></relation><relation><property>6</property><value>B - Medium</value></relation><relation><property>7</property><value>B - Fast</value></relation></map>
</int>
<int size='2'>
<name>Lamp brightness, hundreths of a percent (0 to 10000)</name>
<min>0</min>
<max>10000</max>
<default>5000</default>
</int>
<int size='4'>
<name>PWM Period, in nanoseconds</name>
<default>1000000</default>
</int>
</group>
</group>
</group>
<group replication='8'>
<name>TRACK CIRCUITS</name>
<repname>Circuit</repname>
<string size='16'>
<name>Remote Mast Description</name>
</string>
<eventid>
<name>(C) Remote Mast Link Address.Copy from 'Next' Mast and Paste here.</name>
</eventid>
</group>
</segment>
<segment space='253'>
<name>Version information</name>
<int size='1'>
<name>ACDI User Data version</name>
<description>Set to 2 and do not change.</description>
</int>
</segment>
</cdi>)xmlpayload";
extern const size_t CDI_SIZE;
const size_t CDI_SIZE = sizeof(CDI_DATA);

extern const uint16_t CDI_EVENT_OFFSETS[] = {
    148, 156, 180, 188, 212, 220, 244, 252, 268, 276, 284, 292, 308, 316, 324, 
    332, 348, 356, 364, 372, 388, 396, 404, 412, 435, 443, 466, 474, 497, 505, 
    528, 536, 559, 567, 590, 598, 621, 629, 652, 660, 670, 678, 688, 696, 706, 
    714, 724, 732, 776, 784, 796, 804, 819, 828, 837, 846, 890, 898, 910, 918, 
    933, 942, 951, 960, 1004, 1012, 1024, 1032, 1047, 1056, 1065, 1074, 1118, 
    1126, 1138, 1146, 1161, 1170, 1179, 1188, 1232, 1240, 1252, 1260, 1275, 
    1284, 1293, 1302, 1346, 1354, 1366, 1374, 1389, 1398, 1407, 1416, 1460, 
    1468, 1480, 1488, 1503, 1512, 1521, 1530, 1574, 1582, 1594, 1602, 1617, 
    1626, 1635, 1644, 1688, 1696, 1708, 1716, 1731, 1740, 1749, 1758, 1802, 
    1810, 1822, 1830, 1845, 1854, 1863, 1872, 1916, 1924, 1936, 1944, 1959, 
    1968, 1977, 1986, 2030, 2038, 2050, 2058, 2073, 2082, 2091, 2100, 2144, 
    2152, 2164, 2172, 2187, 2196, 2205, 2214, 2258, 2266, 2278, 2286, 2301, 
    2310, 2319, 2328, 2372, 2380, 2392, 2400, 2415, 2424, 2433, 2442, 2486, 
    2494, 2506, 2514, 2529, 2538, 2547, 2556, 2600, 2608, 2620, 2628, 2643, 
    2652, 2661, 2670, 2714, 2722, 2734, 2742, 2757, 2766, 2775, 2784, 2828, 
    2836, 2848, 2856, 2871, 2880, 2889, 2898, 2942, 2950, 2962, 2970, 2985, 
    2994, 3003, 3012, 3056, 3064, 3076, 3084, 3099, 3108, 3117, 3126, 3170, 
    3178, 3190, 3198, 3213, 3222, 3231, 3240, 3284, 3292, 3304, 3312, 3327, 
    3336, 3345, 3354, 3398, 3406, 3418, 3426, 3441, 3450, 3459, 3468, 3512, 
    3520, 3532, 3540, 3555, 3564, 3573, 3582, 3626, 3634, 3646, 3654, 3669, 
    3678, 3687, 3696, 3740, 3748, 3760, 3768, 3783, 3792, 3801, 3810, 3854, 
    3862, 3874, 3882, 3897, 3906, 3915, 3924, 3968, 3976, 3988, 3996, 4011, 
    4020, 4029, 4038, 4082, 4090, 4102, 4110, 4125, 4134, 4143, 4152, 4196, 
    4204, 4216, 4224, 4239, 4248, 4257, 4266, 4310, 4318, 4330, 4338, 4353, 
    4362, 4371, 4380, 4397, 4407, 4415, 4423, 4465, 4473, 4481, 4523, 4531, 
    4539, 4581, 4589, 4597, 4639, 4647, 4655, 4697, 4705, 4713, 4755, 4763,
    4771, 4813, 4821, 4829, 4878, 4888, 4896, 4904, 4946, 4954, 4962, 5004,
    5012, 5020, 5062, 5070, 5078, 5120, 5128, 5136, 5178, 5186, 5194, 5236,
    5244, 5252, 5294, 5302, 5310, 5359, 5369, 5377, 5385, 5427, 5435, 5443,
    5485, 5493, 5501, 5543, 5551, 5559, 5601, 5609, 5617, 5659, 5667, 5675,
    5717, 5725, 5733, 5775, 5783, 5791, 5840, 5850, 5858, 5866, 5908, 5916,
    5924, 5966, 5974, 5982, 6024, 6032, 6040, 6082, 6090, 6098, 6140, 6148,
    6156, 6198, 6206, 6214, 6256, 6264, 6272, 6321, 6331, 6339, 6347, 6389,
    6397, 6405, 6447, 6455, 6463, 6505, 6513, 6521, 6563, 6571, 6579, 6621,
    6629, 6637, 6679, 6687, 6695, 6737, 6745, 6753, 6802, 6812, 6820, 6828,
    6870, 6878, 6886, 6928, 6936, 6944, 6986, 6994, 7002, 7044, 7052, 7060,
    7102, 7110, 7118, 7160, 7168, 7176, 7218, 7226, 7234, 7283, 7293, 7301,
    7309, 7351, 7359, 7367, 7409, 7417, 7425, 7467, 7475, 7483, 7525, 7533,
    7541, 7583, 7591, 7599, 7641, 7649, 7657, 7699, 7707, 7715, 7764, 7774,
    7782, 7790, 7832, 7840, 7848, 7890, 7898, 7906, 7948, 7956, 7964, 8006,
    8014, 8022, 8064, 8072, 8080, 8122, 8130, 8138, 8180, 8188, 8196, 8252,
    8276, 8300, 8324, 8348, 8372, 8396, 8420, 0};
}  // namespace openlcb


#endif // __CDI_HXX

