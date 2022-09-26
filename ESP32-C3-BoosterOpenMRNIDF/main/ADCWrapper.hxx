// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Sep 25 19:45:35 2022
//  Last Modified : <220926.0900>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2022  Robert Heller D/B/A Deepwoods Software
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

#ifndef __ADCWRAPPER_HXX
#define __ADCWRAPPER_HXX

#if CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32H2
// Missing declaration?  Assume noop for now.
#define adc_set_data_width(UNIT, BITS) (ESP_OK)
#endif

#include <freertos_drivers/arduino/DummyGPIO.hxx>
#include <freertos_drivers/esp32/Esp32Gpio.hxx>
#include <driver/adc.h>


class ADC
{
public:
    /** Constructor. It is important that this is constexpr so that the Gpio
     * object instances can be initialized by the data segment and be avalable
     * before constructor calls. */
    constexpr ADC()
    {
    }
    
    virtual int sample() const = 0;
};
    

template <class ADCPIN> class ADCWrapper : public ADC
{
public:
    /// This constructor is constexpr which ensures that the object can be
    /// initialized in the data section.
    constexpr ADCWrapper()
    {
    }
    virtual int sample() const
    {
        return ADCPIN::sample();
    }
    /// @return the static ADC object instance controlling this pin.
    static constexpr const ADC *instance()
    {
        return &instance_;
    }
    static const ADCWrapper instance_;
};

/// Defines the linker symbol for the wrapped Gpio instance.
template <class ADCPIN> const ADCWrapper<ADCPIN> ADCWrapper<ADCPIN>::instance_;

#endif // __ADCWRAPPER_HXX

