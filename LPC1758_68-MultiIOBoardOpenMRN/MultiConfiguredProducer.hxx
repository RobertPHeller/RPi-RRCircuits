// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Sep 12 13:04:11 2022
//  Last Modified : <220912.1532>
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

#ifndef __MULTICONFIGUREDPRODUCER_HXX
#define __MULTICONFIGUREDPRODUCER_HXX

#include "openlcb/ConfigRepresentation.hxx"
#include "openlcb/ConfiguredProducer.hxx"
#include "utils/Debouncer.hxx"
#include "utils/format_utils.hxx"

/// Version of the @ref ConfiguredProducer class that can handle many GPIO pins
/// with two events each. This saves very significant amount of memory compared
/// to instantiating individual ConfiguredProducer  instances -- helpful when a
/// single small MCU is exporting a large number of IOs via for example shift
/// register outputs.

class MultiConfiguredProducer : public ConfigUpdateListener,
                                private openlcb::SimpleEventHandler,
                                private openlcb::Polling,
                                private Notifiable
{
public:
    typedef openlcb::ProducerConfig config_entry_type;
    typedef QuiesceDebouncer debouncer_type;
    /// Usage: ```
    ///
    /// constexpr const Gpio *const kDirectGpio[] = {
    /// TDRV1_Pin::instance(), TDRV2_Pin::instance(),
    /// TDRV3_Pin::instance(), TDRV4_Pin::instance(),
    /// };
    /// MultiConfiguredProducer direct_producers(stack.node(),
    ///    kDirectGpio, ARRAYSIZE(kDirectGpio), cfg.seg().direct_producers())resented by the Gpio* object
    /// instances. Can be constant from FLASH space.
    /// @param size is the length of the list of pins array.
    /// @param config is the repeated group object from the configuration space
    /// that represents the locations of the events.
    template <unsigned N>
          __attribute__((noinline)) 
          MultiConfiguredProducer(openlcb::Node *node,
                                  const Gpio *const *pins, 
                                  unsigned size,
                                  const openlcb::RepeatedGroup<config_entry_type, N> &config)
                : node_(node)
          , pins_(pins)
          , size_(N)
          , offset_(config)
    {
        // Mismatched sizing of the GPIO array from the configuration array.
        HASSERT(size == N);
        ConfigUpdateService::instance()->register_update_listener(this);
        producedEvents_ = new EventId[size * 2];
        std::allocator<debouncer_type> alloc;
        debouncers_ = alloc.allocate(size_);
        for (unsigned i = 0; i < size_; ++i)
        {
            alloc.construct(debouncers_ + i, 3);
        }
    }
    
    ~MultiConfiguredProducer()
    {
        do_unregister();
        ConfigUpdateService::instance()->unregister_update_listener(this);
        delete[] producedEvents_;                                      
        std::allocator<debouncer_type> alloc;
        for (unsigned i = 0; i < size_; ++i)
        {
            alloc.destroy(debouncers_ + i);
        }
        alloc.deallocate(debouncers_, size_);
    }
    
    /// @return the instance to give to the RefreshLoop object.
    Polling *polling()
    {
        return this; 
    }

    /// Call from the refresh loop. 
    void poll_33hz(openlcb::WriteHelper *helper, Notifiable *done) override
    {
        nextPinToPoll_ = 0;
        pollingHelper_ = helper;
        pollingDone_ = done;
        this->notify();
    }
    /// Asynchronous callback when the previous polling message has left via
    /// the bus. Used as a poor man's iterative state machine.
    void notify() override
    {
        for (; nextPinToPoll_ < size_; ++nextPinToPoll_)
        {
            auto i = nextPinToPoll_;
            if (debouncers_[i].update_state(pins_[i]->is_set()))
            {
                // Pin flipped.
                ++nextPinToPoll_; // avoid infinite loop.
                auto event = producedEvents_[2 * i +
                    (debouncers_[i].current_state() ? 1 : 0)];
                pollingHelper_->WriteAsync(node_, openlcb::Defs::MTI_EVENT_REPORT,
                    openlcb::WriteHelper::global(), openlcb::eventid_to_buffer(event), this);
                return;
            }
        }
        pollingDone_->notify();
    }

    
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE
    {
        AutoNotify n(done);
        
        if (!initial_load)
        {
            // There is no way to figure out what the previously registered
            // eventid values were for the individual pins. Therefore we always
            // unregister everything and register them anew. It also causes us
            // to identify all. This is not a problem since apply_configuration
            // is coming from a user action.
            do_unregister();
        }
        openlcb::RepeatedGroup<config_entry_type, UINT_MAX> grp_ref(offset_.offset());
        for (unsigned i = 0; i < size_; ++i)
        {
            const config_entry_type cfg_ref(grp_ref.entry(i));
            EventId cfg_event_on = cfg_ref.event_on().read(fd);
            EventId cfg_event_off = cfg_ref.event_off().read(fd);
            producedEvents_[i * 2] = cfg_event_off;
            producedEvents_[i * 2 + 1] = cfg_event_on;
            openlcb::EventRegistry::instance()->register_handler(
                                                        EventRegistryEntry(this, cfg_event_off, i * 2), 0);
            openlcb::EventRegistry::instance()->register_handler(
                                                        EventRegistryEntry(this, cfg_event_on, i * 2 + 1), 0);
            uint8_t param = cfg_ref.debounce().read(fd);
            debouncers_[i].reset_options(param);
            debouncers_[i].initialize(pins_[i]->read());
        }
        return REINIT_NEEDED; // Causes events identify.
    }
    
    void factory_reset(int fd) OVERRIDE
    {
        openlcb::RepeatedGroup<config_entry_type, UINT_MAX> grp_ref(offset_.offset());
        for (unsigned i = 0; i < size_; ++i)
        {
            grp_ref.entry(i).description().write(fd, "");
            CDI_FACTORY_RESET(grp_ref.entry(i).debounce);
        }
    }
    
    /// Factory reset helper function. Sets all names to something 1..N.
    /// @param fd pased on from factory reset argument.
    /// @param basename name of repeats.
    void factory_reset_names(int fd, const char *basename)
    {
        openlcb::RepeatedGroup<config_entry_type, UINT_MAX> grp_ref(offset_.offset());
        for (unsigned i = 0; i < size_; ++i)
        {
            string v(basename);
            v.push_back(' ');
            char buf[10];
            unsigned_integer_to_buffer(i+1, buf);
            v += buf;
            grp_ref.entry(i).description().write(fd, v);
        }
    }
    
    void handle_identify_global(const EventRegistryEntry &entry,
                              EventReport *event,
                                BarrierNotifiable *done) override
    {
        AutoNotify an(done);
        if (event->event != entry.event)
        {
            return;
        }
        SendProducerIdentified(entry, event, done);
    }
    
    void handle_identify_producer(const EventRegistryEntry &entry,
                                EventReport *event,
                                  BarrierNotifiable *done) override
    {
        AutoNotify an(done);
        if (event->event != entry.event)
        {
            return;
        }
        SendProducerIdentified(entry, event, done);
    }
private:
    /// Removes registration of this event handler from the global event
    /// registry.
    void do_unregister()
    {
        openlcb::EventRegistry::instance()->unregister_handler(this);
    }

    /// Sends out a ProducerIdentified message for the given registration
    /// entry.
    void SendProducerIdentified(const EventRegistryEntry &registry_entry,
        EventReport *event, BarrierNotifiable *done)
    {
        openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        unsigned b1 = pins_[registry_entry.user_arg >> 1]->is_set() ? 1 : 0;
        unsigned b2 = registry_entry.user_arg & 1; // on or off event?
        if (b1 ^ b2)
        {
            mti++; // INVALID
        }
        event->event_write_helper<4>()->WriteAsync(node_, mti,
            openlcb::WriteHelper::global(), openlcb::eventid_to_buffer(registry_entry.event),
            done->new_child());
    }

    // Variables used for asynchronous state during the polling loop.
    /// Which pin to next check when polling.
    unsigned nextPinToPoll_;
    /// Write helper to use for producing messages during the polling loop.
    openlcb::WriteHelper *pollingHelper_;
    /// Notifiable to call when the polling loop is done.
    Notifiable *pollingDone_;

    /// virtual node to export the consumer / producer on.
    openlcb::Node *node_;
    /// Array of all GPIO pins to use. Externally owned.
    const Gpio *const *pins_;
    /// Number of GPIO pins to export.
    size_t size_;
    /// Offset in the configuration space for our configs.
    openlcb::ConfigReference offset_;
    /// Event IDs shadowing from the config file for producing them. We own
    /// this memory.
    EventId *producedEvents_;
    /// One debouncer per pin, created for produced pins. We own this memory.
    debouncer_type *debouncers_;
};
    


                                                            
    
#endif // __MULTICONFIGUREDPRODUCER_HXX

