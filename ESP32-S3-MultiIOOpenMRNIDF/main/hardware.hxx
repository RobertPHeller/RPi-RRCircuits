// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Jun 25 09:07:59 2022
//  Last Modified : <221008.1226>
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
#include "MCP23017Gpio.hxx"

//#define PRODUCTION

#ifdef PRODUCTION

#define Card2PortCPin GpioInputPU

GPIO_PIN(C2C0, Card2PortCPin, 10);
GPIO_PIN(C2C1, Card2PortCPin, 11);
GPIO_PIN(C2C2, Card2PortCPin, 12);
GPIO_PIN(C2C3, Card2PortCPin, 13);
GPIO_PIN(C2C4, Card2PortCPin, 14);
GPIO_PIN(C2C5, Card2PortCPin, 17);
GPIO_PIN(C2C6, Card2PortCPin, 18);
GPIO_PIN(C2C7, Card2PortCPin, 21);

#endif

#define ActPin GpioOutputSafeHighInvert

GPIO_PIN(LED_ACT1, ActPin, 15);
GPIO_PIN(LED_ACT2, ActPin, 0);

// Create an initializer that can initialize all the GPIO pins in one shot
typedef GpioInitializer<LED_ACT1_Pin, LED_ACT2_Pin
#ifdef PRODUCTION
                        , C2C0_Pin, C2C1_Pin, , C2C2_Pin, C2C3_Pin
                        , C2C4_Pin, C2C5_Pin, , C2C6_Pin, C2C7_Pin
#endif
                       > GpioInit;

/// GPIO Pin connected to the TWAI (CAN) Transceiver RX pin.
// ADC2_CHANNEL_0
static constexpr gpio_num_t CONFIG_TWAI_RX_PIN = GPIO_NUM_4;

/// GPIO Pin connected to the TWAI (CAN) Transceiver TX pin.
static constexpr gpio_num_t CONFIG_TWAI_TX_PIN = GPIO_NUM_5;

/// GPIO Pin used for I2C SDA.
static constexpr gpio_num_t CONFIG_SDA_PIN = GPIO_NUM_2;

/// GPIO Pin used for I2C SCL.
static constexpr gpio_num_t CONFIG_SCL_PIN = GPIO_NUM_3;

#endif // __HARDWARE_HXX

