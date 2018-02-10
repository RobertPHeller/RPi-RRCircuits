// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Feb 10 11:27:17 2018
//  Last Modified : <180210.1238>
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

#ifndef __PARSER_H
#define __PARSER_H

#include <Energia.h>
#include <ctype.h>
#include <string.h>
#define DCCLIB
#include "DCCState.h"

class Parser {
public:
    Parser();
    ~Parser();
    void setup(void);
    int ProcessCommandLine(const char *line); 
    enum Token {EOL = 256, unsignedInt, signedInt, EMERGENCY, STOP, ALL, SET, 
              SPEED, FUNCTIONS1, FUNCTIONS2, FUNCTIONS3, ACCESSORY, SAVE, 
              STATE, UNSET, LOAD, DUMP, INIT, HELP, GET, ESTABLISH, DOUBLE, 
              HEADER, DISSOLVE, ADD, REMOVE, NEXT, PREVIOUS, BADSYMBOL=9999};
private:
    const char *currentPos;
    uint8_t expectedBase;
    bool expectingSigned;
    void printHelp();
    Token processNextWord();
    union {
        uint16_t uval;
        int16_t ival;
    } currentValue;
};

#endif // __PARSER_H

