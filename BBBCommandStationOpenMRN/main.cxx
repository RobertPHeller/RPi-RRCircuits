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
 * Main file for the BBBCommandStationOpenMRN program.
 *
 * @author Robert Heller
 * @date 3 Feb 2019 -- 11 May 2021
 * 
 * @defgroup BBBCommandStationOpenMRN Beagle Board Command Station
 * 
 * @section SYNOPSIS SYNOPSIS
 * 
 * BBBCommandStationOpenMRN [options]
 * 
 * @section DESCRIPTION DESCRIPTION
 * 
 * This is the program for the Beagle Board LCC/DCC/Railcom command 
 * station.  It uses OpenMRN to interface with tha LCC/OpenLCB network
 * to implement a LCC/OpenLCB aware command station node that can 
 * operate DCC/Railcom locomotives.  It should work with any of the
 * Beagle Board addon boards I have designed.  It uses the AM335X's
 * PRUs to generate the DCC signals.  It ises the OpenMRN Console
 * class to communicate over a Tcp/Ip channel with a Tcl/Tk coded
 * GUI program to provide a user friendly point-and-click high level
 * user interface.
 * 
 * @section OPTIONS OPTIONS
 * 
 * @arg -e EEPROM_file_path is the path to use to implement the 
 *         EEProm device.
 * @arg -t Persistent_Train_file_path is the path to use to the 
 *         implement the train persistent data.
 * @arg -u upstream_host   is the host name for an upstream hub.
 * @arg -q upstream_port   is the port number for the upstream hub.
 * @arg -c can_socketname   is the name of the CAN socket.
 * @arg -M mainPRUfirmware  is the path to the Main (PRU0) firmware
 * @arg -P progPRUfirmware  is the path to the Prog (PRU1) firmware
 * @arg -W name:port Start a WiThrottle named name on port (if :port
 *         is ommited, on the default port).
 * @par
 * 
 * The -u and -q options are only available if the program was built 
 * to support either a OpenLCB Tcp host or a GRIDCONNECT host.
 * The -c option is only available if the program was built to 
 * support CAN Sockets.
 * 
 * @section PARAMETERS PARAMETERS
 * 
 * None.
 * 
 * @section FILES FILES
 * @section AUTHOR AUTHOR
 * @author Robert Heller
 * @date 29 Apr 2021
 * 
 * @mainpage Introduction
 * 
 * This is the program for the Beagle Board LCC/DCC/Railcom command 
 * station.  It uses OpenMRN to interface with tha LCC/OpenLCB network
 * to implement a LCC/OpenLCB aware command station node that can 
 * operate DCC/Railcom locomotives.  It should work with any of the
 * Beagle Board addon boards I have designed.  It uses the AM335X's
 * PRUs to generate the DCC signals.  It uses the OpenMRN Console
 * class to communicate over a Tcp/Ip channel with a Tcl/Tk coded
 * GUI program to provide a user friendly point-and-click high level
 * user interface.
 * 
 * @page BUILD Building
 * 
 * This program needs the OpenMRN library installed in a "standard"
 * place: /opt/openmrn or ~/openmrn or else in the location specified
 * by the environment variable OPENMRNPATH.
 * 
 * Various build options can be controlled in the Hardware.hxx header
 * file in the target directory.  Specificly, the GPIO pin 
 * assignments, whether to build a binary OpenLCB Tcp/Ip connected 
 * node, a GridConnect connected node, or a LCC CAN connected node. 
 * And if a network connected node the default port and host to 
 * connect to, also the console port to or to use a terminal console 
 * (for debugging).
 * 
 * These build options include:
 * 
 * @arg USE_OPENLCB_TCP_HOST
 * Use a binary OpenLCB over Tcp/Ip connection.  Normally NOT defined.
 * 
 * @arg DEFAULT_OPENLCB_TCP_HOST
 * Default OpenLCB over Tcp/Ip host to connect to normally "localhost". 
 * @arg DEFAULT_OPENLCB_TCP_PORT
 * Default OpenLCB over Tcp/Ip port -- normally 12020.
 * 
 * @arg USE_GRIDCONNECT_HOST
 * Use a binary GridConnect over Tcp/Ip connection.  Normally NOT defined.
 * @arg DEFAULT_TCP_GRIDCONNECT_HOST
 * Default GridConnect over Tcp/Ip host to connect to -- normally "localhost".
 * @arg DEFAULT_TCP_GRIDCONNECT_PORT
 * Default GridConnect over Tcp/Ip port -- normally 12021.
 * 
 * @arg PRINT_ALL_PACKETS
 * Print all LCC Packets.  Normally NOT defined.
 * 
 * @arg USE_SOCKET_CAN_PORT
 * Use a hardware CAN connection.  Normally defined.
 * @arg DEFAULT_CAN_SOCKET
 * CAN family socket name.  Normally "can1".
 * 
 * @arg START_GCTCP_HUB
 * Start a Grid Connect Hub server
 * @arg DEFAULT_GRIDCONNECT_HUB_PORT
 * Default port the Grid Connect Hub server should listen on -- normally 12021.
 * 
 * @arg TERMINALCONSOLE
 * Use a terminal console. Normally NOT defined -- debug use only.
 * @arg CONSOLEPORT
 * Console port to listen on -- normally 9900.
 * 
 * 
 * @page Configuration
 * 
 * There are three configuration sections, one for each of the DCC
 * outputs (Main and Programming) and one for the fan control.
 * 
 * The two DCC outputs have these configuration options:
 * 
 * @arg The event to send when there is a short.
 * @arg The event to send when short is cleared.
 * @arg The event to send when the command station is shutdown
 *      due to over current.
 * @arg The event to send when the shutdown is cleared.
 * @arg The event to send when the thermal flag goes on.
 * @arg The event to send when the thermal flag goes off.
 * @par
 * 
 * The fan control section has these configuration options:
 * 
 * @arg The alarm temperature threshold, in tenths of degree 
 *      centitrade.
 * @arg The event to send when the temperature excedes the alarm 
 *      temperature threshold.
 * @arg The event to send when the temperature drops below the alarm 
 *      temperature threshold.
 * @arg The fan temperature threshold, in tenths of degree
 *      centitrade.
 * @arg The event to send when the temperature excedes the fan
 *      temperature threshold.
 * @arg The event to send when the temperature drops below the fan
 *      temperature threshold.
 * @par
 * 
 */

#include <ctype.h>
#include "os/os.h"
#include "nmranet_config.h"

// Override local_nodes_count -- allow for many virtual train nodes.
OVERRIDE_CONST(local_nodes_count,50);
// Override num_memory_spaces -- allow for additional memory spaces 
// for train mode memory. 
OVERRIDE_CONST(num_memory_spaces, 7);

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
#include "Hardware.hxx"
#include "withrottle/Server.hxx"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

// Changes the default behavior by adding a newline after each gridconnect
// packet. Makes it easier for debugging the raw device.
OVERRIDE_CONST(gc_generate_newlines, 1);
// Specifies how much RAM (in bytes) we allocate to the stack of the main
// thread. Useful tuning parameter in case the application runs out of memory.
OVERRIDE_CONST(main_thread_stack_size, 2500);

// Specifies the default 48-bit OpenLCB node identifier. This must be unique for every
// hardware manufactured, so in production this should be replaced by some
// easily incrementable method.
#define DefaultNODEID 0x050101012200ULL // 05 01 01 01 22 00
static openlcb::NodeID NODE_ID = DefaultNODEID;
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

// Persistant train DB file.
char persistenttrainfile[256];
extern const char *const BeagleCS::TRAIN_DB_JSON_FILE = persistenttrainfile;

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
};



// Set up the connection mode: either over Tcp/Ip (GridConnect or
// OpenLCB) or over the CAN netork.

#ifdef USE_OPENLCB_TCP_HOST
int upstream_port = DEFAULT_OPENLCB_TCP_PORT;
const char *upstream_host = DEFAULT_OPENLCB_TCP_HOST;
#else
#  ifdef USE_GRIDCONNECT_HOST
int upstream_port = DEFAULT_TCP_GRIDCONNECT_PORT;
const char *upstream_host = DEFAULT_TCP_GRIDCONNECT_HOST;
#  endif
#  ifdef USE_SOCKET_CAN_PORT
const char *cansocket = DEFAULT_CAN_SOCKET;
#    ifdef START_GCTCP_HUB
int gctcp_hub_port = DEFAULT_GRIDCONNECT_HUB_PORT;
#    endif
#  endif
#endif

// CLI Usage output.

void usage(const char *e)
{
    fprintf(stderr, "Usage: %s [-e EEPROM_file_path] [-t Persistent_Train_file_path]", e);
    fprintf(stderr, "\n\n");    
    fprintf(stderr, "OpenMRN-Cxx-Node.\nManages a Beagle Bone Command Station Cape.\n");
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, " [-n nodeid]");
    fprintf(stderr, " [-M mainPRUfirmware] [-P progPRUfirmware]");
#if defined(USE_OPENLCB_TCP_HOST) || defined(USE_GRIDCONNECT_HOST)
    fprintf(stderr, " [-u upstream_host] [-q upstream_port]");
#endif
#ifdef USE_SOCKET_CAN_PORT
    fprintf(stderr, " [-c can_socketname]");
#endif
    fprintf(stderr, "\t-n nodeid is the node id, as a 12 hex digit number (optionally with colons between pairs of hex digits.\n");
    fprintf(stderr, "\t-e EEPROM_file_path is the path to use to implement the EEProm device.\n");
    fprintf(stderr, "\t-t Persistent_Train_file_path is the path to use to the implement the train persistent data.\n");
    fprintf(stderr, "\t-M mainPRUfirmware is the path to the mains PRU (PRU0) firmware\n");
    fprintf(stderr, "\t-P progPRUfirmware is the path to the prog PRU (PRU1) firmware\n");
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
    fprintf(stderr,"\t-W name:port Start a WiThrottle named name on port (if :port\n");
    fprintf(stderr,"\t             is ommited, on the default port).\n");
    exit(1);
}

// Files containing the PRU Firmware programs.

static char mainPRUfirmware[256] = "MainTrackDCC.out", 
            progPRUfirmware[256] = "ProgTrackDCC.out";

// Parse CLI options.

openlcb::NodeID parseNodeID(const char *nidstring)
{
    uint64_t result = 0ULL;
    int nibcount = 0, coloncount = 0;
    const char *p = NULL;
    for (p = nidstring; *p != '\0'; p++)
    {
        if (isxdigit(*p))
        {
            nibcount++;
            if (isdigit(*p))
            {
                result = (result<<4)+(*p-'0');
            }
            else if (islower(*p))
            {
                result = (result<<4)+(*p-'a'+10);
            }
            else
            {
                result = (result<<4)+(*p-'A'+10);
            }
        }
        else if (*p == ':')
        {
            coloncount++;
        }
        else
        {
            // not a hex digit or colon
            fprintf(stderr, "Syntax error: Illformed node id: %s\n",nidstring);
            return (openlcb::NodeID) -1;
        }
    }
    if (nibcount != 12)
    {
        // Wrong number of digits
        fprintf(stderr, "Syntax error: Illformed node id: %s\n",nidstring);
        return (openlcb::NodeID) -1;
    }
    if (coloncount != 0 && coloncount != 5)
    {
        // Wrong number of colons (some number other than 0 or 5)
        fprintf(stderr, "Syntax error: Illformed node id: %s\n",nidstring);
        return (openlcb::NodeID) -1;
    }
    return (openlcb::NodeID) result;
}

static bool start_WiThrottle = false;
static char WiThrottle_Name[256] = "PocketBeagle";
int WiThrottle_port = -1;

withrottle::Server *WiThrottleServer;

void parse_args(int argc, char *argv[])
{
    int opt;
#if defined(USE_OPENLCB_TCP_HOST) || defined(USE_GRIDCONNECT_HOST)
#  ifdef USE_SOCKET_CAN_PORT
#    ifdef START_GCTCP_HUB
#      define OPTSTRING "hn:e:t:M:P:u:q:c:p:W:"
#    else
#      define OPTSTRING "hn:e:t:M:P:u:q:c:W:"
#    endif
#  else
#    define OPTSTRING "hn:e:t:M:P:u:q:W:"
#  endif
#else
#  ifdef USE_SOCKET_CAN_PORT
#    ifdef START_GCTCP_HUB
#      define OPTSTRING "hn:e:t:M:P:c:p:W:"
#    else
#      define OPTSTRING "hn:e:t:M:P:c:W:"
#    endif
#  else
#    define OPTSTRING "hn:e:t:M:P:W:"
#  endif
#endif
    while ((opt = getopt(argc, argv, OPTSTRING)) >= 0)
    {
        switch (opt)
        {
        case 'h':
            usage(argv[0]);
            break;
        case 'n':
            {
                openlcb::NodeID nid = parseNodeID(optarg);
                if (((int64_t)nid) == -1) 
                {
                    usage(argv[0]);
                }
                else
                {
                    NODE_ID = nid;
                }
            }
            break;
        case 'e':
            strncpy(pathnamebuffer,optarg,sizeof(pathnamebuffer));
            break;
        case 't':
            strncpy(persistenttrainfile,optarg,sizeof(persistenttrainfile));
            break;
        case 'M':
            strncpy(mainPRUfirmware,optarg,sizeof(mainPRUfirmware));
            break;
        case 'P':
            strncpy(progPRUfirmware,optarg,sizeof(progPRUfirmware));
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
#ifdef START_GCTCP_HUB
        case 'p':
            gctcp_hub_port = atoi(optarg);
            break;
#endif
        case 'W':
            {
                start_WiThrottle = true;
                char *c = strchr(optarg,':');
                if (c != NULL)
                {
                    WiThrottle_port = atoi(c+1);
                    *c = '\0';
                }
                else
                {
                    WiThrottle_port = withrottle::Defs::DEFAULT_PORT;
                }
                strcpy(WiThrottle_Name,optarg);
                break;
            }
        default:
            fprintf(stderr, "Unknown option %c\n", opt);
            usage(argv[0]);
        }
    }
}


#ifdef USE_OPENLCB_TCP_HOST
#endif

/** Entry point to application.
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return 0, should never return
 */
int appl_main(int argc, char *argv[])
{
    // Compute default EEProm and persistant train file pathnames.
    snprintf(pathnamebuffer,sizeof(pathnamebuffer),
             "/tmp/config_eeprom_%012llX",NODE_ID);
    snprintf(persistenttrainfile,sizeof(persistenttrainfile),
             "/tmp/persistent_train_file_%012llX",NODE_ID);
    // Parse command line.
    parse_args(argc, argv);
    
    // Initialize GPIO
    GpioInit::hw_init();
    
    // Sets up a comprehensive OpenLCB stack for a single virtual node. This stack
    // contains everything needed for a usual peripheral node -- all
    // CAN-bus-specific components, a virtual node, PIP, SNIP, Memory configuration
    // protocol, ACDI, CDI, a bunch of memory spaces, etc.
    //openlcb::SimpleCanStack stack(NODE_ID);
#if defined(USE_GRIDCONNECT_HOST) || defined(USE_SOCKET_CAN_PORT) || defined(START_GCTCP_HUB)
    openlcb::SimpleCommandStationCanStack stack(NODE_ID);
#else
#ifdef USE_OPENLCB_TCP_HOST
    Executor<1> g_connect_executor("connect_executor", 0, 2048);
    openlcb::SimpleCommandStationTcpStack stack(NODE_ID);
    // OpenLCB connection callback.
    auto connect_callback = [&stack] (int fd, Notifiable *on_error)
    {
        LOG(INFO, "Connected to hub.");
        stack.add_tcp_port_select(fd, on_error);
        stack.restart_stack();
    }
#endif
#endif

    // Console executor.
    Executor<1> console_executor("console_executor", 0, 2048);
    
    // Console
#ifdef TERMINALCONSOLE
    CommandStationConsole commandProcessorConsole(&stack,
                                                  stack.traction_service(),
                                                  &console_executor,
                                                  Console::FD_STDIN,
                                                  Console::FD_STDOUT);
#else
    CommandStationConsole commandProcessorConsole(&stack,
                                                  stack.traction_service(),
                                                  &console_executor,
                                                  CONSOLEPORT);
#endif
    
    FactoryResetHelper  factory_reset_helper;
    
    // Create the config file
    stack.create_config_file_if_needed(cfg.seg().internal_config(), openlcb::CANONICAL_VERSION, openlcb::CONFIG_FILE_SIZE);
    // Start things up in the Console.
    CommandStationConsole::Begin(&stack,stack.traction_service(),
                                 cfg.seg().maindcc(),
                                 cfg.seg().progdcc(),
                                 cfg.seg().fancontrol(),
                                 mainPRUfirmware,
                                 progPRUfirmware);
    
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
#if defined(START_GCTCP_HUB)
    stack.start_tcp_hub_server(gctcp_hub_port);
#endif
    
    if (start_WiThrottle)
    {
        WiThrottleServer = new withrottle::Server(WiThrottle_Name,WiThrottle_port,stack.node());
    }
    
    // This command donates the main thread to the operation of the
    // stack. Alternatively the stack could be started in a separate stack and
    // then application-specific business logic could be executed ion a busy
    // loop in the main thread.
    stack.loop_executor();
    return 0;
}
