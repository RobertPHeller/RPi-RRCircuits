// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Feb 25 15:59:18 2019
//  Last Modified : <220926.1507>
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

#ifndef __MAST_HXX
#define __MAST_HXX

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"
#include <os/Gpio.hxx>
#include <stdio.h>

#include "Lamp.hxx"
#include "Rule.hxx"
#include "TrackCircuit.hxx"

#define MASTCOUNT 8

static const char MastProcessingMap[] = 
"<relation><property>0</property><value>Unused</value></relation>"
"<relation><property>1</property><value>Normal</value></relation>"
"<relation><property>2</property><value>Linked to previous</value></relation>";


static const char LampFadeMap[] =
"<relation><property>0</property><value>None</value></relation>"
"<relation><property>1</property><value>Incandescent</value></relation>";



/// CDI Configuration for a @ref Mast
CDI_GROUP(MastConfig);
CDI_GROUP_ENTRY(processing,openlcb::Uint8ConfigEntry,
                Name("Function"),Default(0),
                Description("Mast Processing"),
                MapValues(MastProcessingMap));
CDI_GROUP_ENTRY(mastid, openlcb::StringConfigEntry<8>,
                Name("Mast ID"));
CDI_GROUP_ENTRY(linkevent,openlcb::EventConfigEntry,
                Name("(P) Track Circuit Link Address. Copy and Paste into linked Track Circuit. (Read Only)"));
#define TRACKCIRCUITBASE 2048 // Leave lots of room
#ifdef HAVEPWM
CDI_GROUP_ENTRY(fade,openlcb::Uint8ConfigEntry,
                Name("Lamp Fade"),Default(0),
                MapValues(LampFadeMap));
#endif
CDI_GROUP_ENTRY(rules,RulesGroup,Name("Rules"),RepName("Rule"));
CDI_GROUP_END();


class Mast : public ConfigUpdateListener, 
             public openlcb::SimpleEventHandler {
public:
    enum MastProcessing {Unused, Normal, Linked};
#ifdef HAVEPWM
    enum LampFade {None, Incandescent};
#endif
    Mast(openlcb::Node *n,const MastConfig &cfg, Mast *previous) 
      : node_(n), cfg_(cfg)
    {
//        fprintf(stderr,"*** Mast::Mast(): cfg.offset() is %d, cfg_.offset() is %d\n",cfg.offset(),cfg_.offset());
//        fprintf(stderr,"*** Mast::Mast(): &cfg_ is %p\n", &cfg_);
        processing_ = Unused;
#ifdef HAVEPWM
        fade_ = None;
#endif
        previous_ = previous;
        currentRule_  = nullptr;
        currentSpeed_ = TrackCircuit::Stop_;
        for (int i = 0; i < RULESCOUNT; i++) {
            rules_[i] = new Rule(node_,cfg_.rules().entry(i),this);
        }
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    virtual UpdateAction apply_configuration(int fd, 
                                             bool initial_load,
                                             BarrierNotifiable *done) override;
    virtual void factory_reset(int fd) override;
    void handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                EventReport *event, BarrierNotifiable *done) override;
    void handle_identify_producer(const EventRegistryEntry &registry_entry,
                                  EventReport *event, 
                                  BarrierNotifiable *done) override;
     void ClearCurrentRule(BarrierNotifiable *done);
     void SetCurrentRuleAndSpeed(Rule *r, TrackCircuit::TrackSpeed s, 
                                 BarrierNotifiable *done);
     const std::string Mastid() const {return mastid_;}
private:
    openlcb::Node *node_;
    const MastConfig cfg_;
    MastProcessing processing_;
#ifdef HAVEPWM
    LampFade fade_;
#endif
    openlcb::EventId linkevent_;
    static uint16_t baseLinkEvent_;
    Rule *rules_[RULESCOUNT];
    Rule *currentRule_;
    TrackCircuit::TrackSpeed currentSpeed_;
    Mast *previous_;
    void register_handler();
    void unregister_handler();
    void SendAllProducersIdentified(EventReport *event,BarrierNotifiable *done);
    void SendProducerIdentified(EventReport *event,BarrierNotifiable *done);
    openlcb::WriteHelper write_helper[8];
    std::string mastid_{""};
};


#endif // __MAST_HXX

