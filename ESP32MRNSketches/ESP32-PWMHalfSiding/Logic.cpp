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
//  Last Modified : <211128.1517>
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

#include "TrackCircuit.h"
#include "Logic.h"

void Variable::trigger(const TrackCircuit *caller,BarrierNotifiable *done)
{
    LOG(ALWAYS,"***Variable::trigger(%p,%p)",caller,done);
    bool changed = false;
    if (caller->CurrentSpeed() == speed_) // true
    {
        if (value_ != true) changed = true;
        value_ = true;
    } else {
        if (value_ != false) changed = true;
        value_ = false;
    }
    LOG(ALWAYS,"***Variable::trigger(): value_ is %d, trigger_ is %d, parent_ is %s",value_,trigger_,parent_->Description().c_str());
    switch (trigger_) {
    case Change:
        if (changed) parent_->Evaluate(which_,done);
        break;
    case Event:
        parent_->Evaluate(which_,done);
        break;
    case None:
        break;
    }
}

ConfigUpdateListener::UpdateAction Variable::apply_configuration(int fd, 
                                                                 bool initial_load,
                                                                 BarrierNotifiable *done)
{
    AutoNotify n(done);
    trigger_ = (Trigger) cfg_.trigger().read(fd);
    Source source_cfg = (Source) cfg_.source().read(fd);
    speed_ = (TrackCircuit::TrackSpeed) cfg_.trackspeed().read(fd);
    openlcb::EventId event_true_cfg = cfg_.eventtrue().read(fd);
    openlcb::EventId event_false_cfg = cfg_.eventfalse().read(fd);
    LOG(ALWAYS,"*** Variable::apply_configuration(): initial_load is %d",initial_load);
    LOG(ALWAYS,"*** Variable::apply_configuration(): source_cfg = %d, source_ = %d",source_cfg,source_);
    LOG(ALWAYS,"*** Variable::apply_configuration(): event_true_cfg is %llx, event_true_ is %llx",event_true_cfg,event_true_);
    LOG(ALWAYS,"*** Variable::apply_configuration(): event_false_cfg is %llx, event_false_ is %llx",event_false_cfg,event_false_);
    if (source_cfg != source_ ||
        event_true_cfg != event_true_ ||
        event_false_cfg != event_false_ || initial_load) {
        if (!initial_load && source_ == Events) {
            unregister_handler();
        }
        int tc = ((int) source_) -1;
        if (source_ != Events) {
            circuits[tc]->UnregisterCallback(this);
        }
        source_ = source_cfg;
        event_true_ = event_true_cfg;
        event_false_ = event_false_cfg;
        LOG(ALWAYS,"*** Variable::apply_configuration(): source_ == Events is %d",source_ == Events);
        if (source_ == Events) {
            register_handler();
            return REINIT_NEEDED; // Causes events identify.
        } else {
            tc = ((int) source_) -1;
            circuits[tc]->RegisterCallback(this);
        }
    }
    return UPDATED;
}

void Variable::factory_reset(int fd)
{
    CDI_FACTORY_RESET(cfg_.trigger);
    CDI_FACTORY_RESET(cfg_.source);
    CDI_FACTORY_RESET(cfg_.trackspeed);
}

void Variable::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                      EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node_)
    {
        done->notify();
        return;
    }
    SendAllConsumersIdentified(event,done);
    done->maybe_done();
}

void Variable::handle_event_report(const EventRegistryEntry &entry, 
                                   EventReport *event,
                                   BarrierNotifiable *done)
{
    bool maybetrigger = false;
    bool changed = false;
    if (event->event == event_true_) {
        if (value_ != true) changed = true;
        value_ = true;
        maybetrigger = true;
    } else if (event->event == event_false_) {
        if (value_ != false) changed = true;
        value_ = false;
        maybetrigger = true;
    }
    if (maybetrigger) {
        LOG(ALWAYS,"***Variable::handle_event_report(): value_ is %d, parent_ is %s",value_,parent_->Description().c_str());
        switch (trigger_) {
        case Change:
            if (changed) parent_->Evaluate(which_,done);
            break;
        case Event:
            parent_->Evaluate(which_,done);
            break;
        case None:
            break;
        }
    }
    done->maybe_done();
}

void Variable::handle_identify_consumer(const EventRegistryEntry &registry_entry,
                                        EventReport *event,
                                        BarrierNotifiable *done)
{
    SendConsumerIdentified(event,done);
    done->maybe_done();
}


void Variable::register_handler()
{
    LOG(ALWAYS,"*** Variable::register_handler(): parent_ is %s, which_ is %d, event_true_ is %llx, event_false_ is %llx",parent_->Description().c_str(),which_,event_true_,event_false_);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this, event_true_), 0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this, event_false_), 0);
}

void Variable::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}

void Variable::SendAllConsumersIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_t, mti_f;
    if (value_) {
        mti_t = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
        mti_f = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
    } else {
        mti_f = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
        mti_t = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
    }
    event->event_write_helper<1>()->WriteAsync(node_, mti_t, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_true_),
                                   done->new_child());
    event->event_write_helper<2>()->WriteAsync(node_, mti_f, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_false_),
                                   done->new_child());
}

void Variable::SendConsumerIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN;
    if (event->event == event_true_) {
        if (value_) {
            mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
        } else {
            mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
        }
    } else if (event->event == event_false_) {
        if (!value_) {
            mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
        } else {
            mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
        }
    }
    event->event_write_helper<1>()->WriteAsync(node_, mti, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event->event),
                                   done->new_child());
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
    LOG(ALWAYS,"*** Logic[%s]::eval_(%d)",Description().c_str(),v);
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
    LOG(ALWAYS,"***Logic::Evaluate() [%s] logicFunction_ = %d, v1_->Value() is %d, v2_->Value() is %d",Description().c_str(),logicFunction_,v1_->Value(),v2_->Value());
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
    LOG(ALWAYS,"***Logic::_processAction(): result is %d, action_ is %d",result,action_);
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

