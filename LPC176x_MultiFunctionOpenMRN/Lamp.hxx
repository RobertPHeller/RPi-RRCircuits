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
//  Last Modified : <221004.1807>
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
#include "freertos_drivers/common/PWM.hxx"
#include "utils/Uninitialized.hxx"
#include <stdio.h>
#include "HardwareDefs.hxx"

#include "Blink.hxx"

#define LAMPCOUNT 4

static const char LampSelectMap[] = 
    "<relation><property>0</property><value>Unused</value></relation>"

    "<relation><property>1</property><value>A0</value></relation>"
    "<relation><property>2</property><value>A1</value></relation>"
    "<relation><property>3</property><value>A2</value></relation>"
    "<relation><property>4</property><value>A3</value></relation>"
    "<relation><property>5</property><value>A4</value></relation>"
    "<relation><property>6</property><value>A5</value></relation>"
    "<relation><property>7</property><value>A6</value></relation>"
    "<relation><property>8</property><value>A7</value></relation>"

    "<relation><property>9</property><value>B0</value></relation>"
    "<relation><property>10</property><value>B1</value></relation>"
    "<relation><property>11</property><value>B2</value></relation>"
    "<relation><property>12</property><value>B3</value></relation>"
    "<relation><property>13</property><value>B4</value></relation>"
    "<relation><property>14</property><value>B5</value></relation>"
    "<relation><property>15</property><value>B6</value></relation>"
    "<relation><property>16</property><value>B7</value></relation>"
#if NUM_PWMCHIPS == 2
    "<relation><property>17</property><value>C0</value></relation>"
    "<relation><property>18</property><value>C1</value></relation>"
    "<relation><property>19</property><value>C2</value></relation>"
    "<relation><property>20</property><value>C3</value></relation>"
    "<relation><property>21</property><value>C4</value></relation>"
    "<relation><property>22</property><value>C5</value></relation>"
    "<relation><property>23</property><value>C6</value></relation>"
    "<relation><property>24</property><value>C7</value></relation>"

    "<relation><property>25</property><value>D0</value></relation>"
    "<relation><property>26</property><value>D1</value></relation>"
    "<relation><property>27</property><value>D2</value></relation>"
    "<relation><property>28</property><value>D3</value></relation>"
    "<relation><property>29</property><value>D4</value></relation>"
    "<relation><property>30</property><value>D5</value></relation>"
    "<relation><property>31</property><value>D6</value></relation>"
    "<relation><property>32</property><value>D7</value></relation>"
#endif
;

static const char LampPhaseMap[] = 
    "<relation><property>0</property><value>Steady</value></relation>"

    "<relation><property>1</property><value>A - Slow</value></relation>"
    "<relation><property>2</property><value>A - Medium</value></relation>"
    "<relation><property>3</property><value>A - Fast</value></relation>"

    "<relation><property>4</property><value>None (not used)</value></relation>"

    "<relation><property>5</property><value>B - Slow</value></relation>"
    "<relation><property>6</property><value>B - Medium</value></relation>"
    "<relation><property>7</property><value>B - Fast</value></relation>";


/// CDI Configuration for a @ref Lamp
CDI_GROUP(LampConfig);
/// Specifies the lamp selection for this lamp
CDI_GROUP_ENTRY(selection, openlcb::Uint8ConfigEntry, 
                Name("Lamp Selection"), MapValues(LampSelectMap),
                Default(0));
CDI_GROUP_ENTRY(phase, openlcb::Uint8ConfigEntry,
                Name("Lamp Phase (A-B) - Flash Rate"),
                MapValues(LampPhaseMap), Default(0));
CDI_GROUP_ENTRY(brightness, openlcb::Uint16ConfigEntry,
                Name("Lamp brightness, hundreths of a percent (0 to 10000)"),
                Min(0), Max(10000), Default(5000));
CDI_GROUP_ENTRY(period, openlcb::Uint32ConfigEntry,
                Name("PWM Period, in nanoseconds"),
                Default(1000000));
CDI_GROUP_END();


using LampGroup = openlcb::RepeatedGroup<LampConfig, LAMPCOUNT>;

#define BRIGHNESSHUNDRETHSPERCENT(b) ((b)*.0001)

class Lamp : public Blinking {
public:
    enum LampID {Unused, A0_, A1_, A2_, A3_, A4_, A5_, A6_, A7_, 
              B0_, B1_, B2_, B3_, B4_, B5_, B6_, B7_
#if NUM_PWMCHIPS == 2
              , C0_, C1_, C2_, C3_, C4_, C5_, C6_, C7_, 
              D0_, D1_, D2_, D3_, D4_, D5_, D6_, D7_
#endif
              , MAX_LAMPID
};
    enum LampPhase {Steady,A_Slow,A_Medium,A_Fast,None,B_Slow,
              B_Medium,B_Fast};
    Lamp() 
    {
        lampid_ = Unused;
        phase_  = Steady;
        isOn_ = 0;
        hasChanged_ = 0;
        // Default: 50%
        brightness_ = 5000;
        // 1Khz
        period_ = 1000000;
        BlinkTimer::instance()->AddMe(this);
    }
    void UpdateConfig(LampID lampid, LampPhase phase, uint16_t brightness, 
                      uint32_t period)
    {
        lampid_ = lampid;
        phase_ = phase;
        brightness_ = brightness;
        period_ = period;
    }
    PWM* Pin() const       {return pinlookup_[(int)lampid_];}
    const LampPhase Phase() const {return phase_;}
    static PWM* PinLookup(LampID id) {
        return pinlookup_[(int)id];
    }
    void On() {isOn_ = 1;hasChanged_ = 1;}
    void Off() {isOn_ = 0;hasChanged_ = 1;}
    virtual void blink(bool AFast, bool AMedium, bool ASlow) override
    {
        if (lampid_ == Unused) return;
        //LOG(ALWAYS, "*** Lamp::blink(): lampid_ = %d, AFast = %d, AMedium = %d, ASlow = %d",
        //    lampid_,AFast,AMedium,ASlow);
        PWM * p = Pin();
        //LOG(ALWAYS, "*** Lamp::blink(): p = %p",p);
        if (p == nullptr) return;
        //LOG(ALWAYS, "*** Lamp::blink(): isOn_ = %d",isOn_);
#if 1
        if (isChanged()) {
            //LOG(ALWAYS, "*** Lamp::blink(): lampid_ = %d, isOn_ = %d, phase_ is %d, p is %p",lampid_,isOn_,phase_,p);
        }
#endif
        if (!isOn() && isChanged()) {
            p->set_duty(0); 
        } else {
            switch (phase_) {
            case Steady: if (isChanged()) p->set_duty((uint32_t)(BRIGHNESSHUNDRETHSPERCENT(brightness_)*p->get_period())); break;
            case A_Slow: p->set_duty(ASlow?(uint32_t)(BRIGHNESSHUNDRETHSPERCENT(brightness_)*p->get_period()):0);break;
            case A_Medium: p->set_duty(AMedium?(uint32_t)(BRIGHNESSHUNDRETHSPERCENT(brightness_)*p->get_period()):0); break;
            case A_Fast: p->set_duty(AFast?(uint32_t)(BRIGHNESSHUNDRETHSPERCENT(brightness_)*p->get_period()):0); break;
            case None: if (isChanged()) p->set_duty(0); break;
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
    bool isOn() const {return isOn_ == 1;}
    bool isChanged() const {return hasChanged_ == 1;}
private:
    static PWM* pinlookup_[MAX_LAMPID];
    LampID lampid_:6;
    LampPhase  phase_:3;
    unsigned isOn_:1;
    unsigned hasChanged_:1;
    uint16_t brightness_;
    uint32_t period_;    
};

#endif // __LAMP_HXX

