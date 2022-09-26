// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Jun 21 22:27:50 2021
//  Last Modified : <220926.1636>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2021  Robert Heller D/B/A Deepwoods Software
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

#ifndef __LED_H
#define __LED_H

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"
#include <stdio.h>

#include "Blink.hxx"

static const char LEDPhaseMap[] =
    "<relation><property>0</property><value>Steady Highside</value></relation>"
    "<relation><property>1</property><value>Steady Lowside</value></relation>"

    "<relation><property>2</property><value>Pulse Highside</value></relation>"
    "<relation><property>3</property><value>Pulse Lowside</value></relation>"

    "<relation><property>4</property><value>A - Slow</value></relation>"
    "<relation><property>5</property><value>A - Medium</value></relation>"
    "<relation><property>6</property><value>A - Fast</value></relation>"

    "<relation><property>7</property><value>B - Slow</value></relation>"
    "<relation><property>8</property><value>B - Medium</value></relation>"
    "<relation><property>9</property><value>B - Fast</value></relation>";

/// CDI Configuration for a @ref LED
CDI_GROUP(LEDConfig);
CDI_GROUP_ENTRY(phase, openlcb::Uint8ConfigEntry,
                Name("LED Steady, Pulse, or Blink Phase (A-B)"),
                MapValues(LEDPhaseMap),Default(0));
CDI_GROUP_ENTRY(pulsewidth, openlcb::Uint8ConfigEntry,
                Name("Pulse width in seconds, 1 to 127"),
                Min(1), Max(127), Default(1));
/// This event will be consumed to turn the output on.
CDI_GROUP_ENTRY(
    event_on, openlcb::EventConfigEntry, //
    Name("LED on"),
    Description("This event will be consumed to turn the output on."));
/// This event will be consumed to turn the output off.
CDI_GROUP_ENTRY(
    event_off, openlcb::EventConfigEntry, //
    Name("LED off"),
    Description("This event will be consumed to turn the output off."));
CDI_GROUP_END();



class LED 
      : public ConfigUpdateListener
, public Blinking
{
public:
    using Impl = openlcb::MemoryBit<uint8_t>;
    
    enum LEDPhase {SteadyHighside, SteadyLowside, PulseHighside, 
              PulseLowside, A_Slow, A_Medium, A_Fast, B_Slow, B_Medium,
              B_Fast};

    LED(openlcb::Node *node, const LEDConfig &cfg, const Gpio *gpio)
                : impl_(node, 0, 0, &state_, 1)
          , consumer_(&impl_)
          , cfg_(cfg)
          , gpio_(gpio)
          , phase_(SteadyHighside)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
        BlinkTimer::instance()->AddMe(this);
    }
    template <class HW>
    LED(openlcb::Node *node, const LEDConfig &cfg, const HW &, const Gpio* g = HW::instance())
                : impl_(node, 0, 0, &state_, 1)
          , consumer_(&impl_)
          , cfg_(cfg)
          , gpio_(g)
          , phase_(SteadyHighside)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
        BlinkTimer::instance()->AddMe(this);
    }
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE
    {
        AutoNotify n(done);
        openlcb::EventId cfg_event_on = cfg_.event_on().read(fd);
        openlcb::EventId cfg_event_off = cfg_.event_off().read(fd);
        phase_ = (LEDPhase) cfg_.phase().read(fd);
        if (initial_load) {
            if (phase_ == SteadyLowside || phase_ == PulseLowside) {
                gpio_->set();
            } else {
                gpio_->clr();
            }
        }
        pulseWidth_ = cfg_.pulsewidth().read(fd)  << 1;
        if (cfg_event_off != impl_.event_on() ||
            cfg_event_on != impl_.event_on())
        {
            auto saved_node = impl_.node();
            // Need to reinitialize the consumer. We do this with in-place
            // destruction and construction.
            consumer_.~BitEventConsumer();
            impl_.Impl::~Impl();
            new (&impl_)
                Impl(saved_node, cfg_event_on, cfg_event_off, &state_, 1);
            new (&consumer_) openlcb::BitEventConsumer(&impl_);
            return REINIT_NEEDED; // Causes events identify.
        }
        return UPDATED;
    }
    void factory_reset(int fd) OVERRIDE
    {
        CDI_FACTORY_RESET(cfg_.phase);
        CDI_FACTORY_RESET(cfg_.pulsewidth);
    }
    const LEDPhase Phase() const {return phase_;}
    const uint8_t PulseWidth() const {return pulseWidth_;}
    virtual void blink(bool AFast, bool AMedium, bool ASlow) override
    {
        if (gpio_ == nullptr) return;
        if (!IsOn()) {
            if (phase_ == SteadyLowside || phase_ == PulseLowside) {
                gpio_->set();
            } else {
                gpio_->clr();
            }
            return;
        }
        switch (phase_) {
        case SteadyHighside:
            gpio_->set();
            break;
        case SteadyLowside:
            gpio_->clr();
            break;
        case PulseHighside:
            if (pulseCount_ > 0)
            {
                gpio_->set();
                if (AFast) pulseCount_--;
            } else {
                gpio_->clr();
                state_ = 0;
            }
            break;
        case PulseLowside:
            if (pulseCount_ > 0)
            {
                gpio_->clr();
                if (AFast) pulseCount_--;
            } else {
                gpio_->set();
                state_ = 0;
            }
            break;
        case A_Slow: 
            if (ASlow) 
            {
                gpio_->set();
            } else {
                gpio_->clr();
            }
            break;
        case A_Medium: 
            if (AMedium)
            {
                gpio_->set();
            } else {
                gpio_->clr();
            }
            break;
        case A_Fast: 
            if (AFast)
            {
                gpio_->set();
            } else {
                gpio_->clr();
            }
            break;
        case B_Slow: 
            if (ASlow)
            {
                gpio_->clr();
            } else {
                gpio_->set();
            }
            break;
        case B_Medium: 
            if (AMedium)
            {
                gpio_->clr();
            } else {
                gpio_->set();
            }
            break;
        case B_Fast: 
            if (AFast)
            {
                gpio_->clr();
            } else {
                gpio_->set();
            }
            break;
        }
    }
    bool IsOn() const 
    {
        return ((state_ & 1) != 0);
    }
private:
    Impl impl_;
    openlcb::BitEventConsumer consumer_;
    const LEDConfig cfg_;
    const Gpio* gpio_;
    LEDPhase phase_;
    uint8_t state_{0};
    uint8_t pulseWidth_;
    uint8_t pulseCount_;
};

    
    

#endif // __LED_H

