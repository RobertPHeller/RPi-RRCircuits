/** \copyright
 * Copyright (c) 2019, Robert Heller
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
 * Main file for the 
 *
 * @author Robert Heller
 * @date 3 Feb 2019
 */

#include "os/os.h"
#include "nmranet_config.h"

#include "openlcb/SimpleStack.hxx"
#include "openlcb/ConfiguredConsumer.hxx"
#include "openlcb/ConfiguredProducer.hxx"

#include "config.hxx"
#include "freertos_drivers/common/DummyGPIO.hxx"
#include "freertos_drivers/common/LoggingGPIO.hxx"
#include "os/LinuxGpio.hxx"
#include "os/LinuxPWM.hxx"
#include "utils/GpioInitializer.hxx"                                   

#include "Hardware.hxx"

#include "Turnout.hxx"
#include "Points.hxx"

#include "OccupancyDetector.hxx"

#include "Mast.hxx"
#include "Blink.hxx"
#include "TrackCircuit.hxx"
#include "Logic.hxx"

LinuxPWM A0_Pin(PWMCHIP,A0);
LinuxPWM A1_Pin(PWMCHIP,A1);
LinuxPWM A2_Pin(PWMCHIP,A2);
LinuxPWM A3_Pin(PWMCHIP,A3);
LinuxPWM A4_Pin(PWMCHIP,A4);
LinuxPWM A5_Pin(PWMCHIP,A5);
LinuxPWM A6_Pin(PWMCHIP,A6);
LinuxPWM A7_Pin(PWMCHIP,A7);

LinuxPWM B0_Pin(PWMCHIP,B0);
LinuxPWM B1_Pin(PWMCHIP,B1);
LinuxPWM B2_Pin(PWMCHIP,B2);
LinuxPWM B3_Pin(PWMCHIP,B3);
LinuxPWM B4_Pin(PWMCHIP,B4);
LinuxPWM B5_Pin(PWMCHIP,B5);
LinuxPWM B6_Pin(PWMCHIP,B6);
LinuxPWM B7_Pin(PWMCHIP,B7);

// Changes the default behavior by adding a newline after each gridconnect
// packet. Makes it easier for debugging the raw device.
OVERRIDE_CONST(gc_generate_newlines, 1);
// Specifies how much RAM (in bytes) we allocate to the stack of the main
// thread. Useful tuning parameter in case the application runs out of memory.
OVERRIDE_CONST(main_thread_stack_size, 2500);

// Specifies the 48-bit OpenLCB node identifier. This must be unique for every
// hardware manufactured, so in production this should be replaced by some
// easily incrementable method.
#include "NODEID.hxx"
//extern const openlcb::NodeID NODE_ID = MyAddress;

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

Turnout turnout1(stack.node(), cfg.seg().turnouts().entry<0>(),Motor1_Pin());
Turnout turnout2(stack.node(), cfg.seg().turnouts().entry<1>(),Motor2_Pin());

Points  points1(stack.node(), cfg.seg().points().entry<0>(),Points1_Pin());
Points  points2(stack.node(), cfg.seg().points().entry<1>(),Points2_Pin());

OccupancyDetector oc1(stack.node(), cfg.seg().ocs().entry<0>(),OD1_Pin());
OccupancyDetector oc2(stack.node(), cfg.seg().ocs().entry<1>(),OD2_Pin());

BlinkTimer blinker(stack.executor()->active_timers());

Mast m1(stack.node(),cfg.seg().masts().entry<0>(),nullptr);
Mast m2(stack.node(),cfg.seg().masts().entry<1>(),&m1);
Mast m3(stack.node(),cfg.seg().masts().entry<2>(),&m2);
Mast m4(stack.node(),cfg.seg().masts().entry<3>(),&m3);
Mast m5(stack.node(),cfg.seg().masts().entry<4>(),&m4);
Mast m6(stack.node(),cfg.seg().masts().entry<5>(),&m5);
Mast m7(stack.node(),cfg.seg().masts().entry<6>(),&m6);
Mast m8(stack.node(),cfg.seg().masts().entry<7>(),&m7);

TrackCircuit c1(stack.node(),cfg.seg().circuits().entry<0>());
TrackCircuit c2(stack.node(),cfg.seg().circuits().entry<1>());
TrackCircuit c3(stack.node(),cfg.seg().circuits().entry<2>());
TrackCircuit c4(stack.node(),cfg.seg().circuits().entry<3>());
TrackCircuit c5(stack.node(),cfg.seg().circuits().entry<4>());
TrackCircuit c6(stack.node(),cfg.seg().circuits().entry<5>());
TrackCircuit c7(stack.node(),cfg.seg().circuits().entry<6>());
TrackCircuit c8(stack.node(),cfg.seg().circuits().entry<7>());

TrackCircuit *circuits[8] = {&c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8};

Logic l32(stack.node(),cfg.seg().logics().entry<31>(),stack.executor()->active_timers(),nullptr);
Logic l31(stack.node(),cfg.seg().logics().entry<30>(),stack.executor()->active_timers(),&l32);
Logic l30(stack.node(),cfg.seg().logics().entry<29>(),stack.executor()->active_timers(),&l31);
Logic l29(stack.node(),cfg.seg().logics().entry<28>(),stack.executor()->active_timers(),&l30);
Logic l28(stack.node(),cfg.seg().logics().entry<27>(),stack.executor()->active_timers(),&l29);
Logic l27(stack.node(),cfg.seg().logics().entry<26>(),stack.executor()->active_timers(),&l28);
Logic l26(stack.node(),cfg.seg().logics().entry<25>(),stack.executor()->active_timers(),&l27);
Logic l25(stack.node(),cfg.seg().logics().entry<24>(),stack.executor()->active_timers(),&l26);
Logic l24(stack.node(),cfg.seg().logics().entry<23>(),stack.executor()->active_timers(),&l25);
Logic l23(stack.node(),cfg.seg().logics().entry<22>(),stack.executor()->active_timers(),&l24);
Logic l22(stack.node(),cfg.seg().logics().entry<21>(),stack.executor()->active_timers(),&l23);
Logic l21(stack.node(),cfg.seg().logics().entry<20>(),stack.executor()->active_timers(),&l22);
Logic l20(stack.node(),cfg.seg().logics().entry<19>(),stack.executor()->active_timers(),&l21);
Logic l19(stack.node(),cfg.seg().logics().entry<18>(),stack.executor()->active_timers(),&l20);
Logic l18(stack.node(),cfg.seg().logics().entry<17>(),stack.executor()->active_timers(),&l19);
Logic l17(stack.node(),cfg.seg().logics().entry<16>(),stack.executor()->active_timers(),&l18);
Logic l16(stack.node(),cfg.seg().logics().entry<15>(),stack.executor()->active_timers(),&l17);
Logic l15(stack.node(),cfg.seg().logics().entry<14>(),stack.executor()->active_timers(),&l16);
Logic l14(stack.node(),cfg.seg().logics().entry<13>(),stack.executor()->active_timers(),&l15);
Logic l13(stack.node(),cfg.seg().logics().entry<12>(),stack.executor()->active_timers(),&l14);
Logic l12(stack.node(),cfg.seg().logics().entry<11>(),stack.executor()->active_timers(),&l13);
Logic l11(stack.node(),cfg.seg().logics().entry<10>(),stack.executor()->active_timers(),&l12);
Logic l10(stack.node(),cfg.seg().logics().entry<9>(),stack.executor()->active_timers(),&l11);
Logic l9(stack.node(),cfg.seg().logics().entry<8>(),stack.executor()->active_timers(),&l10);
Logic l8(stack.node(),cfg.seg().logics().entry<7>(),stack.executor()->active_timers(),&l9);
Logic l7(stack.node(),cfg.seg().logics().entry<6>(),stack.executor()->active_timers(),&l8);
Logic l6(stack.node(),cfg.seg().logics().entry<5>(),stack.executor()->active_timers(),&l7);
Logic l5(stack.node(),cfg.seg().logics().entry<4>(),stack.executor()->active_timers(),&l6);
Logic l4(stack.node(),cfg.seg().logics().entry<3>(),stack.executor()->active_timers(),&l5);
Logic l3(stack.node(),cfg.seg().logics().entry<2>(),stack.executor()->active_timers(),&l4);
Logic l2(stack.node(),cfg.seg().logics().entry<1>(),stack.executor()->active_timers(),&l3);
Logic l1(stack.node(),cfg.seg().logics().entry<0>(),stack.executor()->active_timers(),&l2);

// The producers need to be polled repeatedly for changes and to execute the
// debouncing algorithm. This class instantiates a refreshloop and adds the two
// producers to it.
openlcb::RefreshLoop loop(stack.node(),{points1.polling()
          , points2.polling()
          , oc1.polling()
          , oc2.polling()
});

class FactoryResetHelper : public DefaultConfigUpdateListener {
public:
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE 
    {
        AutoNotify n(done);
        return UPDATED;
    }
    void factory_reset(int fd) override
    {
        cfg.userinfo().name().write(fd, HARDWARE_IMPL);
        cfg.userinfo().description().write(
            fd, HARDWARE_IMPL);
    }
} factory_reset_helper;
                                           
                                           
                                           

void usage(const char *e)
{
    fprintf(stderr, "Usage: %s [-e EEPROM_file_path]\n\n", e);
    fprintf(stderr, "OpenMRN-Cxx-Node.\nManages a QuadSMCSense HAT.\n");
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
    GpioInit::hw_init();
    A0_Pin.export_pin();
    A1_Pin.export_pin();
    A2_Pin.export_pin();
    A3_Pin.export_pin();
    A4_Pin.export_pin();
    A5_Pin.export_pin();
    A6_Pin.export_pin();
    A7_Pin.export_pin();

    B0_Pin.export_pin();
    B1_Pin.export_pin();
    B2_Pin.export_pin();
    B3_Pin.export_pin();
    B4_Pin.export_pin();
    B5_Pin.export_pin();
    B6_Pin.export_pin();
    B7_Pin.export_pin();
    
    stack.create_config_file_if_needed(cfg.seg().internal_config(), openlcb::CANONICAL_VERSION, openlcb::CONFIG_FILE_SIZE);
    
    blinker.start(500000000);
    
    // Connects to a TCP hub on the internet.
    //stack.connect_tcp_gridconnect_hub("28k.ch", 50007);
#ifdef HAVE_TCP_GRIDCONNECT_HOST
    stack.connect_tcp_gridconnect_hub(TCP_GRIDCONNECT_HOST, TCP_GRIDCONNECT_PORT);
#endif
#ifdef PRINT_ALL_PACKETS
    // Causes all packets to be dumped to stdout.
    stack.print_all_packets();
#endif
#if defined(HAVE_SOCKET_CAN_PORT)
    stack.add_socketcan_port_select(SOCKET_CAN_PORT);
#endif
// This command donates the main thread to the operation of the
    // stack. Alternatively the stack could be started in a separate stack and
    // then application-specific business logic could be executed ion a busy
    // loop in the main thread.
    stack.loop_executor();
    return 0;
}
