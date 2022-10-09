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
//  Last Modified : <221006.0954>
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

#define SPEED_100KHZ         100000
#define SPEED_400KHZ         400000


LPC17xx_40xxI2C::LPC17xx_40xxI2C(const char *name, I2C_ID_T port)
      : I2C(name), port_(port)
{
    Chip_I2C_Init(port);
    Chip_I2C_SetClockRate(port,SPEED_100KHZ);
    Chip_I2C_SetMasterEventHandler(port, Chip_I2C_EventHandler);
    switch (port) {
    case I2C0: NVIC_EnableIRQ(I2C0_IRQn); break;
    case I2C1: NVIC_EnableIRQ(I2C1_IRQn); break;
    case I2C2: NVIC_EnableIRQ(I2C2_IRQn); break;
    default: break;
    }
    instances[(unsigned)port] = this;
}

int LPC17xx_40xxI2C::transfer(struct i2c_msg *msg, bool stop)
{
    if (msg->flags & I2C_M_RD)
    {
        /* this is a read transfer */
        int tmp = Chip_I2C_MasterRead(port_, msg->addr, (uint8_t *)msg->buf, msg->len);
        return tmp;
    }
    else
    {
        /* this is a write transfer */
        int tmp = Chip_I2C_MasterSend(port_, msg->addr, (uint8_t *)msg->buf, msg->len);
        return tmp;
    }    
}


void LPC17xx_40xxI2C::interrupt_handler()
{
    if (Chip_I2C_IsMasterActive(port_))
    {
        Chip_I2C_MasterStateHandler(port_);
    }
    else
    {
        Chip_I2C_SlaveStateHandler(port_);
    }
}

LPC17xx_40xxI2C* LPC17xx_40xxI2C::instances[3];

extern "C" {
void i2c0_interrupt_handler(void)
{
    LPC17xx_40xxI2C::interrupt_handler(I2C0);
}
void i2c1_interrupt_handler(void)
{
    LPC17xx_40xxI2C::interrupt_handler(I2C1);
}
void i2c2_interrupt_handler(void)
{
    LPC17xx_40xxI2C::interrupt_handler(I2C2);
}
};
