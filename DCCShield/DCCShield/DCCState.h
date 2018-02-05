// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Feb 1 09:33:11 2018
//  Last Modified : <180205.1530>
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

#ifndef __DCCSTATE_H
#define __DCCSTATE_H

#include "Arduino.h"
#include <EEPROM.h>
#ifdef DCCLIB
#include <DCCPacket.h>
#include <DCCPacketQueue.h>
#include <DCCPacketScheduler.h>
#else
#define DCCPacketScheduler void
#endif

#define DCCSTATE_MAXSTACK 32

class DCCState {
private:
    uint16_t address;
    uint8_t  currentFunctions[3];
    int8_t   currentSpeed;
    uint8_t  currentSteps;
    static DCCState stack[DCCSTATE_MAXSTACK];
    static uint8_t stack_size;
    static uint8_t currentPointer;
    static uint8_t insertPointer;
    static DCCPacketScheduler *dps;
protected:
    DCCState(uint16_t addr=0);
public:
    static void setup(DCCPacketScheduler *d) {
        dps = d;
        stack_size = 0;
        currentPointer = 0;
        insertPointer = 0;
    }
    static DCCState *LookupDecoder(uint16_t addr=3);
    static bool eStop(void); //all locos
    static bool eStop(uint16_t address); //just one specific loco
    static bool getInformation(uint16_t address);
    static bool setSpeed(uint16_t address,int8_t new_speed, uint8_t steps = 0);
    static bool setFunctions0to4(uint16_t address, uint8_t functions);
    static bool setFunctions5to8(uint16_t address, uint8_t functions);
    static bool setFunctions9to12(uint16_t address, uint8_t functions);
    static bool setBasicAccessory(uint16_t address, uint8_t function);
    static bool unsetBasicAccessory(uint16_t address, uint8_t function);
    static bool saveState();
    static bool loadState();
    static bool dumpState();
    static bool initState();
    static bool updateDCC();
};    
    

#endif // __DCCSTATE_H

