// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Tue Feb 26 21:02:01 2019
//  Last Modified : <221129.0822>
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

#ifndef __BLINK_HXX
#define __BLINK_HXX

#include <vector>
//#include "os/os.h"
//#include "nmranet_config.h"

#include "openlcb/SimpleStack.hxx"
#include "executor/Timer.hxx"
#include "utils/Singleton.hxx"
#include "utils/logging.h"

class Blinking
{
public:
    virtual void blink(bool AFast, bool AMedium, bool ASlow) = 0;
};

class BlinkTimer : public Timer, public Singleton<BlinkTimer> {
public:
    BlinkTimer(ActiveTimers *timers) 
                : Timer(timers)
                , count_(0)
    {
        //LOG(ALWAYS, "*** BlinkTimer::BlinkTimer()");
    }
    long long timeout() override
    {
        bool af = (count_ & 0x01) == 0;
        bool am = (count_ & 0x03) == 0;
        bool as = (count_ & 0x07) == 0;
        if (as) LOG(ALWAYS, "*** BlinkTimer::timeout(): count_  = %d",count_);
        count_++;
        count_ &= 0x07;
        for (blinkers_type::iterator m = blinkers_.begin();
             m != blinkers_.end();
             m++) {
            (*m)->blink(af,am,as);
        }
        return RESTART;
    }
    void AddMe(Blinking * blinker) {
        //LOG(ALWAYS, "*** BlinkTimer::AddMe(%p)",blinker);
        blinkers_.push_back(blinker);
    }
private:
    int count_;
    typedef vector<Blinking *> blinkers_type;
    blinkers_type blinkers_;
};

#endif // __BLINK_HXX

