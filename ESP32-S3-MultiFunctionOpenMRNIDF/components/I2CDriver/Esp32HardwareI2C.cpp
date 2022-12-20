// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Oct 8 12:47:31 2022
//  Last Modified : <221219.1737>
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

static const char rcsid[] = "@(#) : $Id$";

#include "sdkconfig.h"
#include <esp_idf_version.h>

#include <driver/i2c.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_ota_ops.h>
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5,0,0)
#include <esp_private/periph_ctrl.h>
#else // IDF v4.x (or earlier)
#include <driver/periph_ctrl.h>
#endif // IDF v5+
#include <esp_ipc.h>
#include <esp_log.h>
#include <esp_rom_gpio.h>
#include <esp_intr_alloc.h>
#include <esp_task.h>
#include <esp_vfs.h>
#include <fcntl.h>
#include <string.h>
#include <esp_system.h>
#include <esp_task_wdt.h>
#include <esp32s3/rom/rtc.h>
#include <hal/gpio_types.h>
#include <freertos_includes.h>   
#include "utils/logging.h"
#include "utils/macros.h"

#include "i2c.h"
#include "i2c-dev.h"
#include "Esp32HardwareI2C.hxx"

#include <cstdarg>

void Esp32HardwareI2C::hw_init()
{
    LOG(INFO,
        "ESP-I2C: Configuring I2C (SDA:%d, SCL:%d, Master:%d)",
        sda_, scl_, master_);
    
    HASSERT(master_ < I2C_NUM_MAX);
    int i2c_master_port = master_;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_,
        .scl_io_num = scl_,
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
}

Esp32HardwareI2C::~Esp32HardwareI2C()
{
    ESP_ERROR_CHECK(i2c_driver_delete(master_));
}

Esp32HardwareI2C::FD *Esp32HardwareI2C::open() const
{
    return new FD(0);
}

void Esp32HardwareI2C::close(Esp32HardwareI2C::FD *f) const
{
    delete f;
}



ssize_t Esp32HardwareI2C::write(Esp32HardwareI2C::FD* f, const void *buf, size_t size) const
{
    int result;
    struct i2c_msg msg;
    msg.addr = (uintptr_t)f->address;
    msg.flags = 0;
    msg.len = size;
    msg.buf = (uint8_t*)buf;
    result = transfer(&msg, true);
    return result;
}

ssize_t Esp32HardwareI2C::read(Esp32HardwareI2C::FD* f, void *buf, size_t size) const
{
    int result;
    
    struct i2c_msg msg;
    msg.addr = (uintptr_t)f->address;
    msg.flags = I2C_M_RD;
    msg.len = size;
    msg.buf = (uint8_t*)buf;
    result = transfer(&msg, true);
    return result;
}

int Esp32HardwareI2C::ioctl(Esp32HardwareI2C::FD* f, int cmd, ...) const
{
    va_list args;
    va_start(args, cmd);
    
    
    HASSERT(IOC_TYPE(cmd) == I2C_MAGIC);
    
    switch (cmd)
    {
    default:
        return -EINVAL;
    case I2C_SLAVE:
        f->address = va_arg(args, uint32_t);
        return 0;
    case I2C_RDWR:
        {
            struct i2c_rdwr_ioctl_data *rdwr_data = (struct i2c_rdwr_ioctl_data *) (va_arg(args, uintptr_t));
            return transfer_messages(rdwr_data->msgs, rdwr_data->nmsgs);
        }
    case I2C_SMBUS:
        return smbus(f,(struct i2c_smbus_ioctl_data*)(va_arg(args, uintptr_t)));
    }
    return 0;
}

int Esp32HardwareI2C::transfer_messages(struct i2c_msg *msgs, int num) const
{
    HASSERT(num > 0);
    
    int count = 0;
    int result;
    for (int i = 0; i < num; ++i)
    {
        count += msgs[i].len;
        result = transfer(msgs + i, (num == (i + 1)));
        if (result < 0)
        {
            return result;
        }
    }
    return  count;
}

int Esp32HardwareI2C::smbus(FD *f, struct i2c_smbus_ioctl_data* sm_data) const
{
    /* check that we have a valid transaction type */
    if ((sm_data->size != I2C_SMBUS_QUICK) &&
        (sm_data->size != I2C_SMBUS_BYTE) &&
        (sm_data->size != I2C_SMBUS_BYTE_DATA) &&
        (sm_data->size != I2C_SMBUS_WORD_DATA) &&
        (sm_data->size != I2C_SMBUS_PROC_CALL) &&
        (sm_data->size != I2C_SMBUS_BLOCK_DATA) &&
        (sm_data->size != I2C_SMBUS_I2C_BLOCK_BROKEN) &&
        (sm_data->size != I2C_SMBUS_BLOCK_PROC_CALL) &&
        (sm_data->size != I2C_SMBUS_I2C_BLOCK_DATA))
    {
        return -EINVAL;
    }

    /* check that we have a read or write */
    if ((sm_data->read_write != I2C_SMBUS_READ) &&
        (sm_data->read_write != I2C_SMBUS_WRITE))
    {
        return -EINVAL;
    }

    /// @todo need to finish implementation (Stuart Baker)
    return 0;
}

int Esp32HardwareI2C::transfer(struct i2c_msg *msg, bool stop) const
{
    //LOG(INFO,"[Esp32HardwareI2C::transfer]: msg->flags is %d.",msg->flags);
    //LOG(INFO,"[Esp32HardwareI2C::transfer]: msg->len = %d", msg->len);
    //LOG(INFO,"[Esp32HardwareI2C::transfer]: msg->addr = %d", msg->addr);
    //LOG(INFO,"[Esp32HardwareI2C::transfer]: msg->buf = %p",msg->buf);
    int bytes = msg->len;
    if (msg->flags & I2C_M_RD)
    {
        esp_err_t err = i2c_master_read_from_device(master_,
                                                    (uint16_t)msg->addr,
                                                    (uint8_t *)msg->buf,
                                                    bytes,
                                                    I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
        //LOG(INFO,"[Esp32HardwareI2C::transfer]: (i2c_master_read_from_device) err = %d.",err);
        if (err != ESP_OK) return -EIO;
        else return bytes;
    }
    else
    {
        esp_err_t err = i2c_master_write_to_device(master_,
                                                   (uint16_t)msg->addr,
                                                   (uint8_t *)msg->buf,
                                                   bytes,
                                                   I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
        //LOG(INFO,"[Esp32HardwareI2C::transfer]: (i2c_master_write_to_device) err = %d.",err);
        if (err != ESP_OK) return -EIO;
        else return bytes;
    }
}

