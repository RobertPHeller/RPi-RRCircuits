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
//  Last Modified : <221217.0921>
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
#include "OccupancyDetectorConfig.hxx"

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

private:
    ProducerClass producer_;
    const OccupancyDetectorConfig cfg_;
};





#endif // __OCCUPANDCYDETECTOR_HXX

