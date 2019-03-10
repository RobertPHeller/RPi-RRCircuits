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
//  Last Modified : <190309.2058>
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

#include <freertos_drivers/common/PWM.hxx>
 
#include "Lamp.hxx"
#include "Hardware.hxx"

PWM* const Lamp::pinlookup_[17] = {
    nullptr,
    
    &A0_Pin,
    &A1_Pin,
    &A2_Pin,
    &A3_Pin,
    &A4_Pin,
    &A5_Pin,
    &A6_Pin,
    &A7_Pin,
    
    &B0_Pin,
    &B1_Pin,
    &B2_Pin,
    &B3_Pin,
    &B4_Pin,
    &B5_Pin,
    &B6_Pin,
    &B7_Pin
};
    
