// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Oct 8 11:16:35 2022
//  Last Modified : <221127.0843>
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

#ifndef __ESP32HARDWAREI2C_HXX
#define __ESP32HARDWAREI2C_HXX

namespace openmrn_arduino
{

#include <driver/i2c.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_ota_ops.h>
#include <esp_system.h>
#include <esp_task_wdt.h>
#include <esp32s3/rom/rtc.h>
#include <hal/gpio_types.h>
#include <freertos_includes.h>   
#include "utils/logging.h"
#include "utils/macros.h"
#include <soc/soc.h>
#include <soc/soc_caps.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <map>
using std::string;
using std::map;
using std::pair;

#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000



class Esp32HardwareI2C
{
public:
    /// Constructor.
    ///
    /// @param sda is the GPIO pin connected to SDA 
    /// @param scl is the GPIO pin connected to SCL
    /// @param master I2C master number, default is 0
    /// @param path is the VFS mount point for the for this I2C port, default 
    /// is "/dev/i2c".
    
    Esp32HardwareI2C(uint8_t sda, uint8_t scl, uint8_t master = 0, 
                     const char *path = "/dev/i2c/i2c0")
                : sda_(sda), scl_(scl), master_(master), vfsPath_(path)
    {
    }
    /// Destructor.
    ~Esp32HardwareI2C();
    
    /// Initializes the I2C hardware and VFS adapter.
    ///
    /// NOTE: This must be called prior to opening connections the I2C
    void hw_init();
    static void Mount(const char *mp = "/dev/i2c");
    static void Unmount();
private:
    /// Default constructor.
    Esp32HardwareI2C() {}
    
    /// GPIO pin connected to SDA.
    uint8_t sda_;
    /// GPIO pin connected to SCL. 
    uint8_t scl_;
    /// I2C master.
    uint8_t master_;
    /// device path. relative to the VFS mount point.
    const char *vfsPath_;
    
    struct FD {
        const Esp32HardwareI2C *parent;
        unsigned address;
        FD(const Esp32HardwareI2C *p,unsigned addr) 
                    : parent(p), address(addr) {}
    };
    
    DISALLOW_COPY_AND_ASSIGN(Esp32HardwareI2C);
    
    ssize_t write(FD* f,const void *buf, size_t size) const;
    ssize_t read(FD *f,void *buf, size_t size) const;
    int ioctl(FD *f,int cmd, va_list args) const;
    
    /** Conduct multiple message transfers with one stop at the end.
     * @param msgs array of messages to transfer
     * @param num number of messages to transfer
     * @return total number of bytes transfered, -errno upon failure
     */
    int transfer_messages(struct i2c_msg *msgs, int num) const;

    /** Request an smbus (ioctl) transaction.
     * @param file file reference for this device
     * @param data smbus data
     * @return 0 upon success, -errno upon failure
     */
    int smbus(FD *f, struct i2c_smbus_ioctl_data* data) const;    
    
    /** Method to transmit/receive the data.
     * @param msg message to transact.
     * @param stop produce a stop condition at the end of the transfer
     * @return bytes transfered upon success or -1 with errno set
     */
    int transfer(struct i2c_msg *msg, bool stop) const;
    
    static char mountpoint[ESP_VFS_PATH_MAX];
    typedef map<const string,Esp32HardwareI2C *> pathMapType;
    typedef pathMapType::iterator pathMapType_iterator;
    typedef pathMapType::const_iterator pathMapType_const_iterator;
    static pathMapType instances;
    typedef map<const int, Esp32HardwareI2C::FD *> fdMap;
    typedef fdMap::iterator fdMap_iterator;
    typedef fdMap::const_iterator fdMap_const_iterator;
    static fdMap opened;
    
    static ssize_t vfs_write(int fd, const void *buf, size_t size);
    static ssize_t vfs_read(int fd, void *buf, size_t size);
    static int vfs_open(const char *path, int flags, int mode);
    static int vfs_close(int fd);
    static int vfs_ioctl(int fd, int cmd, va_list args);
    
};

} // namespace openmrn_arduino

using openmrn_arduino::Esp32HardwareI2C;

#endif // __ESP32HARDWAREI2C_HXX

