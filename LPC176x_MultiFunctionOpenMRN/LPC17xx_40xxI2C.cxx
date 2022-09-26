// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Sep 26 15:13:10 2022
//  Last Modified : <220926.1602>
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

#include "chip.h"
#include "core_cm3.h"
#include <i2c_17xx_40xx.h>

#include "LPC17xx_40xxI2C.hxx"


LPC17xx_40xxI2C::LPC17xx_40xxI2C(const char *name, I2C_ID_T port)
      : I2C(name), port_(port)
{
    Chip_I2C_Init(port);
}

int LPC17xx_40xxI2C::transfer(struct i2c_msg *msg, bool stop)
{
    I2C_XFER_T transferMsg;
    
    transferMsg.slaveAddr = msg->addr;
    if (msg->flags & I2C_M_RD)
    {
        /* this is a read transfer */
        transferMsg.txBuff = nullptr;
        transferMsg.txSz = 0;
        transferMsg.rxBuff = msg->buf;
        transferMsg.rxSz = msg->len;
        int result = Chip_I2C_MasterTransfer(port_, &transferMsg);
        if (result != I2C_STATUS_DONE)
        {
            return -EIO;
        }
        else
        {
            return msg->len-transferMsg.rxSz;
        }
    }
    else
    {
        /* this is a write transfer */
        transferMsg.txBuff = msg->buf;
        transferMsg.txSz = msg->len;
        transferMsg.rxBuff = nullptr;
        transferMsg.rxSz = 0;
        int result = Chip_I2C_MasterTransfer(port_, &transferMsg);
        if (result != I2C_STATUS_DONE)
        {
            return -EIO;
        }
        else
        {
            return msg->len-transferMsg.txSz;
        }
    }    
}
