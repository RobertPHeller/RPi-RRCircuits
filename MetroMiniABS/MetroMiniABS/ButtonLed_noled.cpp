// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri May 25 20:01:55 2018
//  Last Modified : <180525.2006>
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

#include "ButtonLed_noled.h"

void ButtonLed_noled::init(uint8_t s) {
    sense = s;
    lastState=false;                  // start with led off
    duration=lastDuration=0;          // init durations
    state=false;                      // start with button off
}

//     state: F T T F
// lastState: F T T F
//   returns: F T F F 

void ButtonLed_noled::process() {
  long now = millis();
  int period = now & 0xFFE0;                    // each period is 32 ms
  if(period != lastButtonPeriod) {              // If we are in a new period
    lastButtonPeriod = period;                  // .. remember it
    pinMode(pin, INPUT);                        // .. change the pin to input..
    digitalWrite(pin, HIGH);                    // .. and activate pull up
    newState = (sense == digitalRead(pin));     // .. and read it, is the button up or down
    pinMode(pin, OUTPUT);                       // .. return pin to output mode
    digitalWrite(pin, ledState);                // .. and make sure its showing its state
    if(newState != lastState) {                 // ..if button changed then..
      lastState = newState;                     // ....remember the button state
    } else {                                    // ..else its position is stable and so debounced
      if(state != newState) {                   // ....But is it a new state?..
        state = newState;                       // .....yes, so update
        lastDuration = duration + 32;           //       and remember the duration of the last state
        timeOfLastChange = now;                 //       so we can calc duration
        duration = 0;                           //       start timing new state
      } else {                                  // .....no, the same state is continuing, so
        duration = millis() - timeOfLastChange; // ......calculate its duration
      } 
    }
  }
  return;
}

