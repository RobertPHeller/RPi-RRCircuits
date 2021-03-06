/* -*- C -*- ****************************************************************
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : Robert Heller
 *  Created       : Mon Jun 4 09:29:08 2018
 *  Last Modified : <180606.1439>
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
#ifdef	SUPPORT_FOR_MCP2517__

uint8_t mcp2517_get_message(tCAN *msg)
{
    uint8_t addr;
    uint16_t buffer_address;
    
    uint32_t dataword;
    uint8_t  idflags, length, i, ibyte;
    union {
        uint32_t R;
        uint8_t  D[4];
    } datablock;
    
    dataword = mcp2517_read_register(C1FIFOSTA1);
    if ((dataword & (1 << CiFIFOSTAm_TFNRFNIF)) == 0) return 0;
    idflags = mcp2517_read_id(&msg->id);
#if SUPPORT_EXTENDED_CANID
    // Set extended bit if extended message.
    msg->flags.extended = idflags & 0x01; 
#else
    if (idflags & 0x01) {
        // bump receive counter and drop extended message, since we don't support extended messages
        dataword = mcp2517_read_register(C1FIFOCON1);
        dataword |= (1 << CiFIFOCONm_UINC);
        mcp2517_write_register(C1FIFOCON1,dataword);
        return 0;
    }
#endif
    length = (idflags>>2)&RBUFFR1_DLC_M;
    msg->length = length;
    msg->flags.rtr = (idflags & 0x02) ? 1 : 0;
    dataword = mcp2517_read_register(C1FIFOUA1);
#if SUPPORT_TIMESTAMPS
    buffer_address = 0x400 + dataword + 12;
#else
    buffer_address = 0x400 + dataword + 8;
#endif
    datablock.R = mcp2517_read_register(buffer_address);
    for (i = 0, ibyte = 0; ibyte < 4 && i < length; i++, ibyte++) {
        msg->data[i] = datablock.D[ibyte];
    }
    if (i < length) {
        datablock.R = mcp2517_read_register(buffer_address+4);
        for (ibyte = 0; ibyte < 4 && i < length; i++, ibyte++) {
            msg->data[i] = datablock.D[ibyte];
        }
    }
    // bump receive counter 
    dataword = mcp2517_read_register(C1FIFOCON1);
    dataword |= (1 << CiFIFOCONm_UINC);
    mcp2517_write_register(C1FIFOCON1,dataword);
    return 1;
}

#endif // SUPPORT_FOR_MCP2517__
    
