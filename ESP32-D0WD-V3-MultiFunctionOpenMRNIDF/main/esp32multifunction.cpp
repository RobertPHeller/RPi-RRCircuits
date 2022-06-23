// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Jun 23 12:17:40 2022
//  Last Modified : <220623.1345>
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
#include "fs.hxx"
#include "hardware.hxx"
#include "NodeRebootHelper.hxx"
#include "nvs_config.hxx"

#include <algorithm>
#include <driver/i2c.h>
#include <driver/uart.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_ota_ops.h>
#include <esp_system.h>
#include <esp_task_wdt.h>
#include <esp32/rom/rtc.h>
#include <freertos_includes.h>   
#include <openlcb/SimpleStack.hxx>

///////////////////////////////////////////////////////////////////////////////
// Increase the CAN RX frame buffer size to reduce overruns when there is high
// traffic load (ie: large datagram transport).
///////////////////////////////////////////////////////////////////////////////
OVERRIDE_CONST(can_rx_buffer_size, 64);

namespace esp32multifunction
{

void start_openlcb_stack(node_config_t *config, bool reset_events
                       , bool brownout_detected);

} // namespace esp32multifunction

void start_bootloader_stack(uint64_t node_id);

/// Halts execution with a specific blink pattern for the two LEDs that are on
/// the IO base board.
///
/// @param activity1 Sets the initial state of the Activity1 (Green) LED.
/// @param activity2 Sets the initial state of the Activity2 (Red)   LED.
/// @param period Sets the delay between blinking the LED(s).
/// @param toggle_both Controls if both LEDs will blink or if only the activity
/// LED will blink.
void die_with(bool activity1, bool activity2, unsigned period = 1000
            , bool toggle_both = false)
{
    LED_ACT1_Pin::set(activity1);
    LED_ACT2_Pin::set(activity2);

    while(true)
    {
        if (toggle_both)
        {
            LED_ACT1_Pin::toggle();
        }
        LED_ACT2_Pin::toggle();
        usleep(period);
    }
}


extern "C"
{

void *node_reboot(void *arg)
{
    Singleton<esp32multifunction::NodeRebootHelper>::instance()->reboot();
    return nullptr;
}

void reboot()
{
    os_thread_create(nullptr, nullptr, uxTaskPriorityGet(NULL) + 1, 2048
                   , node_reboot, nullptr);
}

ssize_t os_get_free_heap()
{
    return heap_caps_get_free_size(MALLOC_CAP_8BIT);
}

static const char * const reset_reasons[] =
{
    "unknown",                  // NO_MEAN                  0
    "power on reset",           // POWERON_RESET            1
    "unknown",                  // no key                   2
    "software reset",           // SW_RESET                 3
    "watchdog reset (legacy)",  // OWDT_RESET               4
    "deep sleep reset",         // DEEPSLEEP_RESET          5
    "reset (SLC)",              // SDIO_RESET               6
    "watchdog reset (group0)",  // TG0WDT_SYS_RESET         7
    "watchdog reset (group1)",  // TG1WDT_SYS_RESET         8
    "RTC system reset",         // RTCWDT_SYS_RESET         9
    "Intrusion test reset",     // INTRUSION_RESET          10
    "WDT Timer group reset",    // TGWDT_CPU_RESET          11
    "software reset (CPU)",     // SW_CPU_RESET             12
    "RTC WDT reset",            // RTCWDT_CPU_RESET         13
    "software reset (CPU)",     // EXT_CPU_RESET            14
    "Brownout reset",           // RTCWDT_BROWN_OUT_RESET   15
    "RTC Reset (Normal)",       // RTCWDT_RTC_RESET         16
};


void app_main()
{
    // capture the reason for the CPU reset
    uint8_t reset_reason = rtc_get_reset_reason(PRO_CPU_NUM);
    uint8_t orig_reset_reason = reset_reason;
    // Ensure the reset reason it within bounds.
    if (reset_reason > ARRAYSIZE(reset_reasons))
    {
        reset_reason = 0;
    }
    // silence all but error messages by default
    esp_log_level_set("*", ESP_LOG_ERROR);

    GpioInit::hw_init();

    const esp_app_desc_t *app_data = esp_ota_get_app_description();
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    LOG(INFO, "\n\n%s %s starting up (%d:%s)...", app_data->project_name
      , app_data->version, reset_reason, reset_reasons[reset_reason]);
    LOG(INFO
      , "[SoC] model:%s, rev:%d, cores:%d, flash:%s, WiFi:%s, BLE:%s, BT:%s"
      , chip_info.model == CHIP_ESP32 ? "ESP32" :
        chip_info.model == CHIP_ESP32S2 ? "ESP32-S2" : "unknown"
      , chip_info.revision, chip_info.cores
      , chip_info.features & CHIP_FEATURE_EMB_FLASH ? "Yes" : "No"
      , chip_info.features & CHIP_FEATURE_WIFI_BGN ? "Yes" : "No"
      , chip_info.features & CHIP_FEATURE_BLE ? "Yes" : "No"
      , chip_info.features & CHIP_FEATURE_BT ? "Yes" : "No");
    LOG(INFO, "[SoC] Heap: %.2fkB / %.2fKb"
      , heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / 1024.0f
      , heap_caps_get_total_size(MALLOC_CAP_INTERNAL) / 1024.0f);
    LOG(INFO, "Compiled on %s %s using ESP-IDF %s", app_data->date
      , app_data->time, app_data->idf_ver);
    LOG(INFO, "Running from: %s", esp_ota_get_running_partition()->label);
    LOG(INFO, "%s uses the OpenMRN library\n"
              "Copyright (c) 2019-2020, OpenMRN\n"
              "All rights reserved.", app_data->project_name);
    if (reset_reason != orig_reset_reason)
    {
        LOG(WARNING, "Reset reason mismatch: %d vs %d", reset_reason
          , orig_reset_reason);
    }
    nvs_init();

    // load non-CDI based config from NVS.
    bool cleanup_config_tree = false;
    node_config_t config;
    if (load_config(&config) != ESP_OK)
    {
        default_config(&config);
        cleanup_config_tree = true;
    }
    bool reset_events = false;
    bool run_bootloader = false;
    
    // Ensure the LEDs are both OFF when we startup.
    LED_ACT1_Pin::instance()->clr();
    LED_ACT2_Pin::instance()->clr();
    
    // Check for and reset factory reset flag.
    if (config.force_reset)
    {
        cleanup_config_tree = true;
        config.force_reset = false;
        save_config(&config);
    }

    if (config.bootloader_req)
    {
        run_bootloader = true;
        // reset the flag so we start in normal operating mode next time.
        config.bootloader_req = false;
        save_config(&config);
    }
    
    if (config.reset_events_req)
    {
        reset_events = true;
        // reset the flag so we start in normal operating mode next time.
        config.reset_events_req = false;
        save_config(&config);
    }

    dump_config(&config);

    if (run_bootloader)
    {
        start_bootloader_stack(config.node_id);
    }
    else
    {
        mount_fs(cleanup_config_tree);
        esp32multifunction::start_openlcb_stack(&config, reset_events
                                                , reset_reason == RTCWDT_BROWN_OUT_RESET
                                                );
    }

    // At this point the OpenMRN stack is running in it's own task and we can
    // safely exit from this one. We do not need to cleanup as that will be
    // handled automatically by ESP-IDF.
}

} // extern "C"
    
