// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri Mar 17 16:13:53 2023
//  Last Modified : <230321.1259>
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
 * \file PNETControlService.hxx
 * 
 * API for PNET Control message service.
 * Heavily borrowed from OpenMRN's EventService.hxx 
 *  (Copyright (c) 2013, Balazs Racz)
 * 
 * 
 * @author Robert Heller
 * @date Fri Mar 17 16:13:53 2023
 */


#ifndef __PNETCONTROLSERVICE_HXX
#define __PNETCONTROLSERVICE_HXX

#include <stdint.h>
#include <unordered_map>
#include "PNETIf.hxx"
#include "PNETDefs.hxx"
#include "utils/Singleton.hxx"
#include "executor/Notifiable.hxx"

namespace pnet
{

/// Control message data.
/// This structure contains the control data elements and it is used
/// as the key for control message handlers and for the buffers
/// used by control message flows.
struct ControlData
{
    /// Default (empty) constructor.
    ControlData()
                : slot(0), value(0), attributes(0)
    {
    }
    /// Fully qualified contructor.
    ControlData(uint8_t slot_, uint8_t value_, uint8_t attributes_)
                : slot(slot_), value(value_), attributes(attributes_)
    {
    }
    /// Constructor from a GenMessage.
    ControlData(GenMessage *m)
    {
        const unsigned char *buf = 
              (const unsigned char *) m->payload.data();
        slot = buf[3];
        value = buf[4];
        attributes = buf[5];
    }
    /// Initialize a ControlData from a GenMessage.
    void InitFromGenMessage(GenMessage *m)
    {
        const unsigned char *buf = 
              (const unsigned char *) m->payload.data();
        slot = buf[3];
        value = buf[4];
        attributes = buf[5];
    }
    /// Fill in a GenMessage from a ControlData.
    void FillMessage(GenMessage *m)
    {
        m->identifier = Defs::Control;
        unsigned char buf[8];
        buf[0] = 0xaa;
        buf[1] = 0xaa;
        buf[2] = 0x55;
        buf[3] = slot;
        buf[4] = value;
        buf[5] = attributes;
        buf[6] = 0;
        buf[7] = 0;
        m->payload.assign((const char *)(buf), 8);
    }
    /// Control Slot
    uint8_t slot;
    /// Control value
    uint8_t value;
    /// Control attributes
    uint8_t attributes;
};

/// Abstract class for Control Processing.
class ControlProcess
{
public:
    virtual ~ControlProcess()
    {
    }
    
    /// User callback function.
    /// @param cd The control data.
    /// @param done Notifiable.
    virtual void process_control(const ControlData &cd,
                                 BarrierNotifiable *done) = 0;
};


/// Control Registry Entry
/// Contains the data for a Control handler in the registry.
class ControlRegistryEntry
{
public:
    /// The matching Control Data
    ControlData cd;
    /// The Control handler class instance.
    ControlProcess *handler;
    /// A user arg.
    uint32_t user_arg;
    /// Constructor without a user arg.
    /// @param _handler The handler class instance.
    /// @param _cd The control data to match.
    /// @returns a ControlRegistryEntry.
    ControlRegistryEntry(ControlProcess *_handler,
                         const ControlData &_cd)
                : cd(_cd)
          , handler(_handler)
          , user_arg(0)
    {
    }
    /// Constructor with a user arg.
    /// @param _handler The handler class instance.
    /// @param _cd The control data to match.
    /// @param _user_arg The user arg.
    /// @returns a ControlRegistryEntry.
    ControlRegistryEntry(ControlProcess *_handler,
                         const ControlData &_cd,
                         unsigned _user_arg)
                : cd(_cd)
          , handler(_handler)
          , user_arg(_user_arg)
    {
    }
};


/// Equality operator.
/// @param lhs the left hand side.
/// @param rhs the right hand side.
/// @returns true if the left hand side is exactly equal to the right hand side.
bool operator==(const ControlData& lhs, const ControlData& rhs);

/// Hash function for control data.
/// This is just the three 8-bit values logically appended into a 
/// 24-bit number.
struct ControlDataHash
{
    /// Return the hash
    /// @param cd the control data
    /// @returns a 24-bit number.
    std::size_t operator()(ControlData const& cd) const
    {
        return ((cd.slot << 16) | (cd.value << 8) | cd.attributes);
    }
};

/// Iterator class for accessing Control handlers.
/// Contains the Registry container and provides the register and
/// unregister methods, along with the indexing methods.
class ControlRegistryIterator
{
public:
    /// Constructor: initialize the iterator to select no elements.
    ControlRegistryIterator()
    {
        clear_iteration();
    }
    /// Destructor: does nothing.
    ~ControlRegistryIterator()
    {
    }
    /// Return the next entry or null if no more entries are available.
    /// @returns a ControlRegistryEntry pointer or null if none are
    /// available.
    ControlRegistryEntry *next_entry()
    {
        if (it_ == last_) return nullptr;
        ControlRegistryEntry * h = &(it_->second);
        ++it_; 
        return h; 
    }
    /// Resets the interator to select no elements.
    void clear_iteration()
    {
        it_ = registry_.end();
        last_ = registry_.end();
    }
    /// Initializes the interator based on a ControlData key.
    void init_iteration(ControlData cd)
    {
        auto range = registry_.equal_range(cd);
        it_ = range.first;
        last_ = range.second;
    }
    /// Register a ControlRegistryEntry
    /// @param entry the entry to register.
    void register_handler(const ControlRegistryEntry &entry);
    /// Unregister a ControlRegistryEntry
    /// @param entry the entry to unregister.
    void unregister_handler(const ControlRegistryEntry &entry);
private:
    /// Container type.
    typedef std::unordered_multimap<ControlData, ControlRegistryEntry,
          ControlDataHash> ControlRegistryContainer;
    /// The container itself.
    ControlRegistryContainer registry_;
    /// The current element.
    ControlRegistryContainer::iterator it_;
    /// The last element.
    ControlRegistryContainer::iterator last_;
};

/// Public API for Control message Handler.
class ControlHandler : public Service,
                       public Singleton<ControlHandler>
{
public:
    /// Constructor:
    /// @param service the PNET If
    /// @returns a new ControlHandler object.  This is a Singleton,
    /// so there can be only one.
    ControlHandler(If *service);
    ~ControlHandler();
    /// Public API to register a Control handler.
    /// @param entry a ControlRegistryEntry.
    void register_handler(const ControlRegistryEntry &entry);
    /// Public API to unregister a Control handler.
    /// @param entry a ControlRegistryEntry.
    void unregister_handler(const ControlRegistryEntry &entry);
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


#endif // __PNETCONTROLSERVICE_HXX

