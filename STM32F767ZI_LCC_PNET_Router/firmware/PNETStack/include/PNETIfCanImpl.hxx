// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 14:36:31 2023
//  Last Modified : <230322.0919>
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
 * \file PNETIfCanImpl.hxx
 * Heavily borrowed from openmrn/src/openlcb/IfCanImpl.hxx
 * (Copyright 2014, Balazs Racz)
 * 
 * PNET Constants.
 * 
 * @author Robert Heller
 * @date Thu Mar 16 14:36:31 2023
 */

#ifndef __PNETIFCANIMPL_HXX
#define __PNETIFCANIMPL_HXX

#include "executor/StateFlow.hxx"
#include "PNETIfImpl.hxx"

namespace pnet
{

/** Implements the write-side conversion logic from generic messages to CAN
 *  * frames. */
class CanMessageWriteFlow : public WriteFlowBase
{
public:
    CanMessageWriteFlow(IfCan *if_can) : WriteFlowBase(if_can)
    {
    }
    
    IfCan *if_can()
    {
        return static_cast<IfCan *>(async_if());
    }
    
protected:
    Action send_to_hardware() override
    {
        if (nmsg()->payload.size())
        {
            // We have limited space for counting offsets. In practice this
            // value will be max 10 for certain traction control protocol
            // messages. Longer data usually travels via datagrams or streams.
            HASSERT(nmsg()->payload.size() <= 8);
        }
        return allocate_and_call(if_can()->frame_write_flow(),
                                 STATE(fill_can_frame_buffer));
    }
private:
    virtual Action fill_can_frame_buffer()
    {
        auto *b = get_allocation_result(if_can()->frame_write_flow());
        b->set_done(message()->new_child());
        struct can_frame *f = b->data()->mutable_frame();
        // Sets the CAN id.
        SET_CAN_FRAME_ID_EFF(*f, nmsg()->id());
        
        const string &data = nmsg()->payload;
        if (data.size())
        {
            HASSERT(data.size() <= 8); // too big frame for global msg
            memcpy(f->data, data.data(), data.size());
            f->can_dlc = data.size();
        }
        if_can()->frame_write_flow()->send(b);
        return call_immediately(STATE(send_finished));
    }
};
    
}

#endif // __PNETIFCANIMPL_HXX

