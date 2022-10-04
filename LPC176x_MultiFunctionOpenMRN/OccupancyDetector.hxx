// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Feb 24 14:51:54 2019
//  Last Modified : <221004.1026>
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

#ifndef __OCCUPANCYDETECTOR_HXX
#define __OCCUPANCYDETECTOR_HXX

#include "openlcb/PolledProducer.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "utils/Debouncer.hxx"

#include <vector>

/// CDI Configuration for a @ref ConfiguredProducer.
CDI_GROUP(OccupancyDetectorConfig);
/// Allows the user to assign a name for this input.
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<15>, //
                Name("Description"), Description("User name of this block."));
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
/// This event will be produced when the input goes to HIGH.
CDI_GROUP_ENTRY(
    event_occupied, openlcb::EventConfigEntry, //
    Name("Block Occupied"),
    Description("This event will be produced when the block becomes occupied."));
/// This event will be produced when the input goes to LOW.
CDI_GROUP_ENTRY(
    event_clear, openlcb::EventConfigEntry, //
    Name("Block Clear"),
    Description("This event will be produced when the block becomes clear."));
CDI_GROUP_END();

/// OpenLCB Producer class integrating a simple CDI-based configuration for two
/// event IDs, and an input GPIO object whose value will determine when to
/// produce events. This is usually the most important object for a simple IO
/// node.  (Specialized CDI for ocupancy detectors.)
///
/// Usage: Must be called repeatedly via the Polling implementation exposed by
/// @ref polling(). Use for example the @ref RefreshLoop class and supply the
/// polling argument at the constructor to it:
/// 
/// openlcb::RefreshLoop loop(
///    stack.node(), {producer_sw1.polling(), producer_sw2.polling()});
class OccupancyDetector : public ConfigUpdateListener
{
public:
    using Impl = openlcb::GPIOBit;
    using ProducerClass = openlcb::PolledProducer<QuiesceDebouncer, Impl>;

    OccupancyDetector(openlcb::Node *node, const OccupancyDetectorConfig &cfg, const Gpio *gpio)
        : producer_(QuiesceDebouncer::Options(3), node, 0, 0, gpio)
        , cfg_(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }

    template <class HW>
    OccupancyDetector(openlcb::Node *node, const OccupancyDetectorConfig &cfg, const HW &,
        const Gpio *g = HW::instance(), decltype(HW::instance) * = 0)
        : producer_(QuiesceDebouncer::Options(3), node, 0, 0, g)
        , cfg_(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }

    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) override
    {
        AutoNotify n(done);
        uint8_t debounce_arg = cfg_.debounce().read(fd);
        openlcb::EventId cfg_event_occupied = cfg_.event_occupied().read(fd);
        openlcb::EventId cfg_event_clear = cfg_.event_clear().read(fd);
        if (cfg_event_clear != producer_.event_off() ||
            cfg_event_occupied != producer_.event_on())
        {
            auto saved_gpio = producer_.gpio_;
            auto saved_node = producer_.node();
            // Need to reinitialize the producer. We do this with in-place
            // destruction and construction.
            producer_.ProducerClass::~ProducerClass();
#ifdef ARDUINO
            new (&producer_) ProducerClass(
                QuiesceDebouncer::Options(debounce_arg), saved_node,
                                           cfg_event_clear, cfg_event_occupied, saved_gpio);
#else
            new (&producer_) ProducerClass(
                QuiesceDebouncer::Options(debounce_arg), saved_node,
                                           cfg_event_occupied, cfg_event_clear, saved_gpio);
#endif
            return REINIT_NEEDED; // Causes events identify.
        }
        return UPDATED;
    }

    void factory_reset(int fd) OVERRIDE
    {
        cfg_.description().write(fd, "");
        CDI_FACTORY_RESET(cfg_.debounce);
    }

    openlcb::Polling *polling()
    {
        return &producer_;
    }
    template <unsigned N>
          __attribute__((noinline))
          static void Init(openlcb::Node *node,
                           const openlcb::RepeatedGroup<OccupancyDetectorConfig, N> &config,
                           const Gpio *const *pins, unsigned size)
    {
        HASSERT(size == N);
        openlcb::ConfigReference offset_(config);
        openlcb::RepeatedGroup<OccupancyDetectorConfig, UINT_MAX> grp_ref(offset_.offset());
        for (unsigned i = 0; i < size; i++)
        {
            OccupancyDetector *o = new OccupancyDetector(node,grp_ref.entry(i),pins[i]);
            ocs.push_back(o);
            pollers.push_back(new openlcb::RefreshLoop(node,{o->polling()}));
        }
    }
    
private:
    ProducerClass producer_;
    const OccupancyDetectorConfig cfg_;
    static vector<OccupancyDetector *> ocs;
    static vector<openlcb::RefreshLoop *> pollers;
};





#endif // __OCCUPANDCYDETECTOR_HXX

