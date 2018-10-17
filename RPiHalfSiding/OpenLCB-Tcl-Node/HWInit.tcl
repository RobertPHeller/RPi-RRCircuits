#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Tue Oct 9 14:03:55 2018
#  Last Modified : <181017.1341>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
#			51 Locke Hill Road
#			Wendell, MA 01379-9728
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# 
#
#*****************************************************************************


package require GpioPin

gpiopin::MotorPin create M0_Control -pinconstructor gpiopins::GPIOPin \
      -pinnumber 17 -description "Motor 0 Control"
gpiopin::MotorPin create M1_Control -pinconstructor gpiopins::GPIOPin \
      -pinnumber 18 -description "Motor 1 Control"
gpiopin::InputPin create M0_Sense -pinconstructor gpiopins::GPIOPin \
      -pinnumber 27 -pinpullmode tri \
      -debouncerclass "debouncer::NullDebouncer" \
      -description "Motor 0 Point Sense" -createproducer yes
gpiopin::InputPin create M1_Sense -pinconstructor gpiopins::GPIOPin \
      -pinnumber 22 -pinpullmode tri \
      -debouncerclass "debouncer::NullDebouncer" \
      -description "Motor 1 Point Sense" -createproducer yes

gpiopin::InputPin create Occupancy -pinconstructor gpiopins::GPIOPin \
      -pinnumber 23 -pinpullmode tri -createproducer yes \
      -description "OS Occupancy Detector"
gpiopin::InputPin create EastOcc -pinconstructor gpiopins::GPIOPin \
      -pinnumber 24 -pinpullmode tri \
      -description "East (Points) Occupancy Detector"
gpiopin::InputPin create WestMain -pinconstructor gpiopins::GPIOPin \
      -pinnumber 25 -pinpullmode tri \
      -description "West Main (Frog Normal) Occupancy Detector"
gpiopin::InputPin create WestDiverg -pinconstructor gpiopins::GPIOPin \
      -pinnumber 4 -pinpullmode tri \
      -description "West Diverg (Frog Reversed) Occupancy Detector"

mcp23017gpiopins::MCP23017GPIOPin Init 64 7

gpiopin::LEDPin create PointsHighGreen \
      -pinconstructor mcp23017gpiopins::MCP23017GPIOPin \
      -pinnumber [expr {0 + 64}] \
      -description "Points High Green Lamp"
gpiopin::LEDPin create PointsHighYellow \
      -pinconstructor mcp23017gpiopins::MCP23017GPIOPin \
      -pinnumber [expr {1 + 64}] \
      -description "Points High Yellow Lamp"
gpiopin::LEDPin create PointsHighRed \
      -pinconstructor mcp23017gpiopins::MCP23017GPIOPin \
      -pinnumber [expr {2 + 64}] \
      -description "Points High Red Lamp"
gpiopin::LEDPin create PointsLowYellow \
      -pinconstructor mcp23017gpiopins::MCP23017GPIOPin \
      -pinnumber [expr {3 + 64}] \
      -description "Points Low Yellow Lamp"
gpiopin::LEDPin create PointsLowRed \
      -pinconstructor mcp23017gpiopins::MCP23017GPIOPin \
      -pinnumber [expr {4 + 64}] \
      -description "Points Low Red Lamp"

gpiopin::LEDPin create FrogMainGreen \
      -pinconstructor mcp23017gpiopins::MCP23017GPIOPin \
      -pinnumber [expr {5 + 64}] \
      -description "Frog Main Green Lamp"
gpiopin::LEDPin create FrogMainYellow \
      -pinconstructor mcp23017gpiopins::MCP23017GPIOPin \
      -pinnumber [expr {6 + 64}] \
      -description "Frog Main Yellow Lamp"
gpiopin::LEDPin create FrogMainRed \
      -pinconstructor mcp23017gpiopins::MCP23017GPIOPin \
      -pinnumber [expr {7 + 64}] \
      -description "Frog Main Red Lamp"

gpiopin::LEDPin create FrogDivGreen \
      -pinconstructor mcp23017gpiopins::MCP23017GPIOPin \
      -pinnumber [expr {8 + 64}] \
      -description "Frog Div Green Lamp"
gpiopin::LEDPin create FrogDivYellow \
      -pinconstructor mcp23017gpiopins::MCP23017GPIOPin \
      -pinnumber [expr {9 + 64}] \
      -description "Frog Div Yellow Lamp"
gpiopin::LEDPin create FrogDivRed \
      -pinconstructor mcp23017gpiopins::MCP23017GPIOPin \
      -pinnumber [expr {10 + 64}] \
      -description "Frog Div Red Lamp"

set SerialPort "/dev/ttyAMA0"

package provide HWInit 1.0
