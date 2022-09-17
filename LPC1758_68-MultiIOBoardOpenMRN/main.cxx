// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Sep 12 14:55:18 2022
//  Last Modified : <220917.1610>
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

#include <ctype.h>
#include "os/os.h"
#include "nmranet_config.h"

#include <openlcb/RefreshLoop.hxx>
#include "openlcb/SimpleStack.hxx"
#include "openlcb/ConfiguredConsumer.hxx"
#include "openlcb/ConfiguredProducer.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "openlcb/MemoryConfig.hxx"
#include "openlcb/MultiConfiguredConsumer.hxx"
#include "MultiConfiguredProducer.hxx" // Not in OpenMRN?

#include "config.hxx"
#include "utils/GpioInitializer.hxx"
#include "Hardware.hxx"
#include "nvs_config.hxx"
#include "TrackCircuit.hxx"
#include "Logic.hxx"

// ConfigDef comes from config.hxx and is specific to the particular device and
// target. It defines the layout of the configuration memory space and is also
// used to generate the cdi.xml file. Here we instantiate the configuration
// layout. The argument of offset zero is ignored and will be removed later.
openlcb::ConfigDef cfg(0);
// Defines weak constants used by the stack to tell it which device contains
// the volatile configuration information. This device name appears in
// HwInit.cxx that creates the device drivers.
extern const char *const openlcb::CONFIG_FILENAME = "/dev/eeprom";
// The size of the memory space to export over the above device. We verify that
// the available eeprom is not too full (8192 max) to avoid quick wear-out
// of the flash. Recommended to have at least 10% spare.
extern const size_t openlcb::CONFIG_FILE_SIZE =
cfg.seg().size() + cfg.seg().offset();
static_assert(openlcb::CONFIG_FILE_SIZE <= 7000, "Need to adjust eeprom size");

// The SNIP user-changeable information in also stored in the above eeprom
// device. In general this could come from different eeprom segments, but it is
// simpler to keep them together.
extern const char *const openlcb::SNIP_DYNAMIC_FILENAME =
openlcb::CONFIG_FILENAME;

// Input card pins.

#if NUM_INPUTS == 24
constexpr const static Gpio *const kInputCard[] = {
    C0P0B0_Pin::instance(), C0P0B1_Pin::instance(), //
    C0P0B2_Pin::instance(), C0P0B3_Pin::instance(), //
    C0P0B4_Pin::instance(), C0P0B5_Pin::instance(), //
    C0P0B6_Pin::instance(), C0P0B7_Pin::instance(), //
    C0P1B0_Pin::instance(), C0P1B1_Pin::instance(), //
    C0P1B2_Pin::instance(), C0P1B3_Pin::instance(), //
    C0P1B4_Pin::instance(), C0P1B5_Pin::instance(), //
    C0P1B6_Pin::instance(), C0P1B7_Pin::instance(), //
    C0P2B0_Pin::instance(), C0P2B1_Pin::instance(), //
    C0P2B2_Pin::instance(), C0P2B3_Pin::instance(), //
    C0P2B4_Pin::instance(), C0P2B5_Pin::instance(), //
    C0P2B6_Pin::instance(), C0P2B7_Pin::instance()  //
};
#else
constexpr const static Gpio *const kInputCard[] = {
    C0P0B0_Pin::instance(), C0P0B1_Pin::instance(), //
    C0P0B2_Pin::instance(), C0P0B3_Pin::instance()  //
};
#endif

// Output card pins

#if NUM_OUTPUTS == 24
constexpr const static Gpio *const kOutputCard[] = {
    C1P0B0_Pin::instance(), C1P0B1_Pin::instance(), //
    C1P0B2_Pin::instance(), C1P0B3_Pin::instance(), //
    C1P0B4_Pin::instance(), C1P0B5_Pin::instance(), //
    C1P0B6_Pin::instance(), C1P0B7_Pin::instance(), //
    C1P1B0_Pin::instance(), C1P1B1_Pin::instance(), //
    C1P1B2_Pin::instance(), C1P1B3_Pin::instance(), //
    C1P1B4_Pin::instance(), C1P1B5_Pin::instance(), //
    C1P1B6_Pin::instance(), C1P1B7_Pin::instance(), //
    C1P2B0_Pin::instance(), C1P2B1_Pin::instance(), //
    C1P2B2_Pin::instance(), C1P2B3_Pin::instance(), //
    C1P2B4_Pin::instance(), C1P2B5_Pin::instance(), //
    C1P2B6_Pin::instance(), C1P2B7_Pin::instance()  //
};
#else
constexpr const static Gpio *const kOutputCard[] = {
    C1P0B0_Pin::instance(), C1P0B1_Pin::instance(), //
    C1P0B2_Pin::instance(), C1P0B3_Pin::instance()  //
};
#endif

class FactoryResetHelper : public DefaultConfigUpdateListener {
public:
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE {
                                         AutoNotify n(done);
                                         return UPDATED;
                                     }
    
    void factory_reset(int fd) override
    {
        cfg.userinfo().name().write(fd, HARDWARE_IMPL);
        cfg.userinfo().description().write(fd, "" );
    }
} factory_reset_helper;

TrackCircuit *circuits[TRACKCIRCUITCOUNT];

/** Entry point to application.
 *  * @param argc number of command line arguments
 *  * @param argv array of command line arguments
 *  * @return 0, should never return
 *  */
int appl_main(int argc, char *argv[])
{
    bool reset_events = false;
    GpioInit::hw_init();
    nvs_init();
    
    // load non-CDI based config from NVS.
    bool cleanup_config_tree = false;
    node_config_t config;
    if (load_config(&config) != 0)
    {
        default_config(&config);
        cleanup_config_tree = true;
    }
    
    load_config(&config);// Reload config
    
    // Ensure the LEDs are both OFF when we startup.
    ACT1_Pin::instance()->clr();
    ACT2_Pin::instance()->clr();
    
    // Check for and reset factory reset flag.
    if (config.force_reset)
    {
        cleanup_config_tree = true;
        config.force_reset = false;
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
    openlcb::SimpleCanStack stack(config.node_id);
    stack.set_tx_activity_led(ACT1_Pin::instance());
    stack.add_can_port_select("/dev/can0");
    
    openlcb::MultiConfiguredConsumer outputCard(stack.node(), 
                                                kOutputCard, 
                                                ARRAYSIZE(kOutputCard), 
                                                cfg.seg().outputs());
    MultiConfiguredProducer inputCard(stack.node(),
                                      kInputCard,
                                      ARRAYSIZE(kInputCard),
                                      cfg.seg().inputs());
    openlcb::RefreshLoop loop(stack.node(), {inputCard.polling()});
    
    
    for (int i = 0; i < TRACKCIRCUITCOUNT; i++) {
        circuits[i] = new TrackCircuit(stack.node(),cfg.seg().circuits().entry(i));
    }
    Logic *prevLogic = nullptr;
    Logic *logics[LOGICCOUNT];
    for (int i = LOGICCOUNT-1; i >= 0; i--) {
        logics[i] = new Logic(stack.node(), cfg.seg().logics().entry(i),stack.executor()->active_timers(),prevLogic);
        prevLogic = logics[i];
    }
    
    int fd = stack.create_config_file_if_needed(cfg.seg().internal_config(), 
                                                openlcb::CANONICAL_VERSION, 
                                                openlcb::CONFIG_FILE_SIZE);

    if (cleanup_config_tree)
    {
        stack.check_version_and_factory_reset(cfg.seg().internal_config(), 
                                              openlcb::CANONICAL_VERSION, 
                                              true);
    }
    
    if (reset_events)
    {
        stack.factory_reset_all_events(cfg.seg().internal_config(), 
                                       config.node_id,
                                       fd);
    }
    // This command donates the main thread to the operation of the
    // stack. Alternatively the stack could be started in a separate stack and
    // then application-specific business logic could be executed ion a busy
    // loop in the main thread.
    stack.loop_executor();
    return 0;
}    
