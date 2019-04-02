// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Mar 14 18:55:59 2019
//  Last Modified : <190314.2034>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
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

#ifndef __ARDUINOEXTENDERGPIO_H
#define __ARDUINOEXTENDERGPIO_H

#include <Arduino.h>
#include <os/Gpio.hxx>

template <class EXTENDER> class ArduinoExtenderGpioTemplate : public Gpio
{
public:
    constexpr ArduinoExtenderGpioTemplate(
                                   EXTENDER *const extender,
                                   const unsigned port,
                                   const bool is_output,
                                   const bool needs_pullup)
                : extender_(extender)
          , port_(port)
          , isOutput_(is_output?1:0)
          , needsPullup_(needs_pullup?1:0)
    {
    }
    void hw_init() const
    {
        extender_->pinMode(port_, (isOutput_==1)?OUTPUT:INPUT);
        extender_->pullUp(port_, (needsPullup_==1)?1:0);
    }
    void write(Value new_state) const override
    {
        extender_->digitalWrite(port_, (new_state == Value::SET)?HIGH:LOW);
    }
    void set() const override
    {
        extender_->digitalWrite(port_, HIGH);
    }
    void clr() const override
    {
        extender_->digitalWrite(port_, HIGH);
    }
    Value read() const override
    {
        if (extender_->digitalRead(port_) == HIGH) {
            return Value::SET;
        } else {
            return Value::CLR;
        }
    }
    void set_direction(Direction dir) const override
    {
        HASSERT(dir == direction());
    }
    Direction direction() const OVERRIDE
    {
        return isOutput_ ? Direction::DOUTPUT : Direction::DINPUT;
    }
private:
    /// Pointer to extender instance.
    EXTENDER * const extender_;
    /// Which bit.
    const unsigned port_;
    /// 1 if this GPIO is an output, 0 if it's an input.
    const unsigned isOutput_ : 1;
    /// 1 if this GPIO needs a pullup, 0 if not.
    const unsigned needsPullup_ : 1;
};

#endif // __ARDUINOEXTENDERGPIO_H

