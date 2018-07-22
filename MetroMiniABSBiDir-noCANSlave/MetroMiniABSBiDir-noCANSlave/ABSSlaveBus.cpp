// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Tue Jun 12 23:52:07 2018
//  Last Modified : <180722.1636>
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

#include "ABSSlaveBus.h"

void ABSSlaveBus::process() {
    if (Serial.available() > 0) {
        //Serial.print("*** Serial.available() = "), Serial.println(Serial.available());
        // Message from the master format:
        // :Gnn; + CRLF
        // Where nn is an id (1 or 2 digit decimal)
        // 7 bytes at 115200 BAUD, assuming 10 bits (start, 8 data, 2 stop) 
        // takes .6ms
        char byte = Serial.read();
        //Serial.print("*** byte = "); Serial.println(byte);
        if (byte != ':') return;
        if (Serial.available() > 0) {
            byte = Serial.read();
        } else {
            return;
        }
        //Serial.print("*** byte = "); Serial.println(byte);
        if (byte != 'G') return;
        if (Serial.available() < 1) return;
        int id = Serial.parseInt();
        while (Serial.available() > 0 && Serial.read() != '\n') ;
        if (id != myid) return;
        // At 115200 BAUD, assuming 10 bits (start, 8 data, 2 stop)
        // a 12 byte message takes 1.04ms
        // Response message format:
        //  :RnnoEaWa; + CRLF
        // Where: nn == My id (1 or 2 digit decimal)
        //        o  == O (block occupied) or C (block clear)
        //        a  == S (stop aspect), A (approach aspect), or C (clear aspect).
        //
        Serial.print(':');
        Serial.print('R');
        Serial.print(myid,DEC);
        if (block->state) Serial.print('O');
        else Serial.print('C');
        Serial.print('E');
        if (eastStop->state) Serial.print('S');
        else if (eastApproach->state) Serial.print('A');
        else Serial.print('C');
        Serial.print('W');
        if (westStop->state) Serial.print('S');
        else if (westApproach->state) Serial.print('A');
        else Serial.print('C');
        Serial.println(';');
    }
}
