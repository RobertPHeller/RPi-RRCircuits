// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Oct 17 13:05:06 2018
//  Last Modified : <181020.1840>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
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

#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "RPi Halfsiding HAT"

// On chip GPIO:

#define MotorPin GpioOutputSafeLow
#define SensePin GpioInputActiveHigh
#define ODPin  GpioInputActiveLow
#define LEDPin GpioOutputSafeLow

GPIO_PIN(Motor1, MotorPin, 17);
GPIO_PIN(Motor2, MotorPin, 18);
GPIO_PIN(Points1, SensePin, 27);
GPIO_PIN(Points2, SensePin, 22);


GPIO_PIN(Occupancy, ODPin, 23);
GPIO_PIN(EastPoints, ODPin, 24);
GPIO_PIN(WestMain, ODPin, 25);
GPIO_PIN(WestDiverg, ODPin, 4);

// Assumes Kernel config CONFIG_ARCH_NR_GPIO is 512!
#define GPIO_BASE 512

// MCP230xx expanders.  Assumes this order for dtoverlay calls:
//
// dtoverlay mcp23017 addr=0x27
//
// mcp23017 @ 0x27 == Signals 
//
//   GPIO_0.0    PointsHighGreen
//   GPIO_0.1    PointsHighYellow
//   GPIO_0.2    PointsHighRed
//   GPIO_0.3    PointsLowYellow
//   GPIO_0.4    PointsLowRed
//   GPIO_0.5    FrogMainGreen
//   GPIO_0.6    FrogMainYellow
//   GPIO_0.7    FrogMainRed
//   GPIO_1.0    FrogDivGreen
//   GPIO_1.1    FrogDivYellow
//   GPIO_1.2    FrogDivRed
//   GPIO_1.3    N/C
//   GPIO_1.4    N/C
//   GPIO_1.5    N/C
//   GPIO_1.6    N/C
//   GPIO_1.7    N/C

#define MCP23017RPiHalfSidingBase ((GPIO_BASE)-16)

GPIO_PIN(PointsHighGreen, LEDPin, MCP23017RPiHalfSidingBase+(0*8)+0);
GPIO_PIN(PointsHighYellow, LEDPin, MCP23017RPiHalfSidingBase+(0*8)+1);
GPIO_PIN(PointsHighRed, LEDPin, MCP23017RPiHalfSidingBase+(0*8)+2);
GPIO_PIN(PointsLowYellow, LEDPin, MCP23017RPiHalfSidingBase+(0*8)+3);
GPIO_PIN(PointsLowRed, LEDPin, MCP23017RPiHalfSidingBase+(0*8)+4);
GPIO_PIN(FrogMainGreen, LEDPin, MCP23017RPiHalfSidingBase+(0*8)+5);
GPIO_PIN(FrogMainYellow, LEDPin, MCP23017RPiHalfSidingBase+(0*8)+6);
GPIO_PIN(FrogMainRed, LEDPin, MCP23017RPiHalfSidingBase+(0*8)+7);
GPIO_PIN(FrogDivGreen, LEDPin, MCP23017RPiHalfSidingBase+(1*8)+0);
GPIO_PIN(FrogDivYellow, LEDPin, MCP23017RPiHalfSidingBase+(1*8)+1);
GPIO_PIN(FrogDivRed, LEDPin, MCP23017RPiHalfSidingBase+(1*8)+2);

typedef GpioInitializer<Motor1_Pin, Motor2_Pin, Points1_Pin, 
                        Points2_Pin, Occupancy_Pin, EastPoints_Pin,
                        WestMain_Pin, WestDiverg_Pin, 
                        PointsHighGreen_Pin, PointsHighYellow_Pin,
                        PointsHighRed_Pin, PointsLowYellow_Pin,
                        PointsLowRed_Pin, FrogMainGreen_Pin,
                        FrogMainYellow_Pin, FrogMainRed_Pin,
                        FrogDivGreen_Pin, FrogDivYellow_Pin,
                        FrogDivRed_Pin> GpioInit;

#define ABSSlaveBus_Serial "/dev/ttyAMA0"

#define HAVE_TCP_GRIDCONNECT_HOST
#define TCP_GRIDCONNECT_HOST "localhost"
#define TCP_GRIDCONNECT_PORT 12021

//#define PRINT_ALL_PACKETS
//#define HAVE_SOCKET_CAN_PORT
//#define SOCKET_CAN_PORT "vcan0"


#endif // __HARDWARE_HXX

