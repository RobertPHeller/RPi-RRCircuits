// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Aug 29 13:18:38 2018
//  Last Modified : <180829.1812>
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

#ifndef __CONFIGUREDPOINTSENSE_HXX
#define __CONFIGUREDPOINTSENSE_HXX

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"

#include "openlcb/EventHandler.hxx"
#include "openlcb/WriteHelper.hxx"
#include "os/Gpio.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
//#include "utils/Debouncer.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "ThreeStateDebouncer.hxx"

CDI_GROUP(PointSenseConfig);
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<15>, //
                Name("Description"),
                Description("User name of this set of points."));
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
CDI_GROUP_ENTRY(
    normal_event, openlcb::EventConfigEntry, //
    Name("Normal Event"),
    Description("This event will be produced when the points are aligned at normal."));
CDI_GROUP_ENTRY(
    reverse_event, openlcb::EventConfigEntry, //
    Name("Reverse Event"),
    Description("This event will be produced when the points are aligned at reverse."));
CDI_GROUP_END();

class PolledPointSense : public openlcb::SimpleEventHandler, public openlcb::Polling
{
public:
    PolledPointSense(openlcb::Node *node,
                     const ThreeStateDebouncer::Options &debounce_args,
                     openlcb::EventId normal_event, 
                     openlcb::EventId reverse_event, 
                     const Gpio *normal, 
                     const Gpio *reverse,
                     signed char initstate = -1)
                : node_(node)
          , normal_(normal)
          , reverse_(reverse)
          , normal_event_(normal_event)
          , reverse_event_(reverse_event)
          , debouncer_(debounce_args)
    {
        register_handler();
        debouncer_.initialize(initstate);
    }
    
    template <class NORMAL, class REVERSE>
          PolledPointSense(openlcb::Node *node, 
                           const ThreeStateDebouncer::Options &debounce_args,
                           openlcb::EventId normal_event, openlcb::EventId reverse_event, 
                           const NORMAL&, const REVERSE&,
                           signed char initstate = -1,
                           const Gpio *normal = NORMAL::instance(), 
                           const Gpio *reverse = REVERSE::instance())
                : PolledPointSense(node, debounce_args, normal_event, reverse_event, initstate, normal, reverse)
    {
    }
    
    ~PolledPointSense()
    {
        unregister_handler();
    }
    
    openlcb::EventState get_current_state()
    {
        switch (debouncer_.current_state())
        {
        case 0: return openlcb::EventState::VALID;
        case 1: return openlcb::EventState::INVALID;
        default: return openlcb::EventState::UNKNOWN;
        }
    }
    void set_state(signed char new_value)
    {
        debouncer_.override(new_value);
    }
    void poll_33hz(openlcb::WriteHelper *helper, Notifiable *done) OVERRIDE
    {
        signed char new_state = -1;
        if (normal_->is_clr() && reverse_->is_set()) {
            new_state = 0;
        } else if (reverse_->is_clr() && normal_->is_set()) {
            new_state = 1;
        }
        if (debouncer_.update_state(new_state)) {
            SendEventReport(helper, done);
        } else {
            done->notify();
        }
    }
    openlcb::Node *node()
    {
        return node_;
    }
    void handle_identify_producer(const openlcb::EventRegistryEntry &entry,
                                  openlcb::EventReport *event,
                                  BarrierNotifiable *done) override;
    void handle_identify_global(const openlcb::EventRegistryEntry &entry,
                                openlcb::EventReport *event,
                                BarrierNotifiable *done) override;
    /// Requests the event associated with the current value of the bit to be
    /// produced (unconditionally): sends an event report packet ot the bus.
    ///
    /// @param writer is the output flow to be used.
    ///
    /// @param done is the notification callback. If it is NULL, the writer will
    /// be invoked inline and potentially block the calling thread.
    void SendEventReport(openlcb::WriteHelper *writer, Notifiable *done);
    /// Registers this event handler with the global event manager. Call this
    /// from the constructor of the derived class.
    void register_handler();
    /// Removes this event handler from the global event manager. Call this
    /// from the destructor of the derived class.
    void unregister_handler();
    uint64_t normal_event()
    {
        return normal_event_;
    }
    uint64_t reverse_event()
    {
        return reverse_event_;
    }
    const Gpio *normal()
    {
        return normal_;
    }
    const Gpio *reverse()
    {
        return reverse_;
    }
private:
    openlcb::Node *node_;
    const Gpio *normal_;
    const Gpio *reverse_;
    uint64_t normal_event_;
    uint64_t reverse_event_;
    ThreeStateDebouncer debouncer_;
    void SendProducerIdentified(EventReport *event, BarrierNotifiable *done);
    void SendAllProducersIdentified(BarrierNotifiable *done);
};

class PointSense : public ConfigUpdateListener
{
public:
    PointSense(openlcb::Node *node, const PointSenseConfig &cfg, 
               const Gpio *normal, const Gpio *reverse)
                : producer_(node, ThreeStateDebouncer::Options(3), 
                            0, 0, normal, reverse)
          , cfg_(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE;
    void factory_reset(int fd) OVERRIDE
    {
        cfg_.description().write(fd, "");
        CDI_FACTORY_RESET(cfg_.debounce);
    }
    openlcb::Polling * polling() {return &producer_;}
    openlcb::EventState get_current_state()
    {
        return producer_.get_current_state();
    }
private:
    PolledPointSense producer_;
    const PointSenseConfig cfg_;
};


#endif // __CONFIGUREDPOINTSENSE_HXX

