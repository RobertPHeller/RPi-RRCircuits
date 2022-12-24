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
//  Last Modified : <221224.1227>
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
#include "cdidata.hxx"
#include "EventBroadcastHelper.hxx"
#include "FactoryResetHelper.hxx"
#include "HealthMonitor.hxx"
#include "fs.hxx"
#include "hardware.hxx"
#include "NvsManager.hxx"
#include "BootPauseHelper.hxx"

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
#include <freertos_drivers/esp32/Esp32HardwareTwai.hxx>
#include <freertos_drivers/esp32/Esp32BootloaderHal.hxx>
#include <freertos_drivers/esp32/Esp32SocInfo.hxx>
#include <openlcb/MemoryConfigClient.hxx>
#include <openlcb/RefreshLoop.hxx>
#include <openlcb/SimpleStack.hxx>
#include <utils/constants.hxx>
#include <utils/format_utils.hxx>

#include "Lamp.hxx"
#include "Mast.hxx"
#include "Blink.hxx"
#include "TrackCircuit.hxx"
#include "Logic.hxx"
#include "Turnout.hxx"
#include "Points.hxx"
#include "OccupancyDetector.hxx"
#include "Esp32HardwareI2C.hxx"
#include "PCA9685PWM.hxx"


///////////////////////////////////////////////////////////////////////////////
// Increase the CAN RX frame buffer size to reduce overruns when there is high
// traffic load (ie: large datagram transport).
///////////////////////////////////////////////////////////////////////////////
OVERRIDE_CONST(can_rx_buffer_size, 64);

TrackCircuit *circuits[TRACKCIRCUITCOUNT];

esp32pwmhalfsiding::ConfigDef cfg(0);
Esp32HardwareTwai twai(CONFIG_TWAI_RX_PIN, CONFIG_TWAI_TX_PIN);
Esp32HardwareI2C i2c0(CONFIG_SDA_PIN, CONFIG_SCL_PIN, 0);

namespace openlcb
{
    /// Name of CDI.xml to generate dynamically.
    const char CDI_FILENAME[] = "/fs/cdi.xml";

    // Path to where OpenMRN should persist general configuration data.
    extern const char *const CONFIG_FILENAME = "/fs/config";

    // The size of the memory space to export over the above device.
    extern const size_t CONFIG_FILE_SIZE =
        cfg.seg().size() + cfg.seg().offset();

    // Default to store the dynamic SNIP data is stored in the same persistant
    // data file as general configuration data.
    extern const char *const SNIP_DYNAMIC_FILENAME = "/fs/config";

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
    extern const SimpleNodeStaticValues SNIP_STATIC_DATA =
    {
        4,
        SNIP_PROJECT_PAGE,
        SNIP_PROJECT_NAME,
        SNIP_HW_VERSION,
        SNIP_SW_VERSION
    };

    /// Modify this value every time the EEPROM needs to be cleared on the node
    /// after an update.
    static constexpr uint16_t CANONICAL_VERSION = CDI_VERSION;

} // namespace openlcb


DEFINE_SINGLETON_INSTANCE(BlinkTimer);
PWM* Lamp::pinlookup_[17];
PCA9685PWM pwmchip(&i2c0);
PCA9685PWMBit LampA0(&pwmchip,0);
PCA9685PWMBit LampA1(&pwmchip,1);
PCA9685PWMBit LampA2(&pwmchip,2);
PCA9685PWMBit LampA3(&pwmchip,3);
PCA9685PWMBit LampA4(&pwmchip,4);
PCA9685PWMBit LampA5(&pwmchip,5);
PCA9685PWMBit LampA6(&pwmchip,6);
PCA9685PWMBit LampA7(&pwmchip,7);

PCA9685PWMBit LampB0(&pwmchip,8);
PCA9685PWMBit LampB1(&pwmchip,9);
PCA9685PWMBit LampB2(&pwmchip,10);
PCA9685PWMBit LampB3(&pwmchip,11);
PCA9685PWMBit LampB4(&pwmchip,12);
PCA9685PWMBit LampB5(&pwmchip,13);
PCA9685PWMBit LampB6(&pwmchip,14);
PCA9685PWMBit LampB7(&pwmchip,15);

extern "C"
{

ssize_t os_get_free_heap()
{
    return heap_caps_get_free_size(MALLOC_CAP_8BIT);
}

void app_main()
{
    // capture the reason for the CPU reset
    uint8_t reset_reason = Esp32SocInfo::print_soc_info();
    
    LOG(INFO, "[SNIP] version:%d, manufacturer:%s, model:%s, hw-v:%s, sw-v:%s"
      , openlcb::SNIP_STATIC_DATA.version
      , openlcb::SNIP_STATIC_DATA.manufacturer_name
      , openlcb::SNIP_STATIC_DATA.model_name
      , openlcb::SNIP_STATIC_DATA.hardware_version
      , openlcb::SNIP_STATIC_DATA.software_version);
    bool reset_events = false;
    bool cleanup_config_tree = false;
    
    GpioInit::hw_init();
    
    esp32pwmhalfsiding::NvsManager nvs;
    nvs.init(reset_reason);
    
    LOG(INFO, "[BootPauseHelper] starting...");
    
    esp32pwmhalfsiding::BootPauseHelper pause;
    
    pause.CheckPause();
    LOG(INFO, "[BootPauseHelper] returned...");
    
    // Check for and reset factory reset flag.
    if (nvs.should_reset_config())
    {
        cleanup_config_tree = true;
        nvs.clear_factory_reset();
    }
    
    if (nvs.should_reset_events())
    {
        reset_events = true;
        // reset the flag so we start in normal operating mode next time.
        nvs.clear_reset_events();
    }
    nvs.CheckPersist();
    
    nvs.DisplayNvsConfiguration();
    mount_fs(cleanup_config_tree);
    openlcb::SimpleCanStack stack(CONFIG_OLCB_NODE_ID);
    LOG(INFO, "[MAIN] SimpleCanStack allocated");
#if CONFIG_OLCB_PRINT_ALL_PACKETS
    stack.print_all_packets();
#endif
    openlcb::MemoryConfigClient memory_client(stack.node(), stack.memory_config_handler());
    LOG(INFO, "[MAIN] MemoryConfigClient allocated");
    esp32pwmhalfsiding::FactoryResetHelper factory_reset_helper();
    LOG(INFO, "[MAIN] FactoryResetHelper allocated");
    esp32pwmhalfsiding::EventBroadcastHelper event_helper();
    LOG(INFO, "[MAIN] EventBroadcastHelper allocated");
    esp32pwmhalfsiding::HealthMonitor health_mon(stack.service());
    LOG(INFO, "[MAIN] HealthMonitor allocated");
    
    BlinkTimer blinker(stack.executor()->active_timers());
    LOG(INFO, "[MAIN] BlinkTimer allocated");
    int i = 0;
    Mast *masts[MASTCOUNT];
    Mast *prevMast = nullptr;
    for (i = 0; i < MASTCOUNT; i++) {
        masts[i] = new Mast(stack.node(),cfg.seg().masts().entry(i),prevMast);
        prevMast = masts[i];
    }
    LOG(INFO, "[MAIN] Masts allocated");
    for (i = 0; i < TRACKCIRCUITCOUNT; i++) {
        circuits[i] = new TrackCircuit(stack.node(),cfg.seg().circuits().entry(i));
    }
    LOG(INFO, "[MAIN] Track Circuits allocated");
    Logic *prevLogic = nullptr;
    Logic *logics[LOGICCOUNT];
    for (i = LOGICCOUNT-1; i >= 0; i--) {
        logics[i] = new Logic(stack.node(), cfg.seg().logics().entry(i),stack.executor()->active_timers(),prevLogic);
        prevLogic = logics[i];
    }
    LOG(INFO, "[MAIN] Logics allocated");
    Turnout turnout1(stack.node(), cfg.seg().turnouts().entry<0>(),Motor1_Pin());
    Turnout turnout2(stack.node(), cfg.seg().turnouts().entry<1>(),Motor2_Pin());
    LOG(INFO, "[MAIN] Turnouts allocated");
    Points points1(stack.node(), cfg.seg().points().entry<0>(),Points1_Pin());
    Points points2(stack.node(), cfg.seg().points().entry<1>(),Points2_Pin());
    LOG(INFO, "[MAIN] Points allocated");
    OccupancyDetector oc1(stack.node(), cfg.seg().ocs().entry<0>(),OD1_Pin());
    OccupancyDetector oc2(stack.node(), cfg.seg().ocs().entry<1>(),OD2_Pin());
    LOG(INFO, "[MAIN] OccupancyDetectors allocated");
    
    openlcb::RefreshLoop points_refresh_loop(stack.node(),{
                                             points1.polling()
                                             , points2.polling()
                                             , oc1.polling()
                                             , oc2.polling()
                                         });
    
    pwmchip.init(PCA9685_SLAVE_ADDRESS);
    LOG(INFO, "[MAIN] pwmchip initialized");
    Lamp::PinLookupInit(0,nullptr);
    
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
    
    LOG(INFO, "[MAIN] Lamps setup");
    LOG(INFO, "[MAIN] config file size is %d",openlcb::CONFIG_FILE_SIZE);
    // Create config file and initiate factory reset if it doesn't exist or is
    // otherwise corrupted.
    int config_fd =
          stack.create_config_file_if_needed(cfg.seg().internal_config(),
                                             openlcb::CANONICAL_VERSION,
                                             openlcb::CONFIG_FILE_SIZE);
    stack.check_version_and_factory_reset(cfg.seg().internal_config(),
                                          CDI_VERSION,
                                          cleanup_config_tree);
    if (reset_events)
    {
        LOG(WARNING, "[CDI] Resetting event IDs");
        stack.factory_reset_all_events(cfg.seg().internal_config(), 
                                       CONFIG_OLCB_NODE_ID, config_fd);
        fsync(config_fd);
    }
    
    
    // Initialize the TWAI driver.
    twai.hw_init();
    
    // Add the TWAI port to the stack.
    stack.add_can_port_select("/dev/twai/twai0");
    
    // Start the stack in the background using it's own task.
    stack.loop_executor();
    // At this point the OpenMRN stack is running in it's own task and we can
    // safely exit from this one. We do not need to cleanup as that will be
    // handled automatically by ESP-IDF.
}



} // extern "C"
    
