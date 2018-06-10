// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Jun 10 09:27:34 2018
//  Last Modified : <180610.1022>
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

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Wire.h>
#include <util/delay.h>
#ifdef __AVR
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif
#include "TLC59208F.h"

void TLC59208F::begin(uint8_t addr) {
    if (addr > 7) {
        addr = 7;
    }
    i2caddr = addr;
    Wire.begin();
    
    // set defaults!
    Wire.beginTransmission(TLC59208F_ADDRESS | i2caddr);
#if ARDUINO >= 100
    Wire.write((byte)TLC59208F_MODE1);
    Wire.write((byte)0x00); // No auto incr, Normal mode, no sub addresses, no all call address
#else
    Wire.send((byte)TLC59208F_MODE1);
    Wire.send((byte)0x00); // No auto incr, Normal mode, no sub addresses, no all call address
#endif
    Wire.endTransmission();
    _delay_us(0.5); // Wait for TLC59208F to wake up
    
}


void TLC59208F::begin()
{
    begin(0);
}

void TLC59208F::dimblinkmode(bool mode)
{
    uint8_t current = read_register(TLC59208F_MODE2);
    if (mode) {
        current |= 1 << TLC59208F_MODE2_DMBLNK;
    } else {
        current &= ~(1 << TLC59208F_MODE2_DMBLNK);
    }
    write_register(TLC59208F_MODE2,current);
}

void TLC59208F::set_pwm(uint8_t channel, uint8_t dutycycle)
{
    if (channel > 7) return;
    write_register(TLC59208F_PWM0+channel,dutycycle);
}

void TLC59208F::group_dutycycle(uint8_t dutycycle)
{
    write_register(TLC59208F_GRPPWM,dutycycle);
}

void TLC59208F::group_freq(uint8_t groupfreq)
{
    write_register(TLC59208F_GRPFREQ,groupfreq);
}

void TLC59208F::led_driver_state_control(uint8_t channel,bool on,bool off,
                                         bool indivual,bool group)
{
    uint8_t addr, shift, current, bits;
    if (channel > 7) return;
    if (channel > 3) {
        addr = TLC59208F_LEDOUT1;
        channel -= 4;
    } else {
        addr = TLC59208F_LEDOUT0;
    }
    switch (channel) {
    case 0: shift = TLC59208F_LEDOUTn_LDR0; break;
    case 1: shift = TLC59208F_LEDOUTn_LDR1; break;
    case 2: shift = TLC59208F_LEDOUTn_LDR2; break;
    case 3: shift = TLC59208F_LEDOUTn_LDR3; break;
    };
    current = read_register(addr);
    current &= ~(TLC59208F_LEDOUTn_LDRm_M << shift);
    if (on) {
        bits = 0;
    } else if (off) {
        bits = 1;
    } else if (indivual) {
        bits = 2;
    } else if (group) {
        bits = 3;
    }
    current |= (bits << shift);
    write_register(addr,current);
}
    
uint8_t TLC59208F::read_register(uint8_t addr)
{
    Wire.beginTransmission(TLC59208F_ADDRESS|i2caddr);
#if ARDUINO >= 100
    Wire.write((byte)addr);
#else
    Wire.send(addr);
#endif
    Wire.endTransmission();
    Wire.requestFrom(TLC59208F_ADDRESS | i2caddr, 1);
    
#if ARDUINO >= 100
    return Wire.read();
#else
    return Wire.receive();
#endif
}

void TLC59208F::write_register(uint8_t addr, uint8_t data)
{
    Wire.beginTransmission(TLC59208F_ADDRESS | i2caddr);
#if ARDUINO >= 100
    Wire.write((byte)addr);
    Wire.write((byte)data);
#else
    Wire.send(addr);      
    Wire.send(data);
#endif
    Wire.endTransmission();
}

