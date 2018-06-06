/* -*- C -*- ****************************************************************
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : Robert Heller
 *  Created       : Wed Jun 6 09:54:05 2018
 *  Last Modified : <180606.1438>
 *
 *  Description	
 *
 *  Notes
 *
 *  History
 *	
 ****************************************************************************
 *
 *    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
 *			51 Locke Hill Road
 *			Wendell, MA 01379-9728
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * 
 *
 ****************************************************************************/

static const char rcsid[] = "@(#) : $Id$";



#include "mcp2517_private.h"
#ifdef  SUPPORT_FOR_MCP2517__

#if     SUPPORT_EXTENDED_CANID

uint8_t mcp2517_read_id( uint32_t *id )
{
    uint16_t buffer_address;
    uint32_t dataword, R0, R1;
    uint8_t result = 0;
    
    dataword = mcp2517_read_register(C1FIFOUA1);
    buffer_address = 0x400 + dataword;
    R0 = mcp2517_read_register(buffer_address);
    R1 = mcp2517_read_register(buffer_address+4);
    
    if (R1 & (1 << RBUFFR1_IDE)) {
        *id = ((R0 >> RBUFFR0_EID)&RBUFFR0_EID_M)|((R0 & RBUFFR0_SID_M)<<18);
        result = 1;  // Extended
    } else {
        *id = R0 & RBUFFR0_SID_M;
        result = 0;  // Not Extended
    }
    if (R1 & (1 << RBUFFR1_RTR)) {
        result |= 0x2; // RTR flag;
    }
    result |= ((R1 & RBUFFR1_DLC_M) >> RBUFFR1_DLC) << 2; // Or in DLC
    return result;
}

#else  // SUPPORT_EXTENDED_CANID

uint8_t mcp2517_read_id( uint16_t *id )
{
    uint16_t buffer_address;
    uint32_t dataword, R0, R1;
    uint8_t result = 0;
    
    dataword = mcp2517_read_register(C1FIFOUA1);
    buffer_address = 0x400 + dataword;
    R0 = mcp2517_read_register(buffer_address); 
    R1 = mcp2517_read_register(buffer_address+4);
    
    if (R1 & (1 << RBUFFR1_IDE)) {
        result = 1;
    } else {
        *id = R0 & RBUFFR0_SID_M;
        result = 0;
    }
    if (R1 & (1 << RBUFFR1_RTR)) {
        result |= 0x2; // RTR flag;
    }
    result |= ((R1 & RBUFFR1_DLC_M) >> RBUFFR1_DLC) << 2; // Or in DLC
    return result;
}


#endif // SUPPORT_EXTENDED_CANID



#endif // SUPPORT_FOR_MCP2517__

