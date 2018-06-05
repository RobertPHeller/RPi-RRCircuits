/* -*- C -*- ****************************************************************
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : Robert Heller
 *  Created       : Mon Jun 4 09:11:05 2018
 *  Last Modified : <180604.0921>
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

// ----------------------------------------------------------------------------
tCANErrorRegister mcp2517_read_error_register(void)
{
    tCANErrorRegister error;
    
    uint32_t ecoutreg = mcp2517_read_register(C1TREC);
    error.tx = ((ecoutreg>>CiTREC_TEC)&CiTREC_TEC_M);
    error.rx = ((ecoutreg>>CiTREC_REC)&CiTREC_REC_M);
	
    return error;
}

#endif	// SUPPORT_FOR_MCP2515__
