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
//  Last Modified : <200121.1438>
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

#define HARDWARE_IMPL "RPi QuadSMCSense HAT"

// On chip GPIO:

#define MotorPin GpioOutputSafeLow
#define SensePin GpioInputActiveHigh

GPIO_PIN(Motor1, MotorPin, 17);
GPIO_PIN(Motor2, MotorPin, 18);
GPIO_PIN(Motor3, MotorPin, 23);
GPIO_PIN(Motor4, MotorPin, 24);

GPIO_PIN(Points1, SensePin, 27);
GPIO_PIN(Points2, SensePin, 22);
GPIO_PIN(Points3, SensePin, 25);
GPIO_PIN(Points4, SensePin, 4);

typedef GpioInitializer<Motor1_Pin, Motor2_Pin, Motor3_Pin, 
                        Motor4_Pin, Points1_Pin, Points2_Pin, 
                        Points3_Pin, Points4_Pin> GpioInit;



#define USE_OPENLCB_TCP_HOST
//#define USE_GRIDCONNECT_HOST
//#define DEFAULT_TCP_GRIDCONNECT_HOST "fruitloops"
//#define DEFAULT_TCP_GRIDCONNECT_PORT 12021
#define DEFAULT_OPENLCB_TCP_HOST "localhost"
#define DEFAULT_OPENLCB_TCP_PORT 12000

//#define PRINT_ALL_PACKETS
//#define HAVE_SOCKET_CAN_PORT
//#define SOCKET_CAN_PORT "vcan0"

#endif // __HARDWARE_HXX

