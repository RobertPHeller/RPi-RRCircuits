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
//  Last Modified : <221008.1329>
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

#include "Esp32HardwareI2C.hxx"
#include "MCP23017Gpio.hxx"
#include "BootPauseHelper.hxx"
#include "TrackCircuit.hxx"
#include "Logic.hxx"
#include "openlcb/MultiConfiguredConsumer.hxx"
#include "MultiConfiguredProducer.hxx" // Not in OpenMRN?


///////////////////////////////////////////////////////////////////////////////
// Increase the CAN RX frame buffer size to reduce overruns when there is high
// traffic load (ie: large datagram transport).
///////////////////////////////////////////////////////////////////////////////
OVERRIDE_CONST(can_rx_buffer_size, 64);

TrackCircuit *circuits[TRACKCIRCUITCOUNT];

esp32multiio::ConfigDef cfg(0);
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


extern "C"
{

void *node_reboot(void *arg)
{
    Singleton<esp32multiio::NodeRebootHelper>::instance()->reboot();
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

Esp32HardwareI2C i2c0(CONFIG_SDA_PIN, CONFIG_SCL_PIN, 0, "/dev/i2c/i2c0");

Executor<1> io_executor("io_thread", 1, 1300);

MCP23017 gpioChip0(&io_executor, 0, 0, 0);
constexpr const MCP23017Gpio C0A0(&gpioChip0, MCP23017::PORTA, 0);
constexpr const MCP23017Gpio C0A1(&gpioChip0, MCP23017::PORTA, 1);
constexpr const MCP23017Gpio C0A2(&gpioChip0, MCP23017::PORTA, 2);
constexpr const MCP23017Gpio C0A3(&gpioChip0, MCP23017::PORTA, 3);
constexpr const MCP23017Gpio C0A4(&gpioChip0, MCP23017::PORTA, 4);
constexpr const MCP23017Gpio C0A5(&gpioChip0, MCP23017::PORTA, 5);
constexpr const MCP23017Gpio C0A6(&gpioChip0, MCP23017::PORTA, 6);
constexpr const MCP23017Gpio C0A7(&gpioChip0, MCP23017::PORTA, 7);
#ifdef PRODUCITION
constexpr const MCP23017Gpio C0B0(&gpioChip0, MCP23017::PORTB, 0);
constexpr const MCP23017Gpio C0B1(&gpioChip0, MCP23017::PORTB, 1);
constexpr const MCP23017Gpio C0B2(&gpioChip0, MCP23017::PORTB, 2);
constexpr const MCP23017Gpio C0B3(&gpioChip0, MCP23017::PORTB, 3);
constexpr const MCP23017Gpio C0B4(&gpioChip0, MCP23017::PORTB, 4);
constexpr const MCP23017Gpio C0B5(&gpioChip0, MCP23017::PORTB, 5);
constexpr const MCP23017Gpio C0B6(&gpioChip0, MCP23017::PORTB, 6);
constexpr const MCP23017Gpio C0B7(&gpioChip0, MCP23017::PORTB, 7);
#else
constexpr const MCP23017Gpio C2A0(&gpioChip0, MCP23017::PORTB, 0);
constexpr const MCP23017Gpio C2A1(&gpioChip0, MCP23017::PORTB, 1);
constexpr const MCP23017Gpio C2A2(&gpioChip0, MCP23017::PORTB, 2);
constexpr const MCP23017Gpio C2A3(&gpioChip0, MCP23017::PORTB, 3);
constexpr const MCP23017Gpio C2A4(&gpioChip0, MCP23017::PORTB, 4);
constexpr const MCP23017Gpio C2A5(&gpioChip0, MCP23017::PORTB, 5);
constexpr const MCP23017Gpio C2A6(&gpioChip0, MCP23017::PORTB, 6);
constexpr const MCP23017Gpio C2A7(&gpioChip0, MCP23017::PORTB, 7);

#endif
#if PRODUCITION
MCP23017 gpioChip1(&io_executor, 0, 0, 1);

constexpr const MCP23017Gpio C0C0(&gpioChip1, MCP23017::PORTA, 0);
constexpr const MCP23017Gpio C0C1(&gpioChip1, MCP23017::PORTA, 1);
constexpr const MCP23017Gpio C0C2(&gpioChip1, MCP23017::PORTA, 2);
constexpr const MCP23017Gpio C0C3(&gpioChip1, MCP23017::PORTA, 3);
constexpr const MCP23017Gpio C0C4(&gpioChip1, MCP23017::PORTA, 4);
constexpr const MCP23017Gpio C0C5(&gpioChip1, MCP23017::PORTA, 5);
constexpr const MCP23017Gpio C0C6(&gpioChip1, MCP23017::PORTA, 6);
constexpr const MCP23017Gpio C0C7(&gpioChip1, MCP23017::PORTA, 7);

constexpr const MCP23017Gpio C1A0(&gpioChip1, MCP23017::PORTB, 0);
constexpr const MCP23017Gpio C1A1(&gpioChip1, MCP23017::PORTB, 1);
constexpr const MCP23017Gpio C1A2(&gpioChip1, MCP23017::PORTB, 2);
constexpr const MCP23017Gpio C1A3(&gpioChip1, MCP23017::PORTB, 3);
constexpr const MCP23017Gpio C1A4(&gpioChip1, MCP23017::PORTB, 4);
constexpr const MCP23017Gpio C1A5(&gpioChip1, MCP23017::PORTB, 5);
constexpr const MCP23017Gpio C1A6(&gpioChip1, MCP23017::PORTB, 6);
constexpr const MCP23017Gpio C1A7(&gpioChip1, MCP23017::PORTB, 7);

MCP23017 gpioChip2(&io_executor, 0, 1, 0);

constexpr const MCP23017Gpio C1B0(&gpioChip2, MCP23017::PORTA, 0);
constexpr const MCP23017Gpio C1B1(&gpioChip2, MCP23017::PORTA, 1);
constexpr const MCP23017Gpio C1B2(&gpioChip2, MCP23017::PORTA, 2);
constexpr const MCP23017Gpio C1B3(&gpioChip2, MCP23017::PORTA, 3);
constexpr const MCP23017Gpio C1B4(&gpioChip2, MCP23017::PORTA, 4);
constexpr const MCP23017Gpio C1B5(&gpioChip2, MCP23017::PORTA, 5);
constexpr const MCP23017Gpio C1B6(&gpioChip2, MCP23017::PORTA, 6);
constexpr const MCP23017Gpio C1B7(&gpioChip2, MCP23017::PORTA, 7);


constexpr const MCP23017Gpio C1C0(&gpioChip2, MCP23017::PORTB, 0);
constexpr const MCP23017Gpio C1C1(&gpioChip2, MCP23017::PORTB, 1);
constexpr const MCP23017Gpio C1C2(&gpioChip2, MCP23017::PORTB, 2);
constexpr const MCP23017Gpio C1C3(&gpioChip2, MCP23017::PORTB, 3);
constexpr const MCP23017Gpio C1C4(&gpioChip2, MCP23017::PORTB, 4);
constexpr const MCP23017Gpio C1C5(&gpioChip2, MCP23017::PORTB, 5);
constexpr const MCP23017Gpio C1C6(&gpioChip2, MCP23017::PORTB, 6);
constexpr const MCP23017Gpio C1C7(&gpioChip2, MCP23017::PORTB, 7);

MCP23017 gpioChip3(&io_executor, 0, 1, 1);

constexpr const MCP23017Gpio C2A0(&gpioChip3, MCP23017::PORTA, 0);
constexpr const MCP23017Gpio C2A1(&gpioChip3, MCP23017::PORTA, 1);
constexpr const MCP23017Gpio C2A2(&gpioChip3, MCP23017::PORTA, 2);
constexpr const MCP23017Gpio C2A3(&gpioChip3, MCP23017::PORTA, 3);
constexpr const MCP23017Gpio C2A4(&gpioChip3, MCP23017::PORTA, 4);
constexpr const MCP23017Gpio C2A5(&gpioChip3, MCP23017::PORTA, 5);
constexpr const MCP23017Gpio C2A6(&gpioChip3, MCP23017::PORTA, 6);
constexpr const MCP23017Gpio C2A7(&gpioChip3, MCP23017::PORTA, 7);

constexpr const MCP23017Gpio C2B0(&gpioChip3, MCP23017::PORTB, 0);
constexpr const MCP23017Gpio C2B1(&gpioChip3, MCP23017::PORTB, 1);
constexpr const MCP23017Gpio C2B2(&gpioChip3, MCP23017::PORTB, 2);
constexpr const MCP23017Gpio C2B3(&gpioChip3, MCP23017::PORTB, 3);
constexpr const MCP23017Gpio C2B4(&gpioChip3, MCP23017::PORTB, 4);
constexpr const MCP23017Gpio C2B5(&gpioChip3, MCP23017::PORTB, 5);
constexpr const MCP23017Gpio C2B6(&gpioChip3, MCP23017::PORTB, 6);
constexpr const MCP23017Gpio C2B7(&gpioChip3, MCP23017::PORTB, 7);

#endif

constexpr const static Gpio *const kOutputCard0A[] = {
    &C0A0, &C0A1, &C0A2, &C0A3, &C0A4, &C0A5, &C0A6, &C0A7
};
#ifdef PRODUCITION
constexpr const static Gpio *const kOutputCard0B[] = {
    &C0B0, &C0B1, &C0B2, &C0B3, &C0B4, &C0B5, &C0B6, &C0B7
};
constexpr const static Gpio *const kOutputCard0C[] = {
    &C0C0, &C0C1, &C0C2, &C0C3, &C0C4, &C0C5, &C0C6, &C0C7
};

constexpr const static Gpio *const kOutputCard1A[] = {
    &C1A0, &C1A1, &C1A2, &C1A3, &C1A4, &C1A5, &C1A6, &C1A7
};
constexpr const static Gpio *const kOutputCard1B[] = {
    &C1B0, &C1B1, &C1B2, &C1B3, &C1B4, &C1B5, &C1B6, &C1B7
};
constexpr const static Gpio *const kOutputCard1C[] = {
    &C1C1, &C1C1, &C1C2, &C1C3, &C1C4, &C1C5, &C1C6, &C1C7
};
constexpr const static Gpio *const kInputCard2A[] = {
    &C2A0, &C2A1, &C2A2, &C2A3, &C2A4, &C2A5, &C2A6, &C2A7
};
constexpr const static Gpio *const kInputCard2B[] = {
    &C2B0, &C2B1, &C2B2, &C2B3, &C2B4, &C2B5, &C2B6, &C2B7
};
constexpr const static Gpio *const kInputCard2C[] = {
    C2C0_Pin::instance(), C2C1_Pin::instance(), C2C2_Pin::instance(), 
    C2C3_Pin::instance(), C2C4_Pin::instance(), C2C5_Pin::instance(), 
    C2C6_Pin::instance(), C2C7_Pin::instance()
};
#else
constexpr const static Gpio *const kInputCard2A[] = {
    &C2A0, &C2A1, &C2A2, &C2A3, &C2A4, &C2A5, &C2A6, &C2A7
};
#endif


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
        LOG(INFO, "[esp32multiio] about to start the Simple Can Stack");
        openlcb::SimpleCanStack stack(config.node_id);
        LOG(INFO, "[esp32multiio] stack started");
        stack.set_tx_activity_led(LED_ACT1_Pin::instance());
        LOG(INFO, "[esp32multiio] set activity led");
#if CONFIG_OLCB_PRINT_ALL_PACKETS
        stack.print_all_packets();
#endif
        openlcb::MemoryConfigClient memory_client(stack.node(), stack.memory_config_handler());
        LOG(INFO, "[esp32multiio] MemoryConfigClient done.");
        esp32multiio::FactoryResetHelper factory_reset_helper();
        LOG(INFO, "[esp32multiio] FactoryResetHelper done.");
        esp32multiio::EventBroadcastHelper event_helper();
        LOG(INFO, "[esp32multiio] EventBroadcastHelper done.");
        esp32multiio::DelayRebootHelper delayed_reboot(stack.service());
        LOG(INFO, "[esp32multiio] DelayRebootHelper done.");
        esp32multiio::HealthMonitor health_mon(stack.service());
        LOG(INFO, "[esp32multiio] HealthMonitor done.");
        for (int i = 0; i < TRACKCIRCUITCOUNT; i++) {
            circuits[i] = new TrackCircuit(stack.node(),cfg.seg().circuits().entry(i));
        }
        LOG(INFO, "[esp32multiio] TrackCircuits  done.");
        Logic *prevLogic = nullptr;
        Logic *logics[LOGICCOUNT];
        for (int i = LOGICCOUNT-1; i >= 0; i--) {
            logics[i] = new Logic(stack.node(), cfg.seg().logics().entry(i),stack.executor()->active_timers(),prevLogic);
            prevLogic = logics[i];
        }
        LOG(INFO, "[esp32multiio] Logics done.");
        
        i2c0.hw_init();
        
        int i2cfd = ::open("/dev/i2c/i2c0",O_RDWR);
        gpioChip0.init(i2cfd);
#ifdef PRODUCTION
        gpioChip1.init(i2cfd);
        gpioChip2.init(i2cfd);
        gpioChip3.init(i2cfd);
#endif
                           

        openlcb::MultiConfiguredConsumer outputCard0A(stack.node(),
                                                      kOutputCard0A,
                                                      ARRAYSIZE(kOutputCard0A),
                                                      cfg.seg().Card0().PortA());
#ifdef PRODUCITION
        openlcb::MultiConfiguredConsumer outputCard0B(stack.node(),
                                                      kOutputCard0B,
                                                      ARRAYSIZE(kOutputCard0B),
                                                      cfg.seg().Card0().PortB());
        openlcb::MultiConfiguredConsumer outputCard0C(stack.node(),
                                                      kOutputCard0C,
                                                      ARRAYSIZE(kOutputCard0C),
                                                      cfg.seg().Card0().PortC());
        openlcb::MultiConfiguredConsumer outputCard1A(stack.node(),
                                                      kOutputCard1A,
                                                      ARRAYSIZE(kOutputCard1A),
                                                      cfg.seg().Card1().PortA());
        openlcb::MultiConfiguredConsumer outputCard1B(stack.node(),
                                                      kOutputCard1B,
                                                      ARRAYSIZE(kOutputCard1B),
                                                      cfg.seg().Card1().PortB());
        openlcb::MultiConfiguredConsumer outputCard1C(stack.node(),
                                                      kOutputCard1C,
                                                      ARRAYSIZE(kOutputCard1C),
                                                      cfg.seg().Card1().PortC());
#endif
        MultiConfiguredProducer inputCard2A(stack.node(),
                                            kInputCard2A,
                                            ARRAYSIZE(kInputCard2A),
                                            cfg.seg().Card2().PortA());
#ifdef PRODUCITION
        MultiConfiguredProducer inputCard2B(stack.node(),
                                            kInputCard2B,
                                            ARRAYSIZE(kInputCard2B),
                                            cfg.seg().Card2().PortB());
        MultiConfiguredProducer inputCard2C(stack.node(),
                                            kInputCard2C,
                                            ARRAYSIZE(kInputCard2C),
                                            cfg.seg().Card2().PortC());
#endif
        openlcb::RefreshLoop loop(stack.node(), {inputCard2A.polling()
#ifdef PRODUCITION
                                  , inputCard2B.polling()
                                  , inputCard2C.polling()
#endif
                              });
        
        // Create / update CDI, if the CDI is out of date a factory reset will be
        // forced.
        bool reset_cdi = CDIXMLGenerator::create_config_descriptor_xml(
                                                                       cfg, openlcb::CDI_FILENAME, &stack);
        LOG(INFO, "[esp32multiio] CDIXMLGenerator done.");
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
        esp32multiio::NodeRebootHelper node_reboot_helper(&stack, config_fd);
        
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
    
