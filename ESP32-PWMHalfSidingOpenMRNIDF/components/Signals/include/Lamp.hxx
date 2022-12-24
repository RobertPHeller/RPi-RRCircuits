// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Feb 25 11:37:34 2019
//  Last Modified : <221217.0933>
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

#ifndef __LAMP_HXX
#define __LAMP_HXX

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "freertos_drivers/arduino/PWM.hxx"
#include <stdio.h>
#include "LampConfig.hxx"

#include "Blink.hxx"


#define BRIGHNESSHUNDRETHSPERCENT(b) ((b)*.0001)

class Lamp : public ConfigUpdateListener , public Blinking {
public:
    enum LampID {Unused, A0_, A1_, A2_, A3_, A4_, A5_, A6_, A7_, 
              B0_, B1_, B2_, B3_, B4_, B5_, B6_, B7_};
    enum LampPhase {Steady,A_Slow,A_Medium,A_Fast,None,B_Slow,B_Medium,
              B_Fast};
    Lamp(const LampConfig &cfg) : cfg_(cfg)
    {
        lampid_ = Unused;
        phase_  = Steady;
        isOn_   = false;
        hasChanged_ = false;
        // Default: 50%
        brightness_ = 5000;
        // 1Khz
        period_ = 1000000;
        BlinkTimer::instance()->AddMe(this);
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    void factory_reset(int fd) OVERRIDE
    {
        CDI_FACTORY_RESET(cfg_.selection);
        CDI_FACTORY_RESET(cfg_.phase);
        CDI_FACTORY_RESET(cfg_.brightness);
        CDI_FACTORY_RESET(cfg_.period);
    }
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) override
    {
        AutoNotify n(done);
        lampid_ = (LampID)     cfg_.selection().read(fd);
        phase_  = (LampPhase)  cfg_.phase().read(fd);
        brightness_ = cfg_.brightness().read(fd);
        period_ = cfg_.period().read(fd);
        return UPDATED;
    }
    PWM* Pin() const       {return pinlookup_[(int)lampid_];}
    const LampPhase Phase() const {return phase_;}
    static PWM* PinLookup(LampID id) {
        return pinlookup_[(int)id];
    }
    void On() {isOn_ = true;hasChanged_ = true;}
    void Off() {isOn_ = false;hasChanged_ = true;}
    virtual void blink(bool AFast, bool AMedium, bool ASlow)
    {
        //LOG(ALWAYS, "*** Lamp::blink(%d,%d,%d)",AFast,AMedium,ASlow);
        if (lampid_ == Unused) return;
        //LOG(ALWAYS, "*** Lamp::blink(): lampid_ = %d, AFast = %d, AMedium = %d, ASlow = %d",
        //    lampid_,AFast,AMedium,ASlow);
        PWM * p = Pin();
        //LOG(ALWAYS, "*** Lamp::blink(): p = %p",p);
        if (p == nullptr) return;
        //LOG(ALWAYS, "*** Lamp::blink(): isOn_ = %d",isOn_);
#if 1
        if (hasChanged_) {
            //LOG(ALWAYS, "*** Lamp::blink(): lampid_ = %d, isOn_ = %d, phase_ is %d, p is %p",lampid_,isOn_,phase_,p);
        }
#endif
        if (!isOn_ && hasChanged_) {
            p->set_duty(0); 
        } else {
            switch (phase_) {
            case Steady: if (hasChanged_) p->set_duty((uint32_t)(BRIGHNESSHUNDRETHSPERCENT(brightness_)*p->get_period())); break;
            case A_Slow: p->set_duty(ASlow?(uint32_t)(BRIGHNESSHUNDRETHSPERCENT(brightness_)*p->get_period()):0);break;
            case A_Medium: p->set_duty(AMedium?(uint32_t)(BRIGHNESSHUNDRETHSPERCENT(brightness_)*p->get_period()):0); break;
            case A_Fast: p->set_duty(AFast?(uint32_t)(BRIGHNESSHUNDRETHSPERCENT(brightness_)*p->get_period()):0); break;
            case None: if (hasChanged_) p->set_duty(0); break;
            case B_Slow: p->set_duty(ASlow?0:(uint32_t)(BRIGHNESSHUNDRETHSPERCENT(brightness_)*p->get_period())); break;
            case B_Medium: p->set_duty(AMedium?0:(uint32_t)(BRIGHNESSHUNDRETHSPERCENT(brightness_)*p->get_period())); break;
            case B_Fast: p->set_duty(AFast?0:(uint32_t)(BRIGHNESSHUNDRETHSPERCENT(brightness_)*p->get_period())); break;
            }
        }
        //hasChanged_ = false;
    }
    void Setbrightness(uint16_t brightness) {
        brightness_ = brightness;
    }
    void SetPeriod(uint32_t period) {
        period_ = period;
    }
    static void PinLookupInit(unsigned index,PWM* pin)
    {
        pinlookup_[index] = pin;
    }
private:
    static PWM* pinlookup_[17];
    LampID lampid_;
    LampPhase  phase_;
    const LampConfig cfg_;
    bool isOn_;
    bool hasChanged_;
    uint16_t brightness_;
    uint32_t period_;    
};

#endif // __LAMP_HXX

