// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri Mar 1 10:46:51 2019
//  Last Modified : <220623.1532>
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

static const char rcsid[] = "@(#) : $Id$";

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "openlcb/SimpleStack.hxx"
#include "executor/Timer.hxx"
#include <stdio.h>

#include "TrackCircuit.hxx"
#include "Logic.hxx"

void BitEventConsumerOrTrackCircuit::handle_producer_identified(const EventRegistryEntry& entry, EventReport *event,
                                                BarrierNotifiable *done)
{
    //LOG(ALWAYS,"*** BitEventConsumerOrTrackCircuit::handle_producer_identified()");
    bool value;
    if (event->state == openlcb::EventState::VALID)
    {
        value = true;
    }
    else if (event->state == openlcb::EventState::INVALID)
    {
        value = false;
    }
    else
    {
        done->notify();
        return; // nothing to learn from this message.
    }
    if (event->event == bit_->event_on())
    {
        bit_->set_state(value);
        if (parent_)
        {
            parent_->Apply(done);
        }
    }
    else if (event->event == bit_->event_off())
    {
        bit_->set_state(!value);
        if (parent_)
        {
            parent_->Apply(done);
        }
    }
    else
    {
        done->notify();
        return; // uninteresting event id.
    }
    done->notify();
}

void BitEventConsumerOrTrackCircuit::SendQuery(openlcb::WriteHelper *writer1,
                                               openlcb::WriteHelper *writer2,
                                               BarrierNotifiable *done)
{
    //LOG(ALWAYS,"*** BitEventConsumerOrTrackCircuit::SendQuery()");
    writer1->WriteAsync(bit_->node(), openlcb::Defs::MTI_PRODUCER_IDENTIFY,
                       openlcb::WriteHelper::global(),
                       openlcb::eventid_to_buffer(bit_->event_on()), 
                       done->new_child());
    writer2->WriteAsync(bit_->node(), openlcb::Defs::MTI_PRODUCER_IDENTIFY,
                       openlcb::WriteHelper::global(),
                       openlcb::eventid_to_buffer(bit_->event_off()), 
                       done->new_child());
}

void BitEventConsumerOrTrackCircuit::handle_event_report(const openlcb::EventRegistryEntry& entry, openlcb::EventReport *event,
                                         BarrierNotifiable *done)
{
    //LOG(ALWAYS,"*** BitEventConsumerOrTrackCircuit::handle_event_report()");
    if (event->event == bit_->event_on())
    {
        bit_->set_state(true);
        if (parent_)
        {
            parent_->Apply(done);
        }
    }
    else if (event->event == bit_->event_off())
    {
        bit_->set_state(false);
        if (parent_)
        {
            parent_->Apply(done);
        }
    }
    done->notify();
}

void BitEventConsumerOrTrackCircuit::handle_identify_consumer(const openlcb::EventRegistryEntry& entry, openlcb::EventReport *event,
                                              BarrierNotifiable *done)
{
    //LOG(ALWAYS,"*** BitEventConsumerOrTrackCircuit::handle_identify_consumer()");
    HandlePCIdentify(openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID, event, done);
}

void BitEventConsumerOrTrackCircuit::handle_identify_global(const openlcb::EventRegistryEntry& entry, openlcb::EventReport *event,
                                            BarrierNotifiable *done)
{
    //LOG(ALWAYS,"*** BitEventConsumerOrTrackCircuit::handle_identify_global()");
    if (event->dst_node && event->dst_node != bit_->node())
    {
        return done->notify();
    }
    SendConsumerIdentified(event, done);
    done->maybe_done();
}



void BitEventConsumerOrTrackCircuit::trigger(const TrackCircuit *caller,BarrierNotifiable *done)
{
    //LOG(ALWAYS,"*** BitEventConsumerOrTrackCircuit::trigger(%p,%p)",caller,done);
    if (caller->CurrentSpeed() == speed_) // true
    {
        bit_->set_state(true);
        if (parent_)
        {
            parent_->Apply(done);
        }
    } else {
        bit_->set_state(false);
        if (parent_)
        {
            parent_->Apply(done);
        }
    }
    //LOG(ALWAYS,"*** BitEventConsumerOrTrackCircuit::trigger(): bit was set");
    done->maybe_done();
    //LOG(ALWAYS,"*** BitEventConsumerOrTrackCircuit::trigger(): done was notified");
}

ConfigUpdateListener::UpdateAction Variable::apply_configuration(int fd, 
                                                                 bool initial_load,
                                                                 BarrierNotifiable *done)
{
    AutoNotify n(done);
    trigger_ = (Trigger) cfg_.trigger().read(fd);
    BitEventConsumerOrTrackCircuit::Source source_cfg = (BitEventConsumerOrTrackCircuit::Source) cfg_.source().read(fd);
    TrackCircuit::TrackSpeed speed_cfg = (TrackCircuit::TrackSpeed) cfg_.trackspeed().read(fd);
    openlcb::EventId event_true_cfg = cfg_.eventtrue().read(fd);
    openlcb::EventId event_false_cfg = cfg_.eventfalse().read(fd);
    //LOG(ALWAYS,"*** Variable::apply_configuration(): initial_load is %d",initial_load);
    //LOG(ALWAYS,"*** Variable::apply_configuration(): source_cfg = %d, source_ = %d",source_cfg,consumer_.TheSource());
    //LOG(ALWAYS,"*** Variable::apply_configuration(): event_true_cfg is %llx, event_true_ is %llx",event_true_cfg,impl_.event_on());
    //LOG(ALWAYS,"*** Variable::apply_configuration(): event_false_cfg is %llx, event_false_ is %llx",event_false_cfg,impl_.event_off());
    
    if (source_cfg != consumer_.TheSource() ||
        speed_cfg != consumer_.Speed() ||
        event_true_cfg != impl_.event_on() ||
        event_false_cfg != impl_.event_off())
    {
        auto saved_node = impl_.node();
        consumer_.~BitEventConsumerOrTrackCircuit();
        impl_.Impl::~Impl();
        new (&impl_)
              Impl(saved_node, event_true_cfg, event_false_cfg, false);
        new (&consumer_) BitEventConsumerOrTrackCircuit(&impl_,
                                                        source_cfg, 
                                                        speed_cfg,
                                                        this);
        
        return REINIT_NEEDED; // Causes events identify.
    }
    return UPDATED;
}

void Variable::factory_reset(int fd)
{
    CDI_FACTORY_RESET(cfg_.trigger);
    CDI_FACTORY_RESET(cfg_.source);
    CDI_FACTORY_RESET(cfg_.trackspeed);
}

void Action::trigger(BarrierNotifiable *done)
{
    switch (actionTrigger_) {
    case None:
    case Immediately:
    case ImmediateTrue: 
    case ImmediateFalse:
        break;
    case AfterDelay:
        SendEventReport(done);
        break;
    case DelayedTrue:
        if (lastLogicValue_) {
            SendEventReport(done);
        }
        break;
    case DelayedFalse:
        if (!lastLogicValue_) {
            SendEventReport(done);
        }
        break;
    }
}

bool Action::DoAction(bool logicResult,BarrierNotifiable *done)
{
    switch (actionTrigger_) {
    case None: break;
    case Immediately:
        SendEventReport(done);
        break;
    case ImmediateTrue:
        if (logicResult) {
            SendEventReport(done);
        }
        break;
    case ImmediateFalse:
        if (!logicResult) {
            SendEventReport(done);
        }
        break;
    default:
        lastLogicValue_ = logicResult;
        return true;
        break;
    }
    return false;
}

ConfigUpdateListener::UpdateAction Action::apply_configuration(int fd, 
                                                               bool initial_load,
                                                               BarrierNotifiable *done)
{
    AutoNotify n(done);
    Trigger actionTrigger_cfg = (Trigger) cfg_.actiontrigger().read(fd);
    openlcb::EventId action_event_cfg = cfg_.actionevent().read(fd);
    if (actionTrigger_cfg != actionTrigger_ ||
        action_event_cfg != action_event_ ) {
        switch (actionTrigger_) {
        case AfterDelay:
        case DelayedTrue:
        case DelayedFalse:
            timer_->RemoveDelayedAction(this);
            break;
        default: break;
        }
        actionTrigger_ = actionTrigger_cfg;
        action_event_  = action_event_cfg;
        switch (actionTrigger_) {
        case None: break;
        case AfterDelay:
        case DelayedTrue:
        case DelayedFalse:
            timer_->AddDelayedAction(this);
        default: 
            return REINIT_NEEDED; // Causes events identify.
        }
    }
    return UPDATED;
}

void Action::factory_reset(int fd)
{
    CDI_FACTORY_RESET(cfg_.actiontrigger);
}

void Action::SendEventReport(BarrierNotifiable *done)
{
    write_helper.WriteAsync(node_,openlcb::Defs::MTI_EVENT_REPORT,
                            openlcb::WriteHelper::global(),
                            openlcb::eventid_to_buffer(action_event_),
                            done->new_child());
}

ConfigUpdateListener::UpdateAction Logic::apply_configuration(int fd, 
                                                              bool initial_load,
                                                              BarrierNotifiable *done)
{
    AutoNotify n(done);
    groupFunction_ = (GroupFunction) cfg_.groupFunction().read(fd);
    logicFunction_ = (LogicFunction) cfg_.logic().logicFunction().read(fd);
    trueAction_    = (ActionType)    cfg_.trueAction().read(fd);
    falseAction_   = (ActionType)    cfg_.falseAction().read(fd);
    description_ = cfg_.description().read(fd);
    return UPDATED;
}

void Logic::factory_reset(int fd)
{
    cfg_.description().write(fd,"");
    CDI_FACTORY_RESET(cfg_.groupFunction);
    CDI_FACTORY_RESET(cfg_.logic().logicFunction);
    CDI_FACTORY_RESET(cfg_.trueAction);
    CDI_FACTORY_RESET(cfg_.falseAction);
}

bool Logic::Value()
{
    return eval_(LogicCallback::Unknown);
}

bool Logic::eval_(Which v)
{
    //LOG(ALWAYS,"*** Logic[%s]::eval_(%d)",Description().c_str(),v);
    bool result;
    bool newresult;
    switch (logicFunction_) {
    case AND:
        result = v1_->Value() && v2_->Value();
        break;
    case OR:
        result = v1_->Value() || v2_->Value();
        break;
    case XOR:
        result =  ((v1_->Value() && !v2_->Value()) || ((v2_->Value() && !v1_->Value())));
        break;
    case ANDChange:
        newresult = v1_->Value() && v2_->Value();
        result = newresult != oldValue_;
        oldValue_ = newresult;
        break;
    case ORChange:
        newresult = v1_->Value() || v2_->Value();
        result = newresult != oldValue_;        
        oldValue_ = newresult;
        break;
    case ANDthenV2:
        if (oldValue_ && v1_->Value()) 
        { 
            result = oldValue_;
        }
        else
        {
            result = (v1_->Value() && v2_->Value() && v == LogicCallback::V2);
            oldValue_ = result;
        }
        break;
    case V1:
        result = (v1_->Value() != 0);
        break;
    case V2:
        result = (v2_->Value() != 0);
    case True:
        result = true;
        break;
    }
    return result;
}

void Logic::Evaluate(Which v,BarrierNotifiable *done)
{
    //LOG(ALWAYS,"***Logic::Evaluate() [%s] logicFunction_ = %d, v1_->Value() is %d, v2_->Value() is %d",Description().c_str(),logicFunction_,v1_->Value(),v2_->Value());
    if (groupFunction_ == Blocked) return;
    eval_(v);
    if (groupFunction_ == Group) {
        _topOfGroup()->_processAction(done);
    } else {
        _processAction(done);
    }
}

void Logic::_processAction(BarrierNotifiable *done)
{
    ActionType action_;
    bool result = Value();
    if (result) {
        action_ = trueAction_;
    } else {
        action_ = falseAction_;
    }
    //LOG(ALWAYS,"***Logic::_processAction(): result is %d, action_ is %d",result,action_);
    bool delayStarted = false;
    int i;
    switch (action_) {
    case SendExitGroup:
        for (i = 0; i < 4; i++) {
            if (actions_[i]->DoAction(result,done) && !delayStarted) {
                timer_->startDelay();
                delayStarted = true;
            }
        }
        break;
    case SendEvaluateNext:
        for (i = 0; i < 4; i++) {
            if (actions_[i]->DoAction(result,done) && !delayStarted) {
                timer_->startDelay();
                delayStarted = true;
            }
        }
        if (groupFunction_ == Group && next_ != nullptr) {
            next_->_processAction(done);
        }
        break;
    case ExitGroup:
        break;
    case EvaluateNext:
        if (groupFunction_ == Group && next_ != nullptr) {
            next_->_processAction(done);
        }
        break;
    }
}

