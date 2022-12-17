// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Oct 8 13:12:31 2022
//  Last Modified : <221008.1324>
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

#ifndef __I2C_DEV_H
#define __I2C_DEV_H

#include "freertos_includes.h"
#include "stropts.h"
#include <stdint.h>
/** magic number for this driver's ioctl calls */
#define I2C_MAGIC ('i')

/** Slave address to use.  Slave address is 7 or 10 bits, but 10-bit
 * addresses are NOT supported!
 */
#define I2C_SLAVE IOW(I2C_MAGIC, 1, sizeof(long))

/** Combined R/W transfer, one stop only. */
#define I2C_RDWR  IOWR(I2C_MAGIC, 2, sizeof(void*))

/** Combined R/W transfer, one stop only. */
#define I2C_SMBUS IOWR(I2C_MAGIC, 3, sizeof(void*))

/** This is the structure as used in the I2C_SMBUS ioctl call */
struct i2c_smbus_ioctl_data
{
    uint8_t read_write;
    uint8_t command;
    uint32_t size;
    union i2c_smbus_data *data;
};

/** This is the structure as used in the I2C_RDWR ioctl call */
struct i2c_rdwr_ioctl_data
{
    struct i2c_msg *msgs; /**< pointers to i2c_msgs */
    uint32_t nmsgs; /**< number of i2c_msgs */
};

/** maximum number of message segments in @ref i2c_rdwr_ioctl_data struct */
#define  I2C_RDRW_IOCTL_MAX_MSGS 42


#endif // __I2C_DEV_H

