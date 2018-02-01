/****************************************************************************
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : Robert Heller
 *  Created       : Wed Jan 31 09:43:14 2018
 *  Last Modified : <180201.1839>
 *
 *  Description	
 *
 *  Notes
 *
 *  History
 *	
 ****************************************************************************
 *
 *    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
 *			51 Locke Hill Road
 *			Wendell, MA 01379-9728
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * 
 *
 ****************************************************************************/

%{
#include "Arduino.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "../DCCState.h"

static void     yyerror (const char *s);
static int      yylex (void);
static char    *currentPos;
int ProcessCommandLine (char *line);
//#define YYERROR_VERBOSE 1
//#define COMMANDFAILED F("Command failed")
//#define COMMANDFAILED "Command failed"
static const prog_char COMMANDFAILED[] = {"Command failed"};
%}


%start COMMAND

%union {
    int ival;
}

%token EMERGENCY STOP ALL SET SPEED FUNCTIONS ACCESSORY UNSET SAVE STATE LOAD 
%token DUMP INIT
%token BADSYMBOL
%token <ival> INTEGER

%%

COMMAND : EMERGENCY STOP ALL '\n' {if (!DCCState::eStop()) 
                                    Serial.println (COMMANDFAILED);}
        | EMERGENCY STOP INTEGER '\n' {
            if (!DCCState::eStop($3))
                Serial.println (COMMANDFAILED);}
        | SET SPEED INTEGER INTEGER INTEGER '\n' {
            if (!DCCState::setSpeed($3,$4,$5))
                Serial.println (COMMANDFAILED);}
        | SET FUNCTIONS INTEGER INTEGER '\n' {
            if (!DCCState::setFunctions($3,(uint16_t)$4))
                Serial.println (COMMANDFAILED);}
        | SET ACCESSORY INTEGER INTEGER '\n' {
            if (!DCCState::setBasicAccessory($3,$4))
                Serial.println (COMMANDFAILED);}
        | UNSET ACCESSORY INTEGER INTEGER '\n' {
            if (!DCCState::unsetBasicAccessory($3,$4))
                Serial.println (COMMANDFAILED);}
        | SAVE STATE '\n' {if (!DCCState::saveState())
                            Serial.println (COMMANDFAILED);}
        | LOAD STATE '\n' {if (!DCCState::loadState())
                                Serial.println (COMMANDFAILED);}
        | DUMP STATE '\n' {if (!DCCState::dumpState())
                                Serial.println (COMMANDFAILED);}
        | INIT STATE '\n' {if (!DCCState::initState())
                                Serial.println (COMMANDFAILED);}
        ;
        
%%


static prog_char rcsid[]  = "@(#) : $Id$";

//#define ROMTABLE

#ifndef ROMTABLE
static const struct {
    const char * const string;
    uint16_t token;
} ReservedWords[] = {
    {"EMERGENCY", EMERGENCY},
    {"STOP", STOP},
    {"ALL", ALL},
    {"SET", SET},
    {"SPEED", SPEED},
    {"FUNCTIONS", FUNCTIONS},
    {"ACCESSORY", ACCESSORY},
    {"UNSET", UNSET},
    {"SAVE", SAVE},
    {"STATE", STATE},
    {"LOAD", LOAD},
    {"DUMP", DUMP},
    {"INIT", INIT} };
#define NumberOfReservedWords (sizeof(ReservedWords)/sizeof(ReservedWords[0]))
#else
static char string_EMERGENCY[]  = "EMERGENCY";
static prog_char string_STOP[]  = "STOP";
static prog_char string_ALL[]  = "ALL";
static prog_char string_SET[]  = "SET";
static prog_char string_SPEED[]  = "SPEED";
static prog_char string_FUNCTIONS[]  = "FUNCTIONS";
static prog_char string_ACCESSORY[]  = "ACCESSORY";
static prog_char string_UNSET[]  = "UNSET";
static prog_char string_SAVE[]  = "SAVE";
static prog_char string_STATE[]  = "STATE";
static prog_char string_LOAD []  = "LOAD ";
static prog_char string_DUMP[]  = "DUMP";
static prog_char string_INIT[]  = "INIT";

const char* const PROGMEM Reserved_Words[]  = {string_EMERGENCY,string_STOP,
                                                  string_ALL,string_SET,
                                                  string_SPEED,
                                                  string_FUNCTIONS,
                                                  string_ACCESSORY,
                                                  string_UNSET,string_SAVE,
                                                  string_STATE,string_LOAD,
                                                  string_DUMP,string_INIT};
const prog_uint16_t  Reserved_Tokens[] = {EMERGENCY,STOP,ALL,SET,SPEED,
                                                FUNCTIONS,ACCESSORY,UNSET,
                                                SAVE,STATE,LOAD,DUMP,INIT};
#define NumberOfReservedWords (sizeof(Reserved_Tokens)/sizeof(Reserved_Tokens[0]))                                                
                                                  
#endif

static uint16_t LookupWord(const char *word) {
    size_t i;
#ifndef ROMTABLE
    for (i = 0; i < NumberOfReservedWords; i++) {
        if (strcasecmp(ReservedWords[i].string,word) == 0) {
            return(ReservedWords[i].token);
        }   
    }
#else
    char buffer[10];
    for (i = 0; i < NumberOfReservedWords; i++) {
        // Necessary casts and dereferencing, just copy.
        strcpy_P(buffer, (char*)pgm_read_word(&(Reserved_Words[i])));
        if (strcasecmp(buffer,word) == 0) {
            return(pgm_read_word(&(Reserved_Tokens[i])));
        }
    }
#endif
    return (BADSYMBOL);    
}
int yylex()
{
/*
 * Array of all known reserved words and the token IDs.
 */


                  
    char     word[16];

#if YYDEBUG
    fprintf(stderr,"*** yylex(): currentPos = %p\n",currentPos);
    if (currentPos != NULL) fprintf(stderr,"*** yylex(): currentPos is '%s'\n",currentPos);
#endif
    /* At EOL? return EOF */
    if (currentPos == NULL || *currentPos == '\0') return YYEOF;
    /* Skip white space (not '\n'!) */
    while (*currentPos != '\0' && *currentPos != '\n' && *currentPos <= ' ') currentPos++;
    /* At EOL? return EOF */
#if YYDEBUG
    fprintf(stderr,"*** yylex(): (after skipping whitespace) currentPos is '%s'\n",currentPos);
#endif
    if (*currentPos == '\0') return YYEOF;
    if (isalpha(*currentPos)) {
        /* Accumulate the word. */
        char *s = word;
        while (isalnum(*currentPos)) *s++ = *currentPos++;
        /*yylloc.last_column = currentPos - yylloc.text;*/
        *s = '\0';
#if YYDEBUG
        fprintf(stderr,"*** yylex(): word = '%s'\n",word);
#endif
        /* Lookup word in table, returning corresponding token. */
        return LookupWord(word);
    } else if (isdigit(*currentPos) || 
                (*currentPos == '-' && isdigit(*(currentPos+1)))) {
        /* Accumulate the integer */
        char *s = word;
        *s++ = *currentPos++;
        while (isdigit(*currentPos)) *s++ = *currentPos++;
        /*yylloc.last_column = currentPos - yylloc.text;*/
        *s = '\0';
#if YYDEBUG
        fprintf(stderr,"*** yylex(): word = '%s'\n",word);
#endif
        yylval.ival = atoi(word);
        return(INTEGER);
    } else {
#if YYDEBUG
        fprintf(stderr,"*** yylex(): returning a char: %c\n",*currentPos);
#endif
        return *currentPos++;
    }
}

static void yyerror(const char *message) {
    Serial.println(message);
//    fprintf(stderr,"%s\n",message);
}

int ProcessCommandLine (char *line) {
    currentPos = line;
    return yyparse();
}

#ifdef TESTMAIN

int main(int argc, char *argv[]) 
{
    char line[256];
    
    printf("> ");fflush(stdout);
    while (fgets(line,256,stdin) != NULL) {
#if YYDEBUG
        fprintf(stderr,"*** Line is '%s'\n",line);
#endif
        printf("ProcessCommandLine() returns %d\n",ProcessCommandLine(line,NULL));
        printf("> ");fflush(stdout);
    }
    return 0;
}
        
#endif
