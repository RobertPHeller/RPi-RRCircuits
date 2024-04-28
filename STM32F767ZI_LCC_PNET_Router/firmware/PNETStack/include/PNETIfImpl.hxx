// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 14:44:16 2023
//  Last Modified : <230322.0921>
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
 * \file PNETIfImpl.hxx
 * 
 * Implementation details for the asynchronous PNET interfaces. This file
 * should only be needed in hardware interface implementations.
 * Heavily borrowed from openmrn/src/openlcb/IfImpl.hxx
 * (Copyright 2013 Balazs Racz)
 * 
 * @author Robert Heller
 * @date Thu Mar 16 14:44:16 2023
 */

#ifndef __PNETIFIMPL_HXX
#define __PNETIFIMPL_HXX

#include "PNETIf.hxx"

namespace pnet
{

/** Implementation of the hardware-independent parts of the write flows. */
class WriteFlowBase : public StateFlow<Buffer<GenMessage>, QList<4>>
{
public:
    WriteFlowBase(If *async_if)
        : StateFlow<Buffer<GenMessage>, QList<4>>(async_if)
    {
    }

protected:
    /** This function will be called (on the main executor) to initiate sending
     * this message to the hardware. The flow will then execute the returned
     * action.
     *
     * NOTE: it is possible that this functon will never be called for a given
     * flow. */
    virtual Action send_to_hardware() = 0;

    /** Virtual method called after the send is completed, i.e., all the frames
     * are generated and sent to the hardware. Various flows might need to take
     * additional steps afterwards. */
    virtual Action send_finished()
    {
        return release_and_exit();
    }

    /// @returns the interface that this flow is assigned to.
    If *async_if()
    {
        return static_cast<If *>(service());
    }

    /** Implementations shall call this function when they are done with
     * sending the packet.
     */
    // void cleanup();

    /// Returns the PNET message we are trying to send.
    GenMessage *nmsg()
    {
        return message()->data();
    }

protected:

    /** Global write flows should return to this state AFTER sending the
     * message to the hardware. They should ensure the message is still
     * intact. They will not get back control. */
    Action global_entry();
};




}

#endif // __PNETIFIMPL_HXX

