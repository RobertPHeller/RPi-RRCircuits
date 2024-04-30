// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Mar 19 14:31:22 2023
//  Last Modified : <240429.2116>
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
 * \file ConfiguredPCPNetTrigger.hxx
 * 
 * Configured Producer/Consumer for PNET Trigger messages.
 *
 *  <b>Configuration Options</b>
 * 
 * - Description (16 char string) User name of this Trigger.
 * - Event Produced (Event ID) (P) This event is produced when a matching Control message is received.
 * - Event Consumed (Event ID) (C) This event will cause the defined Control to be sent.
 * - Enable (Yes/No) Enable this Trigger.
 * - Slot (0-31) The Control slot number (0-31).
 * - Trigger Number (1-4) The trigger number.
 * 
 * @author Robert Heller
 * @date Sun Mar 19 14:31:22 2023
 */


#ifndef __CONFIGUREDPCPNETTRIGGER_HXX
#define __CONFIGUREDPCPNETTRIGGER_HXX

#include "openlcb/Node.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "PNETStack.hxx"
#include "PNETWriteHelper.hxx"
#include "PNETTriggerService.hxx"

/// CDI Configuration for a @ref PCPNetTrigger.
CDI_GROUP(PCPNetTriggerConfig);
/// Allows the user to assign a name for this PCPNetTrigger.
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<16>, //
                Name("Description"), 
                Description("User name of this trigger."));
/// Event produced (sent) when a PNet Trigger message is received on the PNet.
CDI_GROUP_ENTRY(event_produced, openlcb::EventConfigEntry,
                Name("Event Produced"),
                Description("(P) This event is produced when a matching Trigger message is received."));
/// Event consumed that sends a PNet Trigger message onto the PNet.
CDI_GROUP_ENTRY(event_consumed, openlcb::EventConfigEntry,
                Name("Event Consumed"),
                Description("(C) This event will cause the defined Trigger to be sent."));
/// Flag to enable or disable this PCPNetTrigger.
CDI_GROUP_ENTRY(enabled, openlcb::Uint8ConfigEntry,
                Min(0), Max(1), Default(0),
                MapValues("<relation><property>0</property><value>No</value></relation>"
                          "<relation><property>1</property><value>Yes</value></relation>"),
                Name("Enable"), Description("Enable this trigger."));
/// The Slot value.
CDI_GROUP_ENTRY(slot, openlcb::Uint8ConfigEntry,
                Min(0), Max(31), Default(0),
                Name("Slot"),
                Description("The trigger slot number (0-31)."));
/// The Trigger number.
CDI_GROUP_ENTRY(trigger, openlcb::Uint8ConfigEntry,
                Min(1), Max(4), Default(1),
                Name("Trigger Number"),
                Description("The trigger number."));
CDI_GROUP_END();

/// OpenLCB Producer/Consumer class integrating a PNET Trigger message
/// for two event IDs, one to be sent when a matching PNET Trigger 
/// message is received and one to cause a PNET Trigger message to
/// sent.
class PCPNetTrigger : public DefaultConfigUpdateListener,
                           public openlcb::SimpleEventHandler,
                           public pnet::TriggerProcess
{
public:
    /// Constructor:
    /// @param node LCC Node object.
    /// @param pnetstack PNET Stack (PNET does not have nodes).
    /// @param cfg Configuration for this PCPNetTrigger.
    /// @returns a new PCPNetTrigger object.
    PCPNetTrigger(openlcb::Node *node, pnet::PNETCanStack *pnetstack,
                  const PCPNetTriggerConfig &cfg)
                : DefaultConfigUpdateListener()
          , node_(node)
          , cfg_(cfg)
          , pnetstack_(pnetstack)
          , event_produced_(0)
          , event_consumed_(0)
          , enabled_(false)
          , slot_(0)
          , trigger_(1)
    {
        if (enabled_) register_trigger_handler();
    }
    /// Destructor:
    /// Unregisters handler.
    ~PCPNetTrigger()
    {
        if (enabled_) unregister_trigger_handler();
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
        const uint8_t cfg_trigger = cfg_.trigger().read(fd);
        bool cfg_enabled = (bool) cfg_.enabled().read(fd);
        if (cfg_slot != slot_ || cfg_trigger != trigger_ ||
            cfg_enabled != enabled_)
        {
            if (enabled_) unregister_trigger_handler();
            slot_ = cfg_slot;
            trigger_ = cfg_trigger;
            enabled_ = cfg_enabled;
            if (enabled_) register_trigger_handler();
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
        CDI_FACTORY_RESET(cfg_.trigger);
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
    /// Sends a PNET Trigger message.
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
            pnet::TriggerData td(slot_, trigger_);
            pnet::GenMessage m;
            td.FillMessage(&m);
            trigger_message_helper.WriteAsync(pnetstack_,
                                              m.identifier,
                                              m.payload,
                                              done);
        }
        else
        {
            done->notify();
        }
    }
    /// Process a trigger message.
    /// Produces (sends) an event report message.
    /// @param td Trigger Data
    /// @param done  Notification object.
    /// @returns nothing.
    void process_trigger(const pnet::TriggerData &td,
                         BarrierNotifiable *done) override
    {
        trigger_event_helper.WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
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
    /// Register a trigger message handler.
    void register_trigger_handler()
    {
        pnet::TriggerHandler::instance()->register_handler(pnet::TriggerRegistryEntry(this,pnet::TriggerData(slot_,trigger_)));
    }
    /// Unregister a trigger message handler.
    void unregister_trigger_handler()
    {
        pnet::TriggerHandler::instance()->unregister_handler(pnet::TriggerRegistryEntry(this,pnet::TriggerData(slot_,trigger_)));
    }
    /// Node.
    openlcb::Node *node_;
    /// Configuration.
    const PCPNetTriggerConfig cfg_;
    /// PNET Stack
    pnet::PNETCanStack *pnetstack_;
    /// Event ID to produce.
    openlcb::EventId event_produced_;
    /// Event ID to consume.
    openlcb::EventId event_consumed_;
    /// Enable flag
    bool enabled_;
    /// Trigger slot.
    uint8_t slot_;
    /// Trigger number.
    uint8_t trigger_;
    /// OpenLCB message write helper.
    openlcb::WriteHelper trigger_event_helper;
    /// PNET message write helper.
    pnet::WriteHelper trigger_message_helper;
    /// Allocated triggers.
    static vector<PCPNetTrigger *> triggers;
public:
    /// Initialization function.  Allocates a bunch of triggers.
    /// @param node The LCC node.
    /// @param pnetstack The PNET stack.
    /// @param config The config repeated group.
    /// @param size The number of triggers to allocate.
    /// @returns nothing.
    template <unsigned N>
          static void Init(openlcb::Node *node, 
                           pnet::PNETCanStack *pnetstack,
                           const openlcb::RepeatedGroup<PCPNetTriggerConfig, N> &config,
                           unsigned size)
    {
        HASSERT(size == N);
        openlcb::ConfigReference offset_(config);
        openlcb::RepeatedGroup<PCPNetTriggerConfig, UINT_MAX> grp_ref(offset_.offset());
        for (unsigned i = 0; i < size; i++)
        {
            triggers.push_back(new PCPNetTrigger(node,pnetstack,grp_ref.entry(i)));
        }
    }
};

/// Static vector of allocated triggers.
#define TRIGGERS vector<PCPNetTrigger *> PCPNetTrigger::triggers

#endif // __CONFIGUREDPCPNETTRIGGER_HXX

