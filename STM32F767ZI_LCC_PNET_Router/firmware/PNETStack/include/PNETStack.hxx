// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Mar 15 15:13:10 2023
//  Last Modified : <240429.2107>
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
 * \file PNETStack.hxx
 * 
 * A complete PNET stack for use with PNET messaging.
 * Heavily borrowed from SimpleStack.hxx (Copyright 2015 Balazs Racz)
 * 
 * @author Robert Heller
 * @date Wed Mar 15 15:13:10 2023
 */

#ifndef __PNETSTACK_HXX
#define __PNETSTACK_HXX

#include <fcntl.h>
#include "executor/Executor.hxx"
#include "utils/HubDevice.hxx"
#include "utils/HubDeviceNonBlock.hxx"
#include "utils/CanIf.hxx"
#include "utils/HubDeviceSelect.hxx"
#include "PNETIfCan.hxx"
#include "PNETControlService.hxx"
#include "PNETDimmerService.hxx"
#include "PNETTriggerService.hxx"

namespace pnet
{

class PNETStackBase
{
protected:
    /// Polymorphic class that can be implemented by CAN and TCP interfaces
    /// separately for appropriate construction order.
    class PhysicalIf
    {
    public:
        virtual ~PhysicalIf()
        {
        }
        /// @return the PNET interface object. Ownership is not transferred.
        virtual If *iface() = 0;
    };
public:
    PNETStackBase(
        std::function<std::unique_ptr<PhysicalIf>()> create_if_helper,
                  ExecutorBase *executor);
    /// @returns the executor that's controlling the main thread of the OpenLCB
    /// stack.
    ExecutorBase *executor()
    {
        return executor_;
    }
    /// @returns a plain service bound to the main thread's executor.
    Service *service()
    {
        return &service_;
    }
    /// @returns the PNET Interface object.
    If *iface()
    {
        return iface_;
    }
protected:
    virtual void start_iface(bool restart) = 0;
    ExecutorBase *executor_;
    Service service_;
    /// Pointer to the polymorphic implementation of the PNET If.
    std::unique_ptr<PhysicalIf> ifaceHolder_;
    /// The PNET interface object. Owned by ifaceHolder_;
    If *iface_ {ifaceHolder_->iface()};
    
    /// Dispatches Control message to the Control message  handlers.
    ControlHandler controlHandler_ {iface()};
    /// Dispatches Dimmer message to the Dimmer message  handlers.
    DimmerHandler dimmerHandler_ {iface()};
    /// Dispatches Trigger message to the Trigger message  handlers.
    TriggerHandler triggerHandler_ {iface()};
    /// Stores and keeps ownership of optional components. 
    std::vector<std::unique_ptr<Destructable>> additionalComponents_;
};

/// PNETStack with a CAN-bus based interface and IO functions for CAN-bus.
class PNETCanStack : public PNETStackBase
{
public:
    PNETCanStack(ExecutorBase *executor);
    
    /// @returns the CanHubFlow to which this stack is talking to. This hub
    /// flow usually has two members: the interface object from the software
    /// stack and the hardware connection via which to connect to the physical
    /// bus (which may be a device driver or a gridconnect protocol converter).
    CanHubFlow *can_hub()
    {
        return &static_cast<CanPhysicalIf *>(ifaceHolder_.get())->canHub0_;
    }
    /// Adds a CAN bus port with synchronous driver API.
    void add_can_port_blocking(const char *device)
    {
        int can_fd = ::open(device, O_RDWR);
        HASSERT(can_fd >= 0);
        auto *port = new FdHubPort<CanHubFlow>(
            can_hub(), can_fd, EmptyNotifiable::DefaultInstance());
        additionalComponents_.emplace_back(port);
    }

#ifdef OPENMRN_FEATURE_FD_CAN_DEVICE
    /// Adds a CAN bus port with asynchronous driver API.
    ///
    /// @deprecated: most current FreeRTOS drivers use the the select-based
    /// asynchronous API, so they need add_can_port_select().
    void add_can_port_async(const char *device)
    {
        auto *port = new HubDeviceNonBlock<CanHubFlow>(can_hub(), device);
        additionalComponents_.emplace_back(port);
    }

    /// Adds a CAN bus port with select-based asynchronous driver API.
    void add_can_port_select(const char *device)
    {
        auto *port = new HubDeviceSelect<CanHubFlow>(can_hub(), device);
        additionalComponents_.emplace_back(port);
    }

    /// Adds a CAN bus port with select-based asynchronous driver API.
    /// @param fd file descriptor to add to can hub
    /// @param on_error Notifiable to wakeup on error
    void add_can_port_select(int fd, Notifiable *on_error = nullptr)
    {
        auto *port = new HubDeviceSelect<CanHubFlow>(can_hub(), fd, on_error);
        additionalComponents_.emplace_back(port);
    }
#endif // OPENMRN_FEATURE_FD_CAN_DEVICE
#if defined(__linux__)
    /// Adds a CAN bus port with select-based asynchronous driver API.
    /// @params device CAN device name, for example: "can0" or "can1"
    /// @params loopback 1 to enable loopback localy to other open references,
    ///                  0 to enable loopback localy to other open references,
    ///                  in most cases, this paramter won't matter
    void add_socketcan_port_select(const char *device, int loopback = 1);
#endif
    
protected:
    /// Helper function for start_stack et al.
    void start_iface(bool restart) override;
    
    IfCan *if_can()
    {
        return &static_cast<CanPhysicalIf *>(ifaceHolder_.get())->ifCan_;
    }
    
private:
    class CanPhysicalIf : public PhysicalIf
    {
    public:
        CanPhysicalIf(Service *service)
                    : canHub0_(service)
              , ifCan_(service->executor(), &canHub0_)
        {
        }
        ~CanPhysicalIf()
        {
        }
        /// @return the PNET  interface object. Ownership is not transfered.
        If *iface() override
        {
            return &ifCan_;
        }
        /// This flow is the connection between the stack and the device
        /// drivers. It also acts as a hub to multiple different clients or CAN
        /// ports.
        CanHubFlow canHub0_;
        /// Implementation of PNET interface.
        IfCan ifCan_;
    };
    /// Constructor helper function. Creates the specific objects needed for
    /// the CAN interface to function. Will be called exactly once by the
    /// constructor of the base class.
    std::unique_ptr<PhysicalIf> create_if();
};

}


#endif // __PNETSTACK_HXX

