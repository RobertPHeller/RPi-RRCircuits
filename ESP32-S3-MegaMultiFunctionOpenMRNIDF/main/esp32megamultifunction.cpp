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
//  Last Modified : <221012.1356>
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
#include <openlcb/MultiConfiguredPC.hxx>
#include <openlcb/MemoryConfigClient.hxx>
#include <openlcb/RefreshLoop.hxx>
#include <openlcb/SimpleStack.hxx>
#include <utils/constants.hxx>
#include <utils/format_utils.hxx>

#include "Esp32HardwareI2C.hxx"
#include "PCA9685PWM.hxx"
#include "MCP23017Gpio.hxx"

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


///////////////////////////////////////////////////////////////////////////////
// Increase the CAN RX frame buffer size to reduce overruns when there is high
// traffic load (ie: large datagram transport).
///////////////////////////////////////////////////////////////////////////////
OVERRIDE_CONST(can_rx_buffer_size, 64);

TrackCircuit *circuits[TRACKCIRCUITCOUNT];

esp32megamultifunction::ConfigDef cfg(0);
Esp32HardwareTwai twai(CONFIG_TWAI_RX_PIN, CONFIG_TWAI_TX_PIN);
Esp32HardwareI2C i2c0(CONFIG_SDA_PIN, CONFIG_SCL_PIN, 0, "/dev/i2c/i2c0");

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
Executor<1> io_executor("io_thread", 1, 1300);
MCP23017 gpioChip0(&io_executor, 0, 0, 0);
#ifndef PRODUCITION
constexpr const MCP23017Gpio Motor1(&gpioChip0, MCP23017::PORTA, 0);
constexpr const MCP23017Gpio Motor2(&gpioChip0, MCP23017::PORTA, 1);

constexpr const static Gpio *const kTurnouts[] = {
    &Motor1, &Motor2
};

constexpr const MCP23017Gpio Points1(&gpioChip0, MCP23017::PORTA, 2);
constexpr const MCP23017Gpio Points2(&gpioChip0, MCP23017::PORTA, 3);

constexpr const static Gpio *const kPointss[] = {
    &Points1, &Points2
};

constexpr const MCP23017Gpio OD1(&gpioChip0, MCP23017::PORTA, 4);
constexpr const MCP23017Gpio OD2(&gpioChip0, MCP23017::PORTA, 5);

constexpr const static Gpio *const kOccupancyDetectors[] = {
    &OD1, &OD2
};

constexpr const MCP23017Gpio LED1(&gpioChip0, MCP23017::PORTA, 6);
constexpr const MCP23017Gpio LED2(&gpioChip0, MCP23017::PORTA, 7);

constexpr const static Gpio *const kLEDs[] = {
    &LED1, &LED2
};

constexpr const MCP23017Gpio Button1(&gpioChip0, MCP23017::PORTB, 0);
constexpr const MCP23017Gpio Button2(&gpioChip0, MCP23017::PORTB, 1);

constexpr const static Gpio *const kButtons[] = {
    &Button1, &Button2
};

constexpr const MCP23017Gpio Line1(&gpioChip0, MCP23017::PORTB, 2);
constexpr const MCP23017Gpio Line2(&gpioChip0, MCP23017::PORTB, 3);

constexpr const static Gpio *const kLines[] = {
    &Line1, &Line2
};

#else
constexpr const MCP23017Gpio Motor1(&gpioChip0, MCP23017::PORTA, 0);
constexpr const MCP23017Gpio Motor2(&gpioChip0, MCP23017::PORTA, 1);
constexpr const MCP23017Gpio Motor3(&gpioChip0, MCP23017::PORTA, 2);
constexpr const MCP23017Gpio Motor4(&gpioChip0, MCP23017::PORTA, 3);
constexpr const MCP23017Gpio Motor5(&gpioChip0, MCP23017::PORTA, 4);
constexpr const MCP23017Gpio Motor6(&gpioChip0, MCP23017::PORTA, 5);
constexpr const MCP23017Gpio Motor7(&gpioChip0, MCP23017::PORTA, 6);
constexpr const MCP23017Gpio Motor8(&gpioChip0, MCP23017::PORTA, 7);

constexpr const static Gpio *const kTurnouts[] = {
    &Motor1, &Motor2, &Motor3, &Motor4, &Motor5, &Motor6, &Motor7, &Motor8
};

constexpr const MCP23017Gpio Points1(&gpioChip0, MCP23017::PORTB, 0);
constexpr const MCP23017Gpio Points2(&gpioChip0, MCP23017::PORTB, 1);
constexpr const MCP23017Gpio Points3(&gpioChip0, MCP23017::PORTB, 2);
constexpr const MCP23017Gpio Points4(&gpioChip0, MCP23017::PORTB, 3);
constexpr const MCP23017Gpio Points5(&gpioChip0, MCP23017::PORTB, 4);
constexpr const MCP23017Gpio Points6(&gpioChip0, MCP23017::PORTB, 5);
constexpr const MCP23017Gpio Points7(&gpioChip0, MCP23017::PORTB, 6);
constexpr const MCP23017Gpio Points8(&gpioChip0, MCP23017::PORTB, 7);

constexpr const static Gpio *const kPointss[] = {
    &Points1, &Points2, &Points3, &Points4, &Points5, &Points6, &Points7, &Points8
};

#endif
#ifdef PRODUCITION
MCP23017 gpioChip1(&io_executor, 0, 0, 1);
constexpr const MCP23017Gpio OD1(&gpioChip1, MCP23017::PORTA, 0);
constexpr const MCP23017Gpio OD2(&gpioChip1, MCP23017::PORTA, 1);
constexpr const MCP23017Gpio OD3(&gpioChip1, MCP23017::PORTA, 2);
constexpr const MCP23017Gpio OD4(&gpioChip1, MCP23017::PORTA, 3);
constexpr const MCP23017Gpio OD5(&gpioChip1, MCP23017::PORTA, 4);
constexpr const MCP23017Gpio OD6(&gpioChip1, MCP23017::PORTA, 5);
constexpr const MCP23017Gpio OD7(&gpioChip1, MCP23017::PORTA, 6);
constexpr const MCP23017Gpio OD8(&gpioChip1, MCP23017::PORTA, 7);

constexpr const static Gpio *const kOccupancyDetectors[] = {
    &OD1, &OD2, &OD3, &OD4, &OD5, &OD6, &OD7, &OD8
};

constexpr const MCP23017Gpio LED1(&gpioChip1, MCP23017::PORTB, 0);
constexpr const MCP23017Gpio LED2(&gpioChip1, MCP23017::PORTB, 1);
constexpr const MCP23017Gpio LED3(&gpioChip1, MCP23017::PORTB, 2);
constexpr const MCP23017Gpio LED4(&gpioChip1, MCP23017::PORTB, 3);
constexpr const MCP23017Gpio LED5(&gpioChip1, MCP23017::PORTB, 4);
constexpr const MCP23017Gpio LED6(&gpioChip1, MCP23017::PORTB, 5);
constexpr const MCP23017Gpio LED7(&gpioChip1, MCP23017::PORTB, 6);
constexpr const MCP23017Gpio LED8(&gpioChip1, MCP23017::PORTB, 7);

constexpr const static Gpio *const kLEDs[] = {
    &LED1, &LED2, &LED3, &LED4, &LED5, &LED6, &LED7, &LED8
};

MCP23017 gpioChip2(&io_executor, 0, 1, 0);
constexpr const MCP23017Gpio Button1(&gpioChip2, MCP23017::PORTA, 0);
constexpr const MCP23017Gpio Button2(&gpioChip2, MCP23017::PORTA, 1);
constexpr const MCP23017Gpio Button3(&gpioChip2, MCP23017::PORTA, 2);
constexpr const MCP23017Gpio Button4(&gpioChip2, MCP23017::PORTA, 3);
constexpr const MCP23017Gpio Button5(&gpioChip2, MCP23017::PORTA, 4);
constexpr const MCP23017Gpio Button6(&gpioChip2, MCP23017::PORTA, 5);
constexpr const MCP23017Gpio Button7(&gpioChip2, MCP23017::PORTA, 6);
constexpr const MCP23017Gpio Button8(&gpioChip2, MCP23017::PORTA, 7);

constexpr const static Gpio *const kButtons[] = {
    &Button1, &Button2, &Button3, &Button4, &Button5, &Button6, &Button7, &Button8
};

constexpr const MCP23017Gpio Line1(&gpioChip2,  MCP23017::PORTB, 0);
constexpr const MCP23017Gpio Line2(&gpioChip2,  MCP23017::PORTB, 1);
constexpr const MCP23017Gpio Line3(&gpioChip2,  MCP23017::PORTB, 2);
constexpr const MCP23017Gpio Line4(&gpioChip2,  MCP23017::PORTB, 3);
constexpr const MCP23017Gpio Line5(&gpioChip2,  MCP23017::PORTB, 4);
constexpr const MCP23017Gpio Line6(&gpioChip2,  MCP23017::PORTB, 5);
constexpr const MCP23017Gpio Line7(&gpioChip2,  MCP23017::PORTB, 6);
constexpr const MCP23017Gpio Line8(&gpioChip2,  MCP23017::PORTB, 7);

constexpr const static Gpio *const kLines[] = {
    &Line1, &Line2, &Line3, &Line4, &Line5, &Line6, &Line7, &Line8
};

#endif

vector<Turnout *> Turnout::turnouts;
vector<Points *> Points::points;
vector<openlcb::RefreshLoop *> Points::pollers;
vector<OccupancyDetector *>OccupancyDetector::ocs;
vector<openlcb::RefreshLoop *>OccupancyDetector::pollers;
vector<Button *> Button::buttons;
vector<openlcb::RefreshLoop *> Button::pollers;
vector<LED *> LED::leds;


PCA9685PWM pwmchip1;
PCA9685PWMBit LampA0(&pwmchip1,0);
PCA9685PWMBit LampA1(&pwmchip1,1);
PCA9685PWMBit LampA2(&pwmchip1,2);
PCA9685PWMBit LampA3(&pwmchip1,3);
PCA9685PWMBit LampA4(&pwmchip1,4);
PCA9685PWMBit LampA5(&pwmchip1,5);
PCA9685PWMBit LampA6(&pwmchip1,6);
PCA9685PWMBit LampA7(&pwmchip1,7);

PCA9685PWMBit LampB0(&pwmchip1,8);
PCA9685PWMBit LampB1(&pwmchip1,9);
PCA9685PWMBit LampB2(&pwmchip1,10);
PCA9685PWMBit LampB3(&pwmchip1,11);
PCA9685PWMBit LampB4(&pwmchip1,12);
PCA9685PWMBit LampB5(&pwmchip1,13);
PCA9685PWMBit LampB6(&pwmchip1,14);
PCA9685PWMBit LampB7(&pwmchip1,15);

#ifdef PRODUCITION
PCA9685PWM pwmchip2;
PCA9685PWMBit LampC0(&pwmchip2,0);
PCA9685PWMBit LampC1(&pwmchip2,1);
PCA9685PWMBit LampC2(&pwmchip2,2);
PCA9685PWMBit LampC3(&pwmchip2,3);
PCA9685PWMBit LampC4(&pwmchip2,4);
PCA9685PWMBit LampC5(&pwmchip2,5);
PCA9685PWMBit LampC6(&pwmchip2,6);
PCA9685PWMBit LampC7(&pwmchip2,7);

PCA9685PWMBit LampD0(&pwmchip2,8);
PCA9685PWMBit LampD1(&pwmchip2,9);
PCA9685PWMBit LampD2(&pwmchip2,10);
PCA9685PWMBit LampD3(&pwmchip2,11);
PCA9685PWMBit LampD4(&pwmchip2,12);
PCA9685PWMBit LampD5(&pwmchip2,13);
PCA9685PWMBit LampD6(&pwmchip2,14);
PCA9685PWMBit LampD7(&pwmchip2,15);
#endif

PWM* Lamp::pinlookup_[Lamp::MAX_LAMPID];

extern "C"
{

void *node_reboot(void *arg)
{
    Singleton<esp32megamultifunction::NodeRebootHelper>::instance()->reboot();
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
        LOG(INFO, "[esp32megamultifunction] about to start the Simple Can Stack");
        openlcb::SimpleCanStack stack(config.node_id);
        LOG(INFO, "[esp32megamultifunction] stack started");
        stack.set_tx_activity_led(LED_ACT1_Pin::instance());
        LOG(INFO, "[esp32megamultifunction] set activity led");
#if CONFIG_OLCB_PRINT_ALL_PACKETS
        stack.print_all_packets();
#endif
        openlcb::MemoryConfigClient memory_client(stack.node(), stack.memory_config_handler());
        LOG(INFO, "[esp32megamultifunction] MemoryConfigClient done.");
        esp32megamultifunction::FactoryResetHelper factory_reset_helper();
        LOG(INFO, "[esp32megamultifunction] FactoryResetHelper done.");
        esp32megamultifunction::EventBroadcastHelper event_helper();
        LOG(INFO, "[esp32megamultifunction] EventBroadcastHelper done.");
        esp32megamultifunction::DelayRebootHelper delayed_reboot(stack.service());
        LOG(INFO, "[esp32megamultifunction] DelayRebootHelper done.");
        esp32megamultifunction::HealthMonitor health_mon(stack.service());
        LOG(INFO, "[esp32megamultifunction] HealthMonitor done.");
        BlinkTimer blinker(stack.executor()->active_timers());
        LOG(INFO, "[esp32megamultifunction] BlinkTimer done.");
        // Set up all I/O
        
        i2c0.hw_init();
        int i2cfd = ::open("/dev/i2c/i2c0",O_RDWR);
        gpioChip0.init(i2cfd);
#ifdef PRODUCTION
        gpioChip1.init(i2cfd);
        gpioChip2.init(i2cfd);
#endif
        Turnout::Init(stack.node(),  cfg.seg().turnouts(), kTurnouts,
                      ARRAYSIZE(kTurnouts));
        Points::Init(stack.node(),  cfg.seg().points(), kPointss,
                     ARRAYSIZE(kPointss));
        OccupancyDetector::Init(stack.node(), cfg.seg().ocs(), 
                                kOccupancyDetectors, 
                                ARRAYSIZE(kOccupancyDetectors));
        Button::Init(stack.node(), cfg.seg().buttons(), kButtons,
                     ARRAYSIZE(kButtons));
        LED::Init(stack.node(), cfg.seg().leds(), kLEDs,
                  ARRAYSIZE(kLEDs));
        openlcb::MultiConfiguredPC lines(stack.node(), kLines, 
                                         ARRAYSIZE(kLines), cfg.seg().lines());
        openlcb::RefreshLoop looplines(stack.node(),
                                {
                                    lines.polling()
                                });
        Logic::Init(stack.node(),stack.executor()->active_timers(),
                    cfg.seg().logics());
        Mast::Init(stack.node(),cfg.seg().masts());
        
        pwmchip1.init("/dev/i2c/i2c0", PWMCHIP_ADDRESS1);
        Lamp::PinLookupInit(Lamp::Unused,nullptr);
        
        Lamp::PinLookupInit(Lamp::A0_,&LampA0);
        Lamp::PinLookupInit(Lamp::A1_,&LampA1);
        Lamp::PinLookupInit(Lamp::A2_,&LampA2);
        Lamp::PinLookupInit(Lamp::A3_,&LampA3);
        Lamp::PinLookupInit(Lamp::A4_,&LampA4);
        Lamp::PinLookupInit(Lamp::A5_,&LampA5);
        Lamp::PinLookupInit(Lamp::A6_,&LampA6);
        Lamp::PinLookupInit(Lamp::A7_,&LampA7);
        
        Lamp::PinLookupInit(Lamp::B0_,&LampB0);
        Lamp::PinLookupInit(Lamp::B1_,&LampB1);
        Lamp::PinLookupInit(Lamp::B2_,&LampB2);
        Lamp::PinLookupInit(Lamp::B3_,&LampB3);
        Lamp::PinLookupInit(Lamp::B4_,&LampB4);
        Lamp::PinLookupInit(Lamp::B5_,&LampB5);
        Lamp::PinLookupInit(Lamp::B6_,&LampB6);
        Lamp::PinLookupInit(Lamp::B7_,&LampB7);
        
#ifdef PRODUCTION
        pwmchip2.init("/dev/i2c/i2c0", PWMCHIP_ADDRESS2);
        
        Lamp::PinLookupInit(Lamp::C0_,&LampC0);
        Lamp::PinLookupInit(Lamp::C1_,&LampC1);
        Lamp::PinLookupInit(Lamp::C2_,&LampC2);
        Lamp::PinLookupInit(Lamp::C3_,&LampC3);
        Lamp::PinLookupInit(Lamp::C4_,&LampC4);
        Lamp::PinLookupInit(Lamp::C5_,&LampC5);
        Lamp::PinLookupInit(Lamp::C6_,&LampC6);
        Lamp::PinLookupInit(Lamp::C7_,&LampC7);
        
        Lamp::PinLookupInit(Lamp::D0_,&LampD0);
        Lamp::PinLookupInit(Lamp::D1_,&LampD1);
        Lamp::PinLookupInit(Lamp::D2_,&LampD2);
        Lamp::PinLookupInit(Lamp::D3_,&LampD3);
        Lamp::PinLookupInit(Lamp::D4_,&LampD4);
        Lamp::PinLookupInit(Lamp::D5_,&LampD5);
        Lamp::PinLookupInit(Lamp::D6_,&LampD6);
        Lamp::PinLookupInit(Lamp::D7_,&LampD7);
        
#endif
        TrackCircuit::Init(stack.node(),cfg.seg().circuits());
        
        LOG(INFO, "[esp32megamultifunction] IO All Initialized");

        // Create / update CDI, if the CDI is out of date a factory reset will be
        // forced.
        bool reset_cdi = CDIXMLGenerator::create_config_descriptor_xml(
                                                                       cfg, openlcb::CDI_FILENAME, &stack);
        LOG(INFO, "[esp32megamultifunction] CDIXMLGenerator done.");
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
        esp32megamultifunction::NodeRebootHelper node_reboot_helper(&stack, config_fd);
        
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
    
