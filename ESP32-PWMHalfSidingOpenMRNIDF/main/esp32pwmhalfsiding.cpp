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
//  Last Modified : <220903.2124>
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
#include "EventBroadcastHelper.hxx"
#include "FactoryResetHelper.hxx"
#include "HealthMonitor.hxx"
#include "fs.hxx"
#include "hardware.hxx"

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
#include <CDIXMLGenerator.hxx>
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
#include "Esp32PCA9685PWM.hxx"


///////////////////////////////////////////////////////////////////////////////
// Increase the CAN RX frame buffer size to reduce overruns when there is high
// traffic load (ie: large datagram transport).
///////////////////////////////////////////////////////////////////////////////
OVERRIDE_CONST(can_rx_buffer_size, 64);

TrackCircuit *circuits[TRACKCIRCUITCOUNT];

esp32pwmhalfsiding::ConfigDef cfg(0);
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


DEFINE_SINGLETON_INSTANCE(BlinkTimer);
PWM* Lamp::pinlookup_[17];


extern "C"
{

ssize_t os_get_free_heap()
{
    return heap_caps_get_free_size(MALLOC_CAP_8BIT);
}

void app_main()
{
    LOG(INFO, "[SNIP] version:%d, manufacturer:%s, model:%s, hw-v:%s, sw-v:%s"
      , openlcb::SNIP_STATIC_DATA.version
      , openlcb::SNIP_STATIC_DATA.manufacturer_name
      , openlcb::SNIP_STATIC_DATA.model_name
      , openlcb::SNIP_STATIC_DATA.hardware_version
      , openlcb::SNIP_STATIC_DATA.software_version);
    bool reset_events = false;

    GpioInit::hw_init();

    mount_fs(false);
    openlcb::SimpleCanStack stack(CONFIG_OLCB_NODE_ID);
#if CONFIG_OLCB_PRINT_ALL_PACKETS
    stack.print_all_packets();
#endif
    openlcb::MemoryConfigClient memory_client(stack.node(), stack.memory_config_handler());
    esp32pwmhalfsiding::FactoryResetHelper factory_reset_helper();
    esp32pwmhalfsiding::EventBroadcastHelper event_helper();
    esp32pwmhalfsiding::HealthMonitor health_mon(stack.service());
    BlinkTimer blinker(stack.executor()->active_timers());
    int i = 0;
    Mast *masts[MASTCOUNT];
    Mast *prevMast = nullptr;
    for (i = 0; i < MASTCOUNT; i++) {
        masts[i] = new Mast(stack.node(),cfg.seg().masts().entry(i),prevMast);
        prevMast = masts[i];
    }
    for (i = 0; i < TRACKCIRCUITCOUNT; i++) {
        circuits[i] = new TrackCircuit(stack.node(),cfg.seg().circuits().entry(i));
    }
    Logic *prevLogic = nullptr;
    Logic *logics[LOGICCOUNT];
    for (i = LOGICCOUNT-1; i >= 0; i--) {
        logics[i] = new Logic(stack.node(), cfg.seg().logics().entry(i),stack.executor()->active_timers(),prevLogic);
        prevLogic = logics[i];
    }
    Turnout turnout1(stack.node(), cfg.seg().turnouts().entry<0>(),Motor1_Pin());
    Turnout turnout2(stack.node(), cfg.seg().turnouts().entry<1>(),Motor2_Pin());
            
    Points points1(stack.node(), cfg.seg().points().entry<0>(),Points1_Pin());
    Points points2(stack.node(), cfg.seg().points().entry<1>(),Points2_Pin());
        
    OccupancyDetector od1(stack.node(), cfg.seg().ocs().entry<0>(),OD1_Pin());
    OccupancyDetector od2(stack.node(), cfg.seg().ocs().entry<1>(),OD2_Pin());
        
    pwmchip.hw_init(PCA9685_SLAVE_ADDRESS);
    Lamp::PinLookupInit(0,nullptr);
    for (i = 0; i < openmrn_arduino::Esp32PCA9685PWM::NUM_CHANNELS; i++)
    {
        Lamp::PinLookupInit(i+1,pwmchip.get_channel(i));
    }
    
    // Create / update CDI, if the CDI is out of date a factory reset will be
    // forced.
    bool reset_cdi = CDIXMLGenerator::create_config_descriptor_xml(
                                                                   cfg, openlcb::CDI_FILENAME, &stack);
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
    
