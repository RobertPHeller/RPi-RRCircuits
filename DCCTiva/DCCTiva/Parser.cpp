// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Feb 10 11:32:16 2018
//  Last Modified : <180210.1537>
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

#include "Parser.h"

static struct {
    char *word;
    Parser::Token token;
} reserverWords[] = {
    { "EMERGENCY" , Parser::EMERGENCY }, 
    { "STOP" , Parser::STOP }, 
    { "ALL" , Parser::ALL }, 
    { "SET" , Parser::SET }, 
    { "SPEED" , Parser::SPEED }, 
    { "FUNCTIONS1" , Parser::FUNCTIONS1 }, 
    { "FUNCTIONS2" , Parser::FUNCTIONS2 }, 
    { "FUNCTIONS3" , Parser::FUNCTIONS3 }, 
    { "ACCESSORY" , Parser::ACCESSORY }, 
    { "SAVE" , Parser::SAVE }, 
    { "STATE" , Parser::STATE }, 
    { "UNSET" , Parser::UNSET }, 
    { "LOAD" , Parser::LOAD }, 
    { "DUMP" , Parser::DUMP }, 
    { "INIT" , Parser::INIT }, 
    { "HELP" , Parser::HELP }, 
    { "GET" , Parser::GET }, 
    { "INFORMATION", Parser::INFORMATION },
    { "ESTABLISH" , Parser::ESTABLISH }, 
    { "DOUBLE" , Parser::DOUBLE }, 
    { "HEADER" , Parser::HEADER }, 
    { "DISSOLVE" , Parser::DISSOLVE }, 
    { "ADD" , Parser::ADD }, 
    { "TO" , Parser::TO },
    { "MULTIPLE" , Parser::MULTIPLE },
    { "UNIT" , Parser::UNIT },
    { "FORWARD" , Parser::FORWARD },
    { "REVERSE" , Parser::REVERSE },
    { "REMOVE" , Parser::REMOVE }, 
    { "FROM" , Parser::FROM },
    { "NEXT" , Parser::NEXT }, 
    { "PREVIOUS" , Parser::PREVIOUS }
};
#define NumberOfReservedWords (sizeof(reserverWords) / sizeof(reserverWords[0]))

Parser::Parser()
{
    currentPos = NULL;
    expectedBase = 10;
    expectingSigned = false;
}

Parser::~Parser()
{
}

void Parser::setup()
{
}

int Parser::ProcessCommandLine(const char* line)
{
    currentPos = line;
    expectedBase = 10;
    expectingSigned = false;
    Token next;
    uint16_t address,address2;
    int8_t speed;
    uint8_t steps, functions,muaddress;
    switch (processNextWord()) {
    case EMERGENCY:
        if (processNextWord() != STOP) {
            return 1;
        }
        expectedBase = 10;
        next = processNextWord();
        if (next == ALL) {
            if (processNextWord() != EOL) return 1;
            if (!DCCState::eStop()) return -1;
            return 0;
        } else if (next == unsignedInt) {
            address = currentValue.uval;
            if (processNextWord() != EOL) return 1;
            if (!DCCState::eStop(address)) return -1;
            return 0;
        } else return 1;
        break;
    case SET:
        switch (processNextWord()) {
        case SPEED:
            expectedBase = 10;
            if (processNextWord() == unsignedInt) {
                address = currentValue.uval;
                expectingSigned = true;
                if (processNextWord() == signedInt) {
                    speed = currentValue.ival;
                    expectingSigned = false;
                    if (processNextWord() == unsignedInt) {
                        steps = currentValue.uval;
                        if (processNextWord() != EOL) return 1;
                        if (!DCCState::setSpeed(address,speed,steps)) return -1;
                        return 0;
                    } else return 1;
                } else return 1;
            } else return 1;
            break;
        case FUNCTIONS1:
            expectedBase = 10;
            if (processNextWord() == unsignedInt) {
                address = currentValue.uval;
                expectedBase = 16;
                if (processNextWord() == unsignedInt) {
                    functions = currentValue.uval;
                    if (processNextWord() != EOL) return 1;
                    if (!DCCState::setFunctions0to4(address,functions)) return -1;
                    return 0;
                } else return 1;
            } else return 1;
            break;
        case FUNCTIONS2:
            expectedBase = 10;
            if (processNextWord() == unsignedInt) {
                address = currentValue.uval;
                expectedBase = 16;
                if (processNextWord() == unsignedInt) {
                    functions = currentValue.uval;
                    if (processNextWord() != EOL) return 1;
                    if (!DCCState::setFunctions5to8(address,functions)) return -1;
                    return 0;
                } else return 1;
            } else return 1;
            break;
        case FUNCTIONS3:
            expectedBase = 10;
            if (processNextWord() == unsignedInt) {
                address = currentValue.uval;
                expectedBase = 16;
                if (processNextWord() == unsignedInt) {
                    functions = currentValue.uval;
                    if (processNextWord() != EOL) return 1;
                    if (!DCCState::setFunctions9to12(address,functions)) return -1;
                    return 0;
                } else return 1;
            } else return 1;
            break;
        case ACCESSORY:
            expectedBase = 10;
            if (processNextWord() == unsignedInt) {
                address = currentValue.uval;
                expectedBase = 16;
                if (processNextWord() == unsignedInt) {
                    functions = currentValue.uval;
                    if (processNextWord() != EOL) return 1;
                    if (!DCCState::setBasicAccessory(address,functions)) return -1;
                    return 0;
                } else return 1;
            } else return 1;
            break;
        default:
            return 1;
        }
        break;
    case SAVE:
        if (processNextWord() != STATE) {
            return 1;
        }
        if (processNextWord() != EOL) return 1;
        if (!DCCState::saveState()) return -1;
        return 0;
        break;
    case UNSET:
        if (processNextWord() != ACCESSORY) return 1;
        expectedBase = 10;
        expectingSigned = false;
        if (processNextWord() == unsignedInt) {
            address = currentValue.uval;
            expectedBase = 16;
            if (processNextWord() == unsignedInt) {
                functions = currentValue.uval;
                if (processNextWord() != EOL) return 1;
                if (!DCCState::unsetBasicAccessory(address,functions)) return -1;
                return 0;
            } else return 1;
        } else return 1;
        break;
    case LOAD:
        if (processNextWord() != STATE) {
            return 1;
        }
        if (processNextWord() != EOL) return 1;
        if (!DCCState::loadState()) return -1;
        return 0;
        break;
    case DUMP:
        if (processNextWord() != STATE) {
            return 1;
        }
        if (processNextWord() != EOL) return 1;
        if (!DCCState::dumpState()) return -1;
        return 0;
        break;
    case INIT:
        if (processNextWord() != STATE) {
            return 1;
        }
        if (processNextWord() != EOL) return 1;
        if (!DCCState::initState()) return -1;
        return 0;
        break;
    case HELP:
        if (processNextWord() != EOL) return 1;
        printHelp();
        return 0;
        break;
    case GET:
        if (processNextWord() != INFORMATION) {
            return 1;
        }
        expectedBase = 10;
        expectingSigned = false;
        if (processNextWord() == unsignedInt) {
            address = currentValue.uval;
            if (processNextWord() != EOL) return 1;
            if (!DCCState::getInformation(address)) return -1;
            return 0;
        } else return 1;
        break;
    case ESTABLISH:
        if (processNextWord() != DOUBLE) {
            return 1;
        }
        if (processNextWord() != HEADER) {
            return 1;
        }
        expectedBase = 10;
        expectingSigned = false;
        if (processNextWord() == unsignedInt) {
            address = currentValue.uval;
            if (processNextWord() == unsignedInt) {
                address2 = currentValue.uval;
                if (processNextWord() != EOL) return 1;
                if (!DHList::createDoubleHeader(address,address2)) return -1;
                return 0;
            } else return 1;
        } else return 1;
        break;
    case DISSOLVE:
        if (processNextWord() != DOUBLE) {
            return 1;
        }
        if (processNextWord() != HEADER) {
            return 1;
        }
        expectedBase = 10;
        expectingSigned = false;
        if (processNextWord() == unsignedInt) {
            address = currentValue.uval;
            if (processNextWord() == unsignedInt) {
                address2 = currentValue.uval;
                if (processNextWord() != EOL) return 1;
                if (!DHList::disolveDoubleHeader(address,address2)) return -1;
                return 0;
            } else return 1;
        } else return 1;
        break;
    case ADD:
        expectedBase = 10;
        expectingSigned = false;
        if (processNextWord() == unsignedInt) {
            address = currentValue.uval;
        } else return 1;
        if (processNextWord() != TO) {
            return 1;
        }
        if (processNextWord() != MULTIPLE) {
            return 1;
        }
        if (processNextWord() != UNIT) {
            return 1;
        }
        expectedBase = 10;
        expectingSigned = false;
        if (processNextWord() == unsignedInt) {
            muaddress = currentValue.uval;
        } else return 1;
        next = processNextWord();
        if (next == FORWARD) {
            if (processNextWord() != EOL) return 1;
            if (!MUList::addUnitToConsist(muaddress,address,true)) return -1;
            return 0;
        } else if (next == REVERSE) {
            if (processNextWord() != EOL) return 1;
            if (!MUList::addUnitToConsist(muaddress,address,false)) return -1;
            return 0;
        }  else return 1;
        break;
    case REMOVE:
        expectedBase = 10;
        expectingSigned = false;
        if (processNextWord() == unsignedInt) {
            address = currentValue.uval;
        } else return 1;
        if (processNextWord() != FROM) {
            return 1;
        }
        if (processNextWord() != MULTIPLE) {
            return 1;
        }
        if (processNextWord() != UNIT) {
            return 1;
        }
        expectedBase = 10;
        expectingSigned = false;
        if (processNextWord() == unsignedInt) {
            muaddress = currentValue.uval;
        } else return 1;
        if (processNextWord() != EOL) return 1;
        if (!MUList::deleteUnitFromConsist(muaddress,address)) return -1;
        return 0;
        break;
    case NEXT:
        if (processNextWord() != UNIT) {
            return 1;
        }
        expectedBase = 10;
        expectingSigned = false;
        if (processNextWord() == unsignedInt) {
            muaddress = currentValue.uval;
        } else return 1;
        expectedBase = 10;
        expectingSigned = false;
        if (processNextWord() == unsignedInt) {
            address = currentValue.uval;
        } else return 1;
        if (processNextWord() != EOL) return 1;
        address2 = MUList::nextUnit(muaddress,address);
        Serial.println(address2);
        return 0;
        break;
    case PREVIOUS:
        if (processNextWord() != UNIT) {
            return 1;
        }
        expectedBase = 10;
        expectingSigned = false;
        if (processNextWord() == unsignedInt) {
            muaddress = currentValue.uval;
        } else return 1;
        expectedBase = 10;
        expectingSigned = false;
        if (processNextWord() == unsignedInt) {
            address = currentValue.uval;
        } else return 1;
        if (processNextWord() != EOL) return 1;
        address2 = MUList::prevUnit(muaddress,address);
        Serial.println(address2);
        return 0;
        break;
    default:
        return 1;
    }
    return 1;
}

Parser::Token Parser::processNextWord()
{
    char word[32];
    char *p, *eow;
    int i;
    while (*currentPos != '\0' && isspace(*currentPos)) currentPos++;
    if (*currentPos == '\0') return EOL;
    if (isalpha(*currentPos)) {
        p = word;
        eow = p+sizeof(word)-1;
        while (isalnum(*currentPos) && p <= eow) {
            *p++ = toupper(*currentPos++);
        }
        *p = '\0';
        for (i = 0; i < NumberOfReservedWords; i++) {
            if (strcmp(word,reserverWords[i].word) == 0) {
                return reserverWords[i].token;
            }
        }
        return BADSYMBOL;
    } else {
        if (expectingSigned) {
            currentValue.ival = strtol(currentPos,&p,expectedBase);
            if (p == currentPos) {
                return (Token)(*currentPos++);
            } else {
                currentPos = p;
                return signedInt;
            }
        } else {
            currentValue.uval = strtoul(currentPos,&p,expectedBase);
            if (p == currentPos) {
                return (Token)(*currentPos++);
            } else {
                currentPos = p;
                return unsignedInt;
            }
        }
    }
    return (Token)(*currentPos++);
}

void Parser::printHelp()
{
    Serial.println("EMERGENCY STOP ALL");
    Serial.println("EMERGENCY STOP address");
    Serial.println("SET SPEED address speed steps");
    Serial.println("SET FUNCTIONS1 address functions");
    Serial.println("SET FUNCTIONS2 address functions");
    Serial.println("SET FUNCTIONS3 address functions");
    Serial.println("SET ACCESSORY address functions");
    Serial.println("UNSET ACCESSORY address functions");
    Serial.println("SAVE STATE");
    Serial.println("LOAD STATE");
    Serial.println("DUMP STATE");
    Serial.println("INIT STATE");
    Serial.println("HELP");
    Serial.println("GET INFORMATION address");
    Serial.println("ESTABLISH DOUBLE HEADER address1 address2");
    Serial.println("DISSOLVE DOUBLE HEADER address1 address2");
    Serial.println("ADD address TO MULTIPLE UNIT muaddress FORWARD");
    Serial.println("ADD address TO MULTIPLE UNIT muaddress REVERSE");
    Serial.println("REMOVE address FROM MULTIPLE UNIT muaddress");
    Serial.println("NEXT UNIT muaddress address");
    Serial.println("PREVIOUS UNIT muaddress address");
}

