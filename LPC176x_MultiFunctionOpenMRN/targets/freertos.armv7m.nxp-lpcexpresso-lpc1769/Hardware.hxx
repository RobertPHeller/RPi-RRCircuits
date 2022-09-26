// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Sep 26 14:44:22 2022
//  Last Modified : <220926.1504>
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

#define MotorPin GpioOutputSafeLow
#define SensePin GpioInputNP

GPIO_PIN(Motor1, MotorPin, 2, 12);
GPIO_PIN(Motor2, MotorPin, 2, 10);

GPIO_PIN(Points1, SensePin, 2, 11);
GPIO_PIN(Points2, SensePin, 2,  8);

#define ODPin GpioInputNP

GPIO_PIN(OC1, ODPin, 2, 7);
GPIO_PIN(OC2, ODPin, 2, 6);

#define LEDPin GpioOutputSafeLow

GPIO_PIN(LED1, LEDPin, 2,  5);
GPIO_PIN(LED2, LEDPin, 2,  4);

#define ButtonPin GpioInputNP

GPIO_PIN(Button1, ButtonPin, 2, 3);
GPIO_PIN(Button2, ButtonPin, 2, 2);

#define ACTPin GpioOutputSafeHigh

GPIO_PIN(ACT1, ACTPin, 2, 1);
GPIO_PIN(ACT2, ACTPin, 2, 0);

// Create an initializer that can initialize all the GPIO pins in one shot
typedef GpioInitializer<Motor1_Pin, Motor2_Pin, Points1_Pin, Points2_Pin, 
                        OC1_Pin, OC2_Pin, LED1_Pin, LED2_Pin,
                        Button1_Pin, Button2_Pin, ACT1_Pin, ACT2_Pin> GpioInit;


#endif // __HARDWARE_HXX

