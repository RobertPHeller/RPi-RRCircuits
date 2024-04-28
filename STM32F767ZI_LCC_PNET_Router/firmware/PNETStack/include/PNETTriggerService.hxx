// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri Mar 17 16:10:40 2023
//  Last Modified : <230321.1445>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
/** \copyright
 * Copyright (c) 2023, Robert Heller
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are  permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \file PNETTriggerService.hxx
 * 
 * API for PNET Control message service.
 * Heavily borrowed from OpenMRN's EventService.hxx 
 *  (Copyright (c) 2013, Balazs Racz)
 * 
 * @author Robert Heller
 * @date Fri Mar 17 16:10:40 2023
 */

#ifndef __PNETTRIGGERSERVICE_HXX
#define __PNETTRIGGERSERVICE_HXX

#include <stdint.h>
#include <unordered_map>
#include "PNETIf.hxx"
#include "PNETDefs.hxx"
#include "utils/Singleton.hxx"
#include "executor/Notifiable.hxx"

namespace pnet
{

/// Trigger message data.
/// This structure contains the trigger data elements and it is used
/// as the key for trigger message handlers and for the buffers
/// used by control message flows.
struct TriggerData 
{
    /// Default (empty) constructor.
    TriggerData()
                : slot(0), trigger(0)
    {
    }
    /// Fully qualified contructor.
    TriggerData(uint8_t slot_, uint8_t trigger_)
                : slot(slot_), trigger(trigger_)
    {
    }
    /// Constructor from a GenMessage.
    TriggerData(GenMessage *m)
    {
        const unsigned char *buf = 
              (const unsigned char *) m->payload.data();
        slot = buf[3];
        trigger = buf[4];
    }
    /// Initialize a TriggerData from a GenMessage.
    void InitFromGenMessage(GenMessage *m)
    {
        const unsigned char *buf = 
              (const unsigned char *) m->payload.data();
        slot = buf[3];
        trigger = buf[4];
    }
    /// Fill in a GenMessage from a TriggerData.
    void FillMessage(GenMessage *m)
    {
        m->identifier = Defs::Trigger;
        unsigned char buf[8];
        buf[0] = 0x55;
        buf[1] = 0xaa;
        buf[2] = 0x55;
        buf[3] = slot;
        buf[4] = trigger;
        buf[5] = 0;
        buf[6] = 0;
        buf[7] = 0;
        m->payload.assign((const char *)(buf), 8);
    }
    /// Control Slot
    uint8_t slot;
    /// Trigger Number
    uint8_t trigger;
};

/// Abstract class for Trigger Processing.                              
class TriggerProcess
{
public:
    virtual ~TriggerProcess()
    {
    }
    
    /// User callback function.
    /// @param cd The trigger data.
    /// @param done Notifiable.
    virtual void process_trigger(const TriggerData &td, 
                                 BarrierNotifiable *done) = 0;
};

/// Trigger Registry Entry
/// Contains the data for a Trigger handler in the registry.
class TriggerRegistryEntry
{
public:
    /// The matching Trigger Data
    TriggerData td;
    /// The Trigger handler class instance.
    TriggerProcess *handler;
    /// A user arg.
    uint32_t user_arg;
    /// Constructor without a user arg.
    /// @param _handler The handler class instance.
    /// @param _cd The trigger data to match.
    /// @returns a TriggerRegistryEntry.
    TriggerRegistryEntry(TriggerProcess *_handler, 
                         const TriggerData &_td)
                : td(_td)
          , handler(_handler)
          , user_arg(0)
    {
    }
    /// Constructor with a user arg.
    /// @param _handler The handler class instance.
    /// @param _cd The trigger data to match.
    /// @param _user_arg The user arg. 
    /// @returns a TriggerRegistryEntry.
    TriggerRegistryEntry(TriggerProcess *_handler, 
                         const TriggerData &_td,
                         unsigned _user_arg)
                : td(_td)
          , handler(_handler)
          , user_arg(_user_arg)
    {
    }
};


/// Equality operator.
/// @param lhs the left hand side.
/// @param rhs the right hand side.
/// @returns true if the left hand side is exactly equal to the right hand side.
bool operator==(const TriggerData& lhs, const TriggerData& rhs);

/// Hash function for trigger data.
/// This is just the two 8-bit values logically appended into a 
/// 16-bit number.
struct TriggerDataHash
{
    /// Return the hash
    /// @param td the trigger data
    /// @returns a 16-bit number.
    std::size_t operator()(TriggerData const& td) const 
    {
        return ((td.slot << 8) | td.trigger);
    }
};

/// Iterator class for accessing Trigger handlers.
/// Contains the Registry container and provides the register and
/// unregister methods, along with the indexing methods.
class TriggerRegistryIterator 
{
public:
    /// Constructor: initialize the iterator to select no elements.
    TriggerRegistryIterator()
    {
        clear_iteration();
    }
    /// Destructor: does nothing.
    ~TriggerRegistryIterator()
    {
    }
    /// Return the next entry or null if no more entries are available.
    /// @returns a TriggerRegistryEntry pointer or null if none are
    /// available.
    TriggerRegistryEntry *next_entry()
    {
        if (it_ == last_) return nullptr;
        TriggerRegistryEntry * h = &(it_->second);
        ++it_;
        return h;
    }
    /// Resets the interator to select no elements.
    void clear_iteration()
    {
        it_ = registry_.end();
        last_ = registry_.end();
    }
    /// Initializes the interator based on a TriggerData key.
    void init_iteration(TriggerData td)
    {
        auto range = registry_.equal_range(td);
        it_ = range.first;
        last_ = range.second;
    }
    /// Register a TriggerRegistryEntry 
    /// @param entry the entry to register.
    void register_handler(const TriggerRegistryEntry &entry);
    /// Unregister a TriggerRegistryEntry 
    /// @param entry the entry to unregister.
    void unregister_handler(const TriggerRegistryEntry &entry);
private:
    /// Container type.
    typedef std::unordered_multimap<TriggerData, TriggerRegistryEntry,
          TriggerDataHash> TriggerRegistryContainer;
    /// The container itself.
    TriggerRegistryContainer registry_;
    /// The current element.
    TriggerRegistryContainer::iterator it_;
    /// The last element.
    TriggerRegistryContainer::iterator last_;
};

/// Public API for Trigger message Handler.
class TriggerHandler : public Service,
                       public Singleton<TriggerHandler>
{
public:
    /// Constructor:
    /// @param service the PNET If
    /// @returns a new TriggerHandler object.  This is a Singleton,
    /// so there can be only one.
    TriggerHandler(If *iface);
    ~TriggerHandler();
    /// Public API to register a Trigger handler.
    void register_handler(const TriggerRegistryEntry &entry);
    /// Public API to unregister a Trigger handler.
    void unregister_handler(const TriggerRegistryEntry &entry);
    class Impl;
    /// Return the implementation object.
    Impl *impl()
    {
        return impl_.get();
    }
private:
    /// The low level (private) implementation object.
    std::unique_ptr<Impl> impl_;
};

}

#endif // __PNETTRIGGERSERVICE_HXX

