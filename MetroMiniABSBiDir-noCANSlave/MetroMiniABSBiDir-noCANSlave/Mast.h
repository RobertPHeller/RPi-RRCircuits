// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Tue Jun 12 22:46:04 2018
//  Last Modified : <180613.0731>
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

#ifndef __MAST_H
#define __MAST_H

#include "GPIOState.h"

/**
 * Implements a signal mast logic tree.
 * 
 */

class Mast {
private:
    GPIOStateIn *block; // Current block
    GPIOStateIn *next;  // Next block
    GPIOStatePWMOut *stop;  // Stop aspect
    GPIOStatePWMOut *approach; // Approach aspect
    GPIOStatePWMOut *clear; // Clear aspect
    bool commonAnode,biPolarSearch;
    uint8_t stopBrite, approachBrite, clearBrite, yellowHue;
public:
    Mast(GPIOStateIn *b, GPIOStateIn *n, 
         GPIOStatePWMOut *s, GPIOStatePWMOut *a, GPIOStatePWMOut *c, 
         uint8_t sB, uint8_t aB, uint8_t cB, uint8_t yH) {
        block = b;
        next  = n;
        stop  = s;
        approach = a;
        clear = c;
        stopBrite = sB;
        approachBrite = aB;
        clearBrite = cB;
        yellowHue = yH;
        commonAnode = 1;
        biPolarSearch = 0;
    }
    void init(uint8_t cA, uint8_t bPS) {
        commonAnode = cA;
        biPolarSearch = bPS;
    }
    void eval();
};



#endif // __MAST_H

