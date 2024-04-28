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
//  Last Modified : <230321.1324>
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
 * \file PNETDimmerServiceImpl.hxx
 * 
 * PNET Dimmer Service Implementation. Heavily borrowed from
 * EventServiceImpl.hxx (Copyright (c) 2014, Balazs Racz) 
 * 
 * @author Robert Heller
 * @date Sat Mar 18 12:46:33 2023 
 */

#ifndef __PNETDIMMERSERVICEIMPL_HXX
#define __PNETDIMMERSERVICEIMPL_HXX
#include <stdint.h>
#include <unordered_map>
#include "PNETIf.hxx"
#include "PNETDefs.hxx"
#include "utils/Singleton.hxx"
#include "executor/Notifiable.hxx"
#include "PNETDimmerService.hxx"

namespace pnet
{

/// Arguments structure for the DimmerProcessCallerFlow. Each such 
/// buffer sent to @ref DimmerProcessCallerFlow means calling one 
/// dimmer message handler's function with a given argument.
struct DimmerHandlerCall
{
    /// The DimmerRegistryEntry.
    const DimmerRegistryEntry *registry_entry;
    /// The DimmerData.
    DimmerData *td;
    /// Copy the data into the buffer.
    /// @param entry the pointer to the DimmerRegistryEntry.
    /// @param td the pointer to the DimmerData
    void reset(const DimmerRegistryEntry *entry, DimmerData *td)
    {
        this->registry_entry = entry;
        this->td = td;
    }
};

/// Control flow that calls individual dimmer message handlers one at 
/// a time and waits until the done callback is invoked before calling 
/// the next dimmer message handler. In essence this control flow 
/// behaves as a global lock for the dimmer message handlers being 
/// called. This global lock is necessary, because the dimmer message
/// handlers are using global buffers for holding the outgoing packets.
class DimmerProcessCallerFlow 
      : public StateFlow<Buffer<DimmerHandlerCall>, QList<5>>
{
public:
    /// Constructor:
    /// @param service the service the flow is owned by,
    DimmerProcessCallerFlow(Service *service)
                : StateFlow<Buffer<DimmerHandlerCall>, QList<5>>(service) {};
private:
    virtual Action entry() override;
    Action call_done();
    BarrierNotifiable n_;
};

/// PImpl class for the DimmerHandler. This class creates and owns all
/// components necessary to the correct operation of the DimmerHandler
/// but does not need to appear on the application-facing API.
class DimmerHandler::Impl
{
public:
    Impl(DimmerHandler *service);
    ~Impl();
    /// Flows that we own. There will be a few entries for each
    /// interface registered.
    std::vector<std::unique_ptr<StateFlowWithQueue>> ownedFlows_;
    DimmerRegistryIterator *iterator()
    {
        return &iterator_;
    }
    /// This flow will serialize calls to PNET Dimmer Process objects.
    /// All such calls need to be sent to this flow.
    DimmerProcessCallerFlow callerFlow_;
    /// The implementation of the PNET Dimmer Process registry.
    DimmerRegistryIterator iterator_;
};

/** Flow to receive incoming messages of PNET Dimmer protocol, and 
 * dispatch them to the registered dimmer message handler. This flow 
 * runs on the executor of the dimmer handler (and not necessarily 
 * the interface). Its main job is to iterate through the matching 
 * dimmer message  handler and call each of them for that dimmer
 * message. */
class DimmerInteratorFlow : public IncomingMessageStateFlow
{
public:
    DimmerInteratorFlow(If *iface, DimmerHandler *Dimmer_handler);
    ~DimmerInteratorFlow();
protected:
    virtual Action entry() override;
    Action iterate_next();
private:
    Action dispatch_Dimmer(const DimmerRegistryEntry *entry);
    DimmerHandler *Dimmer_handler_;
    DimmerData td_;
    DimmerRegistryIterator *iterator_;
    Notifiable *incomingDone_;
    BarrierNotifiable n_;
};
    

}


#endif // __PNETDIMMERSERVICEIMPL_HXX

