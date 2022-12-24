// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Dec 18 12:28:53 2022
//  Last Modified : <221224.1147>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2022  Robert Heller D/B/A Deepwoods Software
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

#ifndef __NODEIDMEMORYCONFIGSPACE_HXX
#define __NODEIDMEMORYCONFIGSPACE_HXX

#include <NodeIdConfigurationGroup.hxx>
#include <NvsManager.hxx>
#include <openlcb/SimpleStack.hxx>
#include <openlcb/VirtualMemorySpace.hxx>
#include <utils/format_utils.hxx>
#include <utils/logging.h>
#include <utils/Singleton.hxx>
#include "StringUtils.hxx"
#include "sdkconfig.h"

namespace esp32pwmhalfsiding {

/// Node configuration holder
NodeIdConfig node_id_config(CONFIG_OLCB_NODEID_MEMORY_SPACE_OFFSET);

/// Virtual memory space that allows reconfiguration of the persistent node
/// identifier.
class NodeIdMemoryConfigSpace
      : public openlcb::VirtualMemorySpace,
public Singleton<NodeIdMemoryConfigSpace>
{
public:
    /// Constructor.
    ///
    /// @param stack is the @ref SimpleStackBase that this memory space should
    /// be registered with.
    /// @param node_id is the current node identifier.
    NodeIdMemoryConfigSpace(openlcb::SimpleStackBase *stack, NvsManager *nvs)
                : nvs_(nvs), id_(utils::node_id_to_string(nvs->node_id())),
          nodeid_(nvs->node_id())
    {
        register_string(node_id_config.node_id(),
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
                        uint64_t new_node_id = utils::string_to_uint64(contents);
                        nvs->node_id(new_node_id);
                        nodeid_ = new_node_id;
                        id_ = std::move(contents);
                    }
                        );
        LOG(INFO, "[NodeIdMemCfg:%02x] NodeID: %s", SPACE, id_.c_str());
        stack->memory_config_handler()->registry()->insert(
                                           stack->node(), SPACE, this);
    }
    
    /// Returns the currently configured node identifier.
    uint64_t node_id()
    {
        return nodeid_;
    }
    
private:
    static constexpr uint8_t SPACE = CONFIG_OLCB_NODEID_MEMORY_SPACE_ID;
    NvsManager *nvs_;
    /// temporary holder for the node id in a hex string format.
    /// NOTE: the value will be a dot expanded hex format,
    /// ie: 05.02.01.03.10.00.
    std::string id_;
    
    /// temporary holder for the currently assigned node id.
    uint64_t nodeid_{0};
};

}

#endif // __NODEIDMEMORYCONFIGSPACE_HXX

