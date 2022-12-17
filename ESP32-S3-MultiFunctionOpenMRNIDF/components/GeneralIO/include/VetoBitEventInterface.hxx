// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Feb 27 10:01:16 2022
//  Last Modified : <220227.1423>
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

#ifndef __VETOBITEVENTINTERFACE_HXX
#define __VETOBITEVENTINTERFACE_HXX

/// Represents a bit of state using four events.
///
/// Two of the events control the state of a veto over allowing the
/// other the two events changing the state of of something else.
/// Typically, this is used to protect a turnout's points from being
/// moved while the turnout is occupied by a train, but could be used
/// for other things.
/// 
/// It encapsulates application-specific information about the
/// event bit:
///
///  - which OpenLCB node this bit is represented on;
///  - the event identifiers for ON and OFF events;
///  - how to query the hardware for the current event state;
///  - how to set the hardware to a new event state;
///  - the event identifiers for Veto ON and Veto OFF events.
///  
/// Adapted/combined from openlcb::BitEventInterface and 
/// openlcb::NetworkInitializedBit by Robert Heller <heller@deepsoft.com>
class VetoBitEventInterface
{
public:
    VetoBitEventInterface(uint64_t event_on, uint64_t event_off,
                          uint64_t veto_on, uint64_t veto_off,
                          bool default_veto)
                : event_on_(event_on)
          , event_off_(event_off)
          , veto_on_(veto_on)
          , veto_off_(veto_off)
          , isKnown_(0)
          , veto_(default_veto ? 1 : 0)
          
    {
    }
    /// returns the current hardware state: true for ON, false for OFF.
    virtual openlcb::EventState get_current_state() = 0;

    /// Get the requested state. @TODO(stbaker): document the difference
    /// between requested state and current state.
    /// @return requested state
    virtual openlcb::EventState get_requested_state()
    {
        return get_current_state();
    }
    virtual openlcb::EventState get_veto_state()
    {
        return (veto_) ? openlcb::EventState::VALID : openlcb::EventState::INVALID;
    }
    
    /// Updates the hardware for the new event state.
    ///
    /// @param new_value is true for state ON, false for state OFF.
    virtual void real_set_state(bool new_value) = 0;
    
    void set_state(bool new_value)
    {
        if (!veto_) {real_set_state(new_value);}
    }
    
    void set_veto_state(bool new_value)
    {
        isKnown_ = 1;
        veto_ = new_value ? 1 : 0;
    }
    
    
    /// returns the event ID for representing the state transition OFF->ON.
    uint64_t event_on()
    {
        return event_on_;
    }

    /// returns the event ID for representing the state transition ON->OFF.
    uint64_t event_off()
    {
        return event_off_;
    }

    /// returns the event ID for representing the veto transition OFF->ON.
    uint64_t veto_on()
    {
        return veto_on_;
    }

    /// returns the event ID for representing the veto transition ON->OFF.
    uint64_t veto_off()
    {
        return veto_off_;
    }

    /// returns the OpenLCB virtual node from which to send the respective
    /// events when the bit changes.
    virtual openlcb::Node *node() = 0;

    
    
private:
    uint64_t event_on_;
    uint64_t event_off_;
    uint64_t veto_on_;
    uint64_t veto_off_;
    /// true when we know the veto state
    uint8_t isKnown_ : 1;
    /// veto state.
    uint8_t veto_ : 1;

    DISALLOW_COPY_AND_ASSIGN(VetoBitEventInterface);
};

class VetoGPIOBit : public VetoBitEventInterface
{
public:
    VetoGPIOBit(openlcb::Node *node, openlcb::EventId event_on, 
                openlcb::EventId event_off, 
                openlcb::EventId veto_on, openlcb::EventId veto_off,
                const Gpio *gpio)
                : VetoBitEventInterface(event_on,event_off,veto_on,veto_off,false)
          , node_(node)
          , gpio_(gpio)
    {
        
    }
    template <class HW>
          VetoGPIOBit(openlcb::Node *node, openlcb::EventId event_on, openlcb::EventId event_off, 
                      openlcb::EventId veto_on, openlcb::EventId veto_off, 
                      const HW &, const Gpio* g = HW::instance(), decltype(HW::instance)* = 0)
        : VetoGPIOBit(node, event_on, event_off, veto_on, veto_off, g)
    {
    }
    openlcb::EventState get_current_state() OVERRIDE
    {
        return gpio_->is_set() ? openlcb::EventState::VALID : openlcb::EventState::INVALID;
    }
    void real_set_state(bool new_value) OVERRIDE
    {
        gpio_->write(new_value);
    }
    openlcb::Node *node() OVERRIDE
    {
        return node_;
    }

public:
    openlcb::Node *node_;
    const Gpio *gpio_;
};
    

    

class VetoBitEventConsumer : public openlcb::SimpleEventHandler
{
public:
    VetoBitEventConsumer(VetoBitEventInterface *bit)
                : bit_(bit)
    {
        register_handler(bit->event_on(), bit->event_off(),
                         bit->veto_on(), bit->veto_off());
    }
    ~VetoBitEventConsumer()
    {
        unregister_handler();
    }
    /// Queries producers and acquires the current state of the bit.
    void SendQuery(openlcb::WriteHelper *writer, BarrierNotifiable *done);

    void handle_event_report(const openlcb::EventRegistryEntry &entry,
                             openlcb::EventReport *event,
                             BarrierNotifiable *done) override;
    void handle_identify_global(const openlcb::EventRegistryEntry &entry,
                                openlcb::EventReport *event,
                                BarrierNotifiable *done) override;
    void handle_identify_consumer(const openlcb::EventRegistryEntry &entry,
                                  openlcb::EventReport *event,
                                  BarrierNotifiable *done) override;
    void handle_producer_identified(const openlcb::EventRegistryEntry &entry,
                                    openlcb::EventReport *event,
                                    BarrierNotifiable *done) override;
protected:
    void register_handler(uint64_t event_on, uint64_t event_off,
                          uint64_t veto_on, uint64_t veto_off);
    void unregister_handler();
    void SendConsumerIdentified(openlcb::EventReport *event, BarrierNotifiable *done);

    /// Checks if the event in the report is something we are interested in, and
    /// if so, sends off a {Producer|Consumer}Identified{Valid|Invalid} message
    /// depending on the current state of the hardware bit. Uses
    /// event_write_helper<1>. Notifies done.
    void HandlePCIdentify(openlcb::Defs::MTI mti_valid, openlcb::EventReport *event,
                          BarrierNotifiable *done);

    VetoBitEventInterface *bit_;
private:
    openlcb::WriteHelper write_helpers[4];
    DISALLOW_COPY_AND_ASSIGN(VetoBitEventConsumer);
};



#endif // __VETOBITEVENTINTERFACE_HXX

