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
//  Last Modified : <180207.1641>
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
#define MULIST_MAXMUS 64
#define MULIST_MAXUNITS 32

#define FLAGMASK 0xC000
#define ADDRMASK 0x3fff

class MUList {
private:
#define DIRECTIONFLAG 0x8000
    uint16_t units[MULIST_MAXUNITS];
    int8_t   unitcount;
    int8_t   currentSpeed;
    uint8_t  currentSteps;
    uint8_t  muaddress;
    static   MUList mus[MULIST_MAXMUS];
    static   uint8_t mucount;
    static   uint8_t currentPointer;
    static   uint8_t insertPointer;
protected:
    MUList(uint8_t muaddress = 1);
public:
    static MUList  *muFromMuAddr(uint8_t muaddr);
    bool eStop();
    bool setSpeed(int8_t new_speed, uint8_t steps);
    static void     addUnitToConsist(uint8_t muaddress, uint16_t unitaddress,bool forward = true);
    static void     deleteUnitFromConsist(uint8_t muaddress, uint16_t unitaddress);
    static bool     isMemberOfConsist(uint8_t muaddress, uint16_t unitaddress);
    static uint16_t nextUnit(uint8_t muaddress, uint16_t unitaddress = 0);
    static uint16_t prevUnit(uint8_t muaddress, uint16_t unitaddress = 0);
    static MUList  *muFromIndex(uint8_t index) {
        if (index < mucount) {
            return &(mus[index]);
        } else {
            return NULL;
        }
    }
    static bool initState();
    static bool dumpState();
    static bool saveState(int &eaddr);
    static bool loadState(int &eaddr);
};

#define DHLIST_MAXDHS 32

class DHList {
private:
    uint16_t unit1, unit2;
    static DHList dhs[DHLIST_MAXDHS];
    static   uint8_t dhcount;
    static   uint8_t currentPointer;
    static   uint8_t insertPointer;
protected:
    DHList(uint16_t u1 = 0, uint16_t u2 = 0);
public: 
    static DHList *dhFromUnit(uint16_t u);
    uint16_t get1() {return unit1;}
    uint16_t get2() {return unit2;}
    static void createDoubleHeader(uint16_t u1, uint16_t u2);
    static void disolveDoubleHeader(uint16_t u1, uint16_t u2);
    static bool isDoubleHeader(uint16_t u);
    static DHList *dhFromIndex(uint8_t index) {
        if (index < dhcount) {
            return &(dhs[index]);
        } else {
            return NULL;
        }
    }
    static bool initState();
    static bool dumpState();
    static bool saveState(int &eaddr);
    static bool loadState(int &eaddr);
};
          
        
#define DCCSTATE_MAXSTACK 255

class DCCState {
private:
#define ISMU 0x8000
#define ISDH 0x4000
    uint16_t address;
    uint8_t  currentFunctions[3];
    int8_t   currentSpeed;
    uint8_t  currentSteps;
    uint8_t  mudhindex;
    static DCCState stack[DCCSTATE_MAXSTACK];
    static uint8_t stack_size;
    static uint8_t currentPointer;
    static uint8_t insertPointer;
    static DCCPacketScheduler *dps;
protected:
    DCCState(uint16_t addr=0);
public:
    bool _eStop();
    bool _setSpeed(int8_t new_speed, uint8_t steps = 0);
    static void setMu(uint16_t address,uint8_t muindex) {
        DCCState *loco = LookupDecoder(address);
        loco->address |= ISMU;
        loco->mudhindex = muindex;
    }
    static void clearMu(uint16_t address) {
        DCCState *loco = LookupDecoder(address);
        loco->address &= ~ISMU;
        loco->mudhindex = 0;
    }
    static void setDh(uint16_t address,uint8_t dhindex) {
        DCCState *loco = LookupDecoder(address);
        loco->address |= ISDH;
        loco->mudhindex = dhindex;
    }
    static void clearDh(uint16_t address) {
        DCCState *loco = LookupDecoder(address);
        loco->address &= ~ISDH;
        loco->mudhindex = 0;
    }
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

