// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Jun 7 13:57:51 2023
//  Last Modified : <250824.2048>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2023  Robert Heller D/B/A Deepwoods Software
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

#define HARDWARE_IMPL "IO CAPE"

// On chip GPIO:
//
// OUTs (J1)
#define OUTPin GpioOutputSafeLow
GPIO_PIN(OUT1,  OUTPin, 115);
GPIO_PIN(OUT2,  OUTPin, 49);
GPIO_PIN(OUT3,  OUTPin, 112);
GPIO_PIN(OUT4,  OUTPin, 117);
GPIO_PIN(OUT5,  OUTPin, 47);
GPIO_PIN(OUT6,  OUTPin, 45);
GPIO_PIN(OUT7,  OUTPin, 46);
GPIO_PIN(OUT8,  OUTPin, 44);

#define NUMOUTS 8

//
// INs  (J2)
#define INPin GpioInputActiveLow
GPIO_PIN(IN1,  INPin, 50);
GPIO_PIN(IN2,  INPin, 60);
GPIO_PIN(IN3,  INPin, 51);
GPIO_PIN(IN4,  INPin, 48);
GPIO_PIN(IN5,  INPin, 69);
GPIO_PIN(IN6,  INPin, 66);
GPIO_PIN(IN7,  INPin, 68);
GPIO_PIN(IN8,  INPin, 67);

#define NUMINS 8


typedef GpioInitializer<OUT1_Pin, OUT2_Pin, OUT3_Pin, OUT4_Pin,
OUT5_Pin, OUT6_Pin, OUT7_Pin, OUT8_Pin,
IN1_Pin, IN2_Pin, IN3_Pin, IN4_Pin, IN5_Pin, IN6_Pin, IN7_Pin, 
IN8_Pin> GpioInit;

#define USE_SOCKET_CAN_PORT
#define DEFAULT_CAN_SOCKET "can1"

#define START_GCTCP_HUB
#define DEFAULT_GRIDCONNECT_HUB_PORT 12021

//#define USE_GRIDCONNECT_HOST
//#define DEFAULT_TCP_GRIDCONNECT_PORT 12021
//#define DEFAULT_TCP_GRIDCONNECT_HOST "localhost"

#endif // __HARDWARE_HXX

