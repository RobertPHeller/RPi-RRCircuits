// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Jun 25 08:52:20 2022
//  Last Modified : <220823.1141>
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

static const char rcsid[] = "@(#) : $Id$";

#include "sdkconfig.h"
#include "hardware.hxx"

#include <bootloader_hal.h>
#include <freertos_drivers/esp32/Esp32BootloaderHal.hxx>

extern "C"
{
/// Initializes the node specific bootloader hardware (LEDs)
void bootloader_hw_set_to_safe(void)
{
    LOG(VERBOSE, "[Bootloader] bootloader_hw_set_to_safe");
    LED_ACT1_Pin::hw_init();
    //LED_ACT2_Pin::hw_init();
}

/// Verifies that the bootloader has been requested.
///
/// @return true (always).
///
/// NOTE: On the ESP32 this defaults to always return true since this code will
/// not be invoked through normal node startup.
bool request_bootloader(void)
{
    LOG(VERBOSE, "[Bootloader] request_bootloader");
    // Default to allow bootloader to run since we are not entering the
    // bootloader loop unless requested by app_main.
    return true;
}

/// Updates the state of a status LED.
///
/// @param led is the LED to update.
/// @param value is the new state of the LED.
///
/// NOTE: Currently the following mapping is being used for the LEDs:
/// LED_ACTIVE -> Activity LED
/// LED_WRITING -> WiFi LED
/// LED_REQUEST -> Used only as a hook for printing bootloader startup.
void bootloader_led(enum BootloaderLed led, bool value)
{
    LOG(VERBOSE, "[Bootloader] bootloader_led(%d, %d)", led, value);
    if (led == LED_ACTIVE)
    {
        LED_ACT1_Pin::instance()->write(value);
    }
    else if (led == LED_WRITING)
    {
        //LED_ACT2_Pin::instance()->write(value);
    }
    else if (led == LED_REQUEST)
    {
        LOG(INFO, "[Bootloader] Preparing to receive firmware");
        //LOG(INFO, "[Bootloader] Current partition: %s", current->label);
        //LOG(INFO, "[Bootloader] Target partition: %s", target->label);
    }
}

} // extern "C"

/// Starts the ESP32 Bootloader "lean" stack.
///
/// @param id is the node identifier to use.
void start_bootloader_stack(uint64_t id)
{
    esp32_bootloader_run(config.node_id, CONFIG_TWAI_TX_PIN, CONFIG_TWAI_RX_PIN, true);
}

