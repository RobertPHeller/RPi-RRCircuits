// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Dec 18 12:09:00 2022
//  Last Modified : <221218.1323>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2022  Robert Heller D/B/A Deepwoods Software
//			51 Locke Hill Road
//			Wendell, MA 01379-9728
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// 
//
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include <string>
#include <string.h>
#include <stdio.h>

using std::string;

namespace utils
{

string node_id_to_string(uint64_t node_id)
{
    string result = "";
    char buffer[8];
    string dot = "";
    for (int i=0; i<6; i++)
    {
        snprintf(buffer,sizeof(buffer),"%02X",(unsigned)(node_id&0x0FF));
        result = buffer + dot + result;
        dot = ".";
        node_id >>= 8;
    }
    return result;
}

uint64_t string_to_uint64(const string node_string)
{
    uint64_t result = 0LL;
    for (int i=0; i<6; i++)
    {
        int index = i*3;
        unsigned byte=0;
        sscanf(node_string.substr(index,2).c_str(),"%02X",&byte);
        result <<= 8;
        result |= byte;
    }
    return result;
}


}
