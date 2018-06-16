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
//  Last Modified : <180616.0915>
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

bool MastPoints::eval()
{
    bool result = false;
    if (occ->read() == Gpio::Value::CLR) {
        if (aspect != stop) result = true;
        aspect = stop;
    } else if (pointstate->read() == Gpio::Value::CLR) {
        if (aspect != approach_limited) result = true;
        aspect = approach_limited;
    } else if (next->read() == Gpio::Value::SET) {
        if (aspect != approach) result = true;
        aspect = approach;
    } else {
        if (aspect != clear) result = true;
        aspect = clear;
    }
    switch (aspect) {
    case stop:
        maingreen->clr();
        mainyellow->clr();
        mainred->set();
        divyellow->clr();
        divred->set();
        break;
    case approach_limited:
        maingreen->clr();
        mainyellow->clr();
        mainred->set();
        divyellow->set();
        divred->clr();
        break;
    case approach:
        maingreen->clr();
        mainyellow->set();
        mainred->clr();
        divyellow->clr();
        divred->set();
        break;
    case clear:
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
    switch (aspect) {
    case stop: 
        writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_stop),done);
        break;
    case approach_limited:
        writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_approach_limited),done);
        break;
    case approach:
        writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_approach),done);
        break;
    case clear:
        writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_clear),done);
        break;
    }
}

void MastPoints::handle_identify_global(const EventRegistryEntry &registry_entry, 
                                                 EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node)
    {
        done->notify();
    }
    SendProducerIdentified(done);
    done->maybe_done();
}

void MastPoints::SendProducerIdentified(BarrierNotifiable *done)
{
//    openlcb::Defs::MTI mti_n, mti_r;
//    if (point_state == normal) {
//        mti_n = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
//        mti_r = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
//    } else if (point_state == reversed) {
//        mti_n = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
//        mti_r = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
//    } else {
//        mti_r = mti_n = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
//    }
//    openlcb::event_write_helper1.WriteAsync(node, mti_n, openlcb::WriteHelper::global(),
//                                   openlcb::eventid_to_buffer(points_normal_event),
//                                   done->new_child());
//    openlcb::event_write_helper2.WriteAsync(node, mti_r, openlcb::WriteHelper::global(),
//                                   openlcb::eventid_to_buffer(points_reversed_event),
//                                   done->new_child());
//    
}

bool MastFrog::eval()
{
    bool result = false;
    if (occ->read() == Gpio::Value::CLR || 
        pointstate->read() == Gpio::Value::CLR) {
        if (aspect != stop) result = true;
        aspect = stop;
    } else if (next->read() == Gpio::Value::SET) {
        if (aspect != approach) result = true;
        aspect = approach;
    } else {
        if (aspect != clear) result = true;
        aspect = clear;
    }
    switch (aspect) {
    case stop:
        green->clr();
        yellow->clr();
        red->set();
        break;
    case approach:
        green->clr();
        yellow->set();
        red->clr();
        break;
    case clear:
        green->set();
        yellow->clr();
        red->clr();
        break;
    }
    return result;
}

void MastFrog::SendEventReport(openlcb::WriteHelper *writer, Notifiable *done)
{
    switch (aspect) {
    case stop: 
        writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_stop),done);
        break;
    case approach:
        writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_approach),done);
        break;
    case clear:
        writer->WriteAsync(node,openlcb::Defs::MTI_EVENT_REPORT,
                           openlcb::WriteHelper::global(),
                           openlcb::eventid_to_buffer(event_clear),done);
        break;
    }
}


void MastFrog::handle_identify_global(const EventRegistryEntry &registry_entry, 
                                                 EventReport *event, BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node)
    {
        done->notify();
    }
    SendProducerIdentified(done);
    done->maybe_done();
}

void MastFrog::SendProducerIdentified(BarrierNotifiable *done)
{
//    openlcb::Defs::MTI mti_n, mti_r;
//    if (point_state == normal) {
//        mti_n = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
//        mti_r = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
//    } else if (point_state == reversed) {
//        mti_n = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_INVALID;
//        mti_r = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_VALID;
//    } else {
//        mti_r = mti_n = openlcb::Defs::MTI_PRODUCER_IDENTIFIED_UNKNOWN;
//    }
//    openlcb::event_write_helper1.WriteAsync(node, mti_n, openlcb::WriteHelper::global(),
//                                   openlcb::eventid_to_buffer(points_normal_event),
//                                   done->new_child());
//    openlcb::event_write_helper2.WriteAsync(node, mti_r, openlcb::WriteHelper::global(),
//                                   openlcb::eventid_to_buffer(points_reversed_event),
//                                   done->new_child());
//    
}

