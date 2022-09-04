// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri Jun 24 13:40:15 2022
//  Last Modified : <220625.1635>
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
// Portions lifted from openmrn/src/freertos_drivers/common/PCA9685PWM.hxx (Stuart W. Baker)
// Portions lifted from openmrn/src/freertos_drivers/esp32/Esp32Ledc.hxx (Mike Dunston)
// Portions lifted from esp-idf-v4.4.1/examples/peripherals/i2c/i2c_simple/main/i2c_simple_main.c 
//
//
// 
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ESP32PCA9685PWM_HXX
#define __ESP32PCA9685PWM_HXX

#include <driver/i2c.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_ota_ops.h>
#include <esp_system.h>
#include <esp_task_wdt.h>
#include <esp32/rom/rtc.h>
#include <hal/gpio_types.h>
#include <freertos_includes.h>   

#include "freertos_drivers/arduino/PWM.hxx"
#include "utils/logging.h"
#include "utils/macros.h"

#include "hardware.hxx"

#define I2C_MASTER_SCL_IO           CONFIG_SCL_PIN             /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           CONFIG_SDA_PIN             /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000


namespace openmrn_arduino
{
/// ESP32 driver for a PCA9685, an I2C 16 channel PWM driver.
///

class Esp32PCA9685PWM 
{
public:
    /// maximum number of PWM channels supported by the PCA9685
    static constexpr size_t NUM_CHANNELS = 16;

    /// maximum number of PWM counts supported by the PCA9685
    static constexpr size_t MAX_PWM_COUNTS = 4096;

    /// Constructor.
    Esp32PCA9685PWM() : i2cAddress_(0)
    {
        duty_.fill(0);
    }
    /// Initialize device.
    /// @param i2c_address I2C address of the device on the bus
    /// @param pwm_frequency target PWM frequency (will truncate at minimum and
    ///                      maximum prescaler values)
    /// @param external_clock frequency of an external clock in Hz, -1
    ///                       if internal clock is used
    void hw_init(uint8_t i2c_address,
                 uint16_t pwm_freq = 200, 
                 int32_t external_clock_freq = -1)
    {
        uint32_t clock_freq = external_clock_freq >= 0 ? external_clock_freq :
                                                         25000000;
        i2cAddress_ = i2c_address;

        int i2c_master_port = I2C_MASTER_NUM;
        i2c_config_t conf = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = I2C_MASTER_SDA_IO,
            .scl_io_num = I2C_MASTER_SCL_IO,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master = {.clk_speed = I2C_MASTER_FREQ_HZ},
            .clk_flags = 0
        };
        
        i2c_param_config(i2c_master_port, &conf);
        
        ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, 
                                           conf.mode, 
                                           I2C_MASTER_RX_BUF_DISABLE, 
                                           I2C_MASTER_TX_BUF_DISABLE, 
                                           0));
        HASSERT(clock_freq <= 50000000);
        HASSERT(pwm_freq < (clock_freq / (4096 * 4)));

        Mode1 mode1;
        mode1.aI = 1;
        mode1.sleep = 1;
        mode1.allCall = 0;
        register_write(MODE1, mode1.byte);

        uint8_t prescaler = (clock_freq / (4096 * (uint32_t)pwm_freq)) - 1;
        register_write(PRE_SCALE, prescaler);

        if (external_clock_freq < 0)
        {
            /* if using internal clock */
            mode1.sleep = 0;
            register_write(MODE1, mode1.byte);
        }

        Mode2 mode2;
        mode2.och = 1;
        register_write(MODE2, mode2.byte);
        for (uint8_t index = 0; index < Esp32PCA9685PWM::NUM_CHANNELS; index++)
        {
            channels_[index].emplace(this, index);
        }
    }
    /// Destructor.
    ~Esp32PCA9685PWM()
    {
    }
    /// @return one PWM channel.
    /// @param index is the channel index.
    PWM *get_channel(unsigned index)
    {
        HASSERT(index < Esp32PCA9685PWM::NUM_CHANNELS);
        return &*channels_[index];
    }
private:
    /// Important device register offsets
    enum Registers
    {
        MODE1 = 0, ///< mode 1 settings
        MODE2, ///< mode 2 settings

        LED0_ON_L = 6, ///< first LED control register

        PRE_SCALE = 254, ///< clock prescale divider
    };

    /// Represent the mode 1 register.
    union Mode1
    {
        /// Constructor
        Mode1()
            : byte(0x01)
        {
        }

        uint8_t byte; ///< full byte value
        struct
        {
            uint8_t allCall : 1; ///< respond to "all call" address
            uint8_t sub3    : 1; ///< sub-address 3
            uint8_t sub2    : 1; ///< sub-address 2
            uint8_t sub1    : 1; ///< sub-address 1
            uint8_t sleep   : 1; ///< sleep enabled
            uint8_t aI      : 1; ///< auto increment
            uint8_t extClk  : 1; ///< external clock
            uint8_t restart : 1; ///< restart
        };
    };

    /// Represent the mode 2 register.
    union Mode2
    {
        /// Constructor.
        Mode2()
            : byte(0x04)
        {
        }

        uint8_t byte; ///< full byte data
        struct
        {
            uint8_t outNE  : 2; ///< output not enable
            uint8_t outDrv : 1; ///< 1 = push/pull, 0 = open drain
            uint8_t och    : 1; ///< 1 = updata on ack, 0 = update on stop
            uint8_t invert : 1; ///< invert output
            uint8_t unused : 3; ///< unused
        };
    };

    /// Representation of the replicative 4 LED control register
    struct LedCtl
    {
        union On
        {
            /// Constructor.
            On()
                : word(0x0000)
            {
            }
            uint16_t word; ///< full word data
            struct
            {
                uint16_t counts : 12; ///< turn on counts
                uint16_t fullOn :  1; ///< set if full on
                uint16_t unused :  3; ///< unused
            };
        };
        union Off
        {
            /// Constructor.
            Off()
                : word(0x1000)
            {
            }
            uint16_t word; ///< full word data
            struct
            {
                uint16_t counts  : 12; ///< turn off counts
                uint16_t fullOff :  1; ///< set if full off
                uint16_t unused  :  3; ///< unused
            };
        };
        On on; ///< on registers instance
        Off off; ///< off registers instance
    };

    /// Bit modify to an I2C register.
    /// @param address address to modify
    /// @param data data to modify
    /// @param mask mask of data to modify, bits where mask is 1 will be
    ///             overwritten, bits where mask is zero will be kept
    void bit_modify(Registers address, uint8_t data, uint8_t mask)
    {
        //uint8_t addr = address;
        uint8_t rd_data;

        rd_data &= ~mask;
        rd_data |= (data & mask);

        register_write(address, rd_data);
    }

    /// Set the pwm duty cycle
    /// @param channel channel index (0 through 15)
    /// @param counts counts for PWM duty cycle
    void set_pwm_duty(unsigned channel, uint16_t counts)
    {
        HASSERT(channel < NUM_CHANNELS);
        duty_[channel] = counts;
        write_pwm_duty(channel,counts);
    }

    /// Get the pwm duty cycle
    /// @param channel channel index (0 through 15)
    /// @return counts for PWM duty cycle
    uint16_t get_pwm_duty(unsigned channel)
    {
        HASSERT(channel < NUM_CHANNELS);
        return duty_[channel];
    }

    /// Set the pwm duty cycle
    /// @param channel channel index (0 through 15)
    /// @param counts counts for PWM duty cycle
    void write_pwm_duty(unsigned channel, uint16_t counts)
    {
        LedCtl ctl;
        if (counts >= MAX_PWM_COUNTS)
        {
            ctl.on.fullOn = 1;
            ctl.off.fullOff = 0;
        }
        else if (counts == 0)
        {
            ctl.on.fullOn = 0;
            ctl.off.fullOff = 1;
        }
        else
        {
            // the "256" count offset is to help average the current accross
            // all 16 channels when the duty cycle is low
            ctl.on.counts = (channel * 256);
            ctl.off.counts = (counts + (channel * 256)) % 0x1000;
        }

        htole16(ctl.on.word);
        htole16(ctl.off.word);

        Registers offset = (Registers)(LED0_ON_L + (channel * 4));
        register_write_multiple(offset, &ctl, sizeof(ctl));
    }

    /// Write to an I2C register.
    /// @param address address to write to
    /// @param data data to write
    void register_write(Registers address, uint8_t data)
    {
        uint8_t payload[] = {address, data};
        i2c_master_write_to_device(I2C_MASTER_NUM, i2cAddress_, 
                                   payload, sizeof(payload), 
                                   I2C_MASTER_TIMEOUT_MS / 
                                   portTICK_RATE_MS);
    }

    /// Write to multiple sequential I2C registers.
    /// @param address address to start write at
    /// @param data array of data to write
    /// @param count number of data registers to write in sequence
    void register_write_multiple(Registers address, void *data, size_t count)
    {
        uint8_t payload[count + 1];
        payload[0] = address;
        memcpy(payload + 1, data, count);
        i2c_master_write_to_device(I2C_MASTER_NUM, i2cAddress_,
                                   payload, sizeof(payload), 
                                   I2C_MASTER_TIMEOUT_MS /
                                   portTICK_RATE_MS);
    }

    class Channel : public PWM
    {
    public:
        /// Constructor.
        /// @param instance reference to the chip
        /// @param index channel index on the chip (0 through 15)
        Channel(Esp32PCA9685PWM *parent, unsigned index)
                    : PWM()
              , instance_(parent)
              , index_(index)
        {
            HASSERT(index < Esp32PCA9685PWM::NUM_CHANNELS);
        }
        /// Destructor.
        ~Channel()
        {
        }
        /// Set PWM period
        /// @param PWM period in counts
        void set_period(uint32_t counts) override
        {
            HASSERT(counts == Esp32PCA9685PWM::MAX_PWM_COUNTS);
        }
        /// Get PWM period.
        /// @return PWM period in counts
        uint32_t get_period() override
        {
            return Esp32PCA9685PWM::MAX_PWM_COUNTS;
        }
        /// Sets the duty cycle.
        /// @param counts duty cycle in counts
        void set_duty(uint32_t counts) override
        {
            instance_->set_pwm_duty(index_, counts);
        }
        /// Gets the duty cycle.
        /// @return counts duty cycle in counts
        uint32_t get_duty() override
        {
            return instance_->get_pwm_duty(index_);
        }
        /// Get max period supported
        /// @return period in counts
        uint32_t get_period_max() override
        {
            return get_period();
        }
        /// Get min period supported
        /// @return period in counts
        uint32_t get_period_min() override
        {
            return get_period();
        }
        /// instance pointer to the whole chip complement
        Esp32PCA9685PWM *instance_;
        /// bit index within PCA968
        unsigned index_;
    };
    /// local cache of the duty cycles
    std::array<uint16_t, NUM_CHANNELS> duty_;

    /// I2C address of the device
    uint8_t i2cAddress_;
    
    /// @ref PWM instances connected to channels
    uninitialized<Channel> channels_[Esp32PCA9685PWM::NUM_CHANNELS];
    
    DISALLOW_COPY_AND_ASSIGN(Esp32PCA9685PWM);
};

}

/** PCA9685 Slave Address register */
#define PCA9685_SLAVE_ADDRESS 0x40

#endif // __ESP32PCA9685PWM_HXX

