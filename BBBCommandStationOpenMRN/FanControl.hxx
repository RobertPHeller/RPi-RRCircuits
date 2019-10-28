// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Oct 28 13:33:43 2019
//  Last Modified : <191028.2136>
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

#ifndef __FANCONTROL_HXX
#define __FANCONTROL_HXX

#include "openlcb/PolledProducer.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"

/// CDI Configuration for a @ref FanControl.
CDI_GROUP(FanControlConfig);
CDI_GROUP_ENTRY(alarmtemperaturethresh,
                openlcb::Uint16ConfigEntry,
                Name("Alarm Temperature threshold, in tenths of degrees Centitrade."),
                Default(350),Min(250),Max(500),
                Description("This is the temperature level to issue an event."));
CDI_GROUP_ENTRY(alarmon,
                openlcb::EventConfigEntry,
                Name("Alarm On Event"));
CDI_GROUP_ENTRY(alarmoff,
                openlcb::EventConfigEntry,
                Name("Alarm Off Event"));
CDI_GROUP_ENTRY(fantemperaturethresh,
                openlcb::Uint16ConfigEntry,
                Name("Fan Temperature threshold, in tenths of degrees Centitrade."),
                Default(250),Min(250),Max(500),
                Description("This is the temperature level to turn on the fan."));
CDI_GROUP_ENTRY(fanon,
                openlcb::EventConfigEntry,
                Name("Fan On Event"));
CDI_GROUP_ENTRY(fanoff,
                openlcb::EventConfigEntry,
                Name("Fan Off Event"));
CDI_GROUP_END();

class FanControl : public ConfigUpdateListener, public openlcb::SimpleEventHandler, public openlcb::Polling {
public:
    FanControl(openlcb::Node *node,
               const FanControlConfig &cfg,
               uint8_t temperatureAIN,
               const Gpio *fanGpio);
    template <class FAN>
          FanControl(openlcb::Node *node,
                     const FanControlConfig &cfg,
                     uint8_t temperatureAIN,
                     const FAN&,
                     const Gpio *fanGpio = FAN::instance());
    ~FanControl();
    virtual void handle_identify_global(const openlcb::EventRegistryEntry &registry_entry,
                                        openlcb::EventReport *event,
                                        BarrierNotifiable *done);
    virtual void handle_identify_producer(const openlcb::EventRegistryEntry &registry_entry,
                                          openlcb::EventReport *event, BarrierNotifiable *done);
    virtual void poll_33hz(openlcb::WriteHelper *helper, Notifiable *done);
    virtual UpdateAction apply_configuration(int fd, bool initial_load,
                                             BarrierNotifiable *done);

    virtual void factory_reset(int fd);
    bool FanOn() const {return fanon_;}
    bool AlarmOn() const {return alarmon_;}
private:
    openlcb::Node *node_;
    const FanControlConfig cfg_;
    uint8_t temperatureAIN_;
    const Gpio *fanGpio_;
    openlcb::EventId alarmon_event_;
    openlcb::EventId alarmoff_event_;
    openlcb::EventId fanon_event_;
    openlcb::EventId fanoff_event_;
    uint16_t alarmthresh_;
    uint16_t fanthresh_;
    bool registered_;
    bool fanon_;
    bool alarmon_;
    /// Requests the event associated with the current value of the bit to be
    /// produced (unconditionally): sends an event report packet ot the bus.
    ///
    /// @param writer is the output flow to be used.
    ///
    /// @param done is the notification callback. If it is NULL, the writer will
    /// be invoked inline and potentially block the calling thread.
    void SendEventReport(openlcb::WriteHelper *writer, Notifiable *done);
    /// Registers this event handler with the global event manager. Call this
    /// from the constructor of the derived class.
    void register_handler();
    /// Removes this event handler from the global event manager. Call this
    /// from the destructor of the derived class.
    void unregister_handler();
    void SendProducerIdentified(openlcb::EventReport *event, BarrierNotifiable *done);
    void SendAllProducersIdentified(openlcb::EventReport *event,BarrierNotifiable *done);
    openlcb::WriteHelper write_helper[8];                              
};            

#endif // __FANCONTROL_HXX

