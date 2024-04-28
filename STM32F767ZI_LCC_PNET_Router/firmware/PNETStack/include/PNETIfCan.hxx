// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 10:18:14 2023
//  Last Modified : <230322.0916>
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
 * \file PNETIfCan.hxx
 * 
 * Asynchronous PNET interface implementation for CANbus.
 * Heavily borrowed from openmrn/src/openlcb/IfCan.hxx
 * (Copyright 2013 Balazs Racz)
 * 
 * 
 * @author Robert Heller
 * @date Thu Mar 16 10:18:14 2023
 */

#ifndef __PNETIFCAN_HXX
#define __PNETIFCAN_HXX

#include <memory>

#include "executor/StateFlow.hxx"
#include "PNETIf.hxx"
#include "PNETDefs.hxx"
#include "utils/CanIf.hxx"

namespace pnet
{

/// Implementation of the PNET interface abstraction for the CAN-bus
/// interface standard. This contains the parsers for CAN frames, dispatcher
/// for the different frame types, and the flows responsible for sending outgoing
/// messages. Inherits handling of the PNET-indexed messages from the base class
/// @ref If.
class IfCan : public If, public CanIf
{
public:
    /**
     * Creates a CAN interface.
     *
     * @param executor will be used to process incoming (and outgoing) messages.
     *
     * @param device is a CanHub. The interface will add a member to this pipe
     * to handle incoming and outgoing traffic. The caller should add the
     * necessary hardware device, GridConnect bridge or mock interface to this
     * pipe (before this constructor or else outgoing packets might be lost).
     *
     */
    IfCan(ExecutorBase *executor, CanHubFlow *device);

    ~IfCan();

    void add_owned_flow(Executable *e) override;
private:
    friend class CanFrameWriteFlow; // accesses the device and the hubport.
    
    
    /// Various implementation control flows that this interface owns.
    std::vector<std::unique_ptr<Executable>> ownedFlows_;

    DISALLOW_COPY_AND_ASSIGN(IfCan);
};

/** Base class for incoming CAN frame handlers. */
class CanFrameStateFlow : public StateFlow<Buffer<CanMessageData>, QList<1>>
{
public:
    CanFrameStateFlow(IfCan *service)
        : StateFlow<Buffer<CanMessageData>, QList<1>>(service)
    {
    }

    IfCan *if_can()
    {
        return static_cast<IfCan *>(service());
    }
};

}

#endif // __PNETIFCAN_HXX

