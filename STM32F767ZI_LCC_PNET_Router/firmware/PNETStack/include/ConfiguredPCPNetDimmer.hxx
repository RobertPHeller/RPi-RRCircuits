// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Mar 20 12:19:01 2023
//  Last Modified : <240501.0912>
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
 * \file ConfiguredPCPNetDimmer.hxx
 * @author Robert Heller
 * @date Mon Mar 20 12:19:01 2023
 */
/** @page ConfiguredPCPNetDimmer PNET Dimmer Configuration
 * 
 * Configured Producer/Consumer for PNET Dimmer messages.
 * 
 * These messages control the brightness of outputs.
 *
 * Upto 32 PNET Dimmer messages can be configured and enabled. 
 * 
 * @section DimmerOpts Configuration Options
 * 
 *  Each message
 * has these configuration options:
 * 
 * - Description (16 char string) User name of this Dimmer.
 *   This is just a name for the message for identifing purposes.
 * - Event Produced (Event ID) (P) This event is produced when a matching Control message is received.
 *   This event id is sent by the router onto the LCC network when a PNET node
 *   sends a matching Dimmer message.
 * - Event Consumed (Event ID) (C) This event will cause the defined Control to be sent.
 *   When the node receives this event id, it sends a PNET Dimmer message out
 *   on the PNET network. 
 * - Enable (Yes/No) Enable this Dimmer.
 *   This enables or disables this message.
 * - Slot (0-31) The Control slot number (0-31).
 *   This the slot number (board id) of the message.
 * - Value A (0-255) Dimmer value A.
 *   This is the brightness of the first output.
 * - Value B (0-255) Dimmer value B.
 *   This is the brightness of the second output.
 * - Value C (0-255) Dimmer value C.
 *   This is the brightness of the third output.
 * - Value D (0-255) Dimmer value D.
 *   This is the brightness of the fourth output.
 */ 
 
#ifndef __CONFIGUREDPCPNETDIMMER_HXX
#define __CONFIGUREDPCPNETDIMMER_HXX

#include "openlcb/Node.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "PNETStack.hxx"
#include "PNETWriteHelper.hxx"
#include "PNETDimmerService.hxx"

/// CDI Configuration for a @ref PCPNetDimmer.
CDI_GROUP(PCPNetDimmerConfig);
/// Allows the user to assign a name for this PCPNetDimmer.
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<16>, //
                Name("Description"), 
                Description("User name of this dimmer."));
/// Event produced (sent) when a PNet Dimmer message is received on the PNet.
CDI_GROUP_ENTRY(event_produced, openlcb::EventConfigEntry,
                Name("Event Produced"),
                Description("(P) This event is produced when a matching Dimmer message is received."));
/// Event consumed that sends a PNet Dimmer message onto the PNet.
CDI_GROUP_ENTRY(event_consumed, openlcb::EventConfigEntry,
                Name("Event Consumed"),
                Description("(C) This event will cause the defined Dimmer to be sent."));
/// Flag to enable or disable this PCPNetControl.
CDI_GROUP_ENTRY(enabled, openlcb::Uint8ConfigEntry,
                Min(0), Max(1), Default(0),
                MapValues("<relation><property>0</property><value>No</value></relation>"
                          "<relation><property>1</property><value>Yes</value></relation>"),
                Name("Enable"), Description("Enable this dimmer."));
/// The Slot value.
CDI_GROUP_ENTRY(slot, openlcb::Uint8ConfigEntry,
                Min(0), Max(31), Default(0),
                Name("Slot"),
                Description("The dimmer slot number (0-31)."));
/// Dimmer value a.
CDI_GROUP_ENTRY(value_a, openlcb::Uint8ConfigEntry,
                Default(0),
                Name("Value A"),
                Description("Dimmer value A."));
/// Dimmer value b.
CDI_GROUP_ENTRY(value_b, openlcb::Uint8ConfigEntry,
                Default(0),
                Name("Value B"),
                Description("Dimmer value B."));
/// Dimmer value c.
CDI_GROUP_ENTRY(value_c, openlcb::Uint8ConfigEntry,
                Default(0),
                Name("Value C"),
                Description("Dimmer value C."));
/// Dimmer value d.
CDI_GROUP_ENTRY(value_d, openlcb::Uint8ConfigEntry,
                Default(0),
                Name("Value D"),
                Description("Dimmer value D."));
CDI_GROUP_END();

/// OpenLCB Producer/Consumer class integrating a PNET Dimmer message
/// for two event IDs, one to be sent when a matching PNET Dimmer 
/// message is received and one to cause a PNET Dimmer message to
/// sent.

class PCPNetDimmer : public DefaultConfigUpdateListener,
                           public openlcb::SimpleEventHandler,
                           public pnet::DimmerProcess
{
public:
    /// Constructor:
    /// @param node LCC Node object.
    /// @param pnetstack PNET Stack (PNET does not have nodes).
    /// @param cfg Configuration for this PCPNetControl.
    /// @returns a new PCPNetDimmer object.
    PCPNetDimmer(openlcb::Node *node, pnet::PNETCanStack *pnetstack,
                  const PCPNetDimmerConfig &cfg)
                : DefaultConfigUpdateListener()
          , node_(node)
          , cfg_(cfg)
          , pnetstack_(pnetstack)
          , event_produced_(0)
          , event_consumed_(0)
          , enabled_(false)
          , slot_(0)
          , value_a_(0)
          , value_b_(0)
          , value_c_(0)
          , value_d_(0)
          
    {
        if (enabled_) register_dimmer_handler();
    }
    /// Destructor:
    /// Unregisters handler.
    ~PCPNetDimmer()
    {
        if (enabled_) unregister_dimmer_handler();
    }
    /// Apply a configuration update.
    /// @param fd File description into the configuration.
    /// @param initial_load True if this is the first time.
    /// @param done Notification object.
    /// @returns the update action, either REINIT_NEEDED or UPDATED.
    UpdateAction apply_configuration(int fd, 
                                     bool initial_load, 
                                     BarrierNotifiable *done) OVERRIDE
    {
        AutoNotify n(done);
        const openlcb::EventId cfg_event_produced = cfg_.event_produced().read(fd);
        const openlcb::EventId cfg_event_consumed = cfg_.event_consumed().read(fd);
        const uint8_t cfg_slot = cfg_.slot().read(fd);
        const uint8_t cfg_value_a = cfg_.value_a().read(fd);
        const uint8_t cfg_value_b = cfg_.value_b().read(fd);
        const uint8_t cfg_value_c = cfg_.value_c().read(fd);
        const uint8_t cfg_value_d = cfg_.value_d().read(fd);
        bool cfg_enabled = (bool) cfg_.enabled().read(fd);
        if (cfg_slot != slot_ || cfg_value_a != value_a_ ||
            cfg_value_b != value_b_ || cfg_value_c != value_c_ ||
            cfg_value_d != value_d_ ||
            cfg_enabled != enabled_)
        {
            if (enabled_) unregister_dimmer_handler();
            slot_ = cfg_slot;
            value_a_ = cfg_value_a;
            value_b_ = cfg_value_b;
            value_c_ = cfg_value_c;
            value_d_ = cfg_value_d;
            enabled_ = cfg_enabled;
            if (enabled_) register_dimmer_handler();
        }
        if (cfg_event_produced != event_produced_ ||
            cfg_event_consumed != event_consumed_)
        {
            if (!initial_load) unregister_event_handler();
            event_produced_ = cfg_event_produced;
            event_consumed_ = cfg_event_consumed;
            register_event_handler();
            return REINIT_NEEDED;
        }
        return UPDATED;
    }
    /// Factory reset.
    /// @param fd File descriptor into the configuration.
    /// @returns nothing.
    void factory_reset(int fd) OVERRIDE
    {
        cfg_.description().write(fd, "");
        CDI_FACTORY_RESET(cfg_.slot);
        CDI_FACTORY_RESET(cfg_.value_a);
        CDI_FACTORY_RESET(cfg_.value_b);
        CDI_FACTORY_RESET(cfg_.value_c);
        CDI_FACTORY_RESET(cfg_.value_d);
        CDI_FACTORY_RESET(cfg_.enabled);
    }
    /// Handle identify global.
    /// @param registry_entry The event registry entry.
    /// @param event The event report.
    /// @param done  Notification object.
    /// @returns nothing.
    void handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                openlcb::EventReport *event, 
                                BarrierNotifiable *done) override
    {
        if (event->dst_node && event->dst_node != node_)
        {
            return done->notify();
        }
        event->event_write_helper<1>()->WriteAsync(node_,
                                                   openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN,
                                                   openlcb::WriteHelper::global(),
                                                   openlcb::eventid_to_buffer(event_produced_), done->new_child());
        event->event_write_helper<2>()->WriteAsync(node_,
                                                   openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN,
                                                   openlcb::WriteHelper::global(),
                                                   openlcb::eventid_to_buffer(event_consumed_), done->new_child());
        done->maybe_done();
    }
    /// Handle identify producer.
    /// @param registry_entry The event registry entry.
    /// @param event The event report.
    /// @param done  Notification object.
    /// @returns nothing.
    void handle_identify_producer(const openlcb::EventRegistryEntry &registry_entry, 
                                  openlcb::EventReport *event, 
                                  BarrierNotifiable *done) override
    {
        if (event->dst_node && event->dst_node != node_)
        {
            return done->notify();
        }
        event->event_write_helper<3>()->WriteAsync(node_,
                                                   openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN,
                                                   openlcb::WriteHelper::global(),
                                                   openlcb::eventid_to_buffer(event_produced_), done->new_child());
        done->maybe_done();
    }
    /// Handle identify consumer.
    /// @param registry_entry The event registry entry.
    /// @param event The event report.
    /// @param done  Notification object.
    /// @returns nothing.
    void handle_identify_consumer(const openlcb::EventRegistryEntry &registry_entry, 
                                  openlcb::EventReport *event, 
                                  BarrierNotifiable *done) override
    {
        if (event->dst_node && event->dst_node != node_)
        {
            return done->notify();
        }
        event->event_write_helper<4>()->WriteAsync(node_,
                                                   openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN,
                                                   openlcb::WriteHelper::global(),
                                                   openlcb::eventid_to_buffer(event_consumed_), done->new_child());
        done->maybe_done();
    }
    /// Handle event report.
    /// Sends a PNET Dimmer message.
    /// @param registry_entry The event registry entry.
    /// @param event The event report.
    /// @param done  Notification object.
    /// @returns nothing.
    void handle_event_report(const openlcb::EventRegistryEntry &registry_entry, 
                             openlcb::EventReport *event, 
                             BarrierNotifiable *done) override
    {
        if (enabled_ && event->event == event_consumed_)
        {
            pnet::DimmerData td(slot_, value_a_, value_b_, value_c_, 
                                value_d_);
            pnet::GenMessage m;
            td.FillMessage(&m);
            dimmer_message_helper.WriteAsync(pnetstack_,
                                              m.identifier,
                                              m.payload,
                                              done);
        }
        else
        {
            done->notify();
        }
    }
    /// Process a dimmer message.
    /// Produces (sends) an event report message.
    /// @param td Dimmer Data
    /// @param done  Notification object.
    /// @returns nothing.
    void process_dimmer(const pnet::DimmerData &td,
                         BarrierNotifiable *done) override
    {
        dimmer_event_helper.WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
                                        openlcb::WriteHelper::global(),
                                        openlcb::eventid_to_buffer(event_produced_),
                                        done);
    }
private:
    /// Register event handlers.
    void register_event_handler()
    {
        openlcb::EventRegistry::instance()->register_handler(openlcb::EventRegistryEntry(this,event_consumed_), 0);
        openlcb::EventRegistry::instance()->register_handler(openlcb::EventRegistryEntry(this,event_produced_), 0);
    }
    /// Unregister event handlers.
    void unregister_event_handler()
    {
        openlcb::EventRegistry::instance()->unregister_handler(this);
    }
    /// Register a dimmer message handler.
    void register_dimmer_handler()
    {
        pnet::DimmerHandler::instance()->register_handler(pnet::DimmerRegistryEntry(this,pnet::DimmerData(slot_,value_a_,value_b_,value_c_,value_d_)));
    }
    /// Unregister a dimmer message handler.
    void unregister_dimmer_handler()
    {
        pnet::DimmerHandler::instance()->unregister_handler(pnet::DimmerRegistryEntry(this,pnet::DimmerData(slot_,value_a_,value_b_,value_c_,value_d_)));
    }
    // Node.
    openlcb::Node *node_;
    /// Configuration.
    const PCPNetDimmerConfig cfg_;
    /// PNET Stack
    pnet::PNETCanStack *pnetstack_;
    /// Event ID to produce.
    openlcb::EventId event_produced_;
    /// Event ID to consume.
    openlcb::EventId event_consumed_;
    /// Enable flag
    bool enabled_;
    /// Control slot.
    uint8_t slot_;
    /// Dimmer value a
    uint8_t value_a_;
    /// Dimmer value b
    uint8_t value_b_;
    /// Dimmer value c
    uint8_t value_c_;
    /// Dimmer value d
    uint8_t value_d_;
    /// OpenLCB message write helper.
    openlcb::WriteHelper dimmer_event_helper;
    /// PNET message write helper.
    pnet::WriteHelper dimmer_message_helper;
    /// Allocated dimmers.
    static vector<PCPNetDimmer *> dimmers;
public:
    /// Initialization function.  Allocates a bunch of dimmers.
    /// @param node The LCC node.
    /// @param pnetstack The PNET stack.
    /// @param config The config repeated group.
    /// @param size The number of dimmers to allocate.
    /// @returns nothing.
    template <unsigned N>
          static void Init(openlcb::Node *node, 
                           pnet::PNETCanStack *pnetstack,
                           const openlcb::RepeatedGroup<PCPNetDimmerConfig, N> &config,
                           unsigned size)
    {
        HASSERT(size == N);
        openlcb::ConfigReference offset_(config);
        openlcb::RepeatedGroup<PCPNetDimmerConfig, UINT_MAX> grp_ref(offset_.offset());
        for (unsigned i = 0; i < size; i++)
        {
            dimmers.push_back(new PCPNetDimmer(node,pnetstack,grp_ref.entry(i)));
        }
    }
};

/// Static vector of allocated dimmers.
#define DIMMERS vector<PCPNetDimmer *> PCPNetDimmer::dimmers

#endif // __CONFIGUREDPCPNETDIMMER_HXX

