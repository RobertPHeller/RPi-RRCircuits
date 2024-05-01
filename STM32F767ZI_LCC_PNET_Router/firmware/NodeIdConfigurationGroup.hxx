// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed May 1 14:16:02 2024
//  Last Modified : <240501.1708>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
/// @copyright
///    Copyright (C) 2024  Robert Heller D/B/A Deepwoods Software
///			51 Locke Hill Road
///			Wendell, MA 01379-9728
///
///    This program is free software; you can redistribute it and/or modify
///    it under the terms of the GNU General Public License as published by
///    the Free Software Foundation; either version 2 of the License, or
///    (at your option) any later version.
///
///    This program is distributed in the hope that it will be useful,
///    but WITHOUT ANY WARRANTY; without even the implied warranty of
///    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///    GNU General Public License for more details.
///
///    You should have received a copy of the GNU General Public License
///    along with this program; if not, write to the Free Software
///    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
/// @file NodeIdConfigurationGroup.hxx
/// @author Robert Heller
/// @date Wed May 1 14:16:02 2024
/// 
///
//////////////////////////////////////////////////////////////////////////////

/** @page NodeIdConfig Node ID Configuration
 * 
 * This is the identifier to use for this device. NOTE: Changing this value 
 * will force a factory reset.
 * 
 * The initial default value is a placeholder value and should be changed
 * the first time the node is powered up.
 * 
 * A node id range can be acquired at 
 * https://registry.openlcb.org/requestuniqueidrange#new_tab
 * 
 * @section NodeIdOpts Configuration Options
 * 
 * There is one configuration option.  It is the string representation of the
 * node id, in the form xx.xx.xx.xx.xx.xx, where each xx is a 2 digit hex
 * number representing one 8-bit byte (00 to ff).
 * 
 * Updating this option will cause the node to reboot with a factory reset
 * configuration (except for the node id).  The node id is stored in a separate
 * persistent storage area.
 */

#ifndef __NODEIDCONFIGURATIONGROUP_HXX
#define __NODEIDCONFIGURATIONGROUP_HXX
#include <openlcb/ConfigRepresentation.hxx>
#include <openlcb/SimpleStack.hxx>
#include <openlcb/VirtualMemorySpace.hxx>
#include <utils/format_utils.hxx>
#include <utils/Singleton.hxx>
#include <fcntl.h>

#include <string>
#include <string.h>
#include <stdio.h>

using std::string;

namespace utils
{

string node_id_to_string(uint64_t node_id)
{
    string result = "";
    char buffer[8];
    string dot = "";
    for (int i=0; i<6; i++)
    {
        snprintf(buffer,sizeof(buffer),"%02X",(unsigned)(node_id&0x0FF));
        result = buffer + dot + result;
        dot = ".";
        node_id >>= 8;
    }
    return result;
}

uint64_t string_to_uint64(const string node_string)
{
    uint64_t result = 0LL;
    for (int i=0; i<6; i++)
    {
        int index = i*3;
        unsigned byte=0;
        sscanf(node_string.substr(index,2).c_str(),"%02X",&byte);
        result <<= 8;
        result |= byte;
    }
    return result;
}


}


extern "C" {
extern void hw_shutdown(void);
};

#define CONFIG_OLCB_NODEID_MEMORY_SPACE_ID 0xAA
#define CONFIG_OLCB_NODEID_MEMORY_SPACE_OFFSET 0

CDI_GROUP(NodeIdConfig, Segment(CONFIG_OLCB_NODEID_MEMORY_SPACE_ID),
          Offset(CONFIG_OLCB_NODEID_MEMORY_SPACE_OFFSET));
CDI_GROUP_ENTRY(node_id, openlcb::StringConfigEntry<32>, Name("Node ID"),
                Description(
                            R"!^!(Identifier to use for this device.
NOTE: Changing this value will force a factory reset.)!^!"))
CDI_GROUP_END();

extern const char *const openlcb::CONFIG_FILENAME; // Config file

/// Virtual memory space that allows reconfiguration of the persistent node
/// identifier.
class NodeIdMemoryConfigSpace
      : public openlcb::VirtualMemorySpace,
public Singleton<NodeIdMemoryConfigSpace>
{
public:
    /// Constructor.
    ///
    /// @param node_id is the current node identifier.
    /// @param nodeid_path is the filename for storing the persistent node id.
    /// @param offset is the memory offset
    NodeIdMemoryConfigSpace(openlcb::NodeID node_id,
                            const char * nodeid_path,
                            int offset = CONFIG_OLCB_NODEID_MEMORY_SPACE_OFFSET
                            )
                : nodeid_path_(nodeid_path)
          , id_(utils::node_id_to_string(node_id))
          , nodeid_(node_id)
    {
        node_id_config_ = new NodeIdConfig(CONFIG_OLCB_NODEID_MEMORY_SPACE_OFFSET);
        register_string(node_id_config_->node_id(),
                        [&](unsigned repeat, string *contents, BarrierNotifiable *done)
                    {
                        AutoNotify n(done);
                        LOG(VERBOSE, "[NodeIdMemCfg-READ] %s", id_.c_str());
                        *contents = id_;
                    },
                        [&](unsigned repeat, string contents, BarrierNotifiable *done)
                    {
                        AutoNotify n(done);
                        LOG(VERBOSE, "[NodeIdMemCfg-WRITE] %s", contents.c_str());
                        nodeid_ = utils::string_to_uint64(contents);
                        id_ = std::move(contents);
                        int fd = ::open(nodeid_path_.c_str(),O_RDWR);
                        if (fd < 0)
                        {
                            fd = ::open(nodeid_path_.c_str(),O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
                        }
                        else
                        {
                            ::lseek(fd,0,SEEK_SET);
                        }
                        ::write(fd,id_.c_str(),id_.size());
                        ::close(fd);
                        // force the config to be reset
                        ::unlink(openlcb::CONFIG_FILENAME);
                        ::hw_shutdown();
                    });
        char buffer[32];
        int fd = ::open(nodeid_path_.c_str(),O_RDONLY);
        if (fd < 0) 
        {
            int fd = ::open(nodeid_path_.c_str(),O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
            ::write(fd,id_.c_str(),id_.size());
            ::close(fd);
            fd = ::open(nodeid_path_.c_str(),O_RDONLY);
        }
        ::read(fd,buffer,sizeof(buffer));
        ::close(fd);
        id_ = buffer;
        nodeid_ = utils::string_to_uint64(id_);
    }
    /// Register Space
    ///
    /// @param stack is the @ref SimpleStackBase that this memory space should
    /// be registered with.
    void RegisterSpace(openlcb::SimpleStackBase *stack)
    {
        LOG(INFO, "[NodeIdMemCfg:%02x] NodeID: %s", SPACE, id_.c_str());
        stack->memory_config_handler()->registry()->insert(stack->node(), 
                                                           SPACE, this);
    }
    /// Returns the currently configured node identifier.
    uint64_t node_id()
    {
        return nodeid_;
    }
private:
    static constexpr uint8_t SPACE = CONFIG_OLCB_NODEID_MEMORY_SPACE_ID;
    std::string nodeid_path_;
    /// temporary holder for the node id in a hex string format.
    /// NOTE: the value will be a dot expanded hex format,
    /// ie: 05.02.01.03.10.00.
    std::string id_;
    
    /// temporary holder for the currently assigned node id.
    uint64_t nodeid_{0};
    NodeIdConfig *node_id_config_;
};

        



#endif // __NODEIDCONFIGURATIONGROUP_HXX

