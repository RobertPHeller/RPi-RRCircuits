// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri May 25 10:51:24 2018
//  Last Modified : <180525.1324>
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
#include "OlcbUtil.h"
#include "ButtonLed.h"
#include "ButtonLed_nobutton.h"
#include "ButtonLed_nc.h"
#include "SignalOptions.h"
/**
 * Implements a signal mast logic tree.
 * 
 */

class Mast {
private:
    ButtonLed *block; // Current block
    ButtonLed_nc *next;  // Next block
    ButtonLed_nobutton *stop;  // Stop aspect
    ButtonLed_nobutton *approach; // Approach aspect
    ButtonLed_nobutton *clear; // Clear aspect
    SignalOptions *signal_options;
public:
    Mast(ButtonLed *b, ButtonLed_nc *n, ButtonLed_nobutton *s, 
         ButtonLed_nobutton *a, ButtonLed_nobutton *c, SignalOptions *sigopt) {
        block = b;
        next  = n;
        stop  = s;
        approach = a;
        clear = c;
        signal_options = sigopt;
    }
    void eval();
};


#endif // __MAST_H

