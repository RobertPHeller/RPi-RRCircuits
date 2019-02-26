// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Feb 25 11:40:38 2019
//  Last Modified : <190225.1821>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
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

#include <unordered_map>
#include <os/Gpio.hxx>
 
#include "Lamp.hxx"
#include "Hardware.hxx"

const Gpio* Lamp::pinlookup_[17] = {
    nullptr,
    
    A0_Pin::instance(),
    A1_Pin::instance(),
    A2_Pin::instance(),
    A3_Pin::instance(),
    A4_Pin::instance(),
    A5_Pin::instance(),
    A6_Pin::instance(),
    A7_Pin::instance(),
    
    B0_Pin::instance(),
    B1_Pin::instance(),
    B2_Pin::instance(),
    B3_Pin::instance(),
    B4_Pin::instance(),
    B5_Pin::instance(),
    B6_Pin::instance(),
    B7_Pin::instance()
};
    
