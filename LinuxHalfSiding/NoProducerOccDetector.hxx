// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Sep 1 11:57:59 2018
//  Last Modified : <180901.1249>
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

#ifndef __NOPRODUCEROCCDETECTOR_HXX
#define __NOPRODUCEROCCDETECTOR_HXX

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/RefreshLoop.hxx"

#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "utils/Debouncer.hxx"

template <class Debouncer>
class PolledGpio : public openlcb::Polling
{
public:
    PolledGpio(const typename Debouncer::Options &debounce_args,
              const Gpio *thebit)
                : bit(thebit)
          , debouncer_(debounce_args)
    {
        debouncer_.initialize(bit->is_clr());
    }
    ~PolledGpio() {}
    void set_state(bool new_value)
    {
          debouncer_.override(new_value);
    }
    void poll_33hz(openlcb::WriteHelper *helper, Notifiable *done) OVERRIDE
    {
        debouncer_.update_state(bit->is_clr());
        done->notify();
    }
    bool get_state()
    {
        return debouncer_.current_state();
    }
    const Gpio *gpio() {return bit;}
private:
    const Gpio *bit;
    Debouncer debouncer_;
};

/// CDI Configuration for a @ref Occupancy Detector (derived from ConfiguredProducer).
CDI_GROUP(NoProducerOccDetectorConfig);
/// Allows the user to assign a name for this input.
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<15>, //
                Name("Description"), Description("User name of this input."));
/// Configures the debounce parameter.
CDI_GROUP_ENTRY(
    debounce, openlcb::Uint8ConfigEntry, Name("Debounce parameter"),
    Default(3),
    Description("Amount of time to wait for the input to stabilize before "
                "producing the event. Unit is 30 msec of time. Usually a value "
                "of 2-3 works well in a non-noisy environment. In high noise "
                "(train wheels for example) a setting between 8 -- 15 makes "
                "for a slower response time but a more stable "
                "signal.\nFormally, the parameter tells how many times of "
                "tries, each 30 msec apart, the input must have the same value "
                "in order for that value to be accepted and the event "
                "transition produced."),
    Default(3));
CDI_GROUP_END();


class NoProducerOccDetector : public ConfigUpdateListener
{
public:
    using DebouncerClass = PolledGpio<QuiesceDebouncer>;
    
    NoProducerOccDetector(const NoProducerOccDetectorConfig &cfg, const Gpio *gpio)
                : debouncer_(QuiesceDebouncer::Options(3), gpio)
          , cfg_(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }

    template <class HW>
    NoProducerOccDetector(const NoProducerOccDetectorConfig &cfg, 
                          const HW &,
                          const Gpio *gpio = HW::instance())
                : debouncer_(QuiesceDebouncer::Options(3), gpio)
          , cfg_(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) override
    {
        AutoNotify n(done);
        uint8_t debounce_arg = cfg_.debounce().read(fd);
        auto saved_gpio = debouncer_.gpio();
        debouncer_.DebouncerClass::~DebouncerClass();
        new (&debouncer_) DebouncerClass(
                                        QuiesceDebouncer::Options(debounce_arg),
                                        saved_gpio);
        return UPDATED;
    }
    void factory_reset(int fd) OVERRIDE 
    {
        cfg_.description().write(fd, "");
        CDI_FACTORY_RESET(cfg_.debounce);
    }
    openlcb::Polling *polling()
    {
        return &debouncer_;
    }
    bool OccupiedP() 
    {
        return (debouncer_.get_state());
    }
private:
    DebouncerClass debouncer_;
    const NoProducerOccDetectorConfig cfg_;
};


#endif // __NOPRODUCEROCCDETECTOR_HXX

