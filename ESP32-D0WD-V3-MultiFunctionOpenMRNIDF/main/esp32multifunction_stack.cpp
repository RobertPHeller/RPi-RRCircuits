// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Jun 23 14:11:33 2022
//  Last Modified : <220623.1458>
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
#include "fs.hxx"
#include "hardware.hxx"
#include "HealthMonitor.hxx"
#include "NodeRebootHelper.hxx"
#include "nvs_config.hxx"

#include <CDIXMLGenerator.hxx>
#include <freertos_drivers/esp32/Esp32HardwareTwai.hxx>
#include <openlcb/MemoryConfigClient.hxx>
#include <openlcb/RefreshLoop.hxx>
#include <openlcb/SimpleStack.hxx>
#include <utils/constants.hxx>
#include <utils/format_utils.hxx>
#include <utils/Uninitialized.hxx>
#include "Lamp.hxx"
#include "Mast.hxx"
#include "Blink.hxx"
#include "TrackCircuit.hxx"
#include "Logic.hxx"
#include "Turnout.hxx"
#include "Points.hxx"
#include "OccupancyDetector.h"
#include "Button.hxx"
#include "LED.hxx"

namespace esp32multifunction
{

ConfigDef cfg(0);

}

namespace openlcb
{
    /// Name of CDI.xml to generate dynamically.
    const char CDI_FILENAME[] = "/fs/cdi.xml";

    // Path to where OpenMRN should persist general configuration data.
    const char *const CONFIG_FILENAME = "/fs/config";

    // The size of the memory space to export over the above device.
    const size_t CONFIG_FILE_SIZE =
        esp32multifunction::cfg.seg().size() + esp32multifunction::cfg.seg().offset();

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

extern "C" void enter_bootloader()
{
    node_config_t config;
    if (load_config(&config) != ESP_OK)
    {
        default_config(&config);
    }
    config.bootloader_req = true;
    save_config(&config);
    LOG(INFO, "[Bootloader] Rebooting into bootloader");
    reboot();
}


namespace esp32multifunction
{
int config_fd;
uninitialized<openlcb::SimpleCanStack> stack;
uninitialized<openlcb::MemoryConfigClient> memory_client;
uninitialized<FactoryResetHelper> factory_reset_helper;
uninitialized<EventBroadcastHelper> event_helper;
uninitialized<DelayRebootHelper> delayed_reboot;
uninitialized<HealthMonitor> health_mon;
uninitialized<NodeRebootHelper> node_reboot_helper;
uninitialized<BlinkTimer> blinker;
uninitialized<Mast> masts[8];
uninitialized<TrackCircuit> c[8];
TrackCircuit *circuits[8];
uninitialized<Logic> logics[32];
uninitialized<Turnout> turnouts[4];
uninitialized<Points> points[4];
uninitialized<OccupancyDetector> ocs[4];
uninitialized<Button> buttons[4];
uninitialized<LED> leds[4];
std::unique_ptr<openlcb::RefreshLoop> refresh_loop;
#if CONFIG_OLCB_ENABLE_TWAI
Esp32HardwareTwai twai(CONFIG_TWAI_RX_PIN, CONFIG_TWAI_TX_PIN);
#endif // CONFIG_OLCB_ENABLE_TWAI

void factory_reset_events()
{
    LOG(WARNING, "[CDI] Resetting event IDs");
    stack->factory_reset_all_events(cfg.seg().internal_config()
                                  , stack->node()->node_id(), config_fd);
    fsync(config_fd);
}

void NodeRebootHelper::reboot()
{
    // make sure we are not called from the executor thread otherwise there
    // will be a deadlock
    HASSERT(os_thread_self() != stack->executor()->thread_handle());
    LOG(INFO, "[Reboot] Shutting down LCC executor...");
    stack->executor()->sync_run([&]()
    {
        close(config_fd);
        unmount_fs();
        // restart the node
        LOG(INFO, "[Reboot] Restarting!");
        esp_restart();
    });
}

void EventBroadcastHelper::send_event(uint64_t eventID)
{
    stack->executor()->add(new CallbackExecutable([&]()
    {
        stack->send_event(eventID);
    }));
}

ConfigUpdateListener::UpdateAction FactoryResetHelper::apply_configuration(
    int fd, bool initial_load, BarrierNotifiable *done)
{
    // nothing to do here as we do not load config
    AutoNotify n(done);
    LOG(VERBOSE, "[CFG] apply_configuration(%d, %d)", fd, initial_load);

    return ConfigUpdateListener::UpdateAction::UPDATED;
}

void FactoryResetHelper::factory_reset(int fd)
{
    LOG(VERBOSE, "[CFG] factory_reset(%d)", fd);
    // set the name of the node to the SNIP model name
    cfg.userinfo().name().write(fd, openlcb::SNIP_STATIC_DATA.model_name);
    string node_id = uint64_to_string_hex(stack->node()->node_id(), 12);
    std::replace(node_id.begin(), node_id.end(), ' ', '0');
    // set the node description to the node id in expanded hex format.
    cfg.userinfo().description().write(fd, node_id.c_str());
    
    // TDB: factory reset things...
}

void start_openlcb_stack(node_config_t *config, bool reset_events
                       , bool brownout_detected, bool wifi_verbose)
{
    LOG(INFO, "[SNIP] version:%d, manufacturer:%s, model:%s, hw-v:%s, sw-v:%s"
      , openlcb::SNIP_STATIC_DATA.version
      , openlcb::SNIP_STATIC_DATA.manufacturer_name
      , openlcb::SNIP_STATIC_DATA.model_name
      , openlcb::SNIP_STATIC_DATA.hardware_version
      , openlcb::SNIP_STATIC_DATA.software_version);
    stack.emplace(config->node_id);
    stack->set_tx_activity_led(LED_ACT1_Pin::instance());
#if CONFIG_OLCB_PRINT_ALL_PACKETS
    stack->print_all_packets();
#endif

    memory_client.emplace(stack->node(), stack->memory_config_handler());
    factory_reset_helper.emplace();
    event_helper.emplace();
    delayed_reboot.emplace(stack->service());
    health_mon.emplace(stack->service());
    node_reboot_helper.emplace();
    
    blinker.emplace(stack->executor()->active_timers());
    int i = 0;
    Mast *prevMast = nullptr;
    for (i = 0; i < 8; i++) {
        masts[i].emplace(stack->node(),cfg.seg().masts().entry(i),prevMast);
        prevMast = masts[i];
    }
    for (i = 0; i < 8; i++) {
        c[i].emplace(stack->node(),cfg.seg().circuits().entry(i));
        circuits[i] = c[i];
    }
    Logic *prevLogic = nullptr;
    for (i = 31; i >= 0; i--) {
        logics[i].emplace(stack->node(), cfg.seg().logics().entry(i),stack->executor()->active_timers(),prevLogic);
        prevLogic = logics[i];
    }
    turnouts[0].emplace(stack->node(), cfg.seg().turnouts().entry<0>(),Motor1_Pin());
    turnouts[1].emplace(stack->node(), cfg.seg().turnouts().entry<1>(),Motor2_Pin());
    turnouts[2].emplace(stack->node(), cfg.seg().turnouts().entry<2>(),Motor3_Pin());
    turnouts[3].emplace(stack->node(), cfg.seg().turnouts().entry<3>(),Motor4_Pin());
    
    points[0].emplace(stack->node(), cfg.seg().points().entry<0>(),Points1_Pin());
    points[1].emplace(stack->node(), cfg.seg().points().entry<1>(),Points2_Pin());
    points[2].emplace(stack->node(), cfg.seg().points().entry<2>(),Points3_Pin());
    points[3].emplace(stack->node(), cfg.seg().points().entry<3>(),Points4_Pin());
    
    ocs[0].emplace(stack->node(), cfg.seg().ocs().entry<0>(),OD1_Pin());
    ocs[1].emplace(stack->node(), cfg.seg().ocs().entry<1>(),OD2_Pin());
    ocs[2].emplace(stack->node(), cfg.seg().ocs().entry<2>(),OD3_Pin());
    ocs[3].emplace(stack->node(), cfg.seg().ocs().entry<3>(),OD4_Pin());
    
    buttons[0].emplace(stack->node(), cfg.seg().buttons().entry<0>(),Button1_Pin());
    buttons[1].emplace(stack->node(), cfg.seg().buttons().entry<1>(),Button2_Pin());
    buttons[2].emplace(stack->node(), cfg.seg().buttons().entry<2>(),Button3_Pin());
    buttons[3].emplace(stack->node(), cfg.seg().buttons().entry<3>(),Button4_Pin());
    
    leds[0].emplace(stack->node(), cfg.seg().leds().entry<0>(),LED1_Pin());
    leds[1].emplace(stack->node(), cfg.seg().leds().entry<1>(),LED2_Pin());
    leds[2].emplace(stack->node(), cfg.seg().leds().entry<2>(),LED3_Pin());
    leds[3].emplace(stack->node(), cfg.seg().leds().entry<3>(),LED4_Pin());
    
    refresh_loop.reset(
        new openlcb::RefreshLoop(stack->node()
                                 , 
                             { points[0]->polling()
                                 , points[1]->polling()
                                 , points[2]->polling()
                                 , points[3]->polling()
                                 , ocs[0]->polling()
                                 , ocs[1]->polling()
                                 , ocs[2]->polling()
                                 , ocs[3]->polling()
                                 , buttons[0]->polling()
                                 , buttons[1]->polling()
                                 , buttons[2]->polling()
                                 , buttons[3]->polling()
                             }));

    // Create / update CDI, if the CDI is out of date a factory reset will be
    // forced.
    bool reset_cdi = CDIXMLGenerator::create_config_descriptor_xml(
        cfg, openlcb::CDI_FILENAME, stack.operator->());
    if (reset_cdi)
    {
        LOG(WARNING, "[CDI] Forcing factory reset due to CDI update");
        unlink(openlcb::CONFIG_FILENAME);
    }

    // Create config file and initiate factory reset if it doesn't exist or is
    // otherwise corrupted.
    config_fd =
        stack->create_config_file_if_needed(cfg.seg().internal_config(),
                                            CDI_VERSION,
                                            openlcb::CONFIG_FILE_SIZE);

    if (reset_events)
    {
        factory_reset_events();
    }


#if CONFIG_OLCB_ENABLE_TWAI
    // Initialize the TWAI driver.
    twai.hw_init();

    // Add the TWAI port to the stack.
    stack->add_can_port_select("/dev/twai/twai0");
#endif // CONFIG_OLCB_ENABLE_TWAI

    if (brownout_detected)
    {
        // Queue the brownout event to be sent.
        LOG_ERROR("[Brownout] Detected a brownout reset, sending event");
        event_helper->send_event(openlcb::Defs::NODE_POWER_BROWNOUT_EVENT);
    }

    // Start the stack in the background using it's own task.
    stack->loop_executor();
}

} // namespace esp32io
