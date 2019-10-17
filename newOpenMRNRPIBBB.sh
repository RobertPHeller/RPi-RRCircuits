#!/bin/bash
name=$1
mkdir -p $1/targets/bbb.linux.armv7a $1/targets/rpi.linux.armv7a $1/targets/pb.linux.armv7a

cp CommonOpenMRNExtras/Makefile $1/Makefile
cp CommonOpenMRNExtras/targets/Makefile $1/targets/Makefile
cp CommonOpenMRNExtras/targets/bbb.linux.armv7a/Makefile $1/targets/bbb.linux.armv7a/Makefile
cp CommonOpenMRNExtras/targets/rpi.linux.armv7a/Makefile $1/targets/rpi.linux.armv7a/Makefile
cp CommonOpenMRNExtras/targets/pb.linux.armv7a/Makefile $1/targets/pb.linux.armv7a/Makefile
cp CommonOpenMRNExtras/GenerateNODEID.tcl $1/
ln -s /home/heller/openmrn/applications/default_config.mk $1/config.mk
ln -s ../../config.hxx $1/targets/rpi.linux.armv7a/config.hxx
ln -s ../../config.hxx $1/targets/bbb.linux.armv7a/config.hxx
ln -s ../../config.hxx $1/targets/pb.linux.armv7a/config.hxx
echo "05 01 01 01 22 40" > $1/NODEID.txt
ln -s ../../NODEID.txt $1/targets/rpi.linux.armv7a/NODEID.txt
ln -s ../../NODEID.txt $1/targets/bbb.linux.armv7a/NODEID.txt
ln -s ../../NODEID.txt $1/targets/pb.linux.armv7a/NODEID.txt
cat <<EOF >$1/config.hxx
#ifndef _CONFIG_HXX_
#define _CONFIG_HXX_

#include "openlcb/ConfiguredConsumer.hxx"
#include "openlcb/ConfiguredProducer.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "openlcb/MemoryConfig.hxx"

namespace openlcb
{

/// Defines the identification information for the node. The arguments are:
///
/// - 4 (version info, always 4 by the standard
/// - Manufacturer name
/// - Model name
/// - Hardware version
/// - Software version
///
/// This data will be used for all purposes of the identification:
///
/// - the generated cdi.xml will include this data
/// - the Simple Node Ident Info Protocol will return this data
/// - the ACDI memory space will contain this data.
extern const SimpleNodeStaticValues SNIP_STATIC_DATA = {
    4,               "Deepwoods Software", HARDWARE_IMPL,
    "linux.armv7a", "1.00"};


/// Used for detecting when the config file stems from a different config.hxx
/// version and needs to be factory reset before using. Change every time that
/// the config eeprom file's layout changes.
static constexpr uint16_t CANONICAL_VERSION = 0x9000;

/// Defines the main segment in the configuration CDI. This is laid out at
/// origin 128 to give space for the ACDI user data at the beginning.
CDI_GROUP(IoBoardSegment, Name(HARDWARE_IMPL), Segment(MemoryConfigDefs::SPACE_CONFIG), Offset(128));
/// Each entry declares the name of the current entry, then the type and then
/// optional arguments list.
CDI_GROUP_ENTRY(internal_config, InternalConfigData);
CDI_GROUP_END();

/// This segment is only needed temporarily until there is program code to set
/// the ACDI user data version byte.
CDI_GROUP(VersionSeg, Segment(MemoryConfigDefs::SPACE_CONFIG),
    Name("Version information"));
CDI_GROUP_ENTRY(acdi_user_version, Uint8ConfigEntry,
    Name("ACDI User Data version"), Description("Set to 2 and do not change."));
CDI_GROUP_ENTRY(buildrevisions,BuildRevisions);
CDI_GROUP_END();

/// The main structure of the CDI. ConfigDef is the symbol we use in main.cxx
/// to refer to the configuration defined here.
CDI_GROUP(ConfigDef, MainCdi());
/// Adds the <identification> tag with the values from SNIP_STATIC_DATA above.
CDI_GROUP_ENTRY(ident, Identification);
/// Adds an <acdi> tag.
CDI_GROUP_ENTRY(acdi, Acdi);
/// Adds a segment for changing the values in the ACDI user-defined
/// space. UserInfoSegment is defined in the system header.
CDI_GROUP_ENTRY(userinfo, UserInfoSegment, Name("User Info"));
/// Adds the main configuration segment.
CDI_GROUP_ENTRY(seg, IoBoardSegment);
/// Adds the versioning segment.
CDI_GROUP_ENTRY(version, VersionSeg);
CDI_GROUP_END();

} // namespace openlcb

#endif // _CONFIG_HXX_
EOF
cat <<EOF >$1/main.cxx
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
#include "utils/GpioInitializer.hxx"                                   

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
    stack.create_config_file_if_needed(cfg.seg().internal_config(), openlcb::CANONICAL_VERSION, openlcb::CONFIG_FILE_SIZE);
    
    
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
EOF
cat <<EOF >$1/targets/bbb.linux.armv7a/Hardware.hxx
#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "BBB "



// On chip GPIO:

typedef GpioInitializer<> GpioInit;


//#define HAVE_TCP_GRIDCONNECT_HOST
//#define TCP_GRIDCONNECT_HOST "localhost"
//#define TCP_GRIDCONNECT_PORT 12021

//#define PRINT_ALL_PACKETS

#define HAVE_SOCKET_CAN_PORT
#define SOCKET_CAN_PORT "can1"


#endif // __HARDWARE_HXX
EOF
cat <<EOF >$1/targets/rpi.linux.armv7a/Hardware.hxx
#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "RPi "

// On chip GPIO:

typedef GpioInitializer<> GpioInit;



#define HAVE_TCP_GRIDCONNECT_HOST
#define TCP_GRIDCONNECT_HOST "127.0.0.1"
#define TCP_GRIDCONNECT_PORT 12021

//#define PRINT_ALL_PACKETS
//#define HAVE_SOCKET_CAN_PORT
//#define SOCKET_CAN_PORT "vcan0"

#endif // __HARDWARE_HXX
EOF
cat <<EOF >$1/targets/bbb.linux.armv7a/Hardware.hxx
#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "BBB "



// On chip GPIO:

typedef GpioInitializer<> GpioInit;


//#define HAVE_TCP_GRIDCONNECT_HOST
//#define TCP_GRIDCONNECT_HOST "localhost"
//#define TCP_GRIDCONNECT_PORT 12021

//#define PRINT_ALL_PACKETS

#define HAVE_SOCKET_CAN_PORT
#define SOCKET_CAN_PORT "can1"


#endif // __HARDWARE_HXX
EOF
cat <<EOF >$1/targets/pb.linux.armv7a/Hardware.hxx
#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "PB "

// On chip GPIO:

typedef GpioInitializer<> GpioInit;



#define HAVE_TCP_GRIDCONNECT_HOST
#define TCP_GRIDCONNECT_HOST "127.0.0.1"
#define TCP_GRIDCONNECT_PORT 12021

//#define PRINT_ALL_PACKETS
//#define HAVE_SOCKET_CAN_PORT
//#define SOCKET_CAN_PORT "vcan0"

#endif // __HARDWARE_HXX
EOF
