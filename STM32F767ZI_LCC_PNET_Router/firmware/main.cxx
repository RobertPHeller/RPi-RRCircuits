// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Jan 12 14:03:18 2023
//  Last Modified : <230322.1019>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
/** \copyright
 * Copyright (c) 2023, Robert Heller
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
 * Main file for the LCC-PNET-Router firmware.
 * 
 * @author Robert Heller
 * @date Thu Jan 12 14:03:18 2023
 * 
 * @defgroup LCC-PNET-Router LCC PNET Router
 * 
 * @section SYNOPSIS SYNOPSIS
 * 
 * LCC-PNET-Router [-n nodeid] [-m mode] [-g host] [-c lcc_can_socketname] [-p pnet_can_socketname] [-e EEPROM_file_path] 
 * 
 * @section DESCRIPTION DESCRIPTION
 * 
 * Implements a "router" between LCC and PNET.  Sends selected PNET 
 * messages (Trigger, Control, and Dimmer) in response to LCC events 
 * and sends LCC event reports in response to selected PNET messages 
 * (Trigger, Control, and Dimmer).
 * 
 * @section OPTIONS OPTIONS
 * 
 * @arg -n nodeid The node id to use for the LCC node. The default is 
 * @c 05:01:01:01:22:00
 * @arg -m mode The LCC connection mode.  One of:
 * - server start a GC hub
 * - gcclient connect a GC hub. 
 * - canclient connect a CAN Socket
 * @arg -g host the GC hub server to connect to (-m gcclient).
 * @arg -c lcc_can_socketname the CAN socket for LCC (-m canclient)
 * @arg -p pnet_can_socketname
 * @arg -e EEPROM_file_path
 * @par
 * 
 * @section PARAMETERS PARAMETERS
 * 
 * None.
 * 
 * @section FILES FILES
 * 
 * None.
 * 
 * @section AUTHOR AUTHOR
 * @author Robert Heller
 * @date Sun Mar 19 12:16:13 2023
 * 
 * @mainpage Introduction
 * 
 * This is an implementation of a "router" between the LCC network and
 * the PNET network.  LCC event reports are translated to PNET messages
 * (Control, Trigger, and Dimmer) and PNET messages (Control, Trigger, 
 * and Dimmer) are translated to LCC events reports.  This allows LCC
 * sensors, etc. to control PNET devices and allows PNET devices to
 * send LCC events to LCC nodes.
 * 
 * This firmware implements 32 each of matching Control, Trigger, and 
 * Dimmer messages.  These numbers can be changed in the config.hxx 
 * file to allow more (or less) matching messages to be implemented.
 * 
 * This firmware is expected to run on a Linux SBC with at least one
 * CAN IF, preferably two CAN IFs.  This includes the BeagleBone and
 * PocketBeagle, but the code should run on any other Linux SBC that
 * has least one CAN IF  -- this would include a Raspberry (or 
 * other flavors) Pi with a Seeed systems CAN Hat.
 * 
 */



static const char rcsid[] = "@(#) : $Id$";

#include <ctype.h>
#include "os/os.h"
#include "nmranet_config.h"

#include "openlcb/SimpleStack.hxx"
#include "openlcb/ConfiguredConsumer.hxx"
#include "openlcb/ConfiguredProducer.hxx"
#include "PNETStack.hxx"
#include "ConfiguredPCPNetTrigger.hxx"
#include "ConfiguredPCPNetControl.hxx"
#include "ConfiguredPCPNetDimmer.hxx"

#include "config.hxx"
#include "utils/GpioInitializer.hxx"
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



enum LCCMode {SERVER, GCCLIENT, CANCLIENT};

LCCMode lccmode = LCCMode::CANCLIENT;
const char *lccgchost = "localhost";
const char *lcccansocket = DEFAULT_LCCCAN_SOCKET;
const char *pnetcansocket = DEFAULT_PNETCAN_SOCKET;

// CLI Usage output.

void usage(const char *e)
{
    fprintf(stderr, "Usage: %s [-e EEPROM_file_path] [-n NID] [-c LCC_CAN_SOCKET] [-p PNET_CAN_SOCKET]", e);
    fprintf(stderr, "\n\n");    
    fprintf(stderr, "LCC-PNET-Router.\n\n");
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "\t[-n nodeid]\n");
    fprintf(stderr, "\t[-m mode]\n");
    fprintf(stderr, "\t[-g host]\n");
    fprintf(stderr, "\t[-c lcc_can_socketname]\n");
    fprintf(stderr, "\t[-p pnet_can_socketname]\n"); 
    fprintf(stderr, "\t[-e EEPROM_file_path]\n");
    fprintf(stderr, "Where:\n");
    fprintf(stderr, "\tnodeid is the node id, as a 12 hex digit number (optionally with colons between pairs of hex digits.\n");
    fprintf(stderr, "\tmode is the LCC connection mode.  One of:\n");
    fprintf(stderr, "\t\tserver - start a GC hub (host 0.0.0.0, port = 12021)\n");
    fprintf(stderr, "\t\tgcclient - connect a GC hub (at the host specificed by -h, port 12021)\n");
    fprintf(stderr, "\t\tcanclient - connect a CAN Socket (-c) (default)\n");
    fprintf(stderr, "\thost the GC hub server to connect to\n");
    fprintf(stderr, "\tlcc_can_socketname the CAN socket for LCC\n");
    fprintf(stderr, "\tpnet_can_socketname the CAN socket for PNET\n");
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
    int opt;
#define OPTSTRING "hn:e:p:c:m:g:"
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
        case 'c':
            lcccansocket = optarg;
            break;
        case 'p':
            pnetcansocket = optarg;
            break;
        case 'g':
            lccgchost = optarg;
            break;
        case 'm':
            if (strncasecmp(optarg,"server",strlen(optarg)) == 0)
            {
                lccmode = LCCMode::SERVER;
            }
            else if (strncasecmp(optarg,"gcclient",strlen(optarg)) == 0)
            {
                lccmode = LCCMode::GCCLIENT;
            }
            else if (strncasecmp(optarg,"canclient",strlen(optarg)) == 0)
            {
                lccmode = LCCMode::CANCLIENT;
            }
            else
            {
                fprintf(stderr, "Unknown mode: %s\n",optarg);
                usage(argv[0]);
            }
            break;
        default:
            fprintf(stderr, "Unknown option %c\n", opt);
            usage(argv[0]);
        }
    }
}

TRIGGERS;
CONTROLS;
DIMMERS;

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
    // Parse command line.
    parse_args(argc, argv);
    
    // Initialize GPIO
    //GpioInit::hw_init();
    
    // Sets up a comprehensive OpenLCB stack for a single virtual node. This stack
    // contains everything needed for a usual peripheral node -- all
    // CAN-bus-specific components, a virtual node, PIP, SNIP, Memory configuration
    // protocol, ACDI, CDI, a bunch of memory spaces, etc.
    openlcb::SimpleCanStack stack(NODE_ID);
    pnet::PNETCanStack pnet(stack.executor());
    
    FactoryResetHelper  factory_reset_helper;
    
    PCPNetTrigger::Init(stack.node(), &pnet, cfg.seg().triggers(), 
                        NUM_TRIGGERS);
    PCPNetControl::Init(stack.node(), &pnet, cfg.seg().controls(), 
                        NUM_CONTROLS);
    PCPNetDimmer::Init(stack.node(), &pnet, cfg.seg().dimmers(), 
                        NUM_DIMMERS);
    
    // Create the config file
    stack.create_config_file_if_needed(cfg.seg().internal_config(), openlcb::CANONICAL_VERSION, openlcb::CONFIG_FILE_SIZE);
    // Start things up in the Console.
#ifdef PRINT_ALL_PACKETS
    // Causes all packets to be dumped to stdout.
    stack.print_all_packets();
#endif
    switch (lccmode) {
    case LCCMode::SERVER:
        stack.start_tcp_hub_server(12021);
        break;
    case LCCMode::GCCLIENT:
        stack.connect_tcp_gridconnect_hub(lccgchost,12021);
        break;
    case LCCMode::CANCLIENT:
        stack.add_socketcan_port_select(lcccansocket);
        break;
    }
    pnet.add_socketcan_port_select(pnetcansocket);
    
    // This command donates the main thread to the operation of the
    // stack. Alternatively the stack could be started in a separate stack and
    // then application-specific business logic could be executed ion a busy
    // loop in the main thread.
    stack.loop_executor();
    return 0;
}
