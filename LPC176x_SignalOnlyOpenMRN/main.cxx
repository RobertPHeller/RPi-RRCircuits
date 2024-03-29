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
//  Last Modified : <221008.2028>
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
#include "utils/logging.h"
#include <freertos_drivers/common/PCA9685PWM.hxx>
#include "config.hxx"
#include "nvs_config.hxx"
#include "utils/GpioInitializer.hxx"
#include "Hardware.hxx"
#include "Blink.hxx"
#include "Mast.hxx"

#define SIGNALS 1
//#define SIZEBRK 1

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
static_assert(openlcb::CONFIG_FILE_SIZE <= EEPROM_CONFIGFILE_SIZE, "Need to adjust eeprom size");

// The SNIP user-changeable information in also stored in the above eeprom
// device. In general this could come from different eeprom segments, but it is
// simpler to keep them together.
extern const char *const openlcb::SNIP_DYNAMIC_FILENAME =
openlcb::CONFIG_FILENAME;


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
};

PWM* Lamp::pinlookup_[Lamp::MAX_LAMPID];

#ifdef SIGNALS
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
#endif

/** Entry point to application.
 *  * @param argc number of command line arguments
 *  * @param argv array of command line arguments
 *  * @return 0, should never return
 *  */
int appl_main(int argc, char *argv[])
{
    bool reset_events = false;
    LOG(INFO, "[SNIP] version:%d, manufacturer:%s, model:%s, hw-v:%s, sw-v:%s"
      , openlcb::SNIP_STATIC_DATA.version
      , openlcb::SNIP_STATIC_DATA.manufacturer_name
      , openlcb::SNIP_STATIC_DATA.model_name
      , openlcb::SNIP_STATIC_DATA.hardware_version
      , openlcb::SNIP_STATIC_DATA.software_version);
    
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
    
#ifdef SIZEBRK
    {
        size_t Mast_sz = sizeof(Mast);
        LOG(INFO, "[CONFIG] Mast:%u", Mast_sz);
        LOG(INFO, "[CONFIG] openlcb::CONFIG_FILE_SIZE:%u",
            openlcb::CONFIG_FILE_SIZE);
        __asm("BKPT #0\n") ; // Break into the debugger 
    }
#endif
    
    dump_config(&config);
    openlcb::SimpleCanStack stack(config.node_id);
    stack.set_tx_activity_led(ACT1_Pin::instance());
    stack.add_can_port_select("/dev/can0");
    
    FactoryResetHelper factory_reset_helper;
    BlinkTimer blinker(stack.executor()->active_timers());
    
#ifdef SIGNALS
    Mast::Init(stack.node(),cfg.seg().masts());

    
    pwmchip1.init("/dev/i2c0", PWMCHIP_ADDRESS1);
    
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

#endif
    
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
