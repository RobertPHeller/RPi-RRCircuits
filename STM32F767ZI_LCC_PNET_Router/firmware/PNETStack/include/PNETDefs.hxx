// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 16 10:51:07 2023
//  Last Modified : <230321.1636>
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
 * \file PNETDefs.hxx
 * 
 * PNET Constants.
 * 
 * @author Robert Heller
 * @date Thu Mar 16 10:51:07 2023
 */

#ifndef __PNETDEFS_HXX
#define __PNETDEFS_HXX

#include <cstdint>

#include "utils/macros.h"

namespace pnet
{

struct Defs
{
    /** Known Message  identifiers.
     */
    enum Identifier
    {
        Trigger =          0x00052000, // CAN message type for Trigger INPUT
        Control =          0x000A4000, // CAN message type for Control OUTPUT
        Dimmer  =          0x000D4000, // CAN message type for PNET Dimmer
        Loconet =          0x000D2000, // CAN message type for LocoNet Messages
        FirmwareRequest =  0x07000000, // CAN message type for Firmware Messages
        FirmwareTransfer = 0x08000000  // CAN message type for Firmware Transfer
    };
    
    /** Status of the pysical layer link */
    enum LinkStatus
    {
        LINK_UP,  /**< link is up and ready for transmit */
        LINK_DOWN /**< link is down and unable to transmit */
    };
    
private:
    /** This struct should not be instantiated. */
    Defs();
};


}

#endif // __PNETDEFS_HXX

