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
//  Last Modified : <220928.0905>
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
#include "cdi.hxx"
#include "DelayRebootHelper.hxx"
#include "EventBroadcastHelper.hxx"
#include "FactoryResetHelper.hxx"
#include "HealthMonitor.hxx"
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
#include <esp32s3/rom/rtc.h>
#include <freertos_includes.h>   
#include <openlcb/SimpleStack.hxx>
#include <CDIXMLGenerator.hxx>
#include <freertos_drivers/esp32/Esp32HardwareTwai.hxx>
#include <freertos_drivers/esp32/Esp32BootloaderHal.hxx>
#include <freertos_drivers/esp32/Esp32SocInfo.hxx>
#include <openlcb/MemoryConfigClient.hxx>
#include <openlcb/RefreshLoop.hxx>
#include <openlcb/SimpleStack.hxx>
#include <utils/constants.hxx>
#include <utils/format_utils.hxx>

#include "BootPauseHelper.hxx"
#include "Lamp.hxx"
#include "Mast.hxx"
#include "Blink.hxx"
#include "TrackCircuit.hxx"
#include "Logic.hxx"
#include "Turnout.hxx"
#include "Points.hxx"
#include "OccupancyDetector.hxx"
#include "Button.hxx"
#include "LED.hxx"
#include "Esp32PCA9685PWM.hxx"


///////////////////////////////////////////////////////////////////////////////
// Increase the CAN RX frame buffer size to reduce overruns when there is high
// traffic load (ie: large datagram transport).
///////////////////////////////////////////////////////////////////////////////
OVERRIDE_CONST(can_rx_buffer_size, 64);

TrackCircuit *circuits[TRACKCIRCUITCOUNT];

esp32multifunction::ConfigDef cfg(0);
openmrn_arduino::Esp32PCA9685PWM pwmchip;
Esp32HardwareTwai twai(CONFIG_TWAI_RX_PIN, CONFIG_TWAI_TX_PIN);

namespace openlcb
{
    /// Name of CDI.xml to generate dynamically.
    const char CDI_FILENAME[] = "/fs/cdi.xml";

    // Path to where OpenMRN should persist general configuration data.
    const char *const CONFIG_FILENAME = "/fs/config";

    // The size of the memory space to export over the above device.
    const size_t CONFIG_FILE_SIZE =
        cfg.seg().size() + cfg.seg().offset();

    // Default to store the dynamic SNIP data is stored in the same persistant
    // data file as general configuration data.
    const char *const SNIP_DYNAMIC_FILENAME = "/fs/config";

    /// Defines the identification information for the node. The arguments are:
    ///
    /// - 4 (version info, always 4 by the standard
    /// - Manufacturer name
    /// - Model name
    /// - Hardware version
    /// - Software version
    ///
    /// This data will be used for all purposes of the identification:
    ///
    /// - the generated cdi.xml will include this data
    /// - the Simple Node Ident Info Protocol will return this data
    /// - the ACDI memory space will contain this data.
    const SimpleNodeStaticValues SNIP_STATIC_DATA =
    {
        4,
        SNIP_PROJECT_PAGE,
        SNIP_PROJECT_NAME,
        SNIP_HW_VERSION,
        SNIP_SW_VERSION
    };
    const char CDI_DATA[] = "";

} // namespace openlcb


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

DEFINE_SINGLETON_INSTANCE(BlinkTimer);
PWM* Lamp::pinlookup_[17];


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

/// Verifies that the bootloader has been requested.
///
/// @return true if bootloader_request is set to one, otherwise false.
bool request_bootloader(void)
{
    LOG(VERBOSE, "[Bootloader] request_bootloader");
    return bootloader_request;
}

/// Updates the state of a status LED.
///
/// @param led is the LED to update.
/// @param value is the new state of the LED.
///
/// NOTE: Currently the following mapping is being used for the LEDs:
/// LED_ACTIVE -> Bootloader LED
/// LED_WRITING -> Bootloader Write LED
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
        LED_ACT2_Pin::instance()->write(value);
    }
    else if (led == LED_REQUEST)
    {
        //LOG(INFO, "[Bootloader] Preparing to receive firmware");
        //LOG(INFO, "[Bootloader] Current partition: %s", current->label);
        //LOG(INFO, "[Bootloader] Target partition: %s", target->label);
    }
}

/// Initializes the node specific bootloader hardware (LEDs)
void bootloader_hw_set_to_safe(void)
{
    LOG(VERBOSE, "[Bootloader] bootloader_hw_set_to_safe");
    LED_ACT1_Pin::hw_init();
    LED_ACT2_Pin::hw_init();
}


void app_main()
{
    
    //bool stackrunning = false;
    // capture the reason for the CPU reset
    uint8_t reset_reason = Esp32SocInfo::print_soc_info();
    // If this is the first power up of the node we need to reset the flag
    // since it will not be initialized automatically.
    if (reset_reason == POWERON_RESET)
    {
        bootloader_request = 0;
    }
    LOG(INFO, "[SNIP] version:%d, manufacturer:%s, model:%s, hw-v:%s, sw-v:%s"
      , openlcb::SNIP_STATIC_DATA.version
      , openlcb::SNIP_STATIC_DATA.manufacturer_name
      , openlcb::SNIP_STATIC_DATA.model_name
      , openlcb::SNIP_STATIC_DATA.hardware_version
      , openlcb::SNIP_STATIC_DATA.software_version);
    bool reset_events = false;

    GpioInit::hw_init();

    nvs_init();

    // load non-CDI based config from NVS.
    bool cleanup_config_tree = false;
    node_config_t config;
    if (load_config(&config) != ESP_OK)
    {
        default_config(&config);
        cleanup_config_tree = true;
    }
    bool run_bootloader = false;
    
    // Ensure the LEDs are both ON for PauseCheck
    LED_ACT1_Pin::instance()->set();
    LED_ACT2_Pin::instance()->set();
    
    LOG(INFO, "[BootPauseHelper] starting...");
    BootPauseHelper pause(&config);
    
    pause.CheckPause();
    LOG(INFO, "[BootPauseHelper] returned...");
    load_config(&config);// Reload config -- CheckPause() might update things.
    
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


    if (run_bootloader)
    {
        LOG(VERBOSE, "[Bootloader] bootloader_hw_set_to_safe");                     
        LED_ACT1_Pin::hw_init();                                                    
        LED_ACT2_Pin::hw_init();
        esp32_bootloader_run(config.node_id, CONFIG_TWAI_TX_PIN, CONFIG_TWAI_RX_PIN, true);
        esp_restart();
    }
    else
    {
        dump_config(&config);
        mount_fs(cleanup_config_tree);
        LOG(INFO, "[esp32multifunction] about to start the Simple Can Stack");
        openlcb::SimpleCanStack stack(config.node_id);
        LOG(INFO, "[esp32multifunction] stack started");
        stack.set_tx_activity_led(LED_ACT1_Pin::instance());
        LOG(INFO, "[esp32multifunction] set activity led");
#if CONFIG_OLCB_PRINT_ALL_PACKETS
        stack.print_all_packets();
#endif
        openlcb::MemoryConfigClient memory_client(stack.node(), stack.memory_config_handler());
        LOG(INFO, "[esp32multifunction] MemoryConfigClient done.");
        esp32multifunction::FactoryResetHelper factory_reset_helper();
        LOG(INFO, "[esp32multifunction] FactoryResetHelper done.");
        esp32multifunction::EventBroadcastHelper event_helper();
        LOG(INFO, "[esp32multifunction] EventBroadcastHelper done.");
        esp32multifunction::DelayRebootHelper delayed_reboot(stack.service());
        LOG(INFO, "[esp32multifunction] DelayRebootHelper done.");
        esp32multifunction::HealthMonitor health_mon(stack.service());
        LOG(INFO, "[esp32multifunction] HealthMonitor done.");
        BlinkTimer blinker(stack.executor()->active_timers());
        LOG(INFO, "[esp32multifunction] BlinkTimer done.");
        int i = 0;
        Mast *masts[MASTCOUNT];
        Mast *prevMast = nullptr;
        for (i = 0; i < MASTCOUNT; i++) {
            masts[i] = new Mast(stack.node(),cfg.seg().masts().entry(i),prevMast);
            prevMast = masts[i];
        }
        LOG(INFO, "[esp32multifunction] Masts  done.");
        for (i = 0; i < TRACKCIRCUITCOUNT; i++) {
            circuits[i] = new TrackCircuit(stack.node(),cfg.seg().circuits().entry(i));
        }
        LOG(INFO, "[esp32multifunction] TrackCircuits  done.");
        Logic *prevLogic = nullptr;
        Logic *logics[LOGICCOUNT];
        for (i = LOGICCOUNT-1; i >= 0; i--) {
            logics[i] = new Logic(stack.node(), cfg.seg().logics().entry(i),stack.executor()->active_timers(),prevLogic);
            prevLogic = logics[i];
        }
        LOG(INFO, "[esp32multifunction] Logics done.");
        Turnout turnout1(stack.node(), cfg.seg().turnouts().entry<0>(),Motor1_Pin());
        Turnout turnout2(stack.node(), cfg.seg().turnouts().entry<1>(),Motor2_Pin());
        Turnout turnout3(stack.node(), cfg.seg().turnouts().entry<2>(),Motor3_Pin());
        Turnout turnout4(stack.node(), cfg.seg().turnouts().entry<3>(),Motor4_Pin());
        LOG(INFO, "[esp32multifunction] Turnouts done.");
        Points points1(stack.node(), cfg.seg().points().entry<0>(),Points1_Pin());
        Points points2(stack.node(), cfg.seg().points().entry<1>(),Points2_Pin());
        Points points3(stack.node(), cfg.seg().points().entry<2>(),Points3_Pin());
        Points points4(stack.node(), cfg.seg().points().entry<3>(),Points4_Pin());
        LOG(INFO, "[esp32multifunction] Points done.");
        OccupancyDetector od1(stack.node(), cfg.seg().ocs().entry<0>(),OD1_Pin());
        OccupancyDetector od2(stack.node(), cfg.seg().ocs().entry<1>(),OD2_Pin());
        OccupancyDetector od3(stack.node(), cfg.seg().ocs().entry<2>(),OD3_Pin());
        OccupancyDetector od4(stack.node(), cfg.seg().ocs().entry<3>(),OD4_Pin());
        LOG(INFO, "[esp32multifunction] OccupancyDetector done."); 
        Button button1(stack.node(), cfg.seg().buttons().entry<0>(),Button1_Pin());
        Button button2(stack.node(), cfg.seg().buttons().entry<1>(),Button2_Pin());
        Button button3(stack.node(), cfg.seg().buttons().entry<2>(),Button3_Pin());
        Button button4(stack.node(), cfg.seg().buttons().entry<3>(),Button4_Pin());
        LOG(INFO, "[esp32multifunction] Buttons done.");
        LED Led1(stack.node(), cfg.seg().leds().entry<0>(),LED1_Pin());
        LED Led2(stack.node(), cfg.seg().leds().entry<1>(),LED2_Pin());
        LED Led3(stack.node(), cfg.seg().leds().entry<2>(),LED3_Pin());
        LED Led4(stack.node(), cfg.seg().leds().entry<3>(),LED4_Pin());
        LOG(INFO, "[esp32multifunction] LEDs done.");
        openlcb::RefreshLoop points_refresh_loop(stack.node(),{
                                                 points1.polling()
                                                 , points2.polling()
                                                 , points3.polling()
                                                 , points4.polling()
                                                 , od1.polling()
                                                 , od2.polling()
                                                 , od3.polling()
                                                 , od4.polling()
                                                 , button1.polling()
                                                 , button2.polling()
                                                 , button3.polling()
                                                 , button4.polling()
                                             });
        LOG(INFO, "[esp32multifunction] RefreshLoop done.");
        pwmchip.hw_init(PCA9685_SLAVE_ADDRESS);
        LOG(INFO, "[esp32multifunction] pwmchip inited.");
        Lamp::PinLookupInit(0,nullptr);
        for (i = 0; i < openmrn_arduino::Esp32PCA9685PWM::NUM_CHANNELS; i++)
        {
            Lamp::PinLookupInit(i+1,pwmchip.get_channel(i));
        }
        LOG(INFO, "[esp32multifunction] Lamps done.");
        // Create / update CDI, if the CDI is out of date a factory reset will be
        // forced.
        bool reset_cdi = CDIXMLGenerator::create_config_descriptor_xml(
                                                                       cfg, openlcb::CDI_FILENAME, &stack);
        LOG(INFO, "[esp32multifunction] CDIXMLGenerator done.");
        if (reset_cdi)
        {
            LOG(WARNING, "[CDI] Forcing factory reset due to CDI update");
            unlink(openlcb::CONFIG_FILENAME);
        }
        
        // Create config file and initiate factory reset if it doesn't exist or is
        // otherwise corrupted.
        int config_fd =
              stack.create_config_file_if_needed(cfg.seg().internal_config(),
                                                  CDI_VERSION,
                                                  openlcb::CONFIG_FILE_SIZE);
        esp32multifunction::NodeRebootHelper node_reboot_helper(&stack, config_fd);
        
        if (reset_events)
        {
            LOG(WARNING, "[CDI] Resetting event IDs");
            stack.factory_reset_all_events(
                    cfg.seg().internal_config(), config.node_id, config_fd);
            fsync(config_fd);
        }
        
        
        // Initialize the TWAI driver.
        twai.hw_init();
        
        // Add the TWAI port to the stack.
        stack.add_can_port_select("/dev/twai/twai0");
        
        // if a brownout was detected send an event as part of node startup.
        if (reset_reason == RTCWDT_BROWN_OUT_RESET)
        {
            //event_helper.send_event(openlcb::Defs::NODE_POWER_BROWNOUT_EVENT);
        }
        
        // Start the stack in the background using it's own task.
        stack.loop_executor();
        //stackrunning = true;
    }
    // At this point the OpenMRN stack is running in it's own task and we can
    // safely exit from this one. We do not need to cleanup as that will be
    // handled automatically by ESP-IDF.
    //if (!stackrunning) {
    //    reboot();
    //}
}


} // extern "C"
    
