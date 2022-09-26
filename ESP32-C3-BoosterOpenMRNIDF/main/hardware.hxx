// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Sep 25 18:43:26 2022
//  Last Modified : <220925.2015>
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

#include <freertos_drivers/arduino/DummyGPIO.hxx>
#include <freertos_drivers/esp32/Esp32Gpio.hxx>
#include <os/Gpio.hxx>
#include <utils/GpioInitializer.hxx>
#include "ADCWrapper.hxx"


#define MAIN_MAX_MILLIAMPS 2800
#define MAIN_LIMIT_MILLIAMPS 2800
ADC_PIN(CurrentSense, ADC1_CHANNEL_0, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12);
typedef ADCWrapper<CurrentSense_Pin> CurrentSense;
// Computed experimentally.
#define CurrentFromAIN(val) ((val)*.002185075)
ADC_PIN(TempSense, ADC1_CHANNEL_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12);
typedef ADCWrapper<TempSense_Pin> TempSense;
// MCP9700T-E/LT: .5V == 0C, .01V/Degree C, 125C == 1.75V, 1.9V Vref,
// 12bit ADC (0 <= val <= 4095).
#define TempFromAIN(val) ((((val)*.0004639448)/.01)+(-50))


#define FANPin GpioOutputSafeLow
#define EnablePin GpioOutputSafeHigh

GPIO_PIN(FAN_Control, FANPin, 2); // high is fan on
GPIO_PIN(Brake, EnablePin, 5); // low is track power on

#define ActPin GpioOutputSafeHighInvert

GPIO_PIN(LED_ACT1, ActPin, 8); 
GPIO_PIN(LED_ACT2, ActPin, 10);

// Create an initializer that can initialize all the GPIO pins in one shot
typedef GpioInitializer<FAN_Control_Pin, Brake_Pin, LED_ACT1_Pin, 
                        LED_ACT2_Pin> GpioInit;

/// GPIO Pin connected to the TWAI (CAN) Transceiver RX pin.
static constexpr gpio_num_t CONFIG_TWAI_RX_PIN = GPIO_NUM_7;

/// GPIO Pin connected to the TWAI (CAN) Transceiver TX pin.
static constexpr gpio_num_t CONFIG_TWAI_TX_PIN = GPIO_NUM_6;



#endif // __HARDWARE_HXX

