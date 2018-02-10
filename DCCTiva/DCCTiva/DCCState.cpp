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
//  Last Modified : <180207.1651>
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

uint8_t MUList::mucount = 0;
MUList MUList::mus[MULIST_MAXMUS];
uint8_t MUList::currentPointer = 0;
uint8_t MUList::insertPointer = 0;

MUList::MUList(uint8_t muaddr) {
    unitcount = 0;
    currentSpeed = 0;
    currentSteps = 0;
    this->muaddress = muaddr;
    uint8_t p;
    for (p = currentPointer;p < mucount;p++) {
        if (mus[p].muaddress == muaddr) {
            mus[p] = *this;
            currentPointer = p;
            return;
        }
    }
    for (p = 0; p < currentPointer; p++) {
        if (mus[p].muaddress == muaddr) {
            mus[p] = *this;
            currentPointer = p;
            return;
        }
    }
    if (mucount < MULIST_MAXMUS) {
        mus[mucount] = *this;
        currentPointer = mucount++;
        return;
    }
    for (p = 0; p < mucount; p++) {
        if (mus[p].muaddress == 0) {
            mus[p] = *this;
            currentPointer = p;
            return;
        }
    }
    if (insertPointer < mucount) {
        mus[insertPointer] = *this;
        currentPointer = insertPointer++;
        return;
    }
    insertPointer = 0;
    mus[insertPointer] = *this;
    currentPointer = insertPointer++;
}

MUList *MUList::muFromMuAddr(uint8_t muaddr)
{
    uint8_t p;
    for (p = currentPointer; p < mucount; p++) {
        if (mus[p].muaddress == muaddr) {
            currentPointer = p;
            return &(mus[p]);
        }
    }
    for (p = 0; p < currentPointer; p++) {
        if (mus[p].muaddress == muaddr) {
            currentPointer = p;
            return &(mus[p]);
        }
    }
    MUList newmu(muaddr);
    return &(mus[currentPointer]);
}

void     MUList::addUnitToConsist(uint8_t muaddress, uint16_t unitaddress,bool forward)
{
    MUList *mu = muFromMuAddr(muaddress);
    if (mu->unitcount < MULIST_MAXUNITS) {
        mu->units[mu->unitcount] = unitaddress;
        if (forward) {
            DCCState::setSpeed(unitaddress,mu->currentSpeed,mu->currentSteps);
        } else {
            mu->units[mu->unitcount] |= DIRECTIONFLAG;
            DCCState::setSpeed(unitaddress,-(mu->currentSpeed),mu->currentSteps);
        }
        DCCState::setMu(unitaddress,mu->unitcount);
        mu->unitcount++;
    }
}

void     MUList::deleteUnitFromConsist(uint8_t muaddress, uint16_t unitaddress)
{
    MUList *mu = muFromMuAddr(muaddress);
    uint8_t ui, uj;
    for (ui = 0; ui < mu->unitcount; ui++) {
        if ((mu->units[ui] & ADDRMASK) == unitaddress) {
            for (uj = ui+1; uj < mu->unitcount; uj++) {
                mu->units[uj-1] = mu->units[uj];
            }
            mu->unitcount--;
            DCCState::clearMu(unitaddress);
            if (mu->unitcount == 0) {
                mu->muaddress = 0;
            }
            return;
        }
    }
}

bool     MUList::isMemberOfConsist(uint8_t muaddress, uint16_t unitaddress)
{
    MUList *mu = muFromMuAddr(muaddress);
    uint8_t ui;
    for (ui = 0; ui < mu->unitcount; ui++) {
        if ((mu->units[ui] & ADDRMASK) == unitaddress) {
            return true;
        }
    }
    return false;
}

bool     MUList::eStop()
{
    bool result = true;
    uint8_t ui;
    for (ui = 0; ui < unitcount; ui++) {
        DCCState *loco = DCCState::LookupDecoder(units[ui] & ADDRMASK);
        result |= loco->_eStop();
    }
    return result;
}

bool     MUList::setSpeed(int8_t new_speed, uint8_t steps)
{
    bool result = true;
    uint8_t ui;
    for (ui = 0; ui < unitcount; ui++) {
        DCCState *loco = DCCState::LookupDecoder(units[ui] & ADDRMASK);
        result |= loco->_setSpeed(new_speed,steps);
    }
    return result;
}

uint16_t MUList::nextUnit(uint8_t muaddress, uint16_t unitaddress)
{
    uint8_t ui;
    MUList *mu = muFromMuAddr(muaddress);
    if (unitaddress == 0) {
        return (mu->units[0]) & ADDRMASK;
    } else {
        for (ui = 0; ui < mu->unitcount; ui++) {
            if ((mu->units[ui] & ADDRMASK) == unitaddress) {
                if (ui+1 == mu->unitcount) {
                    return 0;
                } else {
                    return (mu->units[ui+1] & ADDRMASK);
                }
            }
        }
        return 0;
    }
}

uint16_t MUList::prevUnit(uint8_t muaddress, uint16_t unitaddress)
{
    uint8_t ui;
    MUList *mu = muFromMuAddr(muaddress);
    if (unitaddress == 0) {
        return (mu->units[mu->unitcount-1]) & ADDRMASK;
    } else {
        for (ui = mu->unitcount-1; ui >= 0; ui--) {
            if ((mu->units[ui] & ADDRMASK) == unitaddress) {
                if (ui == 0) {
                    return 0;
                } else {
                    return (mu->units[ui-1] & ADDRMASK);
                }
            }
        }
        return 0;
    }
}

bool MUList::initState()
{
    mucount = 0;
    currentPointer = 0;
    insertPointer = 0;
    return true;
}

bool MUList::saveState(int &eaddr)
{
    uint8_t p,u;
    EEPROM.write(eaddr,mucount);
    EEPROM.write(eaddr+1,currentPointer);
    eaddr += 2;
    for (p = 0; p < mucount; p++) {
        EEPROM.write(eaddr,mus[p].unitcount);
        eaddr++;
        for (u = 0; u < mus[p].unitcount; u++) {
            EEPROM.write(eaddr,(mus[p].units[u]&0x0FF));
            EEPROM.write(eaddr+1,((mus[p].units[u]>>8)&0x0FF));
            eaddr+=2;
        }
        EEPROM.write(eaddr,mus[p].currentSpeed);
        EEPROM.write(eaddr+1,mus[p].currentSteps);
        EEPROM.write(eaddr+2,mus[p].muaddress);
        eaddr+=3;
    }
    return true;
}

bool MUList::loadState(int &eaddr)
{
    uint8_t p,u;
    mucount = EEPROM.read(eaddr);
    currentPointer = EEPROM.read(eaddr+1);
    eaddr += 2;
    for (p = 0; p < mucount; p++) {
        mus[p].unitcount = EEPROM.read(eaddr);
        eaddr++;
        for (u = 0; u < mus[p].unitcount; u++) {
            mus[p].units[u] = EEPROM.read(eaddr) | (EEPROM.read(eaddr+1) << 8);
            eaddr+=2;
        }
        mus[p].currentSpeed = EEPROM.read(eaddr);
        mus[p].currentSteps = EEPROM.read(eaddr+1);
        mus[p].muaddress    = EEPROM.read(eaddr+2);
        eaddr+=3;
    }
    return true;
}


uint8_t DHList::dhcount = 0;
uint8_t DHList::currentPointer = 0;
uint8_t DHList::insertPointer = 0;
DHList  DHList::dhs[DHLIST_MAXDHS];

DHList::DHList(uint16_t u1, uint16_t u2)
{
    unit1 = u1;
    unit2 = u2;
    uint8_t p;
    for (p = currentPointer;p < dhcount;p++) {
        if ((dhs[p].unit1 == u1 && dhs[p].unit2 == u2) ||
            (dhs[p].unit1 == u2 && dhs[p].unit2 == u1)) {
            dhs[p] = *this;
            DCCState::setDh(u1,p);
            DCCState::setDh(u2,p);
            currentPointer = p;
            return;
        }
    }
    for (p = 0; p < currentPointer; p++) {
        if ((dhs[p].unit1 == u1 && dhs[p].unit2 == u2) ||
            (dhs[p].unit1 == u2 && dhs[p].unit2 == u1)) {
            dhs[p] = *this;
            DCCState::setDh(u1,p);
            DCCState::setDh(u2,p);
            currentPointer = p;
            return;
        }
    }
    if (dhcount < DHLIST_MAXDHS) {
        dhs[dhcount] = *this;
        currentPointer = dhcount++;
        DCCState::setDh(u1,currentPointer);
        DCCState::setDh(u2,currentPointer);
        return;
    }
    for (p = 0; p < dhcount; p++) {
        if (dhs[p].unit1 == 0 && dhs[p].unit2 == 0) {
            dhs[p] = *this;
            DCCState::setDh(u1,p);
            DCCState::setDh(u2,p);
            currentPointer = p;
            return;
        }
    }
    if (insertPointer < dhcount) {
        dhs[insertPointer] = *this;
        currentPointer = insertPointer++;
        DCCState::setDh(u1,currentPointer);
        DCCState::setDh(u2,currentPointer);
        return;
    }
    insertPointer = 0;
    dhs[insertPointer] = *this; 
    currentPointer = insertPointer++;
    DCCState::setDh(u1,currentPointer);
    DCCState::setDh(u2,currentPointer);
}

DHList *DHList::dhFromUnit(uint16_t u)
{
    uint8_t p;
    for (p = currentPointer; p < dhcount; p++) {
        if (dhs[p].unit1 == u || dhs[p].unit2 == u) {
            return &(dhs[p]);
        }
    }
    for (p = 0; p < currentPointer; p++) {
        if (dhs[p].unit1 == u || dhs[p].unit2 == u) {
            return &(dhs[p]);
        }
    }
    return NULL;
}

void DHList::createDoubleHeader(uint16_t u1, uint16_t u2)
{
    if (isDoubleHeader(u1) || isDoubleHeader(u2)) return;
    DHList(u1,u2);
}

void DHList::disolveDoubleHeader(uint16_t u1, uint16_t u2)
{
    DHList *dh = dhFromUnit(u1);
    if (dh == NULL) return;
    DCCState::clearDh(u1);
    DCCState::clearDh(u2);
    dh->unit1 = 0;
    dh->unit2 = 0;
}

bool DHList::isDoubleHeader(uint16_t u)
{
    DHList *dh = dhFromUnit(u);
    if (dh == NULL) return false;
    else return true;
}

bool DHList::initState()
{
    dhcount = 0;
    currentPointer = 0;
    insertPointer = 0;
    return true;
}

bool DHList::saveState(int &eaddr)
{
    uint8_t p;
    EEPROM.write(eaddr,dhcount);
    EEPROM.write(eaddr+1,currentPointer);
    eaddr += 2;
    for (p = 0; p < dhcount; p++) {
        EEPROM.write(eaddr,(dhs[p].unit1&0x0FF));
        EEPROM.write(eaddr+1,((dhs[p].unit1>>8)&0x0FF));
        EEPROM.write(eaddr+2,(dhs[p].unit2&0x0FF));
        EEPROM.write(eaddr+3,((dhs[p].unit2>>8)&0x0FF));
        eaddr+=4;
    }
    return true;
}

bool DHList::loadState(int &eaddr)
{
    uint8_t p;
    dhcount = EEPROM.read(eaddr);
    currentPointer = EEPROM.read(eaddr+1);
    eaddr += 2;
    for (p = 0; p < dhcount; p++) {
        dhs[p].unit1 = EEPROM.read(eaddr) | (EEPROM.read(eaddr+1) << 8);
        dhs[p].unit2 = EEPROM.read(eaddr+2) | (EEPROM.read(eaddr+3) << 8);
        eaddr+=4;
    }
    return true;
}



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
        if ((stack[p].address & ADDRMASK) == addr) {
            currentPointer = p;
            return &(stack[p]);
        }
    }
    for (p = 0; p < currentPointer; p++) {
        if ((stack[p].address & ADDRMASK) == addr) {
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
    MUList *mu = MUList::muFromMuAddr(address);
    if (mu) return mu->eStop();
    if (DHList::isDoubleHeader(address)) {
        DHList *dh = DHList::dhFromUnit(address);
        DCCState *loco = LookupDecoder(dh->get1());
        result = loco->_eStop();
        loco = LookupDecoder(dh->get2());
        result |= loco->_eStop();
        return result;
    }
    DCCState *loco = LookupDecoder(address);
    if ((loco->address & ISMU) != 0) {
        MUList *mu = MUList::muFromIndex(loco->mudhindex);
        if (mu) return mu->eStop();
        else return false;
    }  
    return loco->_eStop();
}
bool DCCState::_eStop()
{          
    bool result = true;
    if (address < 100) {
#ifdef DCCLIB
        if (dps) result = dps->eStop(address,DCC_SHORT_ADDRESS);
#endif
    } else {
#ifdef DCCLIB
        if (dps) result = dps->eStop(address,DCC_LONG_ADDRESS);
#endif
    }
    if (result) currentSpeed = 0;
    return result;
}

bool DCCState::setSpeed(uint16_t address,int8_t new_speed, uint8_t steps)
{
    bool result = true;
    MUList *mu = MUList::muFromMuAddr(address);
    if (mu) return mu->setSpeed(new_speed,steps);
    if (DHList::isDoubleHeader(address)) {
        DHList *dh = DHList::dhFromUnit(address);
        DCCState *loco = LookupDecoder(dh->get1());
        result = loco->_setSpeed(new_speed,steps);
        loco = LookupDecoder(dh->get2());
        result |= loco->_setSpeed(new_speed,steps);
        return result;
    }
    DCCState *loco = LookupDecoder(address);
    if ((loco->address & ISMU) != 0) {
        MUList *mu = MUList::muFromIndex(loco->mudhindex);
        if (mu) return mu->setSpeed(new_speed,steps);
        else return false;
    }  
    return loco->_setSpeed(new_speed,steps);
}
bool DCCState::_setSpeed(int8_t new_speed, uint8_t steps)
{
    bool result = true;
    if (steps != 14 && steps != 28 && steps != 128) {
        Serial.print("Invalid number of steps specified: ");
        Serial.println(steps);
        return false;
    }
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
        currentSpeed = new_speed;
        currentSteps = steps;
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
    MUList::initState();
    DHList::initState();
    return saveState();
}

bool DCCState::saveState()
{
    uint8_t p;
    int eaddr;
    EEPROM.write(0,stack_size);
    EEPROM.write(1,currentPointer);
    for (p = 0; p < stack_size; p++) {
        eaddr = 2 + (sizeof(stack[p])*p);
        EEPROM.write(eaddr  ,(stack[p].address&0x0FF));
        EEPROM.write(eaddr+1,((stack[p].address>>8)&0x0FF));
        EEPROM.write(eaddr+2,stack[p].currentFunctions[0]);
        EEPROM.write(eaddr+3,stack[p].currentFunctions[1]);
        EEPROM.write(eaddr+4,stack[p].currentFunctions[2]);
        EEPROM.write(eaddr+5,stack[p].currentSpeed);
        EEPROM.write(eaddr+6,stack[p].currentSteps);
    }
    eaddr = 2 + (sizeof(stack[0])*stack_size);
    bool result = MUList::saveState(eaddr);
    result |= DHList::saveState(eaddr);
    return result;
}

bool DCCState::loadState()
{
    uint8_t p;
    int eaddr;
    stack_size = EEPROM.read(0);
    currentPointer = EEPROM.read(1);
    for (p = 0; p < stack_size; p++) {
        eaddr = 2 + (sizeof(stack[p])*p);
        stack[p].address = 
              EEPROM.read(eaddr) | (EEPROM.read(eaddr+1) << 8);
        stack[p].currentFunctions[0] = EEPROM.read(eaddr+2);
        stack[p].currentFunctions[1] = EEPROM.read(eaddr+3);
        stack[p].currentFunctions[2] = EEPROM.read(eaddr+4);
        stack[p].currentSpeed = EEPROM.read(eaddr+5);
        stack[p].currentSteps  = EEPROM.read(eaddr+6);
    }
    eaddr = 2 + (sizeof(stack[0])*stack_size);
    bool result = MUList::loadState(eaddr);
    result |= DHList::loadState(eaddr);
    return result;
}

bool DCCState::dumpState()
{
    uint8_t p;
    Serial.println("Decoder stack:");
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
    return MUList::dumpState() || DHList::dumpState();
}

bool MUList::dumpState()
{
    uint8_t p, u;
    Serial.println("Multiple Unit List:");
    Serial.println(mucount);
    Serial.println(currentPointer);
    for (p = 0; p < mucount; p++) {
        Serial.print(mus[p].muaddress);
        Serial.print(",");
        Serial.print(mus[p].currentSpeed);
        Serial.print(",");
        Serial.print(mus[p].currentSteps);
        Serial.print(",");
        Serial.print(mus[p].unitcount);
        Serial.print(",");
        for (u = 0; u < mus[p].unitcount; u++) {
            Serial.print(mus[p].units[u]);
            if ((u+1) < mus[p].unitcount) Serial.print(",");
        }
        Serial.println("");
    }
    return true;
}

bool DHList::dumpState()
{
    uint8_t p;
    Serial.println("Double Header List:");
    Serial.println(dhcount);
    Serial.println(currentPointer);
    for (p = 0; p < dhcount; p++) {
        Serial.print(dhs[p].unit1);
        Serial.print(",");
        Serial.println(dhs[p].unit2);
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
