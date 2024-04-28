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
//  Last Modified : <230321.1406>
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
 * \file PNETTriggerService.cxx
 * 
 * PNET Trigger message handling.  Heavily borrowed from EventService.cxx
 * (Copyright (c) 2013, Balazs Racz)
 * 
 * @author Robert Heller
 * @date Sat Mar 18 09:22:40 2023 
 */

static const char rcsid[] = "@(#) : $Id$";

#include "PNETDefs.hxx"
#include "PNETTriggerService.hxx"
#include "PNETTriggerServiceImpl.hxx"

namespace pnet
{

TriggerHandler::TriggerHandler(If *iface)
      : Service(iface->executor())
{
    impl_.reset(new Impl(this));
    impl()->ownedFlows_.emplace_back(new TriggerInteratorFlow(iface,this));
}

TriggerHandler::~TriggerHandler()
{
}

TriggerInteratorFlow::TriggerInteratorFlow(If *iface_, 
                                           TriggerHandler *trigger_handler)
      : IncomingMessageStateFlow(iface_)
      , trigger_handler_(trigger_handler)
      , iterator_(trigger_handler->impl()->iterator())
{
    
    iface()->dispatcher()->register_handler(this,
                                            Defs::Trigger,
                                            0xffffffff);
}

TriggerInteratorFlow::~TriggerInteratorFlow()
{
    iface()->dispatcher()->unregister_handler(this,
                                              Defs::Trigger,
                                              0xffffffff);
}

StateFlowBase::Action TriggerInteratorFlow::entry()
{
    td_.InitFromGenMessage(nmsg()); 
    incomingDone_ = message()->new_child();
    release();
    iterator_->init_iteration(td_);
    return yield_and_call(STATE(iterate_next));
}

StateFlowBase::Action TriggerInteratorFlow::iterate_next()
{
    TriggerRegistryEntry *entry = iterator_->next_entry();
    if (!entry)
    {
        if (incomingDone_)
        {
            incomingDone_->notify();
            incomingDone_ = nullptr;
        }
        return exit();
    }
    return dispatch_trigger(entry);
}

StateFlowBase::Action TriggerInteratorFlow::dispatch_trigger(const TriggerRegistryEntry *entry)
{
    Buffer<TriggerHandlerCall> *b;
    trigger_handler_->impl()->callerFlow_.pool()->alloc(&b, nullptr);
    HASSERT(b);
    b->data()->reset(entry, &td_);
    n_.reset(this);
    b->set_done(&n_);
    trigger_handler_->impl()->callerFlow_.send(b);
    return wait();
}

bool operator==(const TriggerData& lhs, const TriggerData& rhs)
{
    return (lhs.slot == rhs.slot && lhs.trigger == rhs.trigger);
}

void TriggerRegistryIterator::register_handler(const TriggerRegistryEntry &entry)
{
    auto range = registry_.equal_range(entry.td);
    for (auto it = range.first; it != range.second; it++)
    {
        TriggerRegistryEntry tre = it->second;
        if (tre.handler == entry.handler) return;
    }
    registry_.insert(std::pair<TriggerData, TriggerRegistryEntry>(entry.td,entry));
}

void TriggerHandler::register_handler(const TriggerRegistryEntry &entry)
{
    impl()->iterator()->register_handler(entry);
}

void TriggerRegistryIterator::unregister_handler(const TriggerRegistryEntry &entry)
{
    auto range = registry_.equal_range(entry.td);
    for (auto it = range.first; it != range.second; it++)
    {
        TriggerRegistryEntry tre = it->second;
        if (tre.handler == entry.handler)
        {
            registry_.erase(it);
            return;
        }
    }
}

void TriggerHandler::unregister_handler(const TriggerRegistryEntry &entry)
{
    impl()->iterator()->unregister_handler(entry);
}

TriggerHandler::Impl::Impl(TriggerHandler *service)
      : callerFlow_(service)
{
}

TriggerHandler::Impl::~Impl()
{
}

StateFlowBase::Action TriggerProcessCallerFlow::entry()
{
    TriggerHandlerCall *c = message()->data();
    n_.reset(this);
    c->registry_entry->handler->process_trigger(c->registry_entry->td, &n_);
    return wait_and_call(STATE(call_done));
}

StateFlowBase::Action TriggerProcessCallerFlow::call_done()
{
    return release_and_exit();
}



}


