/* -*- C -*- ****************************************************************
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : Robert Heller
 *  Created       : Wed Jun 6 15:00:02 2018
 *  Last Modified : <180606.1517>
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

#if SUPPORT_EXTENDED_CANID

void mcp2517_write_id( const uint32_t *id, uint8_t extended )
{
    uint16_t address = 0x400 + mcp2517_read_register(C1TXQUA);
    uint32_t T0;
    if (extended) {
        T0  = ((*id >> 18) & TBUFFT0_SID_M) << TBUFFT0_SID;
        T0 |= (*id & TBUFFT0_EID_M);
    } else {
        T0 = (*id & TBUFFT0_SID_M);
    }
    mcp2517_write_register(address,T0);
}

#else

void mcp2517_write_id( const uint16_t *id )
{
    uint16_t address = 0x400 + mcp2517_read_register(C1TXQUA);
    uint32_t T0;
    T0 = (*id & TBUFFT0_SID_M);
    mcp2517_write_register(address,T0);
}

#endif

#endif




