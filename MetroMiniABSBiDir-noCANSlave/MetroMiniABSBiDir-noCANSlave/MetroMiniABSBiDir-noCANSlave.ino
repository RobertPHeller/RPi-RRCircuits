// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Tue Jun 12 22:08:01 2018
//  Last Modified : <180613.0815>
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

/**
 * MetroMiniABS.ino -- An OpenLCB node that implements a single Automatic Block
 * Signal.  This node presumes the hardware detailed at 
 * https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/MetroMiniABS
 * This board uses an Adafruit Metro Mini, which is a Arduino Uno R3 clone on 
 * a mini form factor: 1.7"x.7", with 28 pins as two rows of 14 .6" with a .1" 
 * pitch (same footprint as a wide DIL-28).
 *
 * I/O pin usage:
 *
 * D0 (Rx)
 * D1 (Tx) ABS Slave Bus
 * D3 (PWM) East Green
 * D4 Next West (Active Low)
 * D5 (PWM) East Yellow
 * D6 (PWM) East Red
 * D7 Next East (Active Low)
 * D8 Current Block (Active Low)
 * D9 (PWM) West Green
 * D10 (PWM) West Yellow
 * D11 (PWM) West Red
 *
 * User onfiguration Options:
 * 
 * Compile Time Constants:
 * 
 * STOP_BRITE     -- Stop (red) brightness (0-255)
 * APPROACH_BRITE -- Approach (yellow) brightness (0-255)
 * CLEAR_BRITE    -- Clear (green) brightness (0-255)
 * YELLOW_HUE     -- Yellow Hue (ratio of red to green): 0-255
 *
 * Dip Switches:
 * 
 * Blockid        -- Block Id (0-63)
 * Common_Anode   -- Common Anode
 * Bipolar_Search -- use BiPolar Searchlight (Red/Green)
 * 
 */

#define STOP_BRITE 255     //  Stop (red) brightness (0-255)
#define APPROACH_BRITE 255 //  Approach (yellow) brightness (0-255)
#define CLEAR_BRITE 255    //  Clear (green) brightness (0-255)
#define YELLOW_HUE 128     //  Yellow Hue (ratio of red to green): 0-255

#include <Arduino.h>

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
//#include <avr/pgmspace.h>

// The following lines are needed because the Arduino environment 
// won't search a library directory unless the library is included 
// from the top level file (this file)
//#include <EEPROM.h>
#include "GPIOState.h"
#include "Mast.h"
#include "ABSSlaveBus.h"

// init for serial communications if used
#define         BAUD_RATE       57600

// Define pins

// Dip Switch:

#define LEDCOMMONMODE  2
#define BIPOLARSEARCH 12
#define BLOCKID0      A0
#define BLOCKID1      A1
#define BLOCKID2      A2
#define BLOCKID3      A3
#define BLOCKID4      A4
#define BLOCKID5      A5

uint8_t Blockid = 0;
uint8_t Common_Anode = 1;
uint8_t Bipolar_Search = 0;

// PWM Signal Leds
#define ESIGNALGREEN   3
#define ESIGNALYELLOW  5
#define ESIGNALRED     6
#define WSIGNALGREEN   9
#define WSIGNALYELLOW 10
#define WSIGNALRED    11

// Block Occupancy
#define BLOCKOCC       8 // Current block
#define NEXTEAST       7 // Next block east (via daisy chain)
#define NEXTWEST       4 // Next block west (via daisy chain)

// GPIO Pin Wrappers

// Block Occupancy
GPIOStateIn blockOcc(BLOCKOCC,LOW);
GPIOStateIn nextEast(NEXTEAST,LOW);
GPIOStateIn nextWest(NEXTWEST,LOW);

// Signal LEDS
GPIOStatePWMOut eastGreen(ESIGNALGREEN);
GPIOStatePWMOut eastYellow(ESIGNALYELLOW);
GPIOStatePWMOut eastRed(ESIGNALRED);
GPIOStatePWMOut westGreen(WSIGNALGREEN);
GPIOStatePWMOut westYellow(WSIGNALYELLOW);
GPIOStatePWMOut westRed(WSIGNALRED);

Mast mastE(&blockOcc,&nextEast,&eastGreen,&eastYellow,&eastRed,
           STOP_BRITE,APPROACH_BRITE,CLEAR_BRITE,
           YELLOW_HUE);
Mast mastW(&blockOcc,&nextWest,&westGreen,&westYellow,&westRed,
           STOP_BRITE,APPROACH_BRITE,CLEAR_BRITE,
           YELLOW_HUE);

ABSSlaveBus slaveBus(&blockOcc,
                     &eastGreen,&eastYellow,&eastRed,
                     &westGreen,&westYellow,&westRed);

/**
 * Setup does initial configuration
 */
void setup()
{
    Serial.begin(BAUD_RATE);
    // Read the DIP Switches
    pinMode(LEDCOMMONMODE,INPUT_PULLUP);
    pinMode(BIPOLARSEARCH,INPUT_PULLUP);
    pinMode(BLOCKID0,INPUT_PULLUP);
    pinMode(BLOCKID1,INPUT_PULLUP);
    pinMode(BLOCKID2,INPUT_PULLUP);
    pinMode(BLOCKID3,INPUT_PULLUP);
    pinMode(BLOCKID4,INPUT_PULLUP);
    pinMode(BLOCKID5,INPUT_PULLUP);
    Common_Anode = digitalRead(LEDCOMMONMODE);
    Bipolar_Search = digitalRead(BIPOLARSEARCH);
    Blockid = digitalRead(BLOCKID0) + 
          (digitalRead(BLOCKID1) << 1) +
          (digitalRead(BLOCKID2) << 2) +
          (digitalRead(BLOCKID3) << 3) +
          (digitalRead(BLOCKID4) << 4) +
          (digitalRead(BLOCKID5) << 5);
    mastE.init(Common_Anode,Bipolar_Search);
    mastW.init(Common_Anode,Bipolar_Search);
    slaveBus.init(Blockid);
          
    
    blockOcc.process();
    nextEast.process();
    nextWest.process();
    mastE.eval();
    mastW.eval();
}

/**
 * Loop runs the operation
 */
void loop()
{
    blockOcc.process();
    nextEast.process();
    nextWest.process();
    mastE.eval();
    mastW.eval();
    slaveBus.process();
}

