// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed Mar 24 09:44:24 2021
//  Last Modified : <210324.1322>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2021  Robert Heller D/B/A Deepwoods Software
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

#ifndef __BBRAILCOMDRIVER_HXX
#define __BBRAILCOMDRIVER_HXX

#include <dcc/RailCom.hxx>
#include <dcc/RailcomHub.hxx>
#include <stdint.h>
#include <freertos_drivers/common/DeviceBuffer.hxx>
#include <freertos_drivers/common/RailcomDriver.hxx>
#include <os/Gpio.hxx>

template <class HW>
class BBRailComDriver : public RailcomDriver
{
public:
    BBRailComDriver(size_t queue_size)
                : railComFeedbackBuffer_(DeviceBuffer<dcc::RailcomHubData>::create(queue_size))
    {
    }
    
    void hw_init(dcc::RailcomHubFlow *hubFlow)
    {
        railComHubFlow_ = hubFlow;
        HW::hw_init();
        uart_fd_ = HW::openport();
    }
    void disable_output()
    {
        HW::HB_BRAKE::set(true);
        usleep(1);
        enabled_= HW::HB_ENABLE::get();
        HW::HB_ENABLE::set(false);
    }
    void enable_output()
    {
        HW::HB_ENABLE::set(enabled_);
        usleep(1);
        HW::HB_BRAKE::set(false);
    }
    void start_cutout() override 
    {
        disable_output();
        usleep(HW::RAILCOM_TRIGGER_DELAY_USEC);
        rx_to_buf(nullptr, 0);
        HW::RC_ENABLE::set(true);
    }
    void middle_cutout() override
    {
    }
    void end_cutout() override
    {
        HW::RC_ENABLE::set(false);
    }
    void set_feedback_key(uint32_t key) override
    {
        railcomFeedbackKey_ = key;
    }
    void feedback_sample() override
    {
    }
    dcc::RailcomHubData *railcom_buffer()
    {
        dcc::RailcomHubData *data = nullptr;
        if (railComFeedbackBuffer_->data_write_pointer(&data) > 0)
        {
            data->reset(railcomFeedbackKey_);
        }
        return data;
    }
    void advance_railcom_buffer()
    {
        railComFeedbackBuffer_->advance(1);
        railComFeedbackBuffer_->signal_condition_from_isr();
    }
    size_t rx_to_buf(uint8_t *buf, size_t max_len)
    {
        size_t rx_bytes = 0;
        while(HW::data_avail(uart_fd_)>0) {
            uint8_t ch = HW::readbyte(uart_fd_);
            if (rx_bytes < max_len) {
                buf[rx_bytes++] = ch;
            }
        }
        return rx_bytes; 
    }
private:
    int uart_fd_;
    uintptr_t railcomFeedbackKey_{0};
    dcc::RailcomHubFlow *railComHubFlow_;
    DeviceBuffer<dcc::RailcomHubData> *railComFeedbackBuffer_;
    bool enabled_{false};
};

#endif // __BBRAILCOMDRIVER_HXX

