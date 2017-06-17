; -!- gpasm -!- ###########################################################
;
;  System        : 
;  Module        : 
;  Object Name   : $RCSfile$
;  Revision      : $Revision$
;  Date          : $Date$
;  Author        : $Author$
;  Created By    : Robert Heller
;  Created       : Sat Jun 17 13:57:14 2017
;  Last Modified : <170617.1638>
;
;  Description	
;
;  Notes
;
;  History
;	
;############################################################################
; 
;    Copyright (C) 2017  Robert Heller D/B/A Deepwoods Software
;			51 Locke Hill Road
;			Wendell, MA 01379-9728
;
;    This program is free software; you can redistribute it and/or modify
;    it under the terms of the GNU General Public License as published by
;    the Free Software Foundation; either version 2 of the License, or
;    (at your option) any later version.
;
;    This program is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with this program; if not, write to the Free Software
;    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
;
; 
;############################################################################

        LIST
        NOLIST

DO      EQU RA0
DI      EQU RA1
SCLK    EQU RA2
CE      EQU RA3

SPI     EQU PORTA
LAMPS   EQU PORTB
SIGNALS EQU PORTC

L0      EQU RB0
L1      EQU RB1
L2      EQU RB2
L3      EQU RB3
L4      EQU RB4
L5      EQU RB5
L6      EQU RB6
L7      EQU RB7

S0      EQU RC0
S1      EQU RC1
S2      EQU RC2
S3      EQU RC3
S4      EQU RC4
S5      EQU RC5
S6      EQU RC6
S7      EQU RC7

SPIB0   EQU H'0A'
SPIB1   EQU H'0B'
SPISTATE EQU H'0C'
SPICMD  EQU H'0D'
SPICMDM EQU B'11100000'
SPISIGM EQU B'00000111'
SPIASPM EQU B'00001111'
SPISIGNO EQU H'0E'

SIGBUF  EQU H'0F'
SIGINDX EQU H'18'
SIGBITS EQU H'19'
ASPNO   EQU H'1A'
ASPBITS EQU H'1B'

ASPECTS EQU H'30'
