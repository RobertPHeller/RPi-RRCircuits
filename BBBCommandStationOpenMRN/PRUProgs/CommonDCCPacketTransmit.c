/* -*- C -*- ****************************************************************
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : Robert Heller
 *  Created       : Fri Oct 25 16:47:20 2019
 *  Last Modified : <191102.1858>
 *
 *  Description	
 *
 *  Notes
 *
 *  History
 *	
 ****************************************************************************
 *
 *    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
 *			51 Locke Hill Road
 *			Wendell, MA 01379-9728
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * 
 *
 ****************************************************************************/

static const char rcsid[] = "@(#) : $Id$";



#include <stdint.h>
#include <stdio.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_rpmsg.h>
#ifdef MAINDCC
#include "resource_table_0.h"
#else
#include "resource_table_1.h"
#endif

volatile register uint32_t __R31;

/* ??? -- pru1 different? need to check */
/* Host-0 Interrupt sets bit 30 in register R31 */
#define HOST_INT			((uint32_t) 1 << 30)

/* 
 * The PRU-ICSS system events used for RPMsg are defined in the Linux devicetree
 * PRU0 uses system event 16 (To ARM) and 17 (From ARM)
 * PRU1 uses system event 18 (To ARM) and 19 (From ARM)
 */
#ifdef MAINDCC
#define TO_ARM_HOST			16
#define FROM_ARM_HOST			17
#else
#define TO_ARM_HOST                     18
#define FROM_ARM_HOST                   19
#endif

/*
 * Using the name 'rpmsg-pru' will probe the rpmsg_pru driver found
 * at linux-x.y.z/drivers/rpmsg/rpmsg_pru.c
 */
#define CHAN_NAME			"rpmsg-pru"
#ifdef MAINDCC
#define CHAN_DESC			"Channel 30"
#define CHAN_PORT			30
#else
#define CHAN_DESC                       "Channel 31"
#define CHAN_PORT                       31
#endif

/*
 * Used to make sure the Linux drivers are ready for RPMsg communication
 * Found at linux-x.y.z/include/uapi/linux/virtio_config.h
 */
#define VIRTIO_CONFIG_S_DRIVER_OK	4

/* payload receives RPMsg message */
#define RPMSG_BUF_HEADER_SIZE           16
char payload[RPMSG_BUF_SIZE - RPMSG_BUF_HEADER_SIZE];

#include "dccpacket.h" /* define the DCC Packet struct */

/* output GPIOs */
volatile register unsigned int __R30;

#ifdef MAINDCC
#define DCCBit 14 /* __R30 bit 14 (on PRU0) => P8_12 (BBB), P2_24 (PB) */
#else
#define DCCBit 11 /* __R30 bit 11 (on PRU1) => P8_30 (BBB), P1_4  (PB) */
#endif

static void OneBit() 
{
    __R30 |= 1 << DCCBit;
    __delay_cycles(ONEBitTime);
    __R30 = __R30 & ~(1 << DCCBit);
    __delay_cycles(ONEBitTime);
}

static void ZeroBit() 
{
    __R30 |= 1 << DCCBit;
    __delay_cycles(ZEROBitTime);
    __R30 = __R30 & ~(1 << DCCBit);
    __delay_cycles(ZeroBitTime);
}

static void SendPreamble()
{
    int i;
    for (i = 0; i < PreambleLength; i++) 
    {
        OneBit();
    }

static void SendByte(uint8_t byte)
{
    int i;
    for (i = 7; i >= 0; i--) {
        if ((byte >> i)&1) {
            OneBit();
        } else {
            ZeroBit();
        }
    }
}

DCCPacket current;


void main(void)
{
    struct pru_rpmsg_transport transport;
    uint16_t src, dst, len;
    volatile uint8_t *status;
    struct shared_struct message;
    
    /* 
     * Allow OCP master port access by the PRU so the PRU can read 
     * external memories 
     */
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
    
    /* initialize to Idle packet */
    current.dlc = 3;
    current.payload[0] = current.payload[2] = 0xFF;
    current.payload[1] = 0;
    
    /* 
     * Clear the status of the PRU-ICSS system event that the ARM will 
     * use to 'kick' us
     */
    CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;
    
    /* Make sure the Linux drivers are ready for RPMsg communication */
    status = &resourceTable.rpmsg_vdev.status;
    while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK)) {
        /* Optional: implement timeout logic */
    };
    
    /* Initialize the RPMsg transport structure */
    pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0,
                   &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);
    
    /* 
     * Create the RPMsg channel between the PRU and ARM user space using 
     * the transport structure. 
     */
    while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME,
                             CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS) {
        /* Optional: implement timeout logic */
    };
    

    while (1) {
        SendPreamble();
        ZeroBit();
        /* Check register R31 bit 30 to see if the ARM has kicked us */
        if ((__R31 & HOST_INT))
        {
            /* Clear the event status */
            CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;
            
            /* 
             * Receive available message.
             */
            if (pru_rpmsg_receive(&transport, &src, &dst,
                                  payload, &len) == PRU_RPMSG_SUCCESS) {
                if (len > sizeof(DCCPacket)) len = sizeof(DCCPacket);
                /* ARM sends a message using DCCPacket */
                memcpy(&current,payload,len);
            }
        }
        SendByte(current.payload[0]);
        int i;
        for (i=1; i<current.dlc; i++)
        {
            ZeroBit();
            SendByte(current.payload[i]);
        }
        OneBit();
    }
}


