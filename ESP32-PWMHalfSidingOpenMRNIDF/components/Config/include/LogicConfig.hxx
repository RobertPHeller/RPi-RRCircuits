// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Dec 17 09:12:55 2022
//  Last Modified : <221217.0928>
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

#ifndef __LOGICCONFIG_HXX
#define __LOGICCONFIG_HXX

#include "openlcb/ConfigRepresentation.hxx"
#include "TrackCircuitConfig.hxx"

#define LOGICCOUNT 32

static const char GroupFunctionMap[] = 
"<relation><property>0</property><value>Blocked</value></relation>"
"<relation><property>1</property><value>Group</value></relation>"
"<relation><property>2</property><value>Last (Single)</value></relation>";

static const char LogicFunctionMap[] = 
"<relation><property>0</property><value>V1 AND V2</value></relation>"
"<relation><property>1</property><value>V1 OR V2</value></relation>"
"<relation><property>2</property><value>V1 XOR V2</value></relation>"
"<relation><property>3</property><value>V1 AND V2 => Change</value></relation>"
"<relation><property>4</property><value>V1 OR V2 => Change</value></relation>"
"<relation><property>5</property><value>V1 AND then V2 => true</value></relation>"
"<relation><property>6</property><value>V1 only</value></relation>"
"<relation><property>7</property><value>V2 only</value></relation>"
"<relation><property>8</property><value>null => true</value></relation>";

static const char ActionMap[] =
"<relation><property>0</property><value>Send then Exit Group</value></relation>"
"<relation><property>1</property><value>Send then Evaluate Next</value></relation>"
"<relation><property>2</property><value>Exit Group</value></relation>"
"<relation><property>3</property><value>Evaluate Next</value></relation>";

static const char VariableTriggerMap[] = 
"<relation><property>0</property><value>On Variable Change</value></relation>"
"<relation><property>1</property><value>On Matching Event</value></relation>"
"<relation><property>2</property><value>None</value></relation>";


static const char VariableSourceMap[] = 
"<relation><property>0</property><value>Use Variable's (C) Events</value></relation>"
"<relation><property>1</property><value>Track Circuit 1</value></relation>"
"<relation><property>2</property><value>Track Circuit 2</value></relation>"
"<relation><property>3</property><value>Track Circuit 3</value></relation>"
"<relation><property>4</property><value>Track Circuit 4</value></relation>"
"<relation><property>5</property><value>Track Circuit 5</value></relation>"
"<relation><property>6</property><value>Track Circuit 6</value></relation>"
"<relation><property>7</property><value>Track Circuit 7</value></relation>"
"<relation><property>8</property><value>Track Circuit 8</value></relation>";

static const char IntervalMap[] = 
"<relation><property>0</property><value>Milliseconds</value></relation>"
"<relation><property>1</property><value>Seconds</value></relation>"
"<relation><property>2</property><value>Minutes</value></relation>";

static const char RetriggerableMap[] = 
"<relation><property>0</property><value>No</value></relation>"
"<relation><property>1</property><value>Yes</value></relation>"; 

static const char ActionTriggerMap[] =
"<relation><property>0</property><value>None</value></relation>"
"<relation><property>1</property><value>Immediately</value></relation>"
"<relation><property>2</property><value>After delay</value></relation>"
"<relation><property>3</property><value>Immediate if True</value></relation>"
"<relation><property>4</property><value>Immediate if False</value></relation>"
"<relation><property>5</property><value>Delayed if True</value></relation>"
"<relation><property>6</property><value>Delayed if False</value></relation>";


CDI_GROUP(VariableConfig);
CDI_GROUP_ENTRY(trigger,openlcb::Uint8ConfigEntry,
                Name("Variable Trigger"),Default(0),
                MapValues(VariableTriggerMap));
CDI_GROUP_ENTRY(source,openlcb::Uint8ConfigEntry,
                Name("Variable Source"),Default(0),
                MapValues(VariableSourceMap));
CDI_GROUP_ENTRY(trackspeed,openlcb::Uint8ConfigEntry,
                Name("Variable Track Speed"),Default(0),
                MapValues(TrackSpeedMap));
CDI_GROUP_ENTRY(eventtrue,openlcb::EventConfigEntry,
                Name("(C) Event to set variable true."));
CDI_GROUP_ENTRY(eventfalse,openlcb::EventConfigEntry,
                Name("(C) Event to set variable false."));
CDI_GROUP_END();

CDI_GROUP(LogicOperatorConfig);
CDI_GROUP_ENTRY(logicFunction,openlcb::Uint8ConfigEntry,
                Name("Logic function"),Default(0),
                MapValues(LogicFunctionMap));
CDI_GROUP_END();



CDI_GROUP(TimingConfig)
CDI_GROUP_ENTRY(timedelay,openlcb::Uint16ConfigEntry,
                Description("Time Delay before action\n"
                            "Delay Time (1-60000)."),
                Default(0),Min(0),Max(60000));
CDI_GROUP_ENTRY(interval,openlcb::Uint8ConfigEntry,
                Name("Interval"),Default(0),MapValues(IntervalMap));
CDI_GROUP_ENTRY(retriggerable,openlcb::Uint8ConfigEntry,
                Name("Retriggerable"),Default(0),
                MapValues(RetriggerableMap));
CDI_GROUP_END();

CDI_GROUP(ActionConfig);
CDI_GROUP_ENTRY(actiontrigger,openlcb::Uint8ConfigEntry,
                MapValues(ActionTriggerMap),Default(0));
CDI_GROUP_ENTRY(actionevent,openlcb::EventConfigEntry,
                Name("(P) this event will be sent."));
CDI_GROUP_END();

using ActionGroup = openlcb::RepeatedGroup<ActionConfig,4>;

/// CDI Configuration for a @ref Logic
CDI_GROUP(LogicConfig);
CDI_GROUP_ENTRY(description,openlcb::StringConfigEntry<32>,
                Name("Logic description"));
CDI_GROUP_ENTRY(groupFunction,openlcb::Uint8ConfigEntry,
                Name("Group Function"),Default(0),
                MapValues(GroupFunctionMap));
CDI_GROUP_ENTRY(v1,VariableConfig,Name("Variable #1"));
CDI_GROUP_ENTRY(logic,LogicOperatorConfig);
CDI_GROUP_ENTRY(v2,VariableConfig,Name("Variable #2"));
CDI_GROUP_ENTRY(trueAction,openlcb::Uint8ConfigEntry,
                Name("Action when Conditional = True"),
                Default(0),
                MapValues(ActionMap));
CDI_GROUP_ENTRY(falseAction,openlcb::Uint8ConfigEntry,
                Name("Action when Conditional = False"),
                Default(3),
                MapValues(ActionMap));
CDI_GROUP_ENTRY(timing,TimingConfig);
CDI_GROUP_ENTRY(actions,ActionGroup,Name("A trigger or change will generate the following events."),RepName("Action"));
CDI_GROUP_END();



#endif // __LOGICCONFIG_HXX

