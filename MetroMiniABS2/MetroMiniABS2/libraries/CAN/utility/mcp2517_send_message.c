/* -*- C -*- ****************************************************************
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : Robert Heller
 *  Created       : Wed Jun 6 10:55:17 2018
 *  Last Modified : <180606.1549>
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

// ----------------------------------------------------------------------------
uint8_t mcp2517_send_message(const tCAN *msg)
{
    uint16_t address;
    uint32_t T1,dataword;
    uint8_t  length, i, ibyte;
    union {
        uint32_t R;
        uint8_t  D[4];
    } datablock;
    
    
    if ((mcp2517_read_register(C1TXQSTA) & (1 << CiTXQSTA_TXQNIF)) != 0) {
        address = 0x400 + mcp2517_read_register(C1TXQUA);
    } else {
	return 0; // Queue is full
    }
    T1 = 0;
#if SUPPORT_EXTENDED_CANID
    mcp2517_write_id(&msg->id, msg->flags.extended);
    T1 |= (1 << TBUFFT1_IDE);
#else
    mcp2517_write_id(&msg->id);
#endif
    if (msg->flags.rtr) {
        T1 |= (1 << TBUFFT1_RTR);
    }
    length = msg->length & TBUFFT1_DLC_M;
    T1 |= (length << TBUFFT1_DLC);
    mcp2517_write_register(address+4,T1);
    for (i = 0, ibyte = 0; ibyte < 4 && i < length; i++, ibyte++) {
        datablock.D[ibyte] = msg->data[i];
    }
    mcp2517_write_register(address+8,datablock.R);
    if (i < length) {
        for (ibyte = 0; ibyte < 4 && i < length; i++, ibyte++) {
            datablock.D[ibyte] = msg->data[i];
        }
        mcp2517_write_register(address+12,datablock.R);
    }
    dataword = mcp2517_read_register(C1TXQCON);
    dataword |= (1 << CiTXQCON_UINC);
    mcp2517_write_register(C1TXQCON,dataword);
    mcp2517_write_register(C1TXREQ,(1 << CiTXREQ_TXREQ0));
    return 1;   
}

#endif	// SUPPORT_FOR_MCP2517__

