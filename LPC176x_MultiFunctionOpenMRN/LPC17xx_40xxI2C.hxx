// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Sep 26 11:28:23 2022
//  Last Modified : <221004.1227>
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

#ifndef __LPC17XX_40XXI2C_HXX
#define __LPC17XX_40XXI2C_HXX

#include "I2C.hxx"

#include <i2c_17xx_40xx.h>

/** Specialization of I2C driver 17xx_40xx devices.
 */

class LPC17xx_40xxI2C : public I2C
{
public:
    /** Constructor.
     * @param name name of this device instance in the file system
     * @param port hardware instance of this device, e.g. I2C1
     */
    LPC17xx_40xxI2C(const char *name, I2C_ID_T port);
    
    /** Destructor.
     */
    ~LPC17xx_40xxI2C()
    {
    }
    static void interrupt_handler(I2C_ID_T port)
    {
        if (instances[(unsigned)port])
        {
            instances[(unsigned)port]->interrupt_handler();
        }
    }
    
private:
    void enable() override {} /**< function to enable device */
    void disable() override {} /**< function to disable device */
    
    /** Method to transmit/receive the data.
     * @param msg message to transact.
     * @param stop produce a stop condition at the end of the transfer
     * @return bytes transfered upon success or -1 with errno set
     */
    virtual int transfer(struct i2c_msg *msg, bool stop) override;
    
    /** Default constructor.
     */
    LPC17xx_40xxI2C();
    void interrupt_handler();
    
    I2C_ID_T port_;
    static LPC17xx_40xxI2C* instances[3];
    
    DISALLOW_COPY_AND_ASSIGN(LPC17xx_40xxI2C);
};


#endif // __LPC17XX_40XXI2C_HXX

