// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Feb 25 17:12:10 2019
//  Last Modified : <221217.0931>
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

#ifndef __RULE_HXX
#define __RULE_HXX

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"
#include <os/Gpio.hxx>

class Mast;
#include "Lamp.hxx"
#include "TrackCircuit.hxx"
#include "RuleConfig.hxx"

class Rule : public ConfigUpdateListener,
             public openlcb::SimpleEventHandler {
public:
    enum RuleName {Stop,TakeSiding,StopOrders,StopProcede,
                   Restricting,Permissive,SlowApproach,Slow,
                   SlowMedium,SlowLimited,SlowClear,
                   MediumApproach,MediumSlow,Medium,MediumClear,
                   MediumLimited,LimitedApproach,LimitedSlow,
                   LimitedMedium,Limited,LimitedClear,Approach,
                   AdvanceApproach,ApproachSlow,AdvanceApproachSlow,
                   ApproachMedium,AdvanceApproachMedium,
                   ApproachLimited,AdvanceApproachLimited,Clear,
                   CabSpeed,Dark};
#ifdef EFFECTS
    enum Effects {None,Transition,H2RedFlash,Strobe};
#endif
    Rule(openlcb::Node *n,const RuleConfig &cfg, Mast *parent) 
      : node_(n), cfg_(cfg) 
    {
        name_ = Stop;
        speed_ = TrackCircuit::Stop_;
#ifdef EFFECTS
        effects_ = None;
        effectsLamp_ = Lamp::Unused;
#endif
        for (int i = 0; i < LAMPCOUNT; i++) {
            lamps_[i] = new Lamp(cfg_.lamps().entry(i));
        }
        parent_ = parent;
        isSet_ = false;
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    virtual UpdateAction apply_configuration(int fd, 
                                             bool initial_load,
                                             BarrierNotifiable *done) override;
    virtual void factory_reset(int fd);
    void handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                EventReport *event, BarrierNotifiable *done) override;
    void handle_identify_producer(const EventRegistryEntry &registry_entry,
                                  EventReport *event, 
                                  BarrierNotifiable *done) override;
    void handle_event_report(const EventRegistryEntry &entry, 
                             EventReport *event,
                             BarrierNotifiable *done) override;
    void handle_identify_consumer(const EventRegistryEntry &registry_entry,
                                  EventReport *event,
                                  BarrierNotifiable *done) override;
    void ClearRule(BarrierNotifiable *done);
private:
    openlcb::Node *node_;
    const RuleConfig cfg_;
    RuleName name_;
    TrackCircuit::TrackSpeed speed_;
#ifdef EFFECTS
    Effects effects_;
    Lamp::LampID effectsLamp_;
#endif
    Lamp *lamps_[LAMPCOUNT];
    openlcb::EventId eventsets_{0},eventset_{0},eventclear_{0};
    Mast *parent_;
    bool isSet_;
    void register_handler();
    void unregister_handler();
    void SendAllConsumersIdentified(EventReport *event,BarrierNotifiable *done);
    void SendConsumerIdentified(EventReport *event,BarrierNotifiable *done);
    void SendAllProducersIdentified(EventReport *event,BarrierNotifiable *done);
    void SendProducerIdentified(EventReport *event,BarrierNotifiable *done);
    openlcb::WriteHelper write_helpers[3];
};

#endif // __RULE_HXX

