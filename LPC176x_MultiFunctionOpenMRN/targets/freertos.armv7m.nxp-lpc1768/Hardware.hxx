// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Sep 26 13:54:20 2022
//  Last Modified : <220926.1705>
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
GPIO_PIN(Motor3, MotorPin, 0, 21);
GPIO_PIN(Motor4, MotorPin, 0, 19);
GPIO_PIN(Motor5, MotorPin, 0, 17);
GPIO_PIN(Motor6, MotorPin, 0, 16);
GPIO_PIN(Motor7, MotorPin, 2,  7);
GPIO_PIN(Motor8, MotorPin, 2,  6);

GPIO_PIN(Points1, SensePin, 2, 11);
GPIO_PIN(Points2, SensePin, 0, 22);
GPIO_PIN(Points3, SensePin, 0, 20);
GPIO_PIN(Points4, SensePin, 0, 18);
GPIO_PIN(Points5, SensePin, 0, 15);
GPIO_PIN(Points6, SensePin, 2,  9);
GPIO_PIN(Points7, SensePin, 2,  7);
GPIO_PIN(Points8, SensePin, 2,  5);

#define ODPin GpioInputNP

GPIO_PIN(OC1, ODPin, 2, 4);
GPIO_PIN(OC2, ODPin, 2, 3);
GPIO_PIN(OC3, ODPin, 2, 2);
GPIO_PIN(OC4, ODPin, 2, 1);
GPIO_PIN(OC5, ODPin, 2, 0);
GPIO_PIN(OC6, ODPin, 0, 9);
GPIO_PIN(OC7, ODPin, 0, 8);
GPIO_PIN(OC8, ODPin, 0, 7);

#define LEDPin GpioOutputSafeLow

GPIO_PIN(LED1, LEDPin, 0,  6);
GPIO_PIN(LED2, LEDPin, 0,  5);
GPIO_PIN(LED3, LEDPin, 0,  4);
GPIO_PIN(LED4, LEDPin, 4, 28);
GPIO_PIN(LED5, LEDPin, 4, 29);
GPIO_PIN(LED6, LEDPin, 1, 17);
GPIO_PIN(LED7, LEDPin, 1, 16);
GPIO_PIN(LED8, LEDPin, 1, 15);

#define ButtonPin GpioInputNP

GPIO_PIN(Button1, ButtonPin, 1, 14);
GPIO_PIN(Button2, ButtonPin, 1, 10);
GPIO_PIN(Button3, ButtonPin, 1,  9);
GPIO_PIN(Button4, ButtonPin, 1,  8);
GPIO_PIN(Button5, ButtonPin, 1,  4);
GPIO_PIN(Button6, ButtonPin, 1,  1);
GPIO_PIN(Button7, ButtonPin, 1,  0);
GPIO_PIN(Button8, ButtonPin, 0,  2);

#define ACTPin GpioOutputSafeHigh

GPIO_PIN(ACT1, ACTPin, 0, 28);
GPIO_PIN(ACT2, ACTPin, 0, 27);

// Create an initializer that can initialize all the GPIO pins in one shot
typedef GpioInitializer<Motor1_Pin, Motor2_Pin, Motor3_Pin, Motor4_Pin, 
                        Motor5_Pin, Motor6_Pin, Motor7_Pin, Motor8_Pin,
                        Points1_Pin, Points2_Pin, Points3_Pin, Points4_Pin, 
                        Points5_Pin, Points6_Pin, Points7_Pin, Points8_Pin, 
                        OC1_Pin, OC2_Pin, OC3_Pin, OC4_Pin, OC5_Pin, 
                        OC6_Pin, OC7_Pin, OC8_Pin, LED1_Pin, LED2_Pin,
                        LED3_Pin, LED4_Pin, LED5_Pin, LED6_Pin, 
                        LED7_Pin, LED8_Pin, Button1_Pin, Button2_Pin, 
                        Button3_Pin, Button4_Pin, Button5_Pin, 
                        Button6_Pin, Button7_Pin, Button8_Pin, 
                        ACT1_Pin, ACT2_Pin> GpioInit;


#define PWMCHIP_ADDRESS1 0x40
#define PWMCHIP_ADDRESS2 0x41

#endif // __HARDWARE_HXX

