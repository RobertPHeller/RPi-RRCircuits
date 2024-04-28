// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 14:35:22 2023
//  Last Modified : <230322.0952>
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
 * \file PNETIfCan.cxx
 * 
 * Asynchronous PNET interface.
 * Heavily borrowed from openmrn/src/openlcb/IfCan.cxx (Copyright 2013 Balazs Racz)
 * 
 * @author Robert Heller
 * @date Thu Mar 16 14:35:22 2023
 */

static const char rcsid[] = "@(#) : $Id$";


#include "PNETIfCan.hxx"

#include "utils/StlMap.hxx"
#include "utils/Singleton.hxx"
#include "PNETIfImpl.hxx"
#include "PNETIfCanImpl.hxx"
#include "can_frame.h"
#include "PNETTriggerService.hxx"
#include "PNETControlService.hxx"
#include "PNETDimmerService.hxx"

namespace pnet
{

/** This write flow inherits all the business logic from the parent, jus
 * t
 * maintains a separate allocation queue. This allows global messages to go out
 * even if addressed messages are waiting for destination address
 * resolution. */
class GlobalCanMessageWriteFlow : public CanMessageWriteFlow
{
public:
    GlobalCanMessageWriteFlow(IfCan *if_can)
                : CanMessageWriteFlow(if_can)
    {
    }
    
protected:
    Action entry() override
    {
        return call_immediately(STATE(send_to_hardware));
    }
    
    Action send_finished() override
    {
        return call_immediately(STATE(global_entry));
    }
};

/** This class listens for incoming CAN frames of PNET messages, 
 * computing its identifier. The resulting message is then passed to 
 * the generic If for dispatching. 
 */
class FrameToGlobalMessageParser : public CanFrameStateFlow
{
public:
    FrameToGlobalMessageParser(IfCan *service)
                : CanFrameStateFlow(service)
    {
        if_can()->frame_dispatcher()->register_handler(this,0,0);
    }
    ~FrameToGlobalMessageParser()
    {
        if_can()->frame_dispatcher()->unregister_handler(this,0,0);
    }
    /// Handler entry for incoming messages.
    Action entry() OVERRIDE
    {
        struct can_frame *f = message()->data();
        id_ = GET_CAN_FRAME_ID_EFF(*f);
        if (f->can_dlc)
        {
            buf_.assign((const char *)(&f->data[0]), f->can_dlc);
        }
        else
        {
            buf_.clear();
        }
        release();
        return allocate_and_call(if_can()->dispatcher(), STATE(send_to_if));
    }
    
    Action send_to_if()
    {
        auto *b = get_allocation_result(if_can()->dispatcher());
        GenMessage *m = b->data();
        m->identifier = (Defs::Identifier)id_;
        m->payload = buf_;
        if_can()->dispatcher()->send(b/*, b->data()->priority()*/);
        return exit();
    }
private:
    /// CAN frame ID, saved from the incoming frame.
    uint32_t id_;
    /// Payload for the MTI message.
    string buf_;
};

IfCan::IfCan(ExecutorBase *executor, CanHubFlow *device)
      : If(executor)
      , CanIf(this, device)
{
    auto *gflow = new GlobalCanMessageWriteFlow(this);
    globalWriteFlow_ = gflow;
    add_owned_flow(gflow);
    add_owned_flow(new FrameToGlobalMessageParser(this));
}    

IfCan::~IfCan()
{
}

void IfCan::add_owned_flow(Executable *e)
{
    ownedFlows_.push_back(std::unique_ptr<Executable>(e));
}



}
