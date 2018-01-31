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
 *  Last Modified : <180131.1509>
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
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <DCCPacket.h>
#include <DCCPacketQueue.h>
#include <DCCPacketScheduler.h>

static void     yyerror (const char *s);
static int      yylex (void);
static char    *currentPos;
#define YYPARSE_PARAM dcc
int ProcessCommandLine (char *line,DCCPacketScheduler *dps);
%}


%start COMMAND

%union {
    int ival;
}

%token EMERGENCY STOP ALL SET SPEED FUNCTIONS ACCESSORY UNSET
%token BADSYMBOL
%token <ival> INTEGER

%%

COMMAND : EMERGENCY STOP ALL '\n' {
            DCCPacketScheduler *dps = (DCCPacketScheduler *) dcc;
            dps->eStop();}
        | EMERGENCY STOP INTEGER '\n' {
            DCCPacketScheduler *dps = (DCCPacketScheduler *) dcc;
            uint16_t addr = $3;
            if (addr < 100) {
                dps->eStop(addr,DCC_SHORT_ADDRESS);
            } else {
                dps->eStop(addr,DCC_LONG_ADDRESS);
            } }
        | SET SPEED INTEGER INTEGER INTEGER '\n' {
            DCCPacketScheduler *dps = (DCCPacketScheduler *) dcc;
            uint16_t addr = $3;
            if (addr < 100) {
                dps->setSpeed(addr,DCC_SHORT_ADDRESS,$4,$5);
            } else {
                dps->setSpeed(addr,DCC_LONG_ADDRESS,$4,$5);
            }}
        | SET FUNCTIONS INTEGER INTEGER '\n' {
            DCCPacketScheduler *dps = (DCCPacketScheduler *) dcc;
            uint16_t addr = $3;
            if (addr < 100) {
                dps->setFunctions(addr,(uint8_t)DCC_SHORT_ADDRESS,(uint16_t)$4);
            } else {
                dps->setFunctions(addr,(uint8_t)DCC_LONG_ADDRESS,(uint16_t)$4);
            } }
        | SET ACCESSORY INTEGER INTEGER '\n' {
            DCCPacketScheduler *dps = (DCCPacketScheduler *) dcc;
            dps->setBasicAccessory($3,$4);}
        | UNSET ACCESSORY INTEGER INTEGER '\n' {
            DCCPacketScheduler *dps = (DCCPacketScheduler *) dcc;
            dps->unsetBasicAccessory($3,$4);}
        ;
        
%%


static const char rcsid[] = "@(#) : $Id$";


int yylex()
{
/*
 * Array of all known reserved words and the token IDs.
 */
                  
static const struct {
    const char *string;
    int   token;
} ReservedWords[] = {
    {"ACCESSORY", ACCESSORY },
    {"ALL", ALL },
    {"EMERGENCY", EMERGENCY },
    {"FUNCTIONS", FUNCTIONS },
    {"SET", SET },
    {"SPEED", SPEED },
    {"STOP", STOP },
    {"UNSET", UNSET } };
#define NumberOfReservedWords (sizeof(ReservedWords) / sizeof(ReservedWords[0]))
    char     word[4096];

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
        size_t i;
        char *s = word;
        while (isalnum(*currentPos)) *s++ = *currentPos++;
        /*yylloc.last_column = currentPos - yylloc.text;*/
        *s = '\0';
#if YYDEBUG
        fprintf(stderr,"*** yylex(): word = '%s'\n",word);
#endif
        /* Lookup word in table, returning corresponding token. */
        for (i = 0; i < NumberOfReservedWords; i++) {
            
            if (strcasecmp(word,ReservedWords[i].string) == 0) {
                return ReservedWords[i].token;
            }
        }
        return (BADSYMBOL);    
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

void yyerror(const char *message) {
    fprintf(stderr,"%s\n",message);
}
    
        
int ProcessCommandLine (char *line,DCCPacketScheduler *dps) {
    currentPos = line;
    return yyparse((void *)dps);
}

#ifdef TESTMAIN

int main(int argc, char *argv[]) 
{
    char line[4096];
    
    printf("> ");fflush(stdout);
    while (fgets(line,4096,stdin) != NULL) {
#if YYDEBUG
        fprintf(stderr,"*** Line is '%s'\n",line);
#endif
        printf("ProcessCommandLine() returns %d\n",ProcessCommandLine(line,NULL));
        printf("> ");fflush(stdout);
    }
    return 0;
}
        
#endif
