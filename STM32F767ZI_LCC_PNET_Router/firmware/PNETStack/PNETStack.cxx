// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 16:37:58 2023
//  Last Modified : <240429.1329>
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
 * \file PNETStack.cxx
 * 
 * A complete PNET stack for use with PNET messaging.
 * Heavily borrowed from SimpleStack.cxx (Copyright 2015 Balazs Racz)
 * 
 * 
 * @author Robert Heller
 * @date Thu Mar 16 16:37:58 2023
 */

static const char rcsid[] = "@(#) : $Id$";

#if defined(__linux__) || defined(__MACH__)
#include <net/if.h>
#include <termios.h> /* tc* functions */
#endif
#if defined(__linux__)
#include "utils/HubDeviceSelect.hxx"
#include <linux/sockios.h>
#include <sys/ioctl.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "PNETStack.hxx"

namespace pnet
{

PNETStackBase::PNETStackBase(std::function<std::unique_ptr<PhysicalIf>()> create_if_helper,
                             ExecutorBase *executor)
      : executor_(executor)
, service_(executor_)
, ifaceHolder_(create_if_helper())
{
}

PNETCanStack::PNETCanStack(ExecutorBase *executor)
      : PNETStackBase(std::bind(&PNETCanStack::create_if, this),executor)
{
}

std::unique_ptr<PNETCanStack::PhysicalIf> PNETCanStack::create_if()
{
    return std::unique_ptr<PhysicalIf>(new CanPhysicalIf(service()));
}

void PNETCanStack::start_iface(bool restart)
{
    if (restart)
    {
    }
}

void PNETCanStack::add_gridconnect_port(
    const char *path, Notifiable *on_exit)
{
    int fd = ::open(path, O_RDWR);
    HASSERT(fd >= 0);
    LOG(INFO, "Adding device %s as fd %d", path, fd);
    create_gc_port_for_can_hub(can_hub(), fd, on_exit);
}

#if defined(__linux__) || defined(__MACH__)
void PNETCanStack::add_gridconnect_tty(
    const char *device, Notifiable *on_exit)
{
    int fd = ::open(device, O_RDWR);
    HASSERT(fd >= 0);
    LOG(INFO, "Adding device %s as fd %d", device, fd);
    create_gc_port_for_can_hub(can_hub(), fd, on_exit);

    HASSERT(!tcflush(fd, TCIOFLUSH));
    struct termios settings;
    HASSERT(!tcgetattr(fd, &settings));
    cfmakeraw(&settings);
    cfsetspeed(&settings, B115200);
    HASSERT(!tcsetattr(fd, TCSANOW, &settings));
}
#endif
#if defined(__linux__)
void PNETCanStack::add_socketcan_port_select(const char *device, int loopback)
{
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    
    // Set the blocking limit to the minimum allowed, typically 1024 in Linux
    int sndbuf = 0;
    setsockopt(s, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(sndbuf));
    
    // turn on/off loopback
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));
    
    // setup error notifications
    can_err_mask_t err_mask = CAN_ERR_TX_TIMEOUT | CAN_ERR_LOSTARB |
          CAN_ERR_CRTL | CAN_ERR_PROT | CAN_ERR_TRX | CAN_ERR_ACK |
          CAN_ERR_BUSOFF | CAN_ERR_BUSERROR | CAN_ERR_RESTARTED;
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask));
    strcpy(ifr.ifr_name, device);
    
    ::ioctl(s, SIOCGIFINDEX, &ifr);
    
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    
    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    
    auto *port = new HubDeviceSelect<CanHubFlow>(can_hub(), s);
    additionalComponents_.emplace_back(port);
}
#endif

}
