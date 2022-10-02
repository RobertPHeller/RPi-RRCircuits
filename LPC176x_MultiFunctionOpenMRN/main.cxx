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
//  Last Modified : <221002.1643>
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
#include "Button.hxx"
#include "LED.hxx"
#include "Lamp.hxx"
#include "OccupancyDetector.hxx"
#include "Points.hxx"
#include "Turnout.hxx"
#include "Logic.hxx"
#include "Mast.hxx"
#include "TrackCircuit.hxx"

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
static_assert(openlcb::CONFIG_FILE_SIZE <= 57344, "Need to adjust eeprom size");

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
    
    dump_config(&config);
    openlcb::SimpleCanStack stack(config.node_id);
    stack.set_tx_activity_led(ACT1_Pin::instance());
    stack.add_can_port_select("/dev/can0");
    
    FactoryResetHelper factory_reset_helper;
    BlinkTimer blinker(stack.executor()->active_timers());
    
    Turnout turnout1(stack.node(), cfg.seg().turnouts().entry<0>(),Motor1_Pin());
    Turnout turnout2(stack.node(), cfg.seg().turnouts().entry<1>(),Motor2_Pin());
#if NUM_TURNOUTS == 8
    Turnout turnout3(stack.node(), cfg.seg().turnouts().entry<2>(),Motor3_Pin());
    Turnout turnout4(stack.node(), cfg.seg().turnouts().entry<3>(),Motor4_Pin());
    Turnout turnout5(stack.node(), cfg.seg().turnouts().entry<4>(),Motor5_Pin());
    Turnout turnout6(stack.node(), cfg.seg().turnouts().entry<5>(),Motor6_Pin());
    Turnout turnout7(stack.node(), cfg.seg().turnouts().entry<6>(),Motor7_Pin());
    Turnout turnout8(stack.node(), cfg.seg().turnouts().entry<7>(),Motor8_Pin());
#endif
    
    Points points1(stack.node(), cfg.seg().points().entry<0>(),Points1_Pin());
    Points points2(stack.node(), cfg.seg().points().entry<1>(),Points2_Pin());
#if NUM_POINTSS == 8
    Points points3(stack.node(), cfg.seg().points().entry<2>(),Points3_Pin());
    Points points4(stack.node(), cfg.seg().points().entry<3>(),Points4_Pin());
    Points points5(stack.node(), cfg.seg().points().entry<4>(),Points5_Pin());
    Points points6(stack.node(), cfg.seg().points().entry<5>(),Points6_Pin());
    Points points7(stack.node(), cfg.seg().points().entry<6>(),Points7_Pin());
    Points points8(stack.node(), cfg.seg().points().entry<7>(),Points8_Pin());
#endif
    
    OccupancyDetector od1(stack.node(), cfg.seg().ocs().entry<0>(),OD1_Pin());
    OccupancyDetector od2(stack.node(), cfg.seg().ocs().entry<1>(),OD2_Pin());
#if NUM_OCS == 8
    OccupancyDetector od3(stack.node(), cfg.seg().ocs().entry<2>(),OD3_Pin());
    OccupancyDetector od4(stack.node(), cfg.seg().ocs().entry<3>(),OD4_Pin());
    OccupancyDetector od5(stack.node(), cfg.seg().ocs().entry<4>(),OD5_Pin());
    OccupancyDetector od6(stack.node(), cfg.seg().ocs().entry<5>(),OD6_Pin());
    OccupancyDetector od7(stack.node(), cfg.seg().ocs().entry<6>(),OD7_Pin());
    OccupancyDetector od8(stack.node(), cfg.seg().ocs().entry<7>(),OD8_Pin());
#endif
    
    Button button1(stack.node(), cfg.seg().buttons().entry<0>(),Button1_Pin());
    Button button2(stack.node(), cfg.seg().buttons().entry<1>(),Button2_Pin());
#if NUM_BUTTONS == 8
    Button button3(stack.node(), cfg.seg().buttons().entry<2>(),Button3_Pin());
    Button button4(stack.node(), cfg.seg().buttons().entry<3>(),Button4_Pin());
    Button button5(stack.node(), cfg.seg().buttons().entry<4>(),Button5_Pin());
    Button button6(stack.node(), cfg.seg().buttons().entry<5>(),Button6_Pin());
    Button button7(stack.node(), cfg.seg().buttons().entry<6>(),Button7_Pin());
    Button button8(stack.node(), cfg.seg().buttons().entry<7>(),Button8_Pin());
#endif
    
    LED Led1(stack.node(), cfg.seg().leds().entry<0>(),LED1_Pin());
    LED Led2(stack.node(), cfg.seg().leds().entry<1>(),LED2_Pin());
#if NUM_LEDS == 8
    LED Led3(stack.node(), cfg.seg().leds().entry<2>(),LED3_Pin());
    LED Led4(stack.node(), cfg.seg().leds().entry<3>(),LED4_Pin());
    LED Led5(stack.node(), cfg.seg().leds().entry<4>(),LED5_Pin());
    LED Led6(stack.node(), cfg.seg().leds().entry<5>(),LED6_Pin());
    LED Led7(stack.node(), cfg.seg().leds().entry<6>(),LED7_Pin());
    LED Led8(stack.node(), cfg.seg().leds().entry<7>(),LED8_Pin());
#endif
    
#if 0
    Logic::Init(stack.node(),stack.executor()->active_timers(),
                cfg.seg().logics());
    Mast::Init(stack.node(),cfg.seg().masts());
    TrackCircuit::Init(stack.node(),cfg.seg().circuits());
#endif

    
#if 0
    PCA9685PWM pwmchip1;
    pwmchip1.init("/dev/i2c0", PWMCHIP_ADDRESS1);
    
    Lamp::PinLookupInit(Lamp::Unused,nullptr);
    
    PCA9685PWMBit LampA0(&pwmchip1,0);
    Lamp::PinLookupInit(Lamp::A0_,&LampA0);
    PCA9685PWMBit LampA1(&pwmchip1,1);
    Lamp::PinLookupInit(Lamp::A1_,&LampA1);
    PCA9685PWMBit LampA2(&pwmchip1,2);
    Lamp::PinLookupInit(Lamp::A2_,&LampA2);
    PCA9685PWMBit LampA3(&pwmchip1,3);
    Lamp::PinLookupInit(Lamp::A3_,&LampA3);
    PCA9685PWMBit LampA4(&pwmchip1,4);
    Lamp::PinLookupInit(Lamp::A4_,&LampA4);
    PCA9685PWMBit LampA5(&pwmchip1,5);
    Lamp::PinLookupInit(Lamp::A5_,&LampA5);
    PCA9685PWMBit LampA6(&pwmchip1,6);
    Lamp::PinLookupInit(Lamp::A6_,&LampA6);
    PCA9685PWMBit LampA7(&pwmchip1,7);
    Lamp::PinLookupInit(Lamp::A7_,&LampA7);

    PCA9685PWMBit LampB0(&pwmchip1,8);
    Lamp::PinLookupInit(Lamp::B0_,&LampB0);
    PCA9685PWMBit LampB1(&pwmchip1,9);
    Lamp::PinLookupInit(Lamp::B1_,&LampB1);
    PCA9685PWMBit LampB2(&pwmchip1,10);
    Lamp::PinLookupInit(Lamp::B2_,&LampB2);
    PCA9685PWMBit LampB3(&pwmchip1,11);
    Lamp::PinLookupInit(Lamp::B3_,&LampB3);
    PCA9685PWMBit LampB4(&pwmchip1,12);
    Lamp::PinLookupInit(Lamp::B4_,&LampB4);
    PCA9685PWMBit LampB5(&pwmchip1,13);
    Lamp::PinLookupInit(Lamp::B5_,&LampB5);
    PCA9685PWMBit LampB6(&pwmchip1,14);
    Lamp::PinLookupInit(Lamp::B6_,&LampB6);
    PCA9685PWMBit LampB7(&pwmchip1,15);
    Lamp::PinLookupInit(Lamp::B7_,&LampB7);

#if NUM_PWMCHIPS == 2
    PCA9685PWM pwmchip2;
    pwmchip2.init("/dev/i2c0", PWMCHIP_ADDRESS2);
    
    PCA9685PWMBit LampC0(&pwmchip2,0);
    Lamp::PinLookupInit(Lamp::C0_,&LampC0);
    PCA9685PWMBit LampC1(&pwmchip2,1);
    Lamp::PinLookupInit(Lamp::C1_,&LampC1);
    PCA9685PWMBit LampC2(&pwmchip2,2);
    Lamp::PinLookupInit(Lamp::C2_,&LampC2);
    PCA9685PWMBit LampC3(&pwmchip2,3);
    Lamp::PinLookupInit(Lamp::C3_,&LampC3);
    PCA9685PWMBit LampC4(&pwmchip2,4);
    Lamp::PinLookupInit(Lamp::C4_,&LampC4);
    PCA9685PWMBit LampC5(&pwmchip2,5);
    Lamp::PinLookupInit(Lamp::C5_,&LampC5);
    PCA9685PWMBit LampC6(&pwmchip2,6);
    Lamp::PinLookupInit(Lamp::C6_,&LampC6);
    PCA9685PWMBit LampC7(&pwmchip2,7);
    Lamp::PinLookupInit(Lamp::C7_,&LampC7);

    PCA9685PWMBit LampD0(&pwmchip2,8);
    Lamp::PinLookupInit(Lamp::D0_,&LampD0);
    PCA9685PWMBit LampD1(&pwmchip2,9);
    Lamp::PinLookupInit(Lamp::D1_,&LampD1);
    PCA9685PWMBit LampD2(&pwmchip2,10);
    Lamp::PinLookupInit(Lamp::D2_,&LampD2);
    PCA9685PWMBit LampD3(&pwmchip2,11);
    Lamp::PinLookupInit(Lamp::D3_,&LampD3);
    PCA9685PWMBit LampD4(&pwmchip2,12);
    Lamp::PinLookupInit(Lamp::D4_,&LampD4);
    PCA9685PWMBit LampD5(&pwmchip2,13);
    Lamp::PinLookupInit(Lamp::D5_,&LampD5);
    PCA9685PWMBit LampD6(&pwmchip2,14);
    Lamp::PinLookupInit(Lamp::D6_,&LampD6);
    PCA9685PWMBit LampD7(&pwmchip2,15);
    Lamp::PinLookupInit(Lamp::D7_,&LampD7);

#endif
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
