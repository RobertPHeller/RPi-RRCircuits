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
#include "openlcb/MultiConfiguredPC.hxx"
#include "utils/HubDeviceSelect.hxx"
#include "utils/SocketClient.hxx"
#include "utils/SocketClientParams.hxx"

#include "config.hxx"
#include "freertos_drivers/common/DummyGPIO.hxx"
#include "freertos_drivers/common/LoggingGPIO.hxx"
#include "os/LinuxGpio.hxx"
#include "utils/GpioInitializer.hxx"                                   

#include "Hardware.hxx"
#include "Logic.hxx"
#include "TrackCircuit.hxx"

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

#if defined(USE_GRIDCONNECT_HOST) || defined(USE_SOCKET_CAN_PORT)      
// Sets up a comprehensive OpenLCB stack for a single virtual node. This stack
// contains everything needed for a usual peripheral node -- all
// CAN-bus-specific components, a virtual node, PIP, SNIP, Memory configuration
// protocol, ACDI, CDI, a bunch of memory spaces, etc.
openlcb::SimpleCanStack stack(NODE_ID);
#else
#ifdef USE_OPENLCB_TCP_HOST
Executor<1> g_connect_executor("connect_executor", 0, 2048);
openlcb::SimpleTcpStack stack(NODE_ID);
#else
#error "None of USE_GRIDCONNECT_HOST, USE_SOCKET_CAN_PORT, orUSE_OPENLCB_TCP_HOST defined!"
#endif
#endif

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

const Gpio *const kGPIO1_Header[] = {
    GPIO1_1_Pin::instance(), GPIO1_2_Pin::instance(),
    GPIO1_3_Pin::instance(), GPIO1_4_Pin::instance(),
    GPIO1_7_Pin::instance(), GPIO1_8_Pin::instance(),
    GPIO1_9_Pin::instance(), GPIO1_10_Pin::instance()};

const Gpio *const kGPIO2_Header[] = {
    GPIO2_1_Pin::instance(), GPIO2_2_Pin::instance(),
    GPIO2_3_Pin::instance(), GPIO2_4_Pin::instance(),
    GPIO2_7_Pin::instance(), GPIO2_8_Pin::instance(),
    GPIO2_9_Pin::instance(), GPIO2_10_Pin::instance()};

const Gpio *const kGPIO3_Header[] = {
    GPIO3_1_Pin::instance(), GPIO3_2_Pin::instance(),
    GPIO3_3_Pin::instance(), GPIO3_4_Pin::instance(),
    GPIO3_7_Pin::instance(), GPIO3_8_Pin::instance(),
    GPIO3_9_Pin::instance(), GPIO3_10_Pin::instance()};

const Gpio *const kGPIO4_Header[] = {
    GPIO4_1_Pin::instance(), GPIO4_2_Pin::instance(),
    GPIO4_3_Pin::instance(), GPIO4_4_Pin::instance(),
    GPIO4_7_Pin::instance(), GPIO4_8_Pin::instance(),
    GPIO4_9_Pin::instance(), GPIO4_10_Pin::instance()};

#ifdef EXTRAHEADERS
const Gpio *const kGPIO5_Header[] = {
    GPIO5_1_Pin::instance(), GPIO5_2_Pin::instance(),
    GPIO5_3_Pin::instance(), GPIO5_4_Pin::instance(),
    GPIO5_7_Pin::instance(), GPIO5_8_Pin::instance(),
    GPIO5_9_Pin::instance(), GPIO5_10_Pin::instance()};

const Gpio *const kGPIO6_Header[] = {
    GPIO6_1_Pin::instance(), GPIO6_2_Pin::instance(),
    GPIO6_3_Pin::instance(), GPIO6_4_Pin::instance(),
    GPIO6_7_Pin::instance(), GPIO6_8_Pin::instance(),
    GPIO6_9_Pin::instance(), GPIO6_10_Pin::instance()};
#endif
#ifdef HAVEQUADMCP23017
const Gpio *const kGPIO5_Header[] = {
    GPIO5_1_Pin::instance(), GPIO5_2_Pin::instance(),
    GPIO5_3_Pin::instance(), GPIO5_4_Pin::instance(),
    GPIO5_7_Pin::instance(), GPIO5_8_Pin::instance(),
    GPIO5_9_Pin::instance(), GPIO5_10_Pin::instance()};

const Gpio *const kGPIO6_Header[] = {
    GPIO6_1_Pin::instance(), GPIO6_2_Pin::instance(),
    GPIO6_3_Pin::instance(), GPIO6_4_Pin::instance(),
    GPIO6_7_Pin::instance(), GPIO6_8_Pin::instance(),
    GPIO6_9_Pin::instance(), GPIO6_10_Pin::instance()};

const Gpio *const kGPIO7_Header[] = {
    GPIO7_1_Pin::instance(), GPIO7_2_Pin::instance(),
    GPIO7_3_Pin::instance(), GPIO7_4_Pin::instance(),
    GPIO7_7_Pin::instance(), GPIO7_8_Pin::instance(),
    GPIO7_9_Pin::instance(), GPIO7_10_Pin::instance()};

const Gpio *const kGPIO8_Header[] = {
    GPIO8_1_Pin::instance(), GPIO8_2_Pin::instance(),
    GPIO8_3_Pin::instance(), GPIO8_4_Pin::instance(),
    GPIO8_7_Pin::instance(), GPIO8_8_Pin::instance(),
    GPIO8_9_Pin::instance(), GPIO8_10_Pin::instance()};
#endif
openlcb::MultiConfiguredPC gpio1(stack.node(),
   kGPIO1_Header, ARRAYSIZE(kGPIO1_Header),
   cfg.seg().headers().entry<0>().gpio());

openlcb::MultiConfiguredPC gpio2(stack.node(),
   kGPIO2_Header, ARRAYSIZE(kGPIO2_Header),
   cfg.seg().headers().entry<1>().gpio());

openlcb::MultiConfiguredPC gpio3(stack.node(),
   kGPIO3_Header, ARRAYSIZE(kGPIO3_Header),
   cfg.seg().headers().entry<2>().gpio());

openlcb::MultiConfiguredPC gpio4(stack.node(),
   kGPIO4_Header, ARRAYSIZE(kGPIO4_Header),
   cfg.seg().headers().entry<3>().gpio());

#ifdef EXTRAHEADERS
openlcb::MultiConfiguredPC gpio5(stack.node(),
   kGPIO5_Header, ARRAYSIZE(kGPIO5_Header),
   cfg.seg().headers().entry<4>().gpio());

openlcb::MultiConfiguredPC gpio6(stack.node(),
   kGPIO6_Header, ARRAYSIZE(kGPIO6_Header),
   cfg.seg().headers().entry<5>().gpio());
#endif
#ifdef HAVEQUADMCP23017
openlcb::MultiConfiguredPC gpio5(stack.node(),
   kGPIO5_Header, ARRAYSIZE(kGPIO5_Header),
   cfg.seg().headers().entry<4>().gpio());

openlcb::MultiConfiguredPC gpio6(stack.node(),
   kGPIO6_Header, ARRAYSIZE(kGPIO6_Header),
   cfg.seg().headers().entry<5>().gpio());

openlcb::MultiConfiguredPC gpio7(stack.node(),
   kGPIO7_Header, ARRAYSIZE(kGPIO7_Header),
   cfg.seg().headers().entry<6>().gpio());

openlcb::MultiConfiguredPC gpio8(stack.node(),
   kGPIO8_Header, ARRAYSIZE(kGPIO8_Header),
   cfg.seg().headers().entry<7>().gpio());
#endif

openlcb::RefreshLoop gpioLoop(stack.node(), {
                          gpio1.polling(),
                          gpio2.polling(),
                          gpio3.polling(),
                          gpio4.polling(),
#ifdef EXTRAHEADERS
                          gpio5.polling(),
                          gpio6.polling(),
#endif
#ifdef HAVEQUADMCP23017
                          gpio5.polling(),
                          gpio6.polling(),
                          gpio7.polling(),
                          gpio8.polling(),
#endif
          
});



// Instantiates the actual producer and consumer objects for the given GPIO
// pins from above. The ConfiguredConsumer class takes care of most of the
// complicated setup and operation requirements. We need to give it the virtual
// node pointer, the configuration configuration from the CDI definition, and
// the hardware pin definition. The virtual node pointer comes from the stack
// object. The configuration structure comes from the CDI definition object,
// segment 'seg', in which there is a repeated group 'consumers', and we assign
// the individual entries to the individual consumers. Each consumer gets its
// own GPIO pin.

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
                                           
#ifdef USE_OPENLCB_TCP_HOST
int upstream_port = DEFAULT_OPENLCB_TCP_PORT;
const char *upstream_host = DEFAULT_OPENLCB_TCP_HOST;
#else
#ifdef USE_GRIDCONNECT_HOST
int upstream_port = DEFAULT_TCP_GRIDCONNECT_PORT;
const char *upstream_host = DEFAULT_TCP_GRIDCONNECT_HOST;
#endif
#ifdef USE_SOCKET_CAN_PORT
const char *cansocket = DEFAULT_CAN_SOCKET;
#endif
#endif

void usage(const char *e)
{
    fprintf(stderr, "Usage: %s [-e EEPROM_file_path]", e);
#if defined(USE_OPENLCB_TCP_HOST) || defined(USE_GRIDCONNECT_HOST)
    fprintf(stderr, " [-u upstream_host] [-q upstream_port]");
#endif
#ifdef USE_SOCKET_CAN_PORT
    fprintf(stderr, " [-c can_socketname]");
#endif
    fprintf(stderr, "\n\n");    
    fprintf(stderr, "OpenMRN-Cxx-Node.\nManages a QuadSMCSense HAT.\n");
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "\t-e EEPROM_file_path is the path to use to the EEProm device.\n");
#if defined(USE_OPENLCB_TCP_HOST) || defined(USE_GRIDCONNECT_HOST)
    fprintf(stderr,"\t-u upstream_host   is the host name for an "
            "upstream hub.\n");
    fprintf(stderr,
            "\t-q upstream_port   is the port number for the upstream "
            "hub.\n");
#endif
#ifdef USE_SOCKET_CAN_PORT
    fprintf(stderr,"\t-c can_socketname   is the name of the CAN "
            "socket.\n");
#endif
    exit(1);
}

void parse_args(int argc, char *argv[])
{
    int opt;
#if defined(USE_OPENLCB_TCP_HOST) || defined(USE_GRIDCONNECT_HOST)
#ifdef USE_SOCKET_CAN_PORT
#define OPTSTRING "he:u:q:c:"
#else
#define OPTSTRING "he:u:q:"
#endif
#else
#ifdef USE_SOCKET_CAN_PORT
#define OPTSTRING "he:c:"
#else
#define OPTSTRING "he:"
#endif
#endif
    while ((opt = getopt(argc, argv, OPTSTRING)) >= 0)
    {
        switch (opt)
        {
        case 'h':
            usage(argv[0]);
            break;
        case 'e':
            strncpy(pathnamebuffer,optarg,sizeof(pathnamebuffer));
            break;
#if defined(USE_OPENLCB_TCP_HOST) || defined(USE_GRIDCONNECT_HOST)
        case 'u':
            upstream_host = optarg;
            break;
        case 'q':
            upstream_port = atoi(optarg);
            break;
#endif
#ifdef USE_SOCKET_CAN_PORT
        case 'c':
            cansocket = optarg;
            break;
#endif
        default:
            fprintf(stderr, "Unknown option %c\n", opt);
            usage(argv[0]);
        }
    }
}

#ifdef USE_OPENLCB_TCP_HOST
void connect_callback(int fd, Notifiable *on_error)
{
    LOG(INFO, "Connected to hub.");
    stack.add_tcp_port_select(fd, on_error);
    stack.restart_stack();
}
#endif

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
    stack.create_config_file_if_needed(cfg.seg().internal_config(), openlcb::CANONICAL_VERSION, openlcb::CONFIG_FILE_SIZE);
    
    
    // Connects to a TCP hub on the internet.
    //stack.connect_tcp_gridconnect_hub("28k.ch", 50007);
#ifdef USE_GRIDCONNECT_HOST
    stack.connect_tcp_gridconnect_hub(upstream_host, upstream_port);
#endif
#ifdef PRINT_ALL_PACKETS
    // Causes all packets to be dumped to stdout.
    stack.print_all_packets();
#endif
#ifdef USE_OPENLCB_TCP_HOST
    SocketClient socket_client(stack.service(), &g_connect_executor,
      &g_connect_executor,
      SocketClientParams::from_static(upstream_host,upstream_port),
      &connect_callback);
#endif
#if defined(HAVE_SOCKET_CAN_PORT)
    stack.add_socketcan_port_select(cansocket);
#endif
    
    // This command donates the main thread to the operation of the
    // stack. Alternatively the stack could be started in a separate stack and
    // then application-specific business logic could be executed ion a busy
    // loop in the main thread.
    stack.loop_executor();
    return 0;
}
