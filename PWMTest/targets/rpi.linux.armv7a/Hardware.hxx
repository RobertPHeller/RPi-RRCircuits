// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Feb 14 13:03:37 2019
//  Last Modified : <190219.1928>
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

#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxPWM.hxx>
#include "utils/GpioInitializer.hxx"


#define HARDWARE_IMPL "Linux PWM Test"

constexpr uint32_t PWMCHIP = 0;
constexpr uint32_t PWMCHIP_CH1 = 0;
constexpr uint32_t PWMCHIP_CH2 = 1;

typedef GpioInitializer<> GpioInit;

#define HAVE_TCP_GRIDCONNECT_HOST
#define TCP_GRIDCONNECT_HOST "localhost"
#define TCP_GRIDCONNECT_PORT 12021


#define MyAddress 0x050101012270ULL; // 05 01 01 01 22 70

#endif // __HARDWARE_HXX

