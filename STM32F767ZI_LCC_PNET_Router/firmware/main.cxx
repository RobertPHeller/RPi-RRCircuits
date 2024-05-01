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
//  Last Modified : <240501.1651>
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
 * This firmware is expected to run on a NUCLEO-F767ZI - STM32 Nucleo-144.
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
#include "NodeIdConfigurationGroup.hxx"

#include "config.hxx"
#include "utils/GpioInitializer.hxx"


// Changes the default behavior by adding a newline after each gridconnect
// packet. Makes it easier for debugging the raw device.
OVERRIDE_CONST(gc_generate_newlines, 1);
// Specifies how much RAM (in bytes) we allocate to the stack of the main
// thread. Useful tuning parameter in case the application runs out of memory.
OVERRIDE_CONST(main_thread_stack_size, 2500);

static openlcb::NodeID NODE_ID = 0x050101012200ULL; // 05 01 01 01 22 00
// ConfigDef comes from config.hxx and is specific to the particular device and
// target. It defines the layout of the configuration memory space and is also
// used to generate the cdi.xml file. Here we instantiate the configuration
// layout. The argument of offset zero is ignored and will be removed later.
openlcb::ConfigDef cfg(0);
// Defines weak constants used by the stack to tell it which device contains
// the volatile configuration information. This device name appears in
// HwInit.cxx that creates the device drivers.

extern const char *const openlcb::CONFIG_FILENAME = "/ffs/eeprom";
const char *const NODEID_FILENAME = "/ffs/nodeid";

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
    // Initialize GPIO
    //GpioInit::hw_init();
    
    NodeIdMemoryConfigSpace nodeIdSpace(NODE_ID,NODEID_FILENAME);
    NODE_ID = nodeIdSpace.node_id();
    
    // Sets up a comprehensive OpenLCB stack for a single virtual node. This stack
    // contains everything needed for a usual peripheral node -- all
    // CAN-bus-specific components, a virtual node, PIP, SNIP, Memory configuration
    // protocol, ACDI, CDI, a bunch of memory spaces, etc.
    openlcb::SimpleCanStack stack(NODE_ID);
    nodeIdSpace.RegisterSpace(&stack);
    pnet::PNETCanStack pnet(stack.executor());
    
    FactoryResetHelper  factory_reset_helper;
    
    PCPNetTrigger::Init(stack.node(), &pnet, cfg.seg().triggers(), 
                        NUM_TRIGGERS);
    PCPNetControl::Init(stack.node(), &pnet, cfg.seg().controls(), 
                        NUM_CONTROLS);
    PCPNetDimmer::Init(stack.node(), &pnet, cfg.seg().dimmers(), 
                        NUM_DIMMERS);
    
    // Create the config file
    stack.create_config_file_if_needed(cfg.seg().internal_config(), 
                                       openlcb::CANONICAL_VERSION, 
                                       openlcb::CONFIG_FILE_SIZE);
    // Start things up in the Console.
#ifdef PRINT_ALL_PACKETS
    // Causes all packets to be dumped to stdout.
    stack.print_all_packets();
#endif
    stack.add_can_port_select("/dev/can0");
    pnet.add_can_port_select("/dev/can1");
    
    // This command donates the main thread to the operation of the
    // stack. Alternatively the stack could be started in a separate stack and
    // then application-specific business logic could be executed ion a busy
    // loop in the main thread.
    stack.loop_executor();
    return 0;
}
