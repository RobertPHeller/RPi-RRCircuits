// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Sep 12 14:58:29 2022
//  Last Modified : <220917.1029>
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

#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <freertos_drivers/nxp/Lpc17xx40xxGPIO.hxx>
#include <os/Gpio.hxx>
#include <utils/GpioInitializer.hxx>

#include "HardwareDefs.hxx"

#define INPUT_PIN GpioInputPU

GPIO_PIN(C0P0B0, INPUT_PIN, 3, 26);
GPIO_PIN(C0P0B1, INPUT_PIN, 3, 25);
GPIO_PIN(C0P0B2, INPUT_PIN, 0, 29);
GPIO_PIN(C0P0B3, INPUT_PIN, 0, 30);
GPIO_PIN(C0P0B4, INPUT_PIN, 1, 18);
GPIO_PIN(C0P0B5, INPUT_PIN, 1, 19);
GPIO_PIN(C0P0B6, INPUT_PIN, 1, 20);
GPIO_PIN(C0P0B7, INPUT_PIN, 1, 21);
GPIO_PIN(C0P1B0, INPUT_PIN, 1, 22);
GPIO_PIN(C0P1B1, INPUT_PIN, 1, 23);
GPIO_PIN(C0P1B2, INPUT_PIN, 1, 24);
GPIO_PIN(C0P1B3, INPUT_PIN, 1, 25);
GPIO_PIN(C0P1B4, INPUT_PIN, 1, 26);
GPIO_PIN(C0P1B5, INPUT_PIN, 1, 27);
GPIO_PIN(C0P1B6, INPUT_PIN, 1, 18);
GPIO_PIN(C0P1B7, INPUT_PIN, 1, 19);
GPIO_PIN(C0P2B0, INPUT_PIN, 0, 10);
GPIO_PIN(C0P2B1, INPUT_PIN, 0, 11);
GPIO_PIN(C0P2B2, INPUT_PIN, 2, 13);
GPIO_PIN(C0P2B3, INPUT_PIN, 2, 12);
GPIO_PIN(C0P2B4, INPUT_PIN, 2, 11);
GPIO_PIN(C0P2B5, INPUT_PIN, 2, 10);
GPIO_PIN(C0P2B6, INPUT_PIN, 0, 22);
GPIO_PIN(C0P2B7, INPUT_PIN, 0, 21);


#define OUTPUT_PIN GpioOutputSafeLow

GPIO_PIN(C1P0B0, OUTPUT_PIN, 4, 29);
GPIO_PIN(C1P0B1, OUTPUT_PIN, 4, 28);
GPIO_PIN(C1P0B2, OUTPUT_PIN, 0, 4);
GPIO_PIN(C1P0B3, OUTPUT_PIN, 0, 5);
GPIO_PIN(C1P0B4, OUTPUT_PIN, 0, 6);
GPIO_PIN(C1P0B5, OUTPUT_PIN, 0, 7);
GPIO_PIN(C1P0B6, OUTPUT_PIN, 0, 8);
GPIO_PIN(C1P0B7, OUTPUT_PIN, 0, 9);
GPIO_PIN(C1P1B0, OUTPUT_PIN, 2, 0);
GPIO_PIN(C1P1B1, OUTPUT_PIN, 2, 1);
GPIO_PIN(C1P1B2, OUTPUT_PIN, 2, 2);
GPIO_PIN(C1P1B3, OUTPUT_PIN, 2, 3);
GPIO_PIN(C1P1B4, OUTPUT_PIN, 2, 4);
GPIO_PIN(C1P1B5, OUTPUT_PIN, 2, 5);
GPIO_PIN(C1P1B6, OUTPUT_PIN, 2, 6);
GPIO_PIN(C1P1B7, OUTPUT_PIN, 2, 7);
GPIO_PIN(C1P2B0, OUTPUT_PIN, 2, 8);
GPIO_PIN(C1P2B1, OUTPUT_PIN, 2, 9);
GPIO_PIN(C1P2B2, OUTPUT_PIN, 0, 16);
GPIO_PIN(C1P2B3, OUTPUT_PIN, 0, 15);
GPIO_PIN(C1P2B4, OUTPUT_PIN, 0, 17);
GPIO_PIN(C1P2B5, OUTPUT_PIN, 0, 18);
GPIO_PIN(C1P2B6, OUTPUT_PIN, 0, 19);
GPIO_PIN(C1P2B7, OUTPUT_PIN, 0, 20);

#define ACTLed_PIN GpioOutputSafeHigh

GPIO_PIN(ACT1, ACTLed_PIN, 0, 28);
GPIO_PIN(ACT2, ACTLed_PIN, 0, 27);

typedef GpioInitializer<C0P0B0_Pin, C0P0B1_Pin, C0P0B2_Pin, C0P0B3_Pin,
                        C0P0B4_Pin, C0P0B5_Pin, C0P0B6_Pin, C0P0B7_Pin,
                        C0P1B0_Pin, C0P1B1_Pin, C0P1B2_Pin, C0P1B3_Pin,
                        C0P1B4_Pin, C0P1B5_Pin, C0P1B6_Pin, C0P1B7_Pin,
                        C0P2B0_Pin, C0P2B1_Pin, C0P2B2_Pin, C0P2B3_Pin,
                        C0P2B4_Pin, C0P2B5_Pin, C0P2B6_Pin, C0P2B7_Pin,
                        C1P0B0_Pin, C1P0B1_Pin, C1P0B2_Pin, C1P0B3_Pin,
                        C1P0B4_Pin, C1P0B5_Pin, C1P0B6_Pin, C1P0B7_Pin,
                        C1P1B0_Pin, C1P1B1_Pin, C1P1B2_Pin, C1P1B3_Pin,
                        C1P1B4_Pin, C1P1B5_Pin, C1P1B6_Pin, C1P1B7_Pin,
                        C1P2B0_Pin, C1P2B1_Pin, C1P2B2_Pin, C1P2B3_Pin,
                        C1P2B4_Pin, C1P2B5_Pin, C1P2B6_Pin, C1P2B7_Pin,
                        ACT1_Pin, ACT2_Pin> GpioInit;


#endif // __HARDWARE_HXX

