/** \copyright
 * Copyright (c) 2013, Balazs Racz
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are  permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \file main.cxx
 *
 * Main file for the io board application on the Tiva Launchpad board.
 *
 * @author Balazs Racz
 * @date 5 Jun 2015
 */

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "os/os.h"
#include "nmranet_config.h"

#include "openlcb/SimpleStack.hxx"
#include "openlcb/ConfiguredConsumer.hxx"
#include "openlcb/ConfiguredProducer.hxx"

#include "config.hxx"
#include "Hardware.hxx"
#include "Turnout.hxx"
#include "Points.hxx"
#include "OccDetector.hxx"
#include "Mast.hxx"
#include "MadHatter.hxx"

// Changes the default behavior by adding a newline after each gridconnect
// packet. Makes it easier for debugging the raw device.
OVERRIDE_CONST(gc_generate_newlines, 1);
// Specifies how much RAM (in bytes) we allocate to the stack of the main
// thread. Useful tuning parameter in case the application runs out of memory.
OVERRIDE_CONST(main_thread_stack_size, 2500);

// Specifies the 48-bit OpenLCB node identifier. This must be unique for every
// hardware manufactured, so in production this should be replaced by some
// easily incrementable method.
extern const openlcb::NodeID NODE_ID = 0x050101012240ULL; // 05 01 01 01 22 40

// Sets up a comprehensive OpenLCB stack for a single virtual node. This stack
// contains everything needed for a usual peripheral node -- all
// CAN-bus-specific components, a virtual node, PIP, SNIP, Memory configuration
// protocol, ACDI, CDI, a bunch of memory spaces, etc.
openlcb::SimpleCanStack stack(NODE_ID);

// ConfigDef comes from config.hxx and is specific to the particular device and
// target. It defines the layout of the configuration memory space and is also
// used to generate the cdi.xml file. Here we instantiate the configuration
// layout. The argument of offset zero is ignored and will be removed later.
openlcb::ConfigDef cfg(0);
// Defines weak constants used by the stack to tell it which device contains
// the volatile configuration information. This device name appears in
// HwInit.cxx that creates the device drivers.
char pathnamebuffer[256];

extern const char *const openlcb::CONFIG_FILENAME = pathnamebuffer;
// The size of the memory space to export over the above device.
extern const size_t openlcb::CONFIG_FILE_SIZE =
    cfg.seg().size() + cfg.seg().offset();
// The SNIP user-changeable information in also stored in the above eeprom
// device. In general this could come from different eeprom segments, but it is
// simpler to keep them together.
extern const char *const openlcb::SNIP_DYNAMIC_FILENAME =
    openlcb::CONFIG_FILENAME;


// Instantiates the actual producer and consumer objects for the given GPIO
// pins from above. The ConfiguredConsumer class takes care of most of the
// complicated setup and operation requirements. We need to give it the virtual
// node pointer, the configuration configuration from the CDI definition, and
// the hardware pin definition. The virtual node pointer comes from the stack
// object. The configuration structure comes from the CDI definition object,
// segment 'seg', in which there is a repeated group 'consumers', and we assign
// the individual entries to the individual consumers. Each consumer gets its
// own GPIO pin.

// Turnout motors.  Basically exactly the same as a 
// openlcb::ConfiguredConsumer, but with different field names.
Turnout Turnout1(
      stack.node(), cfg.seg().turnouts().entry<0>(), Motor1_Pin());
Turnout Turnout2(
                 stack.node(), cfg.seg().turnouts().entry<1>(), Motor2_Pin());

// Similar syntax for the producers.
// Points is much line a ConfiguredProducer, except there is no 
// debouncer, since these inputs are hardware debounced using a RS 
// FlipFlop formed from a cross-wired 2-input NAND gates (74HCT00).
// There is also an extra accessor function for the benefit of the 
// Mast classes.
Points Points1(
      stack.node(), cfg.seg().points().entry<0>(), Points1_Pin());
Points Points2(
      stack.node(), cfg.seg().points().entry<1>(), Points2_Pin());

// OccupancyDetector is also just like a ConfiguredProducer, except 
// for the field names and the fact that it is active low and has an
// extra accessor function for the benefit of the Mast classes.
OccupancyDetector MainEastOcc(
      stack.node(), cfg.seg().quadsssquadin().maineast(), MainEast_Pin());
OccupancyDetector MainWestOcc(
      stack.node(), cfg.seg().quadsssquadin().mainwest(), MainWest_Pin());
OccupancyDetector SidingOcc(
      stack.node(), cfg.seg().quadsssquadin().siding(), Siding_Pin());
OccupancyDetector S314159Occ(
      stack.node(), cfg.seg().quadsssquadin().s314159(), S314159_Pin());

// The Mad Hatter Lights control is its own logic thing
MadHatter madHatter(
      stack.node(), cfg.seg().quadsssquadin().madhatterlights(), 
      &SidingOcc, &S314159Occ, MadHatterLights_Pin::instance());

// Masts test the occupancy bits, possibly along with point state and
// control the signal LEDs.  Produces events for aspects.

MastFrog CP314W(
     stack.node(), cfg.seg().masts().cp314west(),&S314159Occ,
     &Points1,openlcb::EventState::VALID,&MainWestOcc,
     CP314WUpperGreen_Pin::instance(), 
     CP314WUpperYellow_Pin::instance(),
     CP314WUpperRed_Pin::instance());

MastBlock MainWest(
     stack.node(), cfg.seg().masts().mainwest(),&MainWestOcc,
     &S314159Occ,              
     MainWestGreen_Pin::instance(), 
     MainWestYellow_Pin::instance(), 
     MainWestRed_Pin::instance());

MastBlock Siding(
     stack.node(), cfg.seg().masts().siding(),&SidingOcc,
     &S314159Occ,
     SidingGreen_Pin::instance(), 
     SidingYellow_Pin::instance(), 
     SidingRed_Pin::instance());

MastFrog CP314S(
     stack.node(), cfg.seg().masts().cp314siding(),&S314159Occ,
     &Points1, openlcb::EventState::INVALID, &MainEastOcc,
     CP314SLowerGreen_Pin::instance(), 
     CP314SLowerYellow_Pin::instance(), 
     CP314SLowerRed_Pin::instance());

MastBlock MainEast(
     stack.node(), cfg.seg().masts().maineast(),&MainEastOcc,
     &S314159Occ, MainEastGreen_Pin::instance(), 
     MainEastYellow_Pin::instance(), 
     MainEastRed_Pin::instance());

MastPoints CP314E(
     stack.node(), cfg.seg().masts().cp314east(),&S314159Occ,
     &Points1, openlcb::EventState::VALID,&MainWestOcc,
     CP314EUpperGreen_Pin::instance(), 
     CP314EUpperYellow_Pin::instance(),
     CP314EUpperRed_Pin::instance(), 
     CP314ELowerYellow_Pin::instance(), 
     CP314ELowerRed_Pin::instance());


// The producers need to be polled repeatedly for changes and to execute the
// debouncing algorithm. This class instantiates a refreshloop and adds the two
// producers to it.
openlcb::RefreshLoop loop(
      stack.node(), {Points1.polling(), Points2.polling(),
                     MainEastOcc.polling(),MainWestOcc.polling(),
                     SidingOcc.polling(),S314159Occ.polling(),
                     CP314W.polling(),MainWest.polling(),
                     Siding.polling(),CP314S.polling(),
                     MainEast.polling(),CP314E.polling() });


void usage(const char *e)
{
    fprintf(stderr, "Usage: %s [-e EEPROM_file_path]\n\n", e);
    fprintf(stderr, "PiJunction_OpenMRN.\nManages the Raspberry Pi Junction.\n");
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "\t-e EEPROM_file_path is the path to use to the EEProm device.\n");
    exit(1);
}

void parse_args(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "e:")) >= 0)
    {
        switch (opt)
        {
        case 'e':
            strncpy(pathnamebuffer,optarg,sizeof(pathnamebuffer));
            break;
        default:
            fprintf(stderr, "Unknown option %c\n", opt);
            usage(argv[0]);
        }
    }
}




/** Entry point to application.
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return 0, should never return
 */
int appl_main(int argc, char *argv[])
{
    snprintf(pathnamebuffer,sizeof(pathnamebuffer),
             "/tmp/config_eeprom_%012llX",NODE_ID);
    parse_args(argc, argv);
    
    
    // Initialize the GPIO pins.
    GpioInit::hw_init();
    // Light the "dummy" heads.
    CP314WLowRed_Pin::set(true);
    CP314SUpperRed_Pin::set(true);
    stack.create_config_file_if_needed(cfg.seg().internal_config(), openlcb::CANONICAL_VERSION, openlcb::CONFIG_FILE_SIZE);
    // Connects to a TCP hub on the internet.
    //stack.connect_tcp_gridconnect_hub("28k.ch", 50007);
    stack.connect_tcp_gridconnect_hub("localhost", 12021);
    // Causes all packets to be dumped to stdout.
    //stack.print_all_packets();
    // This command donates the main thread to the operation of the
    // stack. Alternatively the stack could be started in a separate stack and
    // then application-specific business logic could be executed ion a busy
    // loop in the main thread.
    stack.loop_executor();
    return 0;
}
