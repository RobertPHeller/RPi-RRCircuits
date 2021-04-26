// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Oct 17 16:18:01 2019
//  Last Modified : <210426.1841>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
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

#ifndef __COMMANDSTATIONSTACK_HXX
#define __COMMANDSTATIONSTACK_HXX
#include <fcntl.h>

#include "executor/Executor.hxx"
#include "nmranet_config.h"
#include "openlcb/AliasAllocator.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "openlcb/ConfigUpdateFlow.hxx"
#include "openlcb/DatagramCan.hxx"
#include "openlcb/DatagramTcp.hxx"
#include "openlcb/DefaultNode.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/EventService.hxx"
#include "openlcb/IfCan.hxx"
#include "openlcb/MemoryConfig.hxx"
#include "openlcb/NodeInitializeFlow.hxx"
#include "openlcb/ProtocolIdentification.hxx"
#include "openlcb/SimpleNodeInfo.hxx"
#include "openlcb/TractionProxy.hxx"
#include "openlcb/TractionTestTrain.hxx"
#include "openlcb/TractionTrain.hxx"
#include "openlcb/TrainInterface.hxx"
#include "openlcb/SimpleStack.hxx"
#include "utils/GcTcpHub.hxx"
#include "utils/GridConnectHub.hxx"
#include "utils/HubDevice.hxx"
#include "utils/HubDeviceNonBlock.hxx"
#ifdef __FreeRTOS__
#include "utils/HubDeviceSelect.hxx"
#endif

namespace openlcb
{

/// CAN-based Command Station stack 
class SimpleCommandStationCanStack : public SimpleCanStackBase
{
public:
    /// Creates a Traction Proxy OpenLCB stack.
    ///
    SimpleCommandStationCanStack(const openlcb::NodeID node_id)
                : SimpleCanStackBase(node_id)
          , node_(iface(), node_id)
          , traction_service_(iface())
    {
    }
    /// @returns the virtual node pointer of the main virtual node of the stack
    /// (as defined by the NodeID argument of the constructor).
    Node *node() override
    {
        return &node_;
    }
    
    TrainService *traction_service () {return &traction_service_;}
    
private:
    static const auto PIP_RESPONSE = Defs::EVENT_EXCHANGE | Defs::DATAGRAM |
        Defs::MEMORY_CONFIGURATION | Defs::ABBREVIATED_DEFAULT_CDI |
        Defs::SIMPLE_NODE_INFORMATION | Defs::CDI /*| Defs::TRACTION_PROXY*/;

    void start_node() override
    {
        default_start_node();
    }

    /// The actual node.
    DefaultNode node_;
    /// Handles PIP requests.
    ProtocolIdentificationHandler pipHandler_ {&node_, PIP_RESPONSE};
    /// Handles SNIP requests.
    SNIPHandler snipHandler_ {iface(), &node_, &infoFlow_};
    TrainService traction_service_;
};

/// Tcp-based Command Station stack 
class SimpleCommandStationTcpStack : public SimpleTcpStackBase
{
public:
    SimpleCommandStationTcpStack(const openlcb::NodeID node_id)
                : SimpleTcpStackBase(node_id)
          , node_(iface(), node_id)
          , traction_service_(iface())
    {
    }

    /// @returns the virtual node pointer of the main virtual node of the stack
    /// (as defined by the NodeID argument of the constructor).
    Node *node() override
    {
        return &node_;
    }
    
    TrainService *traction_service () {return &traction_service_;}
    
private:
    static const auto PIP_RESPONSE = Defs::EVENT_EXCHANGE | Defs::DATAGRAM |
        Defs::MEMORY_CONFIGURATION | Defs::ABBREVIATED_DEFAULT_CDI |
        Defs::SIMPLE_NODE_INFORMATION | Defs::CDI /*| Defs::TRACTION_PROXY*/;

    void start_node() override
    {
        default_start_node();
    }

    /// The actual node.
    DefaultNode node_;
    /// Handles PIP requests.
    ProtocolIdentificationHandler pipHandler_ {&node_, PIP_RESPONSE};
    /// Handles SNIP requests.
    SNIPHandler snipHandler_ {iface(), &node_, &infoFlow_};
    openlcb::TrainService traction_service_;
};

}

#endif // __COMMANDSTATIONSTACK_HXX

