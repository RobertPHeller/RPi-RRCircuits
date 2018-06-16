// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Jun 13 17:59:03 2018
//  Last Modified : <180613.1828>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
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

#ifndef __SHIELDHARDWARE_HXX
#define __SHIELDHARDWARE_HXX

GPIO_PIN(NEXT_WEST_DIV,  GpioInputPU, A,  5);
GPIO_PIN(NEXT_WEST_MAIN, GpioInputPU, A,  9);
GPIO_PIN(NEXT_EAST,      GpioInputPU, A,  7);
GPIO_PIN(BLOCKOCC,       GpioInputPU, A, 10);


// Serial Port Pin setup... PA2 (USART2_TX) and PA3 (USART2_RX)


#endif // __SHIELDHARDWARE_HXX

