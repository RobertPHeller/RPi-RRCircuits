// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Jun 7 16:21:39 2023
//  Last Modified : <250824.2051>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2023  Robert Heller D/B/A Deepwoods Software
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

#include <ctype.h>
#include "os/os.h"
#include "nmranet_config.h"

#include "openlcb/SimpleStack.hxx"
#include "openlcb/ConfiguredConsumer.hxx"
#include "openlcb/ConfiguredProducer.hxx"
#include "openlcb/MultiConfiguredConsumer.hxx"
#include "MultiConfiguredProducer.hxx"

#include "config.hxx"
#include "utils/GpioInitializer.hxx"
#include "freertos_drivers/common/DummyGPIO.hxx"
#include "freertos_drivers/common/LoggingGPIO.hxx"
#include "os/LinuxGpio.hxx"
#include "Hardware.hxx"

// Changes the default behavior by adding a newline after each gridconnect
// packet. Makes it easier for debugging the raw device.
OVERRIDE_CONST(gc_generate_newlines, 1);
// Specifies how much RAM (in bytes) we allocate to the stack of the main
// thread. Useful tuning parameter in case the application runs out of memory.
OVERRIDE_CONST(main_thread_stack_size, 2500);


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
        cfg.userinfo().description().write(fd, HARDWARE_IMPL);
    }
};

#ifdef START_GCTCP_HUB
int server_port = DEFAULT_GRIDCONNECT_HUB_PORT;
#endif
#ifdef USE_GRIDCONNECT_HOST
int upstream_port = DEFAULT_TCP_GRIDCONNECT_PORT;
const char *upstream_host = DEFAULT_TCP_GRIDCONNECT_HOST;
#endif
#ifdef USE_SOCKET_CAN_PORT
const char *cansocket = DEFAULT_CAN_SOCKET;
#endif

// CLI Usage output.

void usage(const char *e)
{
    fprintf(stderr, "Usage: %s [-e EEPROM_file_path] [-n NID] [-c LCC_CAN_SOCKET]", e);
    fprintf(stderr, "\n\n");    
    fprintf(stderr, "IO_CAPE.\n\n");
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "\t[-n nodeid]\n");
    fprintf(stderr, "\t[-g host]\n");
    fprintf(stderr, "\t[-c can_socketname]\n");
    fprintf(stderr, "\t[-e EEPROM_file_path]\n");
    fprintf(stderr, "Where:\n");
    fprintf(stderr, "\tnodeid is the node id, as a 12 hex digit number (optionally with colons between pairs of hex digits.\n");
    fprintf(stderr, "\thost the GC hub server to connect to\n");
    fprintf(stderr, "\tcan_socketname the CAN socket for LCC\n");
    fprintf(stderr, "\tEEPROM_file_path is the path to use to implement the EEProm device.\n");
    exit(1);
}

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


void parse_args(int argc, char *argv[])
{
    bool sawopte = false;
    int opt;
#define OPTSTRING "hn:e:p:g:c:x"
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
                    if (!sawopte)
                    {
                        snprintf(pathnamebuffer,sizeof(pathnamebuffer),
                                 "/tmp/config_eeprom_%012llX",NODE_ID);
                    }
                }
            }
            break;
        case 'e':
            strncpy(pathnamebuffer,optarg,sizeof(pathnamebuffer));
            sawopte = true;
            break;
#ifdef USE_SOCKET_CAN_PORT
        case 'c':
            cansocket = optarg;
            break;
#endif
#ifdef USE_GRIDCONNECT_HOST
        case 'g':
            upstream_host = optarg;
            break;
#endif
        default:
            fprintf(stderr, "Unknown option %c\n", opt);
            usage(argv[0]);
        }
    }
}

const Gpio *const kOUTs[] = {
    OUT1_Pin::instance(), OUT2_Pin::instance(), OUT3_Pin::instance(), 
    OUT4_Pin::instance(), OUT5_Pin::instance(), OUT6_Pin::instance(), 
    OUT7_Pin::instance(), OUT8_Pin::instance()
};

const Gpio *const kINs[] = {
    IN1_Pin::instance(),  IN2_Pin::instance(),  IN3_Pin::instance(), 
    IN4_Pin::instance(),  IN5_Pin::instance(),  IN6_Pin::instance(), 
    IN7_Pin::instance(),  IN8_Pin::instance()
};

/** Entry point to application.
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return 0, should never return
 */
int appl_main(int argc, char *argv[])
{
    // Compute default EEProm pathname.
    snprintf(pathnamebuffer,sizeof(pathnamebuffer),
             "/tmp/config_eeprom_%012llX",NODE_ID);
    
    // Parse command line.
    parse_args(argc, argv);
    
    // Initialize GPIO
    GpioInit::hw_init();
    
    // Sets up a comprehensive OpenLCB stack for a single virtual node. This stack
    // contains everything needed for a usual peripheral node -- all
    // CAN-bus-specific components, a virtual node, PIP, SNIP, Memory configuration
    // protocol, ACDI, CDI, a bunch of memory spaces, etc.
    openlcb::SimpleCanStack stack(NODE_ID);
    
    openlcb::MultiConfiguredConsumer OUTs(stack.node(),
                                          kOUTs,ARRAYSIZE(kOUTs),
                                          cfg.seg().outputs());
    MultiConfiguredProducer INs(stack.node(),
                                    kINs, ARRAYSIZE(kINs),
                                    cfg.seg().inputs());
    openlcb::RefreshLoop inputRefresh(stack.node(), 
                                   {INs.polling()});
    FactoryResetHelper  factory_reset_helper;
    // Create the config file
    stack.create_config_file_if_needed(cfg.seg().internal_config(), openlcb::CANONICAL_VERSION, openlcb::CONFIG_FILE_SIZE);
#ifdef START_GCTCP_HUB
    stack.start_tcp_hub_server(server_port);
#endif
#ifdef USE_GRIDCONNECT_HOST
    stack.connect_tcp_gridconnect_hub(upstream_host, upstream_port);
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

