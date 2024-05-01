// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Mar 20 12:18:05 2023
//  Last Modified : <240501.0909>
//
//  Description	
//
//  Notes
//
//  History
//	
//////////////////////////////////////////////////////////////////////////////
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
 * \file ConfiguredPCPNetControl.hxx
 * 
 * @author Robert Heller
 * @date Mon Mar 20 12:18:05 2023
 */

/** @page ConfiguredPCPNetControl PNET Control Configuration
 * 
 * 
 * Configured Producer/Consumer for PNET Control messages.
 *
 * PNET Control messages either turn outputs steady on, pulsed on, or off.
 * Upto 32 PNET Control messages can be configured and enabled.  
 * 
 *  @section ControlOpts Configuration Options
 * 
 * Each message has these configuration options:
 * 
 * - Description (16 char string) User name of this Control.
 *   This is just a name for the message for identifing purposes.
 * - Event Produced (Event ID) (P) This event is produced when a matching Control message is received.
 *   This event id is sent by the router onto the LCC network when a PNET node
 *   sends a matching Control message.
 * - Event Consumed (Event ID) (C) This event will cause the defined Control to be sent.
 *   When the node receives this event id, it sends a PNET Control message out
 *   on the PNET network.
 * - Enable (Yes/No) Enable this Control.
 *   This enables or disables this message.
 * - Slot (0-31) The Control slot number (0-31).
 *   This the slot number (board id) of the message.
 * - Control Number (0-255) The control number.
 *   This is the control number (output number) of the message.
 * - Attributes (0-255) The control attributes.
 *   This is the control's attributes.  Zero (0) turns the output off, 255
 *   turns the output steady on. A value in between pulses the output for that
 *   number of milliseconds.
 * 
 * 
 * 
 */

#ifndef __CONFIGUREDPCPNETCONTROL_HXX
#define __CONFIGUREDPCPNETCONTROL_HXX

#include "openlcb/Node.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "PNETStack.hxx"
#include "PNETWriteHelper.hxx"
#include "PNETControlService.hxx"

/// CDI Configuration for a @ref PCPNetControl.
CDI_GROUP(PCPNetControlConfig);
/// Allows the user to assign a name for this PCPNetControl.
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<16>, //
                Name("Description"), 
                Description("User name of this Control."));
/// Event produced (sent) when a PNet Control message is received on the PNet.
CDI_GROUP_ENTRY(event_produced, openlcb::EventConfigEntry,
                Name("Event Produced"),
                Description("(P) This event is produced when a matching Control message is received."));
/// Event consumed that sends a PNet Control message onto the PNet.
CDI_GROUP_ENTRY(event_consumed, openlcb::EventConfigEntry,
                Name("Event Consumed"),
                Description("(C) This event will cause the defined Control to be sent."));
/// Flag to enable or disable this PCPNetControl.
CDI_GROUP_ENTRY(enabled, openlcb::Uint8ConfigEntry,
                Min(0), Max(1), Default(0),
                MapValues("<relation><property>0</property><value>No</value></relation>"
                          "<relation><property>1</property><value>Yes</value></relation>"),
                Name("Enable"), Description("Enable this Control."));
/// The Slot value.
CDI_GROUP_ENTRY(slot, openlcb::Uint8ConfigEntry,
                Min(0), Max(31), Default(0),
                Name("Slot"),
                Description("The Control slot number (0-31)."));
/// The control number.
CDI_GROUP_ENTRY(value, openlcb::Uint8ConfigEntry,
                Default(0),
                Name("Control Number"),
                Description("The control number."));
/// The Control attributes.
CDI_GROUP_ENTRY(attributes, openlcb::Uint8ConfigEntry,
                Default(0),
                Name("Attributes"),
                Description("The control attributes."));
CDI_GROUP_END();

/// OpenLCB Producer/Consumer class integrating a PNET Control message
/// for two event IDs, one to be sent when a matching PNET Control 
/// message is received and one to cause a PNET Control message to
/// sent.

class PCPNetControl : public DefaultConfigUpdateListener,
                           public openlcb::SimpleEventHandler,
                           public pnet::ControlProcess
{
public:
    /// Constructor:
    /// @param node LCC Node object.
    /// @param pnetstack PNET Stack (PNET does not have nodes).
    /// @param cfg Configuration for this PCPNetControl.
    /// @returns a new PCPNetControl object.
    PCPNetControl(openlcb::Node *node, pnet::PNETCanStack *pnetstack,
                  const PCPNetControlConfig &cfg)
                : DefaultConfigUpdateListener()
          , node_(node)
          , cfg_(cfg)
          , pnetstack_(pnetstack)
          , event_produced_(0)
          , event_consumed_(0)
          , enabled_(false)
          , slot_(0)
          , value_(0)
          , attributes_(0)
    {
        if (enabled_) register_control_handler();
    }
    /// Destructor:
    /// Unregisters handler.
    ~PCPNetControl()
    {
        if (enabled_) unregister_control_handler();
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
        const uint8_t cfg_value = cfg_.value().read(fd);
        const uint8_t cfg_attributes = cfg_.attributes().read(fd);
        bool cfg_enabled = (bool) cfg_.enabled().read(fd);
        if (cfg_slot != slot_ || cfg_value != value_ ||
            cfg_attributes != attributes_ ||
            cfg_enabled != enabled_)
        {
            if (enabled_) unregister_control_handler();
            slot_ = cfg_slot;
            value_ = cfg_value;
            attributes_ = cfg_attributes;
            enabled_ = cfg_enabled;
            if (enabled_) register_control_handler();
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
        CDI_FACTORY_RESET(cfg_.value);
        CDI_FACTORY_RESET(cfg_.attributes);
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
    /// Sends a PNET Control message.
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
            pnet::ControlData td(slot_, value_, attributes_);
            pnet::GenMessage m;
            td.FillMessage(&m);
            control_message_helper.WriteAsync(pnetstack_,
                                              m.identifier,
                                              m.payload,
                                              done);
        }
        else
        {
            done->notify();
        }
    }
    /// Process a control message.
    /// Produces (sends) an event report message.
    /// @param td Control Data
    /// @param done  Notification object.
    /// @returns nothing.
    void process_control(const pnet::ControlData &td,
                         BarrierNotifiable *done) override
    {
        control_event_helper.WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
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
    /// Register a control message handler.
    void register_control_handler()
    {
        pnet::ControlHandler::instance()->register_handler(pnet::ControlRegistryEntry(this,pnet::ControlData(slot_,value_,attributes_)));
    }
    /// Unregister a control message handler.
    void unregister_control_handler()
    {
        pnet::ControlHandler::instance()->unregister_handler(pnet::ControlRegistryEntry(this,pnet::ControlData(slot_,value_,attributes_)));
    }
    /// Node.
    openlcb::Node *node_;
    /// Configuration.
    const PCPNetControlConfig cfg_;
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
    /// Control number.
    uint8_t value_;
    /// Control attributes.
    uint8_t attributes_;
    /// OpenLCB message write helper.
    openlcb::WriteHelper control_event_helper;
    /// PNET message write helper.
    pnet::WriteHelper control_message_helper;
    /// Allocated controls.
    static vector<PCPNetControl *> controls;
public:
    /// Initialization function.  Allocates a bunch of controls.
    /// @param node The LCC node.
    /// @param pnetstack The PNET stack.
    /// @param config The config repeated group.
    /// @param size The number of controls to allocate.
    /// @returns nothing.
    template <unsigned N> 
          static void Init(openlcb::Node *node, 
                           pnet::PNETCanStack *pnetstack,
                           const openlcb::RepeatedGroup<PCPNetControlConfig, N> &config,
                           unsigned size)
    {
        HASSERT(size == N);
        openlcb::ConfigReference offset_(config);
        openlcb::RepeatedGroup<PCPNetControlConfig, UINT_MAX> grp_ref(offset_.offset());
        for (unsigned i = 0; i < size; i++)
        {
            controls.push_back(new PCPNetControl(node,pnetstack,grp_ref.entry(i)));
        }
    }
};

/// Static vector of allocated controls.
#define CONTROLS vector<PCPNetControl *> PCPNetControl::controls

#endif // __CONFIGUREDPCPNETCONTROL_HXX

