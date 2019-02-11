// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Jun 11 17:32:41 2018
//  Last Modified : <190211.1411>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
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

#include "Mast.hxx"
#include "openlcb/EventService.hxx"

#ifdef LOGLEVEL
#undef LOGLEVEL
#define LOGLEVEL VERBOSE
#endif

bool MastPoints::eval()
{
    bool result = false;
    //LOG(INFO,"MastPoints::eval(): occ->read() is %d\n",occ->read());
    //LOG(INFO,"MastPoints::eval(): points->get_current_state() is %d\n",pointstate->read());
    //LOG(INFO,"MastPoints::eval(): next->Aspect() is %d\n",next->Aspect());
    if (occ->OccupiedP()) {
        result = SetAspect(Stop);
    } else if (points->get_current_state() == pointseventstate) {
        result = SetAspect(ApproachLimited);
    } else if (next->Aspect() == Stop) {
        result = SetAspect(Approach);
    } else {
        result = SetAspect(Clear);
    }
    //LOG(INFO,"MastPoints::eval(): aspect is %d\n",aspect);
    //LOG(INFO,"MastPoints::eval(): result is %d\n",result);
    switch (Aspect()) {
    case Stop:
        maingreen->clr();
        mainyellow->clr();
        mainred->set();
        divyellow->clr();
        divred->set();
        break;
    case ApproachLimited:
        maingreen->clr();
        mainyellow->clr();
        mainred->set();
        divyellow->set();
        divred->clr();
        break;
    case Approach:
        maingreen->clr();
        mainyellow->set();
        mainred->clr();
        divyellow->clr();
        divred->set();
        break;
    case Clear:
        maingreen->set();
        mainyellow->clr();
        mainred->clr();
        divyellow->clr();
        divred->set();
        break;
    }
    return result;
}

void MastPoints::SendEventReport(openlcb::WriteHelper *writer, Notifiable *done)
{
    switch (Aspect()) {
    case Stop: 
        if (event_stop != 0LL)
            writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_stop),done);
        break;
    case ApproachLimited:
        if (event_approach_limited != 0LL)
            writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_approach_limited),done);
        break;
    case Approach:
        if (event_approach != 0LL)
            writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_approach),done);
        break;
    case Clear:
        if (event_clear != 0LL)
            writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_clear),done);
        break;
    }
}

void MastPoints::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                                 EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node)
    {
        done->notify();
    }
    SendAllProducersIdentified(event,done);
    done->maybe_done();
}

void MastPoints::handle_identify_producer(const openlcb::EventRegistryEntry &registry_entry,     
                                          EventReport *event,
                                          BarrierNotifiable *done)
{
    SendProducerIdentified(event,done);
    done->maybe_done();
}

void MastPoints::SendAllProducersIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_s = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID, 
          mti_al = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID, 
          mti_a = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID, 
          mti_c = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    switch (Aspect()) {
    case Stop:
        mti_s = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    case ApproachLimited:
        mti_al = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    case Approach:
        mti_a = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    case Clear:
        mti_s = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    }
    
    if (event_stop != 0LL)
        event->event_write_helper<1>()->WriteAsync(node, mti_s, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_stop),
                                   done->new_child());
    if (event_approach_limited != 0LL)
        event->event_write_helper<2>()->WriteAsync(node, mti_al, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_approach_limited),
                                   done->new_child());
    if (event_approach != 0LL)
        event->event_write_helper<3>()->WriteAsync(node, mti_a, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_approach),
                                   done->new_child());
    if (event_clear != 0LL)
        event->event_write_helper<4>()->WriteAsync(node, mti_c, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_clear),
                                   done->new_child());
    
}

void MastPoints::SendProducerIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    if (event->event == event_stop && Aspect() == Stop) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else if (event->event == event_approach && Aspect() == Approach) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else if (event->event == event_approach_limited && Aspect() == ApproachLimited) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else if (event->event == event_clear && Aspect() == Clear) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }
    event->event_write_helper<1>()->WriteAsync(node, mti, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event->event),
                                   done->new_child());
}

void MastPoints::register_handler()
{
    if (event_stop != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, event_stop), 0);
    }
    if (event_approach != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, event_approach), 0);
    }
    if (event_approach_limited != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, event_approach_limited), 0);
    }
    if (event_clear != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, event_clear), 0);
    }
}

void MastPoints::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}


void MastPoints::factory_reset(int fd)  {
    LOG(INFO,"MastPoints::factory_reset(%d)",fd);
    config.description().write(fd, "");
}

bool MastFrog::eval()
{
    bool result = false;
    if (occ->OccupiedP() || 
        points->get_current_state() == pointseventstate) {
        result = SetAspect(Stop);
    } else if (next->Aspect() == Stop) {
        result = SetAspect(Approach);
    } else {
        result = SetAspect(Clear);
    }
    switch (Aspect()) {
    case Stop:
        green->clr();
        yellow->clr();
        red->set();
        break;
    case Approach:
        green->clr();
        yellow->set();
        red->clr();
        break;
    case Clear:
        green->set();
        yellow->clr();
        red->clr();
        break;
    default: break; // Not actually used, but needed (-Werror=switch)
    }
    return result;
}

void MastFrog::SendEventReport(openlcb::WriteHelper *writer, Notifiable *done)
{
    switch (Aspect()) {
    case Stop: 
        if (event_stop != 0LL)
            writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_stop),done);
        break;
    case Approach:
        if (event_approach != 0LL)
            writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_approach),done);
        break;
    case Clear:
        if (event_clear != 0LL)
            writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_clear),done);
        break;
    default: break; // Not actually used, but needed (-Werror=switch)
    }
}


void MastFrog::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                      EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node)
    {
        done->notify();
    }
    SendAllProducersIdentified(event,done);
    done->maybe_done();
}

void MastFrog::handle_identify_producer(const openlcb::EventRegistryEntry &registry_entry,     
                                          EventReport *event,
                                          BarrierNotifiable *done)
{
    SendProducerIdentified(event,done);
    done->maybe_done();
}

void MastFrog::SendAllProducersIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_s = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID, 
          mti_a = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID, 
          mti_c = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    switch (Aspect()) {
    case Stop:
        mti_s = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    case Approach:
        mti_a = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    case Clear:
        mti_s = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    default: break; // Not actually used, but needed (-Werror=switch)
    }
    
    if (event_stop != 0LL)
        event->event_write_helper<1>()->WriteAsync(node, mti_s, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_stop),
                                   done->new_child());
    if (event_approach != 0LL)
        event->event_write_helper<2>()->WriteAsync(node, mti_a, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_approach),
                                   done->new_child());
    if (event_clear != 0LL)
        event->event_write_helper<3>()->WriteAsync(node, mti_c, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_clear),
                                   done->new_child());
}

void MastFrog::SendProducerIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    if (event->event == event_stop && Aspect() == Stop) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else if (event->event == event_approach && Aspect() == Approach) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else if (event->event == event_clear && Aspect() == Clear) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }
    event->event_write_helper<1>()->WriteAsync(node, mti, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event->event),
                                   done->new_child());
}

void MastFrog::register_handler()
{
    if (event_stop != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, event_stop, 0), 0);
    }
    if (event_approach != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, event_approach, 0), 0);
    }
    if (event_clear != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, event_clear, 0), 0);
    }
}

void MastFrog::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}


void MastFrog::factory_reset(int fd) {
    LOG(INFO,"MastFrog::factory_reset(%d)",fd);
    config.description().write(fd,"");
}

bool MastBlock::eval()
{
    bool result = false;
    if (occ->OccupiedP()) {
        result = SetAspect(Stop);
    } else if (next->Aspect() == Stop) {
        result = SetAspect(Approach);
    } else {
        result = SetAspect(Clear);
    }
    switch (Aspect()) {
    case Stop:
        green->clr();
        yellow->clr();
        red->set();
        break;
    case Approach:
        green->clr();
        yellow->set();
        red->clr();
        break;
    case Clear:
        green->set();
        yellow->clr();
        red->clr();
        break;
    default: break; // Not actually used, but needed (-Werror=switch)
    }
    return result;
}

void MastBlock::SendEventReport(openlcb::WriteHelper *writer, Notifiable *done)
{
    switch (Aspect()) {
    case Stop: 
        if (event_stop != 0LL)
            writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_stop),done);
        break;
    case Approach:
        if (event_approach != 0LL)
            writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_approach),done);
        break;
    case Clear:
        if (event_clear != 0LL)
            writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_clear),done);
        break;
    default: break; // Not actually used, but needed (-Werror=switch)
    }
}


void MastBlock::handle_identify_global(const openlcb::EventRegistryEntry &registry_entry, 
                                      EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node)
    {
        done->notify();
    }
    SendAllProducersIdentified(event,done);
    done->maybe_done();
}

void MastBlock::handle_identify_producer(const openlcb::EventRegistryEntry &registry_entry,     
                                          EventReport *event,
                                          BarrierNotifiable *done)
{
    SendProducerIdentified(event,done);
    done->maybe_done();
}

void MastBlock::SendAllProducersIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti_s = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID, 
          mti_a = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID, 
          mti_c = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    switch (Aspect()) {
    case Stop:
        mti_s = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    case Approach:
        mti_a = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    case Clear:
        mti_s = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
        break;
    default: break; // Not actually used, but needed (-Werror=switch)
    }
    
    if (event_stop != 0LL)
        event->event_write_helper<1>()->WriteAsync(node, mti_s, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_stop),
                                   done->new_child());
    if (event_approach != 0LL)
        event->event_write_helper<2>()->WriteAsync(node, mti_a, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_approach),
                                   done->new_child());
    if (event_clear != 0LL)
        event->event_write_helper<3>()->WriteAsync(node, mti_c, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event_clear),
                                   done->new_child());
}

void MastBlock::SendProducerIdentified(EventReport *event,BarrierNotifiable *done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
    if (event->event == event_stop && Aspect() == Stop) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else if (event->event == event_approach && Aspect() == Approach) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    } else if (event->event == event_clear && Aspect() == Clear) {
        mti = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
    }
    event->event_write_helper<1>()->WriteAsync(node, mti, openlcb::WriteHelper::global(),
                                   openlcb::eventid_to_buffer(event->event),
                                   done->new_child());
}

void MastBlock::register_handler()
{
    if (event_stop != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, event_stop, 0), 0);
    }
    if (event_approach != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, event_approach, 0), 0);
    }
    if (event_clear != 0LL) {
        openlcb::EventRegistry::instance()->register_handler(
            openlcb::EventRegistryEntry(this, event_clear, 0), 0);
    }
}

void MastBlock::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}


void MastBlock::factory_reset(int fd) {
    LOG(INFO,"MastBlock::factory_reset(%d)",fd);
    config.description().write(fd,"");
}

