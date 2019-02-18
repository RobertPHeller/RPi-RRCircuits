// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Feb 17 13:33:29 2019
//  Last Modified : <190217.1903>
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

#ifndef __PWMTEST_HXX
#define __PWMTEST_HXX

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"
#include "os/LinuxPWM.hxx"

CDI_GROUP(PWMConsumerConfig);
CDI_GROUP_ENTRY(description, openlcb::StringConfigEntry<16>, Name("Description"),
                Description("User name of this output."));
CDI_GROUP_ENTRY(pwm_period, openlcb::Uint32ConfigEntry, //
                Name("PWM period"), Default(1000000), 
                Description("PWM Period in Nanoseconds."));
CDI_GROUP_ENTRY(event_pwm_zero, openlcb::EventConfigEntry, //
    Name("PWM zero Event ID"),
    Description("Receiving this event ID will set the duty cycle to its zeroth "
                "configured point."));
CDI_GROUP_ENTRY(pwm_zero, openlcb::Uint32ConfigEntry, //
                Name("PWM zero point"), Default(0), 
                Description("PWM Zero point"));
CDI_GROUP_ENTRY(event_pwm_one, openlcb::EventConfigEntry, //
    Name("PWM one Event ID"),
    Description("Receiving this event ID will set the duty cycle to its first "
                "configured point."));
CDI_GROUP_ENTRY(pwm_one, openlcb::Uint32ConfigEntry, //
                Name("PWM one point"), Default(100000), 
                Description("PWM One point"));
CDI_GROUP_ENTRY(event_pwm_two, openlcb::EventConfigEntry, //
    Name("PWM two Event ID"),
    Description("Receiving this event ID will set the duty cycle to its second "
                "configured point."));
CDI_GROUP_ENTRY(pwm_two, openlcb::Uint32ConfigEntry, //
                Name("PWM two point"), Default(200000), 
                Description("PWM Two point"));
CDI_GROUP_ENTRY(event_pwm_three, openlcb::EventConfigEntry, //
    Name("PWM three Event ID"),
    Description("Receiving this event ID will set the duty cycle to its third "
                "configured point."));
CDI_GROUP_ENTRY(pwm_three, openlcb::Uint32ConfigEntry, //
                Name("PWM three point"), Default(300000), 
                Description("PWM Three point"));
CDI_GROUP_ENTRY(event_pwm_four, openlcb::EventConfigEntry, //
    Name("PWM four Event ID"),
    Description("Receiving this event ID will set the duty cycle to its fourth "
                "configured point."));
CDI_GROUP_ENTRY(pwm_four, openlcb::Uint32ConfigEntry, //
                Name("PWM four point"), Default(400000), 
                Description("PWM Four point"));
CDI_GROUP_ENTRY(event_pwm_five, openlcb::EventConfigEntry, //
    Name("PWM five Event ID"),
    Description("Receiving this event ID will set the duty cycle to its fifth "
                "configured point."));
CDI_GROUP_ENTRY(pwm_five, openlcb::Uint32ConfigEntry, //
                Name("PWM five point"), Default(500000), 
                Description("PWM Five point"));
CDI_GROUP_ENTRY(event_pwm_six, openlcb::EventConfigEntry, //
    Name("PWM six Event ID"),
    Description("Receiving this event ID will set the duty cycle to its sixth "
                "configured point."));
CDI_GROUP_ENTRY(pwm_six, openlcb::Uint32ConfigEntry, //
                Name("PWM six point"), Default(600000), 
                Description("PWM Six point"));
CDI_GROUP_ENTRY(event_pwm_seven, openlcb::EventConfigEntry, //
    Name("PWM seven Event ID"),
    Description("Receiving this event ID will set the duty cycle to its seventh "
                "configured point."));
CDI_GROUP_ENTRY(pwm_seven, openlcb::Uint32ConfigEntry, //
                Name("PWM seven point"), Default(700000), 
                Description("PWM Seven point"));
CDI_GROUP_ENTRY(event_pwm_eight, openlcb::EventConfigEntry, //
    Name("PWM eight Event ID"),
    Description("Receiving this event ID will set the duty cycle to its eighth "
                "configured point."));
CDI_GROUP_ENTRY(pwm_eight, openlcb::Uint32ConfigEntry, //
                Name("PWM eight point"), Default(800000), 
                Description("PWM Eight point"));
CDI_GROUP_ENTRY(event_pwm_nine, openlcb::EventConfigEntry, //
    Name("PWM nine Event ID"),
    Description("Receiving this event ID will set the duty cycle to its ninth "
                "configured point."));
CDI_GROUP_ENTRY(pwm_nine, openlcb::Uint32ConfigEntry, //
                Name("PWM nine point"), Default(900000), 
                Description("PWM Nine point"));
CDI_GROUP_ENTRY(event_pwm_ten, openlcb::EventConfigEntry, //
    Name("PWM ten Event ID"),
    Description("Receiving this event ID will set the duty cycle to its tenth "
                "configured point."));
CDI_GROUP_ENTRY(pwm_ten, openlcb::Uint32ConfigEntry, //
                Name("PWM ten point"), Default(1000000), 
                Description("PWM Ten point"));
CDI_GROUP_END(); // PWMConsumerConfig

class ConfiguredPWMConsumer : public ConfigUpdateListener, 
                              public openlcb::SimpleEventHandler
{
public:
    ConfiguredPWMConsumer(openlcb::Node *node, const PWMConsumerConfig &cfg, 
                          const PWM *pwm)
                : node_(node)
          , pwm_(pwm)
          , config(cfg)
    {
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    UpdateAction apply_configuration(int fd, bool initial_load,
                                         BarrierNotifiable *done) OVERRIDE;
    void factory_reset(int fd) OVERRIDE;
    void handle_event_report(const EventRegistryEntry &entry,
                             EventReport *event,
                             BarrierNotifiable *done) override;
    void handle_identify_global(const EventRegistryEntry 
                                &registry_entry, 
                                EventReport *event, 
                                BarrierNotifiable *done)
          OVERRIDE;
    void handle_identify_consumer(const EventRegistryEntry 
                                  &registry_entry,
                                  EventReport *event,
                                  BarrierNotifiable *done) override;
    void register_handler();
    void unregister_handler();
private:
    openlcb::Node *node_;
    const PWM *pwm_;
    const PWMConsumerConfig config;
    openlcb::EventId pwm_zero_event, pwm_one_event, pwm_two_event, 
          pwm_three_event, pwm_four_event, pwm_five_event, 
          pwm_six_event, pwm_seven_event, pwm_eight_event, 
          pwm_nine_event, pwm_ten_event;
    uint32_t period, zero_duty, one_duty, two_duty, three_duty, 
          four_duty, five_duty, six_duty, seven_duty, eight_duty, 
          nine_duty, ten_duty;
    void SendAllConsumersIdentified(EventReport *event,BarrierNotifiable *done);
    void SendConsumerIdentified(EventReport *event,BarrierNotifiable *done);
    openlcb::WriteHelper write_helpers[11];
};



#endif // __PWMTEST_HXX

