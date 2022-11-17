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
//  Last Modified : <221117.1550>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//! @copyright
//!    Copyright (C) 2021  Robert Heller D/B/A Deepwoods Software
//!			51 Locke Hill Road
//!			Wendell, MA 01379-9728
//!
//!    This program is free software; you can redistribute it and/or modify
//!    it under the terms of the GNU General Public License as published by
//!    the Free Software Foundation; either version 2 of the License, or
//!    (at your option) any later version.
//!
//!    This program is distributed in the hope that it will be useful,
//!    but WITHOUT ANY WARRANTY; without even the implied warranty of
//!    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//!    GNU General Public License for more details.
//!
//!    You should have received a copy of the GNU General Public License
//!    along with this program; if not, write to the Free Software
//!    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//!
//! @file BBRailComDriver.hxx
//! Beagle Board Railcom driver.  Uses a Beagle Board UART driver.
//!
//////////////////////////////////////////////////////////////////////////////

#ifndef __BBRAILCOMDRIVER_HXX
#define __BBRAILCOMDRIVER_HXX

#include <dcc/RailCom.hxx>
#include <dcc/RailcomHub.hxx>
#include <stdint.h>
#include "ExtendedRingBuffer.hxx"
#include <freertos_drivers/common/RailcomDriver.hxx>
#include <os/Gpio.hxx>
#include <signal.h>
#include <time.h>


template <class HW>
class BBRailComDriver : public RailcomDriver
{
public:
    BBRailComDriver(size_t queue_size)
                : railComFeedbackBuffer_(ExtendedRingBuffer<dcc::RailcomHubData>::create(queue_size))
    {
    }
    
    void hw_init(dcc::RailcomHubFlow *hubFlow)
    {
        struct sigevent sev;         // Timer event
        railComHubFlow_ = hubFlow;
        HW::hw_init();
        uart_fd_ = HW::openport();
        sev.sigev_notify = SIGEV_THREAD;
        sev.sigev_value.sival_ptr = (void *) this;
        sev.sigev_notify_function = BBRailComDriver<HW>::railcom_timer_tick;
        if (timer_create(CLOCK_REALTIME, &sev, &timerid_) == -1) {
            LOG(FATAL, "BBRailComDriver: failed to create timer (%d)", errno);
            exit(errno);
        }
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
        struct itimerspec its;
        disable_output();
        usleep(HW::RAILCOM_TRIGGER_DELAY_USEC);
        HW::flush(uart_fd_);
        HW::RC_ENABLE::set(true);
        railcomPhase_ = RailComPhase::CUTOUT_PHASE1;
        its.it_value.tv_sec = 0;
        its.it_value.tv_nsec = HW::RAILCOM_MAX_READ_DELAY_CH_1*1000;
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 0;
        if (timer_settime(timerid_, 0, &its, NULL) == -1) {
            LOG(FATAL, "BBRailComDriver: failed to start timer (%d)", errno);
            exit(errno);
        }
    }
    size_t rx_to_buf(uint8_t *buf, size_t max_len)
    {
        size_t rx_bytes = 0;
        size_t avail = HW::data_avail(uart_fd_);
        if (avail == 0) return avail;
        if (avail > max_len) avail = max_len;
        rx_bytes = HW::readbuff(uart_fd_,buf,avail);
        return rx_bytes;
    }
    void middle_cutout() override
    {
        dcc::RailcomHubData *fb = railcom_buffer();
        uint8_t rx_buf[6] = {0, 0, 0, 0, 0, 0};
        size_t rx_bytes = rx_to_buf(rx_buf, 6);
        if (fb)
        {
            for (size_t idx = 0; idx < rx_bytes; idx++)
            {
                fb->add_ch1_data(rx_buf[idx]);
            }
        }
    }
    void end_cutout() override
    {
        dcc::RailcomHubData *fb = railcom_buffer();
        uint8_t rx_buf[6] = {0, 0, 0, 0, 0, 0};
        size_t rx_bytes = rx_to_buf(rx_buf, 6);
        if (fb)
        {
            for (size_t idx = 0; idx < rx_bytes; idx++)
            {
                fb->add_ch2_data(rx_buf[idx]);
            }
            advance_railcom_buffer();
        }
        HW::RC_ENABLE::set(false);
    }
    void no_cutout()/* override*/
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
    typedef enum : uint8_t
    {
        PRE_CUTOUT,
        CUTOUT_PHASE1,
        CUTOUT_PHASE2
    } RailComPhase;
    RailComPhase railcom_phase()
    {
        return railcomPhase_;
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
    }
    void timer_tick()
    {
        if (railcomPhase_ == RailComPhase::CUTOUT_PHASE1)
        {
            struct itimerspec its;
            
            middle_cutout();
            railcomPhase_ = RailComPhase::CUTOUT_PHASE2;
            its.it_value.tv_sec = 0;
            its.it_value.tv_nsec = HW::RAILCOM_MAX_READ_DELAY_CH_2*1000;
            its.it_interval.tv_sec = 0;
            its.it_interval.tv_nsec = 0;
            if (timer_settime(timerid_, 0, &its, NULL) == -1) {
                LOG(FATAL, "BBRailComDriver: failed to start timer (%d)", errno);
                exit(errno);
            }
        } else if (railcomPhase_ == RailComPhase::CUTOUT_PHASE2)
        {
            end_cutout();
            railcomPhase_ = RailComPhase::PRE_CUTOUT;
            enable_output();
        }
    }
private:
    int uart_fd_;
    uintptr_t railcomFeedbackKey_{0};
    dcc::RailcomHubFlow *railComHubFlow_;
    ExtendedRingBuffer<dcc::RailcomHubData> *railComFeedbackBuffer_;
    RailComPhase railcomPhase_{RailComPhase::PRE_CUTOUT};
    bool enabled_{false};
    timer_t timerid_;
    static void railcom_timer_tick(union sigval sv)
    {
        BBRailComDriver<HW> * driver = reinterpret_cast<BBRailComDriver<HW> *> (sv.sival_ptr);
        driver->timer_tick();
    }
};

#endif // __BBRAILCOMDRIVER_HXX

