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
 * @date 3 Feb 2019
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
 * Also this program uses scripts in the ../CommonOpenMRNExtras 
 * directory.  These scripts relate to the NODEID.txt.
 * 
 * The NODEID.txt file contains the seed for the node id generation.
 * It contains the node id of the most recent build.  The next build
 * will have a node id one more than this.
 * 
 * The scripts used are:
 *
 * @arg GenerateNODEID.tcl -- generates and increments the node id
 * @arg NodeIDBin.mk -- contains the Makefile includes to auto
 *                       generate successive node ids.
 * @page Configuration
 * 
 * There are three configuration sections, one for each of the DCC
 * outputs (Main and Programming) and one for the fan control.
 * 
 * The two DCC outputs have these configuration options:
 * 
 * @arg The overcurrent threshold, in milliamps.
 * @arg The event to send when the current is exceded.
 * @arg The event to disable the DCC output.
 * @arg The event to enable the DCC output.
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

#include "os/os.h"
#include "nmranet_config.h"

OVERRIDE_CONST(local_nodes_count,50);
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
#include "DuplexUpdateLoop.hxx"
#include "CommandStationDCCMainTrack.hxx"
#include "CommandStationDCCProgTrack.hxx"
#include "HBridgeControl.hxx"
#include "FanControl.hxx"
#include "BBRailComDriver.hxx"
#include "BeagleTrainDatabase.hxx"
#include <AllTrainNodes.hxx>
#include <dcc/RailcomHub.hxx>
#include <dcc/RailcomPortDebug.hxx>
#include "Hardware.hxx"
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

#ifdef NO_THERMFAULT
HBridgeControl mains(stack.node(), cfg.seg().maindcc(), CSenseMainAnalogChannel, MainEN_Pin::instance());
#else
HBridgeControl mains(stack.node(), cfg.seg().maindcc(), CSenseMainAnalogChannel, MainEN_Pin::instance(), MainTF_Pin::instance());
#endif
HBridgeControl progtrack(stack.node(), cfg.seg().progdcc(), CSenseProgAnalogChannel, ProgEN_Pin::instance() );
FanControl     fan(stack.node(), cfg.seg().fancontrol(), TempsensorChannel,FanControl_Pin::instance());

openlcb::RefreshLoop loop(stack.node(),
                      {mains.polling(), 
                          progtrack.polling(), 
                          fan.polling()});

#define RAILCOM_BAUD B230400
struct RailComHW
{
    using HB_BRAKE = MainBRAKE_Pin;
    using HB_ENABLE = ProgEN_Pin;
    using RC_ENABLE = RailcomEN_Pin;
    static void hw_init()
    {
    }
    static int openport()
    {
        struct termios railcomtermios;
        int fd = open(RAILCOM_DATA_PORT,O_RDWR);
        if (fd < 0) {
            LOG(FATAL,"RailComHW: Could not open %s (%d)", RAILCOM_DATA_PORT, errno);
            exit(errno);
        }
        tcgetattr(fd,&railcomtermios);
        cfmakeraw(&railcomtermios);
        cfsetspeed(&railcomtermios,RAILCOM_BAUD);
        // 1 stop bit, 8 data bits
        railcomtermios.c_cflag &= ~CSTOPB;
        railcomtermios.c_cflag &= ~CSIZE;
        railcomtermios.c_cflag |= CS8; 
        tcsetattr(fd,TCSANOW,&railcomtermios);
        return fd;
    }
    static size_t data_avail(int fd)
    {
        int bytes = 0;
        ioctl(fd,TIOCINQ,&bytes);
        return bytes;
    }
    static uint8_t readbyte(int fd)
    {
        uint8_t buff;
        /*size_t s =*/ read(fd,&buff,1);
        return buff;
    }
    static size_t readbuff(int fd,uint8_t *buf, size_t max_len)
    {
        return read(fd,buf,max_len);
    }
    static int flush(int fd)
    {
        return tcflush(fd,TCIFLUSH);
    }
  /// Number of microseconds to wait after the final packet bit completes
  /// before disabling the ENABLE pin on the h-bridge.
  static constexpr uint32_t RAILCOM_TRIGGER_DELAY_USEC = 1;

  /// Number of microseconds to wait for railcom data on channel 1.
  static constexpr uint32_t RAILCOM_MAX_READ_DELAY_CH_1 =
    177 - RAILCOM_TRIGGER_DELAY_USEC;

  /// Number of microseconds to wait for railcom data on channel 2.
  static constexpr uint32_t RAILCOM_MAX_READ_DELAY_CH_2 =
    454 - RAILCOM_MAX_READ_DELAY_CH_1;
};

BBRailComDriver<RailComHW> opsRailComDriver(RAILCOM_FEEDBACK_QUEUE);

static std::unique_ptr<dcc::RailcomHubFlow> railcom_hub;
static std::unique_ptr<dcc::RailcomPrintfFlow> railcom_dumper;

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
    fprintf(stderr, "Usage: %s [-e EEPROM_file_path] [-t Persistent_Train_file_path]", e);
#if defined(USE_OPENLCB_TCP_HOST) || defined(USE_GRIDCONNECT_HOST)
    fprintf(stderr, " [-u upstream_host] [-q upstream_port]");
#endif
#ifdef USE_SOCKET_CAN_PORT
    fprintf(stderr, " [-c can_socketname]");
#endif
    fprintf(stderr, "\n\n");    
    fprintf(stderr, "OpenMRN-Cxx-Node.\nManages a Beagle Bone Command Station Cape.\n");
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "\t-e EEPROM_file_path is the path to use to implement the EEProm device.\n");
    fprintf(stderr, "\t-t Persistent_Train_file_path is the path to use to the implement the train persistent data.\n");
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
#define OPTSTRING "he:t:u:q:c:"
#else
#define OPTSTRING "he:t:u:q:"
#endif
#else
#ifdef USE_SOCKET_CAN_PORT
#define OPTSTRING "he:t:c:"
#else
#define OPTSTRING "he:t:"
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
        case 't':
            strncpy(persistenttrainfile,optarg,sizeof(persistenttrainfile));
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

BeagleCS::BeagleTrainDatabase trainDb(&stack);

commandstation::AllTrainNodes trainNodes(&trainDb
                                         , stack.traction_service()
                                         , stack.info_flow()
                                         , stack.memory_config_handler()
                                         , trainDb.get_train_cdi()
                                         , trainDb.get_temp_train_cdi());



#ifdef TERMINALCONSOLE
CommandStationConsole commandProcessorConsole(&trainNodes,
                                              stack.traction_service(),
                                              stack.executor(),
                                              Console::FD_STDIN,
                                              Console::FD_STDOUT);
#else
CommandStationConsole commandProcessorConsole(&trainNodes,
                                              stack.traction_service(),
                                              stack.executor(),
                                              CONSOLEPORT);
#endif

CommandStationDCCMainTrack mainDCC(stack.service(),2);
CommandStationDCCProgTrack progDCC(stack.service(),2);

//dcc::SimpleUpdateLoop dccUpdateLoop(stack.service(), &mainDCC);

DuplexUpdateLoop DccPacketLoop(stack.service(),&mainDCC,&progDCC);


/** Entry point to application.
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return 0, should never return
 */
int appl_main(int argc, char *argv[])
{
    snprintf(pathnamebuffer,sizeof(pathnamebuffer),
             "/tmp/config_eeprom_%012llX",NODE_ID);
    snprintf(persistenttrainfile,sizeof(persistenttrainfile),
             "/tmp/persistent_train_file_%012llX",NODE_ID);
    parse_args(argc, argv);
    trainDb.Begin();
    //auto trainnodes = Singleton<commandstation::AllTrainNodes>::instance();
    //auto traindb = Singleton<BeagleCS::BeagleTrainDatabase>::instance();

    GpioInit::hw_init();
    
    stack.create_config_file_if_needed(cfg.seg().internal_config(), openlcb::CANONICAL_VERSION, openlcb::CONFIG_FILE_SIZE);
    mainDCC.StartPRU();
    progDCC.StartPRU();
    LOG(INFO, "[Main] PRUs started...");
    railcom_hub.reset(new dcc::RailcomHubFlow(stack.service()));
    LOG(INFO, "[Main] RailcomHub started...");
    opsRailComDriver.hw_init(railcom_hub.get());
    LOG(INFO, "[Main] RailComDriver initialized...");
    railcom_dumper.reset(new dcc::RailcomPrintfFlow(railcom_hub.get()));
    LOG(INFO, "[Main] railcom_dumper started...");
    
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
