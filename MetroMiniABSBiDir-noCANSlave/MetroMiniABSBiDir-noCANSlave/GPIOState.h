// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Tue Jun 12 22:45:54 2018
//  Last Modified : <180613.0640>
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

#ifndef __GPIOSTATE_H
#define __GPIOSTATE_H

#include <Arduino.h>

class GPIOState {
protected:
    long timeOfLastChange;    // time at whihc the button last changed
    int lastButtonPeriod;     // last period processed for button
    bool lastState;           // state of button in last scan
    bool newState;            // state of button this scan
    virtual void init(uint8_t s); // init
public:
    uint16_t sense;            // active sense of button/LED 
    long duration;
    long lastDuration;        // how long it was in the previous state, in msec
    uint8_t pin;              // connected pin number
    bool state;               // current button state

    GPIOState(uint8_t pin, uint8_t sense);   // associate pin and sense for active state
    GPIOState(uint8_t pin);                 // associate pin, default sense=HIGH
    GPIOState();
    void setPinSense(uint8_t p, uint8_t s);
    virtual void process() = 0;
};

class GPIOStateIn : public GPIOState {
public:
    GPIOStateIn(uint8_t pin, uint8_t s) : GPIOState(pin,s) {}
    GPIOStateIn(uint8_t pin) : GPIOState(pin) {}
    virtual void process();
protected:
};

class GPIOStatePWMOut : public GPIOState {
private:
    uint8_t brightness;
public:
    GPIOStatePWMOut(uint8_t pin) : GPIOState(pin) {brightness = 0;}
    virtual void process();
    void setPWM(uint8_t b) {brightness = b;}
    uint8_t getPWM() const {return brightness;}
protected:
    virtual void init(uint8_t s) {}
};

    
#endif // __GPIOSTATE_H

