// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri May 25 11:03:28 2018
//  Last Modified : <180525.1352>
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

static const char rcsid[] = "@(#) : $Id$";

#include "Mast.h"

void Mast::eval() {
    if (block->state) {
        stop->state = true;
        approach->state = false;
        clear->state = false;
    } else if (next->state) {
        stop->state = false;
        approach->state = true;
        clear->state = false;
    } else {
        stop->state = false;
        approach->state = false;
        clear->state = true;
    }
    // Set LED brightnesses...
    if (stop->state) {
        if (signal_options->common_anode == 1) {
            stop->setPWM(signal_options->stop_brite);
            approach->setPWM(0);
            clear->setPWM(0);
        } else {
            stop->setPWM(255-signal_options->stop_brite);
            approach->setPWM(255);
            clear->setPWM(255);
        }
    } else if (approach->state) { 
        if (signal_options->bicolor_search == 1) {
            uint8_t green = signal_options->approach_brite*(signal_options->yellow_hue/256.0);
            uint8_t red   = signal_options->approach_brite*((255-signal_options->yellow_hue)/256.0);
            if (signal_options->common_anode == 1) {
                stop->setPWM(red);
                clear->setPWM(green);
            } else {
                stop->setPWM(255-red);
                clear->setPWM(255-green);
            }
        } else {
            if (signal_options->common_anode) {
                stop->setPWM(0);
                approach->setPWM(signal_options->approach_brite);
                clear->setPWM(0);
            } else {
                stop->setPWM(255);
                approach->setPWM(255-signal_options->approach_brite);
                clear->setPWM(255);
            }
        }
    } else if (clear->state) {
        if (signal_options->common_anode) {
            stop->setPWM(0);
            approach->setPWM(0);
            clear->setPWM(signal_options->clear_brite);
        } else {
            stop->setPWM(255);
            approach->setPWM(255);
            clear->setPWM(255-signal_options->clear_brite);
        }
    }   
}

