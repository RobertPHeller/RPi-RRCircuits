// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri Jan 31 09:31:13 2020
//  Last Modified : <200131.1418>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2020  Robert Heller D/B/A Deepwoods Software
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

#ifndef __FABOPWM_PCA9685_PWMPIN_H
#define __FABOPWM_PCA9685_PWMPIN_H

#include <Arduino.h>
#include "PWM.h"

template <class EXTENDER> class FaBoPWM_PCA9685_PWMChannel : public PWM
{
public:
    FaBoPWM_PCA9685_PWMChannel(
                          EXTENDER *const extender,
                          const unsigned channel)
                : PWM()
          , extender_(extender)
          , channel_(channel)
    {
        HASSERT(channel < 16);
    }
    void hw_init()
    {
        set_period(4096);
        set_duty(0);
    }
    void set_period(uint32_t counts)
    {
        HASSERT(counts == 4096);
    }
    uint32_t get_period()
    {
        return 4096;
    }
    void set_duty(uint32_t counts)
    {
        extender_->set_channel_value(channel_,counts);
    }
    uint32_t get_duty()
    {
        return extender_->get_channel_value(channel_);
    }
    uint32_t get_period_max()
    {
        return 4096;
    }
    uint32_t get_period_min()
    {
        return 4096;
    }
private:
    EXTENDER *const extender_;
    const unsigned channel_;
};

        
            

#endif // __FABOPWM_PCA9685_PWMPIN_H

