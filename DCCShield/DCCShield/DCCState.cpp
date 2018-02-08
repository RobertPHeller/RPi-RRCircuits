// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Feb 1 09:51:00 2018
//  Last Modified : <180207.2130>
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

#define DCCLIB

#include "DCCState.h"

static const char rcsid[] = "@(#) : $Id$";

uint8_t DCCState::stack_size = 0;
uint8_t DCCState::currentPointer = 0;
uint8_t DCCState::insertPointer = 0;
DCCState DCCState::stack[DCCSTATE_MAXSTACK];
DCCPacketScheduler *DCCState::dps = NULL;

DCCState::DCCState(uint16_t addr)
{
    address = addr;
    currentFunctions[0] = 0;
    currentFunctions[1] = 0;
    currentFunctions[2] = 0;
    currentSpeed = 0;
    currentSteps = 0;
    uint8_t p;
    for (p = currentPointer;p < stack_size;p++) {
        if (stack[p].address == addr) {
            stack[p] = *this;
            currentPointer = p;
            return;
        }
    }
    for (p = 0; p < currentPointer; p++) {
        if (stack[p].address == addr) {
            stack[p] = *this;
            currentPointer = p;
            return;
        }
    }
    if (stack_size < DCCSTATE_MAXSTACK) {
        stack[stack_size] = *this;
        currentPointer = stack_size++;
        return;
    }
    for (p = 0; p < stack_size; p++) {
        if (stack[p].address == 0) {
            stack[p] = *this;
            currentPointer = p;
            return;
        }
    }
    if (insertPointer < stack_size) {
        stack[insertPointer] = *this;
        currentPointer = insertPointer++;
        return;
    }
    insertPointer = 0;
    stack[insertPointer] = *this;
    currentPointer = insertPointer++;
}

DCCState* DCCState::LookupDecoder(uint16_t addr)
{
    uint8_t p;
    for (p = currentPointer; p < stack_size; p++) {
        if (stack[p].address == addr) {
            currentPointer = p;
            return &(stack[p]);
        }
    }
    for (p = 0; p < currentPointer; p++) {
        if (stack[p].address == addr) {
            currentPointer = p;
            return &(stack[p]);
        }
    }
    DCCState newloco(addr);
    return &(stack[currentPointer]);
}

bool DCCState::eStop(void) 
{
    uint8_t p;
    for (p = 0; p < stack_size; p++) {
        stack[p].currentSpeed = 0;
    }
#ifdef DCCLIB
    if (dps) return dps->eStop();
    else return true;
#else
    return true;
#endif
}

bool DCCState::eStop(uint16_t address)
{
    bool result = true;
    DCCState *loco = LookupDecoder(address);
    if (address < 100) {
#ifdef DCCLIB
        if (dps) result = dps->eStop(address,DCC_SHORT_ADDRESS);
#endif
    } else {
#ifdef DCCLIB
        if (dps) result = dps->eStop(address,DCC_LONG_ADDRESS);
#endif
    }
    if (result) loco->currentSpeed = 0;
    return result;
}

bool DCCState::setSpeed(uint16_t address,int8_t new_speed, uint8_t steps)
{
    bool result = true;
    if (steps != 14 && steps != 28 && steps != 128) {
        Serial.print("Invalid number of steps specified: ");
        Serial.println(steps);
        return false;
    }
    DCCState *loco = LookupDecoder(address);
    if (address < 100) {
#ifdef DCCLIB
        if (dps) result = dps->setSpeed(address,DCC_SHORT_ADDRESS,new_speed,steps);
#endif
    } else {
#ifdef DCCLIB
        if (dps) result = dps->setSpeed(address,DCC_LONG_ADDRESS,new_speed,steps);
#endif
    }
    if (result) {
        loco->currentSpeed = new_speed;
        loco->currentSteps = steps;
    }
    return result;
}

bool DCCState::setFunctions0to4(uint16_t address, uint8_t functions)
{
    bool result = true;
    DCCState *loco = LookupDecoder(address);
    if (address < 100) {
#ifdef DCCLIB
        if (dps) result = dps->setFunctions0to4(address,DCC_SHORT_ADDRESS,functions);
#endif
    } else {
#ifdef DCCLIB
        if (dps) result = dps->setFunctions0to4(address,DCC_LONG_ADDRESS,functions);
#endif
    }
    if (result) loco->currentFunctions[0] = functions;
    return result;
}

bool DCCState::setFunctions5to8(uint16_t address, uint8_t functions)
{
    bool result = true;
    DCCState *loco = LookupDecoder(address);
    if (address < 100) {
#ifdef DCCLIB
        if (dps) result = dps->setFunctions5to8(address,DCC_SHORT_ADDRESS,functions);
#endif
    } else {
#ifdef DCCLIB
        if (dps) result = dps->setFunctions5to8(address,DCC_LONG_ADDRESS,functions);
#endif
    }
    if (result) loco->currentFunctions[1] = functions;
    return result;
}

bool DCCState::setFunctions9to12(uint16_t address, uint8_t functions)
{
    bool result = true;
    DCCState *loco = LookupDecoder(address);
    if (address < 100) {
#ifdef DCCLIB
        if (dps) result = dps->setFunctions9to12(address,DCC_SHORT_ADDRESS,functions);
#endif
    } else {
#ifdef DCCLIB
        if (dps) result = dps->setFunctions9to12(address,DCC_LONG_ADDRESS,functions);
#endif
    }
    if (result) loco->currentFunctions[2] = functions;
    return result;
}

bool DCCState::setBasicAccessory(uint16_t address, uint8_t function)
{
    bool result = true;
    DCCState *acc = LookupDecoder(address);
#ifdef DCCLIB
    if (dps) result = dps->setBasicAccessory(address,function);
#endif
    if (result) acc->currentFunctions[0] = function;
    return result;
}

bool DCCState::unsetBasicAccessory(uint16_t address, uint8_t function)
{
    bool result = true;
    DCCState *acc = LookupDecoder(address);
#ifdef DCCLIB
    if (dps) result = dps->unsetBasicAccessory(address,function);
#endif
    if (result) acc->currentFunctions[0] = ~function;
    return result;
}

bool DCCState::initState()
{
    stack_size = 0;
    currentPointer = 0;
    return saveState();
}

bool DCCState::saveState()
{
    uint8_t p;
    EEPROM.write(0,stack_size);
    EEPROM.write(1,currentPointer);
    for (p = 0; p < stack_size; p++) {
        int eaddr = 2 + (sizeof(stack[p])*p);
        EEPROM.write(eaddr  ,(stack[p].address&0x0FF));
        EEPROM.write(eaddr+1,((stack[p].address>>8)&0x0FF));
        EEPROM.write(eaddr+2,stack[p].currentFunctions[0]);
        EEPROM.write(eaddr+3,stack[p].currentFunctions[1]);
        EEPROM.write(eaddr+4,stack[p].currentFunctions[2]);
        EEPROM.write(eaddr+5,stack[p].currentSpeed);
        EEPROM.write(eaddr+6,stack[p].currentSteps);
    }
    return true;
}

bool DCCState::loadState()
{
    uint8_t p;
    stack_size = EEPROM.read(0);
    currentPointer = EEPROM.read(1);
    for (p = 0; p < stack_size; p++) {
        int eaddr = 2 + (sizeof(stack[p])*p);
        stack[p].address = 
              EEPROM.read(eaddr) | (EEPROM.read(eaddr+1) << 8);
        stack[p].currentFunctions[0] = EEPROM.read(eaddr+2);
        stack[p].currentFunctions[1] = EEPROM.read(eaddr+3);
        stack[p].currentFunctions[2] = EEPROM.read(eaddr+4);
        stack[p].currentSpeed = EEPROM.read(eaddr+5);
        stack[p].currentSteps  = EEPROM.read(eaddr+6);
    }
    return true;
}

bool DCCState::dumpState()
{
    uint8_t p;
    Serial.println(stack_size);
    Serial.println(currentPointer);
    for (p = 0; p < stack_size; p++) {
        Serial.print(stack[p].address);
        Serial.print(",");
        Serial.print(stack[p].currentFunctions[0],HEX);
        Serial.print(",");
        Serial.print(stack[p].currentFunctions[1],HEX);
        Serial.print(",");
        Serial.print(stack[p].currentFunctions[2],HEX);
        Serial.print(",");
        Serial.print(stack[p].currentSpeed);
        Serial.print(",");
        Serial.println(stack[p].currentSteps);
    }
    return true;
}

bool DCCState::updateDCC()
{
    uint8_t p;
    for (p = 0; p < stack_size; p++) {
        if (stack[p].address != 0 && stack[p].currentSteps != 0) {
#ifdef DCCLIB
            setSpeed(stack[p].address,stack[p].currentSpeed,stack[p].currentSteps);
            setFunctions0to4(stack[p].address,stack[p].currentFunctions[0]);
            setFunctions5to8(stack[p].address,stack[p].currentFunctions[1]);
            setFunctions9to12(stack[p].address,stack[p].currentFunctions[2]);
#endif
        } else if (stack[p].address != 0) {
#ifdef DCCLIB
            setBasicAccessory(stack[p].address,stack[p].currentFunctions[0]);
#endif
        }
    }
    return true;
}

bool DCCState::getInformation(uint16_t address)
{
    bool result = true;
    DCCState *loco = LookupDecoder(address);
    Serial.print("Decoder Info ");
    Serial.print(address);
    Serial.print(",");
    Serial.print(loco->currentSteps);
    Serial.print(",");
    Serial.print(loco->currentSpeed);
    Serial.print(",");
    Serial.print(loco->currentFunctions[0],HEX);
    Serial.print(",");
    Serial.print(loco->currentFunctions[1],HEX);
    Serial.print(",");
    Serial.print(loco->currentFunctions[2],HEX);
    Serial.println("");
}
