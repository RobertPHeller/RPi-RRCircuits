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

OVERRIDE_CONST(local_nodes_count,50);

#include "openlcb/SimpleStack.hxx"
#include "openlcb/ConfiguredConsumer.hxx"
#include "openlcb/ConfiguredProducer.hxx"

#include "config.hxx"
#include "freertos_drivers/common/DummyGPIO.hxx"
#include "freertos_drivers/common/LoggingGPIO.hxx"
#include "os/LinuxGpio.hxx"
#include "utils/GpioInitializer.hxx"
#include "CommandStationStack.hxx"
#include "CommandStationConsole.hxx"
#include "DuplexUpdateLoop.hxx"
#include "CommandStationDCCMainTrack.hxx"
#include "CommandStationDCCProgTrack.hxx"
#include "HBridgeControl.hxx"
#include "FanControl.hxx"

#include "Hardware.hxx"

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
//openlcb::SimpleCanStack stack(NODE_ID);
#if defined(USE_GRIDCONNECT_HOST) || defined(USE_SOCKET_CAN_PORT)
openlcb::SimpleCommandStationCanStack stack(NODE_ID);
#else
#ifdef USE_OPENLCB_TCP_HOST
Executor<1> g_connect_executor("connect_executor", 0, 2048);
openlcb::SimpleCommandStationTcpStack stack(NODE_ID);
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


// Instantiates the actual producer and consumer objects for the given GPIO
// pins from above. The ConfiguredConsumer class takes care of most of the
// complicated setup and operation requirements. We need to give it the virtual
// node pointer, the configuration configuration from the CDI definition, and
// the hardware pin definition. The virtual node pointer comes from the stack
// object. The configuration structure comes from the CDI definition object,
// segment 'seg', in which there is a repeated group 'consumers', and we assign
// the individual entries to the individual consumers. Each consumer gets its
// own GPIO pin.

HBridgeControl mains(stack.node(), cfg.seg().maindcc(), CSenseMainAnalogChannel, MainEN_Pin::instance(), MainTF_Pin::instance());
HBridgeControl progtrack(stack.node(), cfg.seg().progdcc(), CSenseProgAnalogChannel, ProgEN_Pin::instance() );
FanControl     fan(stack.node(), cfg.seg().fancontrol(), TempsensorChannel,FanControl_Pin::instance());

openlcb::RefreshLoop loop(stack.node(),
                      {mains.polling(), 
                          progtrack.polling(), 
                          fan.polling()});

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
    fprintf(stderr, "OpenMRN-Cxx-Node.\nManages a Beagle Bone Command Station Cape.\n");
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

//dcc::UpdateLoopBase DccPacketLoop();

#ifdef TERMINALCONSOLE
CommandStationConsole commandProcessorConsole(stack.info_flow(),
                                              stack.traction_service(),
                                              stack.executor(),
                                              Console::FD_STDIN,
                                              Console::FD_STDOUT);
#else
CommandStationConsole commandProcessorConsole(stack.info_flow(),
                                              stack.traction_service(),
                                              stack.executor(),
                                              CONSOLEPORT);
#endif

CommandStationDCCMainTrack mainDCC(stack.traction_service(),2);
CommandStationDCCProgTrack progDCC(stack.traction_service(),2);


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
    mainDCC.StartPRU();
    progDCC.StartPRU();
    
    // Connects to a TCP hub on the internet.
    //stack.connect_tcp_gridconnect_hub("28k.ch", 50007);
#ifdef USE_TCP_GRIDCONNECT_HOST
    stack.connect_tcp_gridconnect_hub(upstream_host, upstream_port);
#endif
#ifdef USE_OPENLCB_TCP_HOST
    SocketClient socket_client(stack.service(), &g_connect_executor,
      &g_connect_executor,
      SocketClientParams::from_static(upstream_host,upstream_port),
      &connect_callback);
#endif
#ifdef PRINT_ALL_PACKETS
    // Causes all packets to be dumped to stdout.
    stack.print_all_packets();
#endif
#if defined(USE_SOCKET_CAN_PORT)
    stack.add_socketcan_port_select(cansocket);
#endif
        
    // This command donates the main thread to the operation of the
    // stack. Alternatively the stack could be started in a separate stack and
    // then application-specific business logic could be executed ion a busy
    // loop in the main thread.
    stack.loop_executor();
    return 0;
}
