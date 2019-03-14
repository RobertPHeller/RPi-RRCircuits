// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Mar 13 10:22:45 2019
//  Last Modified : <190313.1436>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
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

/** @file ESP32BaseBoard.ino
 * 
 * Generic base for the various ESP32 I/O boards from Deepwoods Software.
 * (Based on ESP32IOBoard.ino by Mike Dunston.)
 * 
 * @author Robert Heller
 * @date 13 March 2019
 */

#include <Arduino.h>
#include <SPIFFS.h>

#include <OpenMRNLite.h>

static const char rcsid[] = "@(#) : $Id$";


#include "config.h"
#include "NODEID.h" // Get nodeid from an externally generated header file

#include <utils/GpioInitializer.hxx>

#include <freertos_drivers/arduino/ArduinoGpio.hxx>
#include "Turnout.h"
#include "Points.h"


#define MotorPin GpioOutputSafeLow
#define SensePin GpioInputNP

GPIO_PIN(Motor1, MotorPin, 0);
GPIO_PIN(Motor2, MotorPin, 12);
GPIO_PIN(Motor3, MotorPin, 15);
GPIO_PIN(Motor4, MotorPin, 25);

GPIO_PIN(Points1, SensePin, 34);
GPIO_PIN(Points2, SensePin, 35);
GPIO_PIN(Points3, SensePin, 32);
GPIO_PIN(Points4, SensePin, 33);

// Create an initializer that can initialize all the GPIO pins in one shot
typedef GpioInitializer<Motor1_Pin, Motor2_Pin, Motor3_Pin, 
                        Motor4_Pin, Points1_Pin, Points2_Pin, 
                        Points3_Pin, Points4_Pin> GpioInit;

constexpr gpio_num_t CAN_RX_PIN = GPIO_NUM_4;
constexpr gpio_num_t CAN_TX_PIN = GPIO_NUM_5;

/// This is the primary entrypoint for the OpenMRN/LCC stack.
OpenMRN openmrn(NODE_ID);

// note the dummy string below is required due to a bug in the GCC compiler
// for the ESP32
string dummystring("abcdef");

/// ConfigDef comes from config.h and is specific to this particular device and
/// target. It defines the layout of the configuration memory space and is also
/// used to generate the cdi.xml file. Here we instantiate the configuration
/// layout. The argument of offset zero is ignored and will be removed later.
static constexpr openlcb::ConfigDef cfg(0);

// Instantiates the actual producer and consumer objects for the given GPIO
// pins from above. The ConfiguredConsumer class takes care of most of the
// complicated setup and operation requirements. We need to give it the virtual
// node pointer, the configuration configuration from the CDI definition, and
// the hardware pin definition. The virtual node pointer comes from the stack
// object. The configuration structure comes from the CDI definition object,
// segment 'seg', in which there is a repeated group 'consumers', and we assign
// the individual entries to the individual consumers. Each consumer gets its
// own GPIO pin.

Turnout turnout1(openmrn.stack()->node(), cfg.seg().turnouts().entry<0>(),Motor1_Pin());
Turnout turnout2(openmrn.stack()->node(), cfg.seg().turnouts().entry<1>(),Motor2_Pin());
Turnout turnout3(openmrn.stack()->node(), cfg.seg().turnouts().entry<2>(),Motor3_Pin());
Turnout turnout4(openmrn.stack()->node(), cfg.seg().turnouts().entry<3>(),Motor4_Pin());

Points  points1(openmrn.stack()->node(), cfg.seg().points().entry<0>(),Points1_Pin());
Points  points2(openmrn.stack()->node(), cfg.seg().points().entry<1>(),Points2_Pin());
Points  points3(openmrn.stack()->node(), cfg.seg().points().entry<2>(),Points3_Pin());
Points  points4(openmrn.stack()->node(), cfg.seg().points().entry<3>(),Points4_Pin());


// The producers need to be polled repeatedly for changes and to execute the
// debouncing algorithm. This class instantiates a refreshloop and adds the two
// producers to it.
openlcb::RefreshLoop points_refresh_loop(openmrn.stack()->node(),{
    points1.polling()
          , points2.polling()
          , points3.polling()
          , points4.polling()
});

class FactoryResetHelper : public DefaultConfigUpdateListener {
public:
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE {
        AutoNotify n(done);
        return UPDATED;
    }

    void factory_reset(int fd) override
    {
        cfg.userinfo().name().write(fd, openlcb::SNIP_STATIC_DATA.model_name);
        cfg.userinfo().description().write(
            fd, "OpenLCB + Arduino-ESP32 on an ESP32QuadSMCSense.");
        for(int i = 0; i < openlcb::NUM_TURNOUTS; i++)
        {
            cfg.seg().turnouts().entry(i).description().write(fd, "");
        }
        for(int i = 0; i < openlcb::NUM_POINTSS; i++)
        {
            cfg.seg().points().entry(i).description().write(fd, "");
        }
    }
} factory_reset_helper;

namespace openlcb
{
    // Name of CDI.xml to generate dynamically.
    const char CDI_FILENAME[] = "/spiffs/cdi.xml";

    // This will stop openlcb from exporting the CDI memory space upon start.
    extern const char CDI_DATA[] = "";

    // Path to where OpenMRN should persist general configuration data.
    extern const char *const CONFIG_FILENAME = "/spiffs/openlcb_config";

    // The size of the memory space to export over the above device.
    extern const size_t CONFIG_FILE_SIZE = cfg.seg().size() + cfg.seg().offset();

    // Default to store the dynamic SNIP data is stored in the same persistant
    // data file as general configuration data.
    extern const char *const SNIP_DYNAMIC_FILENAME = CONFIG_FILENAME;
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200L);
    // Initialize the SPIFFS filesystem as our persistence layer
    if (!SPIFFS.begin())
    {
        printf("SPIFFS failed to mount, attempting to format and remount\n");
        if (!SPIFFS.begin(true))
        {
            printf("SPIFFS mount failed even with format, giving up!\n");
            while (1)
            {
                // Unable to start SPIFFS successfully, give up and wait
                // for WDT to kick in
            }
        }
    }

    // Create the CDI.xml dynamically
    openmrn.create_config_descriptor_xml(cfg, openlcb::CDI_FILENAME);

    // Create the default internal configuration file
    openmrn.stack()->create_config_file_if_needed(cfg.seg().internal_config(),
        openlcb::CANONICAL_VERSION, openlcb::CONFIG_FILE_SIZE);

    // initialize all declared GPIO pins
    GpioInit::hw_init();

    // Start the OpenMRN stack
    openmrn.begin();
    openmrn.start_executor_thread();

    // Add the hardware CAN device as a bridge
    openmrn.add_can_port(
        new Esp32HardwareCan("esp32can", CAN_RX_PIN, CAN_TX_PIN));
}
                
void loop() {
    // put your main code here, to run repeatedly:
    // Call the OpenMRN executor, this needs to be done as often
    // as possible from the loop() method.
    openmrn.loop();
}    
