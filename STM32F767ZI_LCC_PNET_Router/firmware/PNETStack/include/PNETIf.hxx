// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 14:38:39 2023
//  Last Modified : <230322.0911>
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
 * \file PNETIf.hxx
 * 
 * Asynchronous PNET interface.
 * 
 * Heavily borrowed from openmrn/src/If.hxx 
 * (copyright Balazs Racz 3 Dec 2013)
 * 
 * @author Robert Heller
 * @date Thu Mar 16 14:38:39 2023
 */

#ifndef __PNETIF_HXX
#define __PNETIF_HXX

#include <string>

#include "PNETDefs.hxx"
#include "executor/Dispatcher.hxx"
#include "executor/Service.hxx"
#include "executor/Executor.hxx"
#include "executor/StateFlow.hxx"
#include "utils/Buffer.hxx"
#include "utils/Queue.hxx"
#include "utils/Map.hxx"

namespace pnet
{

/// Container that carries the data bytes in a PNET message.
typedef string Payload;

/** A global class / variable for empty or not-yet-initialized payloads. */
extern string EMPTY_PAYLOAD;

/** This class is used in the dispatching of incoming or outgoing PNET
 * messages to the message handlers at the protocol-agnostic level.
 */
struct GenMessage
{
    GenMessage()
          : identifier((Defs::Identifier)0), flagsSrc(0), flagsDst(0)
    {
    }
    
    void clear()
    {
        reset((Defs::Identifier)0, EMPTY_PAYLOAD);
    }
    
    void reset(Defs::Identifier id, string payload)
    {
        this->identifier = id;
        this->payload = std::move(payload);
        this->flagsSrc = 0;
        this->flagsDst = 0;
    }
    
    // PNET Message ID.
    Defs::Identifier identifier;
    // Data content in the message body. Owned by the dispatcher.
    string payload;
    unsigned flagsSrc : 4;
    unsigned flagsDst : 4;
    unsigned get_flags_src() {
        return flagsSrc;
    }
    unsigned get_flags_dst() {
        return flagsDst;
    }
    void set_flag_src(unsigned flags) {
        flagsSrc |= flags;
    }
    void clear_flag_src(unsigned flags) {
        flagsSrc &= ~flags;
    }
        /** Returns true if src flags has all the specified flags set. */
    bool has_flag_src(unsigned flags) {
        return ((flagsSrc & flags) == flags);
    }
    void set_flag_dst(unsigned flags) {
        flagsDst |= flags;
    }
    void clear_flag_dst(unsigned flags) {
        flagsDst &= ~flags;
    }
    /** Returns true if src flags has all the specified flags set. */
    bool has_flag_dst(unsigned flags) {
        return ((flagsDst & flags) == flags);
    }

    typedef uint32_t id_type;
    id_type id() const
    {
        return static_cast<uint32_t>(identifier);
    }
    enum DstFlags {
        // 1, 2, 4, 8: free
    };
    enum SrcFlags {
        // 1, 2, 4, 8: free
    };
};

/// Interface class for all handlers that can be registered in the dispatcher
/// to receive incoming PNET messages.
typedef FlowInterface<Buffer<GenMessage>> MessageHandler;

/// Abstract class representing an PNET Interface. All interaction between
/// the local software stack and the physical bus has to go through this
/// class. The API that's not specific to the wire protocol appears here. The
/// implementations of this class would be specific to the wire protocol
/// (e.g. IfCan for CAN, and a not-yet-implemented class for TCP).
class If : public Service
{
public:
    /** Constructs an PNET interface.
     * @param executor is the thread that will be used for all processing on
     * this interface.
     */
    If(ExecutorBase *executor);

    /** Destructor */
    virtual ~If()
    {
    }

    /** @return Flow to send global messages to the PNET bus. */
    MessageHandler *global_message_write_flow()
    {
        HASSERT(globalWriteFlow_);
        return globalWriteFlow_;
    }

    /** Type of the dispatcher of incoming PNET messages. */
    typedef DispatchFlow<Buffer<GenMessage>, 4> MessageDispatchFlow;

    /** @return Dispatcher of incoming PNET messages. */
    MessageDispatchFlow *dispatcher()
    {
        return &dispatcher_;
    }

    /** Transfers ownership of a module to the interface. It will be brought
     * down in the destructor. The destruction order is guaranteed such that
     * all supporting structures are still available when the flow is destryed,
     * but incoming messages can not come in anymore.
     *
     * @todo(balazs.racz) revise whether this needs to be virtual. */
    virtual void add_owned_flow(Executable *e) = 0;

protected:
    /// Allocator containing the global write flows.
    MessageHandler *globalWriteFlow_;

private:
    /// Flow responsible for routing incoming messages to handlers.
    MessageDispatchFlow dispatcher_;
    
    friend class TriggerHandler;
    friend class ControlHandler;
    friend class DimmerHandler;
    
    DISALLOW_COPY_AND_ASSIGN(If);
};

/// Message handlers that are implemented as state flows should derive from
/// this class.
typedef StateFlow<Buffer<GenMessage>, QList<4>> MessageStateFlowBase;

/** Base class for incoming message handler flows. */
class IncomingMessageStateFlow
    : public MessageStateFlowBase
{
public:
    IncomingMessageStateFlow(If *iface)
        : MessageStateFlowBase(iface)
    {
    }

    If *iface()
    {
        return static_cast<If *>(service());
    }

    /// Returns the PNET message we received.
    GenMessage *nmsg()
    {
        return message()->data();
    }
};

}

#endif // __PNETIF_HXX

