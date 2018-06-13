// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Tue Jun 12 23:46:08 2018
//  Last Modified : <180613.0732>
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

#ifndef __ABSSLAVEBUS_H
#define __ABSSLAVEBUS_H

#include "GPIOState.h"

class ABSSlaveBus {
private:
    uint8_t myid;
    GPIOStateIn *block;
    GPIOStatePWMOut *eastClear;
    GPIOStatePWMOut *eastApproach;
    GPIOStatePWMOut *eastStop;
    GPIOStatePWMOut *westClear;
    GPIOStatePWMOut *westApproach;
    GPIOStatePWMOut *westStop;
public:
    ABSSlaveBus(GPIOStateIn *b,
                GPIOStatePWMOut *eC, GPIOStatePWMOut *eA, GPIOStatePWMOut *eS,
                GPIOStatePWMOut *wC, GPIOStatePWMOut *wA, GPIOStatePWMOut *wS) {
        myid = 0;
        block = b;
        eastClear = eC;
        eastApproach = eA;
        eastStop = eS;
        westClear = wC;
        westApproach = wA;
        westStop = wS;
    }
    void init(uint8_t id) {myid = id;}
    void process();
};


#endif // __ABSSLAVEBUS_H

