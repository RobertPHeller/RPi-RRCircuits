// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Feb 17 15:35:50 2019
//  Last Modified : <190219.1354>
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

#include "PWMTest.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/EventService.hxx"

ConfigUpdateListener::UpdateAction ConfiguredPWMConsumer::apply_configuration(int fd, 
                                                                              bool initial_load,
                                                                              BarrierNotifiable *done)
{
    AutoNotify n(done);
    openlcb::EventId cfg_pwm_zero_event = config.event_pwm_zero().read(fd);
    openlcb::EventId cfg_pwm_one_event = config.event_pwm_one().read(fd);
    openlcb::EventId cfg_pwm_two_event = config.event_pwm_two().read(fd);
    openlcb::EventId cfg_pwm_three_event = config.event_pwm_three().read(fd);
    openlcb::EventId cfg_pwm_four_event = config.event_pwm_four().read(fd);
    openlcb::EventId cfg_pwm_five_event = config.event_pwm_five().read(fd);
    openlcb::EventId cfg_pwm_six_event = config.event_pwm_six().read(fd);
    openlcb::EventId cfg_pwm_seven_event = config.event_pwm_seven().read(fd);
    openlcb::EventId cfg_pwm_eight_event = config.event_pwm_eight().read(fd);
    openlcb::EventId cfg_pwm_nine_event = config.event_pwm_nine().read(fd);
    openlcb::EventId cfg_pwm_ten_event = config.event_pwm_ten().read(fd);
    period = config.pwm_period().read(fd);
    pwm_->set_period(period);
    zero_duty = config.pwm_zero().read(fd);
    one_duty = config.pwm_one().read(fd);
    two_duty = config.pwm_two().read(fd);
    three_duty = config.pwm_three().read(fd);
    four_duty = config.pwm_four().read(fd);
    five_duty = config.pwm_five().read(fd);
    six_duty = config.pwm_six().read(fd);
    seven_duty = config.pwm_seven().read(fd);
    eight_duty = config.pwm_eight().read(fd);
    nine_duty = config.pwm_nine().read(fd);
    ten_duty = config.pwm_ten().read(fd);
    if (cfg_pwm_zero_event != pwm_zero_event ||
        cfg_pwm_one_event != pwm_one_event ||
        cfg_pwm_two_event != pwm_two_event ||
        cfg_pwm_three_event != pwm_three_event ||
        cfg_pwm_four_event != pwm_four_event ||
        cfg_pwm_five_event != pwm_five_event ||
        cfg_pwm_six_event != pwm_six_event ||
        cfg_pwm_seven_event != pwm_seven_event ||
        cfg_pwm_eight_event != pwm_eight_event ||
        cfg_pwm_nine_event != pwm_nine_event ||
        cfg_pwm_ten_event != pwm_ten_event) {
        if (!initial_load) unregister_handler();
        pwm_zero_event = cfg_pwm_zero_event;
        pwm_one_event = cfg_pwm_one_event;
        pwm_two_event = cfg_pwm_two_event;
        pwm_three_event = cfg_pwm_three_event;
        pwm_four_event = cfg_pwm_four_event;
        pwm_five_event = cfg_pwm_five_event;
        pwm_six_event = cfg_pwm_six_event;
        pwm_seven_event = cfg_pwm_seven_event;
        pwm_eight_event = cfg_pwm_eight_event;
        pwm_nine_event = cfg_pwm_nine_event;
        pwm_ten_event = cfg_pwm_ten_event;
        register_handler();
        return REINIT_NEEDED; // Causes events identify.
    }
    return UPDATED;
}

void ConfiguredPWMConsumer::factory_reset(int fd)
{
    config.description().write(fd,"");
    CDI_FACTORY_RESET(config.pwm_period);
    CDI_FACTORY_RESET(config.pwm_zero);
    CDI_FACTORY_RESET(config.pwm_one);
    CDI_FACTORY_RESET(config.pwm_two);
    CDI_FACTORY_RESET(config.pwm_three);
    CDI_FACTORY_RESET(config.pwm_four);
    CDI_FACTORY_RESET(config.pwm_five);
    CDI_FACTORY_RESET(config.pwm_six);
    CDI_FACTORY_RESET(config.pwm_seven);
    CDI_FACTORY_RESET(config.pwm_eight);
    CDI_FACTORY_RESET(config.pwm_nine);
    CDI_FACTORY_RESET(config.pwm_ten);
}

void ConfiguredPWMConsumer::handle_event_report(const EventRegistryEntry &entry,
                                                EventReport *event,
                                                BarrierNotifiable *done)
{
    if (event->event == pwm_zero_event) {
        pwm_->set_duty(zero_duty);
    } else if (event->event == pwm_one_event) {
        pwm_->set_duty(one_duty);
    } else if (event->event == pwm_two_event) {
        pwm_->set_duty(two_duty);
    } else if (event->event == pwm_three_event) {
        pwm_->set_duty(three_duty);
    } else if (event->event == pwm_four_event) {
        pwm_->set_duty(four_duty);
    } else if (event->event == pwm_five_event) {
        pwm_->set_duty(five_duty);
    } else if (event->event == pwm_six_event) {
        pwm_->set_duty(six_duty);
    } else if (event->event == pwm_seven_event) {
        pwm_->set_duty(seven_duty);
    } else if (event->event == pwm_eight_event) {
        pwm_->set_duty(eight_duty);
    } else if (event->event == pwm_nine_event) {
        pwm_->set_duty(nine_duty);
    } else if (event->event == pwm_ten_event) {
        pwm_->set_duty(ten_duty);
    }
    done->notify();
}

void ConfiguredPWMConsumer::handle_identify_global(const EventRegistryEntry               
                                                   &registry_entry,                       
                                                   EventReport *event,                    
                                                   BarrierNotifiable *done)
{
    if (event->dst_node && event->dst_node != node_)
    {
        done->notify();
    }
    SendAllConsumersIdentified(event,done);
    done->maybe_done();
}

void ConfiguredPWMConsumer::SendAllConsumersIdentified(openlcb::EventReport* event, BarrierNotifiable* done)
{
    openlcb::Defs::MTI mti_zero = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID, 
          mti_one = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID, 
          mti_two = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID, 
          mti_three = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID, 
          mti_four = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID, 
          mti_five = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID, 
          mti_six = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID, 
          mti_seven = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID, 
          mti_eight = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID, 
          mti_nine = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID, 
          mti_ten = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
    uint32_t current_duty_cycle = /* pwm_->get_duty() */ 0;
    if (current_duty_cycle == zero_duty) {
        mti_zero = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (current_duty_cycle == one_duty) {
        mti_one = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (current_duty_cycle == two_duty) {
        mti_two = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (current_duty_cycle == three_duty) {
        mti_three = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (current_duty_cycle == four_duty) {
        mti_four = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (current_duty_cycle == five_duty) {
        mti_five = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (current_duty_cycle == six_duty) {
        mti_six = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (current_duty_cycle == seven_duty) {
        mti_seven = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (current_duty_cycle == eight_duty) {
        mti_eight = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (current_duty_cycle == nine_duty) {
        mti_nine = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (current_duty_cycle == ten_duty) {
        mti_ten = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    }
    write_helpers[0].WriteAsync(node_,mti_zero,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(pwm_zero_event),
                                done->new_child());
    write_helpers[1].WriteAsync(node_,mti_one,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(pwm_one_event),
                                done->new_child());
    write_helpers[2].WriteAsync(node_,mti_two,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(pwm_two_event),
                                done->new_child());
    write_helpers[3].WriteAsync(node_,mti_three,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(pwm_three_event),
                                done->new_child());
    write_helpers[4].WriteAsync(node_,mti_four,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(pwm_four_event),
                                done->new_child());
    write_helpers[5].WriteAsync(node_,mti_five,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(pwm_five_event),
                                done->new_child());
    write_helpers[6].WriteAsync(node_,mti_six,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(pwm_six_event),
                                done->new_child());
    write_helpers[7].WriteAsync(node_,mti_seven,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(pwm_seven_event),
                                done->new_child());
    write_helpers[8].WriteAsync(node_,mti_eight,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(pwm_eight_event),
                                done->new_child());
    write_helpers[9].WriteAsync(node_,mti_nine,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(pwm_nine_event),
                                done->new_child());
    write_helpers[10].WriteAsync(node_,mti_ten,
                                openlcb::WriteHelper::global(),
                                openlcb::eventid_to_buffer(pwm_ten_event),
                                done->new_child());
        
    
}

void ConfiguredPWMConsumer::handle_identify_consumer(const EventRegistryEntry             
                                                     &registry_entry,                     
                                                     EventReport *event,                  
                                                     BarrierNotifiable *done)
{
    SendConsumerIdentified(event,done);
    done->maybe_done();
}

void ConfiguredPWMConsumer::SendConsumerIdentified(openlcb::EventReport* event, BarrierNotifiable* done)
{
    openlcb::Defs::MTI mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_INVALID;
    uint32_t current_duty_cycle = /* pwm_->get_duty() */ 0;
    if (event->event == pwm_zero_event &&
        current_duty_cycle == zero_duty) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (event->event == pwm_one_event &&
               current_duty_cycle == one_duty) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (event->event == pwm_two_event &&
               current_duty_cycle == two_duty) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (event->event == pwm_three_event &&
               current_duty_cycle == three_duty) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (event->event == pwm_four_event &&
               current_duty_cycle == four_duty) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (event->event == pwm_five_event &&
               current_duty_cycle == five_duty) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (event->event == pwm_six_event &&
               current_duty_cycle == six_duty) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (event->event == pwm_seven_event &&
               current_duty_cycle == seven_duty) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (event->event == pwm_eight_event &&
               current_duty_cycle == eight_duty) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (event->event == pwm_nine_event &&
               current_duty_cycle == nine_duty) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else if (event->event == pwm_ten_event &&
               current_duty_cycle == ten_duty) {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_VALID;
    } else {
        mti = openlcb::Defs::MTI_CONSUMER_IDENTIFIED_UNKNOWN;
    }
    event->event_write_helper<1>()->WriteAsync(node_, mti, 
                                               openlcb::WriteHelper::global(),
                                               openlcb::eventid_to_buffer(event->event),
                                               done->new_child());
}

void ConfiguredPWMConsumer::register_handler()
{
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,pwm_zero_event),0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,pwm_one_event),0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,pwm_two_event),0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,pwm_three_event),0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,pwm_four_event),0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,pwm_five_event),0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,pwm_six_event),0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,pwm_seven_event),0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,pwm_eight_event),0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,pwm_nine_event),0);
    openlcb::EventRegistry::instance()->register_handler(
        openlcb::EventRegistryEntry(this,pwm_ten_event),0);
}

void ConfiguredPWMConsumer::unregister_handler()
{
    openlcb::EventRegistry::instance()->unregister_handler(this);
}


