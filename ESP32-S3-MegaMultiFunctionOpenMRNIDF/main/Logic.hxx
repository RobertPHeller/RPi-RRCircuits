// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Feb 27 14:08:16 2019
//  Last Modified : <221012.1506>
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

#ifndef __LOGIC_HXX
#define __LOGIC_HXX

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "openlcb/SimpleStack.hxx"
#include "executor/Timer.hxx"
#include "executor/Notifiable.hxx"
#include <stdio.h>
#include "utils/Uninitialized.hxx"
#include "utils/logging.h"
#include <string>
#include "hardware.hxx"
#include "TrackCircuit.hxx"

#ifdef PRODUCTION
#define LOGICCOUNT 64
#else
#define LOGICCOUNT 32
#endif

static const char GroupFunctionMap[] = 
"<relation><property>0</property><value>Blocked</value></relation>"
"<relation><property>1</property><value>Group</value></relation>"
"<relation><property>2</property><value>Last (Single)</value></relation>";

static const char LogicFunctionMap[] = 
"<relation><property>0</property><value>V1 AND V2</value></relation>"
"<relation><property>1</property><value>V1 OR V2</value></relation>"
"<relation><property>2</property><value>V1 XOR V2</value></relation>"
"<relation><property>3</property><value>V1 AND V2 => Change</value></relation>"
"<relation><property>4</property><value>V1 OR V2 => Change</value></relation>"
"<relation><property>5</property><value>V1 AND then V2 => true</value></relation>"
"<relation><property>6</property><value>V1 only</value></relation>"
"<relation><property>7</property><value>V2 only</value></relation>"
"<relation><property>8</property><value>null => true</value></relation>";

static const char ActionMap[] =
"<relation><property>0</property><value>Send then Exit Group</value></relation>"
"<relation><property>1</property><value>Send then Evaluate Next</value></relation>"
"<relation><property>2</property><value>Exit Group</value></relation>"
"<relation><property>3</property><value>Evaluate Next</value></relation>";

static const char VariableTriggerMap[] = 
"<relation><property>0</property><value>On Variable Change</value></relation>"
"<relation><property>1</property><value>On Matching Event</value></relation>"
"<relation><property>2</property><value>None</value></relation>";


static const char VariableSourceMap[] = 
"<relation><property>0</property><value>Use Variable's (C) Events</value></relation>"
"<relation><property>1</property><value>Track Circuit 1</value></relation>"
"<relation><property>2</property><value>Track Circuit 2</value></relation>"
"<relation><property>3</property><value>Track Circuit 3</value></relation>"
"<relation><property>4</property><value>Track Circuit 4</value></relation>"
"<relation><property>5</property><value>Track Circuit 5</value></relation>"
"<relation><property>6</property><value>Track Circuit 6</value></relation>"
"<relation><property>7</property><value>Track Circuit 7</value></relation>"
"<relation><property>8</property><value>Track Circuit 8</value></relation>"
#if TRACKCIRCUITCOUNT == 16
"<relation><property>9</property><value>Track Circuit 9</value></relation>"
"<relation><property>10</property><value>Track Circuit 10</value></relation>"
"<relation><property>11</property><value>Track Circuit 11</value></relation>"
"<relation><property>12</property><value>Track Circuit 12</value></relation>"
"<relation><property>13</property><value>Track Circuit 13</value></relation>"
"<relation><property>14</property><value>Track Circuit 14</value></relation>"
"<relation><property>15</property><value>Track Circuit 15</value></relation>"
"<relation><property>16</property><value>Track Circuit 16</value></relation>"
#endif
;

static const char IntervalMap[] = 
"<relation><property>0</property><value>Milliseconds</value></relation>"
"<relation><property>1</property><value>Seconds</value></relation>"
"<relation><property>2</property><value>Minutes</value></relation>";

static const char RetriggerableMap[] = 
"<relation><property>0</property><value>No</value></relation>"
"<relation><property>1</property><value>Yes</value></relation>"; 

static const char ActionTriggerMap[] =
"<relation><property>0</property><value>None</value></relation>"
"<relation><property>1</property><value>Immediately</value></relation>"
"<relation><property>2</property><value>After delay</value></relation>"
"<relation><property>3</property><value>Immediate if True</value></relation>"
"<relation><property>4</property><value>Immediate if False</value></relation>"
"<relation><property>5</property><value>Delayed if True</value></relation>"
"<relation><property>6</property><value>Delayed if False</value></relation>";


CDI_GROUP(VariableConfig);
CDI_GROUP_ENTRY(trigger,openlcb::Uint8ConfigEntry,
                Name("Variable Trigger"),Default(0),
                MapValues(VariableTriggerMap));
CDI_GROUP_ENTRY(source,openlcb::Uint8ConfigEntry,
                Name("Variable Source"),Default(0),
                MapValues(VariableSourceMap));
CDI_GROUP_ENTRY(trackspeed,openlcb::Uint8ConfigEntry,
                Name("Variable Track Speed"),Default(0),
                MapValues(TrackSpeedMap));
CDI_GROUP_ENTRY(eventtrue,openlcb::EventConfigEntry,
                Name("(C) Event to set variable true."));
CDI_GROUP_ENTRY(eventfalse,openlcb::EventConfigEntry,
                Name("(C) Event to set variable false."));
CDI_GROUP_END();

class Variable;

class BitEventConsumerOrTrackCircuit 
      : public openlcb::BitEventHandler, public TrackCircuitCallback
{
public:
    enum Source {Events, TrackCircuit1, TrackCircuit2, TrackCircuit3,
              TrackCircuit4, TrackCircuit5, TrackCircuit6, TrackCircuit7,
              TrackCircuit8,
#if TRACKCIRCUITCOUNT == 16
              TrackCircuit9, TrackCircuit10, TrackCircuit11, TrackCircuit12,
              TrackCircuit13, TrackCircuit14, TrackCircuit15, TrackCircuit16,
#endif
              MAX_SOURCE};
    BitEventConsumerOrTrackCircuit(openlcb::BitEventInterface *bit,  
                                   Source source, 
                                   TrackCircuit::TrackSpeed speed,
                                   Variable *parent) 
                : BitEventHandler(bit)
          , parent_(parent)
          , source_(source)
          , speed_(speed)
    {
        //LOG(ALWAYS,"*** BitEventConsumerOrTrackCircuit::BitEventConsumerOrTrackCircuit(%p,%d,%d)",bit,source,speed);
        switch (source_) {
        default: break;
        case Events: 
            register_handler(bit->event_on(), bit->event_off()); 
            break;
        case TrackCircuit1: 
            TrackCircuit::circuits[0]->RegisterCallback(this);
            break;
        case TrackCircuit2: 
            TrackCircuit::circuits[1]->RegisterCallback(this);
            break;
        case TrackCircuit3: 
            TrackCircuit::circuits[2]->RegisterCallback(this);
            break;
        case TrackCircuit4: 
            TrackCircuit::circuits[3]->RegisterCallback(this);
            break;
        case TrackCircuit5: 
            TrackCircuit::circuits[4]->RegisterCallback(this);
            break;
        case TrackCircuit6: 
            TrackCircuit::circuits[5]->RegisterCallback(this);
            break;
        case TrackCircuit7: 
            TrackCircuit::circuits[6]->RegisterCallback(this);
            break;
        case TrackCircuit8: 
            TrackCircuit::circuits[7]->RegisterCallback(this);
            break;
#if TRACKCIRCUITCOUNT == 16
        case TrackCircuit9: 
            TrackCircuit::circuits[8]->RegisterCallback(this);
            break;
        case TrackCircuit10: 
            TrackCircuit::circuits[9]->RegisterCallback(this);
            break;
        case TrackCircuit11: 
            TrackCircuit::circuits[10]->RegisterCallback(this);
            break;
        case TrackCircuit12: 
            TrackCircuit::circuits[11]->RegisterCallback(this);
            break;
        case TrackCircuit13: 
            TrackCircuit::circuits[12]->RegisterCallback(this);
            break;
        case TrackCircuit14: 
            TrackCircuit::circuits[13]->RegisterCallback(this);
            break;
        case TrackCircuit15: 
            TrackCircuit::circuits[14]->RegisterCallback(this);
            break;
        case TrackCircuit16: 
            TrackCircuit::circuits[15]->RegisterCallback(this);
            break;
#endif
        }
    }
    ~BitEventConsumerOrTrackCircuit()
    {
        //LOG(ALWAYS,"*** BitEventConsumerOrTrackCircuit::~BitEventConsumerOrTrackCircuit()");

        switch (source_) {
        default: break;
        case Events: 
            unregister_handler(); 
            break;
        case TrackCircuit1: 
            TrackCircuit::circuits[0]->UnregisterCallback(this);
            break;
        case TrackCircuit2: 
            TrackCircuit::circuits[1]->UnregisterCallback(this);
            break;
        case TrackCircuit3: 
            TrackCircuit::circuits[2]->UnregisterCallback(this);
            break;
        case TrackCircuit4: 
            TrackCircuit::circuits[3]->UnregisterCallback(this);
            break;
        case TrackCircuit5: 
            TrackCircuit::circuits[4]->UnregisterCallback(this);
            break;
        case TrackCircuit6: 
            TrackCircuit::circuits[5]->UnregisterCallback(this);
            break;
        case TrackCircuit7: 
            TrackCircuit::circuits[6]->UnregisterCallback(this);
            break;
        case TrackCircuit8: 
            TrackCircuit::circuits[7]->UnregisterCallback(this);
            break;
#if TRACKCIRCUITCOUNT == 16
        case TrackCircuit9: 
            TrackCircuit::circuits[8]->UnregisterCallback(this);
            break;
        case TrackCircuit10: 
            TrackCircuit::circuits[9]->UnregisterCallback(this);
            break;
        case TrackCircuit11: 
            TrackCircuit::circuits[10]->UnregisterCallback(this);
            break;
        case TrackCircuit12: 
            TrackCircuit::circuits[11]->UnregisterCallback(this);
            break;
        case TrackCircuit13: 
            TrackCircuit::circuits[12]->UnregisterCallback(this);
            break;
        case TrackCircuit14: 
            TrackCircuit::circuits[13]->UnregisterCallback(this);
            break;
        case TrackCircuit15: 
            TrackCircuit::circuits[14]->UnregisterCallback(this);
            break;
        case TrackCircuit16: 
            TrackCircuit::circuits[15]->UnregisterCallback(this);
            break;
#endif
        }
    }

    /// Queries producers and acquires the current state of the bit.
    void SendQuery(openlcb::WriteHelper *writer1, 
                   openlcb::WriteHelper *writer2, 
                   BarrierNotifiable *done);

    void handle_event_report(const openlcb::EventRegistryEntry &entry, EventReport *event,
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
    void trigger(const TrackCircuit *caller,BarrierNotifiable *done) override;
    Source TheSource () const  {return source_;}
    TrackCircuit::TrackSpeed Speed () const {return speed_;}
private:
    Variable *parent_;
    Source source_:5;
    TrackCircuit::TrackSpeed speed_:4;
};

class LogicCallback {
public:
    enum Which {V1, V2, Unknown};
    virtual void Evaluate(Which v,BarrierNotifiable *done) = 0;
    virtual const std::string Description() const = 0;
};

class Variable {
public:
    using Impl = openlcb::NetworkInitializedBit;
    
    enum Trigger {Change, Event, None};
    Variable(openlcb::Node *n, LogicCallback *p, const LogicCallback::Which which)
                : node_(n)
          , parent_(p)
          , impl_(node_, 0, 0, false)
          , consumer_(&impl_, 
                      BitEventConsumerOrTrackCircuit::Source::Events, 
                      TrackCircuit::TrackSpeed::Stop_,
                      nullptr)
          , which_(which)
    {
    }
    bool apply_configuration(int fd, 
                             bool initial_load,
                             const VariableConfig &cfg);
    void factory_reset(int fd,const VariableConfig &cfg);
    bool IsKnown() {
        //LOG(ALWAYS,"*** Variable::IsKnown()");
        return impl_.is_network_state_known();
    }
    bool Value() {
        //LOG(ALWAYS,"*** Variable::Value()");
        return impl_.get_local_state();
    }
    void Apply(BarrierNotifiable *done)
    {
        //LOG(ALWAYS,"*** Variable::Apply(%p)",done);
        parent_->Evaluate(which_,done);
    }
    void SendQuery(openlcb::WriteHelper *writer1,
                   openlcb::WriteHelper *writer2,
                   BarrierNotifiable *done)
    {
        consumer_.SendQuery(writer1,writer2,done);
    }
    BitEventConsumerOrTrackCircuit::Source TheSource()
    {
        return consumer_.TheSource();
    }
private:
    openlcb::Node *node_;
    LogicCallback *parent_;
    Impl impl_;
    BitEventConsumerOrTrackCircuit consumer_;
    const LogicCallback::Which which_:2;
    Trigger trigger_:2;
};


CDI_GROUP(LogicOperatorConfig);
CDI_GROUP_ENTRY(logicFunction,openlcb::Uint8ConfigEntry,
                Name("Logic function"),Default(0),
                MapValues(LogicFunctionMap));
CDI_GROUP_END();



CDI_GROUP(TimingConfig)
CDI_GROUP_ENTRY(timedelay,openlcb::Uint16ConfigEntry,
                Description("Time Delay before action\n"
                            "Delay Time (1-60000)."),
                Default(0),Min(0),Max(60000));
CDI_GROUP_ENTRY(interval,openlcb::Uint8ConfigEntry,
                Name("Interval"),Default(0),MapValues(IntervalMap));
CDI_GROUP_ENTRY(retriggerable,openlcb::Uint8ConfigEntry,
                Name("Retriggerable"),Default(0),
                MapValues(RetriggerableMap));
CDI_GROUP_END();

class ActionTrigger {
public:
    virtual void trigger(BarrierNotifiable *done) = 0;
};

class Timing : public Timer {
public:
    enum Interval {Milliseconds, Seconds, Minutes};
    Timing (ActiveTimers *timers) : Timer(timers)
    {
        running_ = false;
    }
    long long timeout() override
    {
        running_ = false;
        for (actionVector_type_iterator i = actions_.begin();
             i != actions_.end(); i++) {
            (*i)->trigger((BarrierNotifiable *)this);
        }
        return NONE;
    }
    void apply_configuration(int fd, 
                             bool initial_load,
                             const TimingConfig &cfg)
    {
        timedelay_ = cfg.timedelay().read(fd);
        interval_ = (Interval) cfg.interval().read(fd);
        retriggerable_ = (cfg.retriggerable().read(fd) != 0);
    }
    void factory_reset(int fd,const TimingConfig &cfg) {
        CDI_FACTORY_RESET(cfg.timedelay);
        CDI_FACTORY_RESET(cfg.interval);
        CDI_FACTORY_RESET(cfg.retriggerable);
    }
    void AddDelayedAction(ActionTrigger *a) {
        actions_.push_back(a);
    }
    void RemoveDelayedAction(ActionTrigger *a) {
        for (actionVector_type_iterator i = actions_.begin();
             i != actions_.end(); i++) {
            if (*i == a) actions_.erase(i);
            return;
        }
    }
    void startDelay() {
        if (running_) {
            if (retriggerable_) restart();
            return;
        }
        long long p = 0LL;
        switch (interval_) {
        case Milliseconds: 
            p = timedelay_ * 1000000ULL;
            break;
        case Seconds:
            p = timedelay_ * 1000000000ULL;
            break;
        case Minutes:
            p = timedelay_ * 1000000000ULL * 60;
            break;
        }
        start(p);
        running_ = true;
    }
private:
    typedef vector<ActionTrigger *> actionVector_type;
    typedef actionVector_type::iterator actionVector_type_iterator;
    actionVector_type actions_;
    uint16_t timedelay_;
    Interval interval_:2;
    bool running_:1;
    bool retriggerable_:1;
};

CDI_GROUP(ActionConfig);
CDI_GROUP_ENTRY(actiontrigger,openlcb::Uint8ConfigEntry,
                MapValues(ActionTriggerMap),Default(0));
CDI_GROUP_ENTRY(actionevent,openlcb::EventConfigEntry,
                Name("(P) this event will be sent."));
CDI_GROUP_END();

using ActionGroup = openlcb::RepeatedGroup<ActionConfig,4>;

class Action : public ActionTrigger, public openlcb::SimpleEventHandler {
public:
    enum Trigger {None, Immediately, AfterDelay, ImmediateTrue, 
              ImmediateFalse, DelayedTrue, DelayedFalse};
    Action(openlcb::Node *n, Timing *timer) 
                : node_(n), timer_(timer)
    {
    }
    void trigger(BarrierNotifiable *done) override;
    bool DoAction(bool logicResult,BarrierNotifiable *done);
    bool apply_configuration(int fd, bool initial_load, 
                             const ActionConfig &cfg);
    void factory_reset(int fd,const ActionConfig &cfg);
    void handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                EventReport *event, BarrierNotifiable *done) override
    {
        if (event->dst_node && event->dst_node != node_)
        {
            return done->notify();
        }
        event->event_write_helper<1>()->WriteAsync(node_,
           openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN,
           openlcb::WriteHelper::global(), openlcb::eventid_to_buffer(action_event_), done);
    }
    void handle_identify_producer(const EventRegistryEntry &registry_entry,
                                  EventReport *event,
                                  BarrierNotifiable *done) override
    {
        if (event->dst_node && event->dst_node != node_)
        {
            return done->notify();
        }
        if (event->event == action_event_)
        {
            event->event_write_helper<1>()->WriteAsync(node_,
               openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN,
               openlcb::WriteHelper::global(), openlcb::eventid_to_buffer(action_event_), done);
        }
        else {
            return done->notify();
        }
    }
private:
    openlcb::Node *node_;
    Timing *timer_;
    openlcb::EventId action_event_;
    Trigger actionTrigger_:3;
    bool lastLogicValue_:1;
    void SendEventReport(BarrierNotifiable *done);
    static openlcb::WriteHelper write_helper;
};

/// CDI Configuration for a @ref Logic
CDI_GROUP(LogicConfig);
CDI_GROUP_ENTRY(description,openlcb::StringConfigEntry<32>,
                Name("Logic description"));
CDI_GROUP_ENTRY(groupFunction,openlcb::Uint8ConfigEntry,
                Name("Group Function"),Default(0),
                MapValues(GroupFunctionMap));
CDI_GROUP_ENTRY(v1,VariableConfig,Name("Variable #1"));
CDI_GROUP_ENTRY(logic,LogicOperatorConfig);
CDI_GROUP_ENTRY(v2,VariableConfig,Name("Variable #2"));
CDI_GROUP_ENTRY(trueAction,openlcb::Uint8ConfigEntry,
                Name("Action when Conditional = True"),
                Default(0),
                MapValues(ActionMap));
CDI_GROUP_ENTRY(falseAction,openlcb::Uint8ConfigEntry,
                Name("Action when Conditional = False"),
                Default(3),
                MapValues(ActionMap));
CDI_GROUP_ENTRY(timing,TimingConfig);
CDI_GROUP_ENTRY(actions,ActionGroup,Name("A trigger or change will generate the following events."),RepName("Action"));
CDI_GROUP_END();

class Logic : public LogicCallback, public ConfigUpdateListener {
public:
    enum GroupFunction {Blocked,Group,Last};
    enum LogicFunction {AND, OR, XOR, ANDChange, ORChange, ANDthenV2, V1, V2, True};
    enum ActionType {SendExitGroup, SendEvaluateNext, ExitGroup, EvaluateNext};
    Logic (openlcb::Node *node, const LogicConfig &cfg, ActiveTimers *timers, Logic *next) 
                : node_(node), cfg_(cfg), next_(next)
    {
        previous_ = nullptr;
        v1_.emplace(node_,this,LogicCallback::V1);
        v2_.emplace(node_,this,LogicCallback::V2);
        timer_.emplace(timers);
        for (int i = 0; i < 4; i++) {
            actions_[i].emplace(node_,timer_.get_mutable());
        }
        if (next_ != nullptr) {
            next_->_setPrevious(this);
        }
        oldValue_ = false;
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    virtual UpdateAction apply_configuration(int fd, 
                                             bool initial_load,
                                             BarrierNotifiable *done) override;
    virtual void factory_reset(int fd) override;
    virtual void Evaluate(Which v,BarrierNotifiable *done) override;
    virtual const std::string Description() const override {return description_;}
    bool Value();
    __attribute__((noinline))
          static void Init(openlcb::Node *node,
                           ActiveTimers *timers,
                           const openlcb::RepeatedGroup<LogicConfig, LOGICCOUNT> &config);
private:
    bool eval_(Which v);
    void _processAction(BarrierNotifiable *done);
    void _setPrevious(Logic *p) {previous_ = p;}
    Logic *_topOfGroup() {
        //LOG(ALWAYS,"***Logic::_topOfGroup() [%p]",this);
        Logic *top = this;
        //LOG(ALWAYS,"***Logic::_topOfGroup(): top is %p",top);
        while (top->previous_ != nullptr && 
               top->previous_->groupFunction_ == Group) {
            top = top->previous_;
            //LOG(ALWAYS,"***Logic::_topOfGroup() in while: top is %p",top);
        }
        return top;
    }
    openlcb::Node *node_;
    const LogicConfig cfg_;
    Logic *next_, *previous_;
    uninitialized<Variable> v1_, v2_;
    uninitialized<Timing> timer_;
    uninitialized<Action> actions_[4];
    std::string description_{""};
    GroupFunction groupFunction_:2;
    LogicFunction logicFunction_:4;
    ActionType trueAction_:2, falseAction_:2;
    bool oldValue_:1;
    static uninitialized<Logic> logics[LOGICCOUNT];
};

#endif // __LOGIC_HXX

