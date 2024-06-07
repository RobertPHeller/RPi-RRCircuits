// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Jun 3 13:53:32 2024
//  Last Modified : <240607.1529>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
/// @copyright
///    Copyright (C) 2024  Robert Heller D/B/A Deepwoods Software
///			51 Locke Hill Road
///			Wendell, MA 01379-9728
///
///    This program is free software; you can redistribute it and/or modify
///    it under the terms of the GNU General Public License as published by
///    the Free Software Foundation; either version 2 of the License, or
///    (at your option) any later version.
///
///    This program is distributed in the hope that it will be useful,
///    but WITHOUT ANY WARRANTY; without even the implied warranty of
///    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///    GNU General Public License for more details.
///
///    You should have received a copy of the GNU General Public License
///    along with this program; if not, write to the Free Software
///    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
/// @file main.cxx
/// @author Robert Heller
/// @date Mon Jun 3 13:53:32 2024
///
/// @mainpage Introduction
/// This in a LCC network segment "repeater".  All traffic on one segment is
/// repeated on the other.  Thus there is one logical network with two
/// separate physical segments.
///
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include <ctype.h>                                                              
#include "os/os.h"                                                              
#include "nmranet_config.h"                                                     

#include "executor/Executor.hxx"
#include "executor/Service.hxx"
#include "utils/Hub.hxx"
#include "utils/HubDeviceSelect.hxx"
#include "utils/constants.hxx"

Executor<1> g_executor("g_executor", 0, 1024);
Service g_service(&g_executor);
CanHubFlow can_hub0(&g_service);

/** Entry point to application.
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return 0, should never return
 */
int appl_main(int argc, char *argv[])
{
    HubDeviceSelect<CanHubFlow> seg1(&can_hub0,"/dev/can0");
    HubDeviceSelect<CanHubFlow> seg2(&can_hub0,"/dev/can1");
    while (1)
    {
        sleep(1);
    }
}
