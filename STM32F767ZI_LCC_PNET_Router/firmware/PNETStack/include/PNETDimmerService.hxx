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
 * \file PNETDimmerService.hxx
 * 
 * API for PNET Dimmer message service.
 * Heavily borrowed from OpenMRN's EventService.hxx 
 *  (Copyright (c) 2013, Balazs Racz)
 * 
 * 
 * @author Robert Heller
 * @date Fri Mar 17 16:13:53 2023
 */

#ifndef __PNETDIMMERSERVICE_HXX
#define __PNETDIMMERSERVICE_HXX

#include <stdint.h>
#include <unordered_map>
#include "PNETIf.hxx"
#include "PNETDefs.hxx"
#include "utils/Singleton.hxx"
#include "executor/Notifiable.hxx"

namespace pnet
{

/// Dimmer message data.
/// This structure contains the dimmer data elements and it is used
/// as the key for control message handlers and for the buffers
/// used by dimmer message flows.

struct DimmerData
{
    /// Default (empty) constructor.
    DimmerData()
                : slot(0), va(0), vb(0), vc(0), vd(0)
    {
    }
    /// Fully qualified contructor.
    DimmerData(uint8_t slot_, uint8_t va_, uint8_t vb_, uint8_t vc_, uint8_t vd_)
                : slot(slot_), va(va_), vb(vb_), vc(vc_), vd(vd_)
    {
    }
    /// Constructor from a GenMessage.
    DimmerData(GenMessage *m)
    {
        const unsigned char *buf = 
              (const unsigned char *) m->payload.data();
        slot = buf[1];
        va = buf[2];
        vb = buf[3];
        vc = buf[4];
        vd = buf[5];
    }
    /// Initialize a DimmerData from a GenMessage.
    void InitFromGenMessage(GenMessage *m)
    {
        const unsigned char *buf = 
              (const unsigned char *) m->payload.data();
        slot = buf[1];
        va = buf[2];
        vb = buf[3];
        vc = buf[4];
        vd = buf[5];
    }
    /// Fill in a GenMessage from a DimmerData.
    void FillMessage(GenMessage *m)
    {
        m->identifier = Defs::Dimmer;
        unsigned char buf[8];
        buf[0] = 0xdd;
        buf[1] = slot;
        buf[2] = va;
        buf[3] = vb;
        buf[4] = vc;
        buf[5] = vd;
        buf[6] = 0;
        buf[7] = 0;
        m->payload.assign((const char *)(buf), 8);
    }
    /// Dimmer slot
    uint8_t slot;
    /// Value A
    uint8_t va;
    /// Value B
    uint8_t vb;
    /// Value C
    uint8_t vc;
    /// Value D
    uint8_t vd;
};

/// Abstract class for Dimmer Processing.
class DimmerProcess
{
public:
    virtual ~DimmerProcess()
    {
    }
    
    /// User callback function.                                         
    /// @param cd The dimmer data.
    /// @param done Notifiable.
    virtual void process_dimmer(const DimmerData &cd,
                                 BarrierNotifiable *done) = 0;
};

/// Dimmer Registry Entry
/// Contains the data for a Dimmer handler in the registry.
class DimmerRegistryEntry
{
public:
    /// The matching Dimmer Data.
    DimmerData cd;
    /// The Dimmer handler class instance.
    DimmerProcess *handler;
    /// A user arg. 
    uint32_t user_arg;
    /// Constructor without a user arg.
    /// @param _handler The handler class instance.
    /// @param _cd The dimmer data to match.
    /// @returns a DimmerRegistryEntry.
    DimmerRegistryEntry(DimmerProcess *_handler,
                         const DimmerData &_cd)
                : cd(_cd)
          , handler(_handler)
          , user_arg(0)
    {
    }
    /// Constructor with a user arg.
    /// @param _handler The handler class instance.
    /// @param _cd The dimmer data to match.
    /// @param _user_arg The user arg.
    /// @returns a DimmerRegistryEntry.
    DimmerRegistryEntry(DimmerProcess *_handler,
                         const DimmerData &_cd,
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
bool operator==(const DimmerData& lhs, const DimmerData& rhs);

/// Hash function for dimmer data.
/// This is just four of the 8-bit values logically appended into a 
/// 32-bit number.
struct DimmerDataHash
{
    /// Return the hash
    /// @param cd the dimmer data
    /// @returns a 32-bit number.
    std::size_t operator()(DimmerData const& cd) const
    {
        return ((cd.va << 24) | (cd.vb << 16) | (cd.vc << 8) | cd.vd);
    }
};


/// Iterator class for accessing Dimmer handlers.
/// Contains the Registry container and provides the register and
/// unregister methods, along with the indexing methods.
class DimmerRegistryIterator
{
public:
    /// Constructor: initialize the iterator to select no elements.
    DimmerRegistryIterator()
    {
        clear_iteration();
    }
    /// Destructor: does nothing.
    ~DimmerRegistryIterator()
    {
    }
    /// Return the next entry or null if no more entries are available.
    /// @returns a DimmerRegistryEntry pointer or null if none are
    /// available. 
    DimmerRegistryEntry *next_entry()
    {
        if (it_ == last_) return nullptr;
        DimmerRegistryEntry * h = &(it_->second);
        ++it_; 
        return h; 
    }
    /// Resets the interator to select no elements.
    void clear_iteration()
    {
        it_ = registry_.end();
        last_ = registry_.end();
    }
    /// Initializes the interator based on a DimmerData key.
    void init_iteration(DimmerData cd)
    {
        auto range = registry_.equal_range(cd);
        it_ = range.first;
        last_ = range.second;
    }
    /// Register a DimmerRegistryEntry
    /// @param entry the entry to register.
    void register_handler(const DimmerRegistryEntry &entry);
    /// Unregister a DimmerRegistryEntry 
    /// @param entry the entry to unregister. 
    void unregister_handler(const DimmerRegistryEntry &entry);
private:
    /// Container type.
    typedef std::unordered_multimap<DimmerData, DimmerRegistryEntry,
          DimmerDataHash> DimmerRegistryContainer;
    /// The container itself.
    DimmerRegistryContainer registry_;
    /// The current element.
    DimmerRegistryContainer::iterator it_;
    /// The last element.
    DimmerRegistryContainer::iterator last_;
};

          
/// Public API for Control message Handler.
class DimmerHandler : public Service,
                       public Singleton<DimmerHandler>
{
public:
    /// Constructor: 
    /// @param service the PNET If
    /// @returns a new DimmerHandler object.  This is a Singleton,
    /// so there can be only one.
    DimmerHandler(If *service);
    ~DimmerHandler();
    /// Public API to register a Dimmer handler.
    /// @param entry a DimmerRegistryEntry.
    void register_handler(const DimmerRegistryEntry &entry);
    /// Public API to unregister a Dimmer handler.
    /// @param entry a DimmerRegistryEntry.
    void unregister_handler(const DimmerRegistryEntry &entry);
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


#endif // __PNETDIMMERSERVICE_HXX

