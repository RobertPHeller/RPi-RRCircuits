// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Mar 18 09:22:40 2023
//  Last Modified : <230321.1150>
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
 * \file PNETControlService.cxx
 * 
 * PNET Control message handling.  Heavily borrowed from EventService.cxx
 * (Copyright (c) 2013, Balazs Racz)
 * 
 * @author Robert Heller
 * @date Sat Mar 18 09:22:40 2023
 */

static const char rcsid[] = "@(#) : $Id$";

#include "PNETDefs.hxx"
#include "PNETControlService.hxx"
#include "PNETControlServiceImpl.hxx"

namespace pnet
{

ControlHandler::ControlHandler(If *iface)
      : Service(iface->executor())
{
    impl_.reset(new Impl(this));
    impl()->ownedFlows_.emplace_back(new ControlInteratorFlow(iface,this));
}

ControlHandler::~ControlHandler()
{
}

ControlInteratorFlow::ControlInteratorFlow(If *iface_, 
                                           ControlHandler *Control_handler)
      : IncomingMessageStateFlow(iface_)
      , Control_handler_(Control_handler)
      , iterator_(Control_handler->impl()->iterator())
{
    
    iface()->dispatcher()->register_handler(this,
                                            Defs::Control,
                                            0xffffffff);
}

ControlInteratorFlow::~ControlInteratorFlow()
{
    iface()->dispatcher()->unregister_handler(this,
                                              Defs::Control,
                                              0xffffffff);
}

StateFlowBase::Action ControlInteratorFlow::entry()
{
    td_.InitFromGenMessage(nmsg()); 
    incomingDone_ = message()->new_child();
    release();
    iterator_->init_iteration(td_);
    return yield_and_call(STATE(iterate_next));
}

StateFlowBase::Action ControlInteratorFlow::iterate_next()
{
    ControlRegistryEntry *entry = iterator_->next_entry();
    if (!entry)
    {
        if (incomingDone_)
        {
            incomingDone_->notify();
            incomingDone_ = nullptr;
        }
        return exit();
    }
    return dispatch_Control(entry);
}

StateFlowBase::Action ControlInteratorFlow::dispatch_Control(const ControlRegistryEntry *entry)
{
    Buffer<ControlHandlerCall> *b;
    Control_handler_->impl()->callerFlow_.pool()->alloc(&b, nullptr);
    HASSERT(b);
    b->data()->reset(entry, &td_);
    n_.reset(this);
    b->set_done(&n_);
    Control_handler_->impl()->callerFlow_.send(b);
    return wait();
}

bool operator==(const ControlData& lhs, const ControlData& rhs)
{
    return (lhs.slot == rhs.slot && lhs.value == rhs.value &&
            lhs.attributes == rhs.attributes);
}

void ControlRegistryIterator::register_handler(const ControlRegistryEntry &entry)
{
    auto range = registry_.equal_range(entry.cd);
    for (auto it = range.first; it != range.second; it++)
    {
        ControlRegistryEntry tre = it->second;
        if (tre.handler == entry.handler) return;
    }
    registry_.insert(std::pair<ControlData, ControlRegistryEntry>(entry.cd,entry));
}

void ControlHandler::register_handler(const ControlRegistryEntry &entry)
{
    impl()->iterator()->register_handler(entry);
}

void ControlRegistryIterator::unregister_handler(const ControlRegistryEntry &entry)
{
    auto range = registry_.equal_range(entry.cd);
    for (auto it = range.first; it != range.second; it++)
    {
        ControlRegistryEntry tre = it->second;
        if (tre.handler == entry.handler)
        {
            registry_.erase(it);
            return;
        }
    }
}

void ControlHandler::unregister_handler(const ControlRegistryEntry &entry)
{
    impl()->iterator()->unregister_handler(entry);
}

ControlHandler::Impl::Impl(ControlHandler *service)
      : callerFlow_(service)
{
}

ControlHandler::Impl::~Impl()
{
}

StateFlowBase::Action ControlProcessCallerFlow::entry()
{
    ControlHandlerCall *c = message()->data();
    n_.reset(this);
    c->registry_entry->handler->process_control(c->registry_entry->cd, &n_);
    return wait_and_call(STATE(call_done));
}

StateFlowBase::Action ControlProcessCallerFlow::call_done()
{
    return release_and_exit();
}



}


