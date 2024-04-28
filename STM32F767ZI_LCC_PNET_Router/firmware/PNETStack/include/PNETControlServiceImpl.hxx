// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Mar 18 12:46:33 2023
//  Last Modified : <230321.1328>
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
 * \file PNETControlServiceImpl.hxx
 * 
 * PNET Control Service Implementation. Heavily borrowed from 
 * EventServiceImpl.hxx (Copyright (c) 2014, Balazs Racz)
 * 
 * @author Robert Heller
 * @date Sat Mar 18 12:46:33 2023
 */

#ifndef __PNETCONTROLSERVICEIMPL_HXX
#define __PNETCONTROLSERVICEIMPL_HXX
#include <stdint.h>
#include <unordered_map>
#include "PNETIf.hxx"
#include "PNETDefs.hxx"
#include "utils/Singleton.hxx"
#include "executor/Notifiable.hxx"
#include "PNETControlService.hxx"

namespace pnet
{


/// Arguments structure for the ControlProcessCallerFlow. Each such 
/// buffer sent to @ref ControlProcessCallerFlow means calling one 
/// control message handler's function with a given argument.
struct ControlHandlerCall
{
    /// The ControlRegistryEntry.
    const ControlRegistryEntry *registry_entry;
    /// The ControlData.
    ControlData *td;
    /// Copy the data into the buffer.
    /// @param entry the pointer to the ControlRegistryEntry.
    /// @param td the pointer to the ControlData.
    void reset(const ControlRegistryEntry *entry, ControlData *td)
    {
        this->registry_entry = entry;
        this->td = td;
    }
};

/// Control flow that calls individual control message handlers one at 
/// a time and waits until the done callback is invoked before calling 
/// the next control message handler. In essence this control flow 
/// behaves as a global lock for the control message handlers being 
/// called. This global lock is necessary, because the control message
/// handlers are using global buffers for holding the outgoing packets.

class ControlProcessCallerFlow 
      : public StateFlow<Buffer<ControlHandlerCall>, QList<5>>
{
public:
    /// Constructor:
    /// @param service the service the flow is owned by,
    ControlProcessCallerFlow(Service *service)
                : StateFlow<Buffer<ControlHandlerCall>, QList<5>>(service) {};
private:
    virtual Action entry() override;
    Action call_done();
    BarrierNotifiable n_;
};


/// PImpl class for the ControlHandler. This class creates and owns all
/// components necessary to the correct operation of the ControlHandler
/// but does not need to appear on the application-facing API.
class ControlHandler::Impl
{
public:
    Impl(ControlHandler *service);
    ~Impl();
    /// Flows that we own. There will be a few entries for each
    /// interface registered.
    std::vector<std::unique_ptr<StateFlowWithQueue>> ownedFlows_;
    ControlRegistryIterator *iterator()
    {
        return &iterator_;
    }
    /// This flow will serialize calls to PNET Control Process objects.
    /// All such calls need to be sent to this flow.
    ControlProcessCallerFlow callerFlow_;
    /// The implementation of the PNET Control Process registry.
    ControlRegistryIterator iterator_;
};

/** Flow to receive incoming messages of PNET Control protocol, and 
 * dispatch them to the registered control message handler. This flow 
 * runs on the executor of the control handler (and not necessarily 
 * the interface). Its main job is to iterate through the matching 
 * control message  handler and call each of them for that control 
 * message. */
class ControlInteratorFlow : public IncomingMessageStateFlow
{
public:
    ControlInteratorFlow(If *iface, ControlHandler *Control_handler);
    ~ControlInteratorFlow();
protected:
    virtual Action entry() override;
    Action iterate_next();
private:
    Action dispatch_Control(const ControlRegistryEntry *entry);
    ControlHandler *Control_handler_;
    ControlData td_;
    ControlRegistryIterator *iterator_;
    Notifiable *incomingDone_;
    BarrierNotifiable n_;
};
    

}


#endif // __PNETCONTROLSERVICEIMPL_HXX

