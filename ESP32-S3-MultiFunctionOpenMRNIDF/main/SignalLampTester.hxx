// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Dec 24 16:13:12 2022
//  Last Modified : <221226.0918>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2022  Robert Heller D/B/A Deepwoods Software
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

#ifndef __SIGNALLAMPTESTER_HXX
#define __SIGNALLAMPTESTER_HXX

#include "freertos_drivers/arduino/PWM.hxx"
#include "esp_timer.h"
#include <esp_err.h>
#include "utils/logging.h"

#define SecondMicros 1000000

namespace esp32multifunction
{

class SignalLampTester
{
public:
    SignalLampTester()
    {
    }
    template <class Container = std::initializer_list<PWM*> >
          void testLamps(const Container &lamps)
    {
        lamps_.assign(lamps);
        esp_timer_create_args_t timer_opts = {
            .callback = nextLamp,
                  .arg = this,
                  .dispatch_method = ESP_TIMER_TASK,
                  .name = "LampTest",
                  .skip_unhandled_events = false
        };
        ESP_ERROR_CHECK(esp_timer_create(&timer_opts,&handle_));
        current_ = -1;
        ESP_ERROR_CHECK(esp_timer_start_periodic(handle_,SecondMicros));
    }
private:
    static void nextLamp(void* arg)
    {
        SignalLampTester *t = (SignalLampTester*) arg;
        t->nextLamp_();
    }
    void nextLamp_()
    {
        LOG(INFO,"[SignalLampTester::nextLamp_] current_ is %d",current_);
        if (current_ >= 0 && current_<PCA9685PWM::NUM_CHANNELS)
        {
            lamps_[current_]->set_duty(0);
        }
        current_++;
        if (current_>=PCA9685PWM::NUM_CHANNELS)
        {
            ESP_ERROR_CHECK(esp_timer_stop(handle_));
            ESP_ERROR_CHECK(esp_timer_delete(handle_));
            return;
        } 
        lamps_[current_]->set_duty(2048);
    }
    esp_timer_handle_t handle_;
    int current_;
    vector<PWM*> lamps_;
};
    
}

#endif // __SIGNALLAMPTESTER_HXX

