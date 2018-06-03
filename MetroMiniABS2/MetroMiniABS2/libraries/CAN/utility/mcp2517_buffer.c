/* -*- C -*- ****************************************************************
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : Robert Heller
 *  Created       : Sun Jun 3 16:13:03 2018
 *  Last Modified : <180603.1647>
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

// ------------------------------------
// Check if there are any new messages waiting

bool mcp2517_check_message(void)
{
#if defined(MCP2517_INT)
    return ((!IS_SET(MCP2515_INT)) ? true : false);
#else 
    return (((mcp2517_read_register(C1RXIF) >> CiRXIF_RFIF) && CiRXIF_RFIF_M) != 0);
}

bool mcp2517_check_free_buffer(void)
{
    return (((mcp2517_read_register(C1TEFSTA) >> CiTEFSTA_TEFFIF) & 0x01) == 0);
}

