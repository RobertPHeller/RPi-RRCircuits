// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri May 25 13:15:00 2018
//  Last Modified : <180525.1353>
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

#ifndef __SIGNALOPTIONS_H
#define __SIGNALOPTIONS_H

#include <Arduino.h>

class SignalOptions {
public:
    uint8_t common_anode;
    uint8_t bicolor_search;
    uint8_t stop_brite;
    uint8_t approach_brite;
    uint8_t clear_brite;
    uint8_t yellow_hue;
    SignalOptions() {
        common_anode = 1;
        bicolor_search = 0;
        stop_brite = 255;
        approach_brite = 255;
        clear_brite = 255;
        yellow_hue = 128;
    }
    void reset() {
        common_anode = 1;
        bicolor_search = 0;
        stop_brite = 255;
        approach_brite = 255;
        clear_brite = 255;
        yellow_hue = 128;
    }
};
    

#endif // __SIGNALOPTIONS_H

