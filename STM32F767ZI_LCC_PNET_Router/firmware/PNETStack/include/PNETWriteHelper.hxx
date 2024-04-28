// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Mar 19 15:56:37 2023
//  Last Modified : <230322.0946>
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
 * \file PNETWriteHelper.hxx
 * 
 * Class that allows enqueing an outgoing message.
 * Heavily borrowed from WriteHelper.hxx (Copyright 2013 Balazs Racz)
 * 
 * @author Robert Heller
 * @date Sun Mar 19 15:56:37 2023
 */

#ifndef __PNETWRITEHELPER_HXX
#define __PNETWRITEHELPER_HXX

#include <string>
#include "PNETIf.hxx"
#include "PNETStack.hxx"
#include "PNETDefs.hxx"

namespace pnet
{

/// A statically allocated buffer for sending one message to the PNET
/// bus. This buffer is reusable, as soon as the done notifiable is called, the
/// buffer is free for sending the next packet.
class WriteHelper : public Executable
{
public:
    typedef PNETCanStack *stack_type;
    typedef string payload_type;
    WriteHelper()
    {
    }
    const payload_type &last_payload()
    {
        return buffer_;
    }
    void clear_last_payload()
    {
        buffer_.clear();
    }
    /** Originates a PNET message.
     * @param stack is the PNET Stack.
     * @param ident is the message identifier.
     * @param buffer is the message payload.
     * @param done will be notified when the packet has been enqueued to the
     * physical layer. If done == nullptr, the sending is invoked synchronously.
     */
    void WriteAsync(PNETCanStack *stack, Defs::Identifier ident,
                    const payload_type &buffer, Notifiable *done)
    {
        if (done)
        {
            done_.reset(done);
        }
        else
        {
            // We don't support synchronous sending anymore.
            HASSERT(0);
        }
        stack_ = stack;
        ident_ = ident;
        buffer_ = buffer;
        stack->iface()->global_message_write_flow()->alloc_async(this);
    }
private:
    // Callback from the allocator.
    void alloc_result(QMember *entry) override
    {
        auto *f = stack_->iface()->global_message_write_flow();
        Buffer<GenMessage> *b = f->cast_alloc(entry);
        b->data()->reset(ident_,buffer_);
        b->set_done(&done_);
        f->send(b);
    }
    void run() override
    {
        HASSERT(0);
    }
    Defs::Identifier ident_;
    PNETCanStack *stack_;
    payload_type buffer_;
    BarrierNotifiable done_;
};


}

#endif // __PNETWRITEHELPER_HXX

