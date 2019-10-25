// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu Oct 24 19:45:55 2019
//  Last Modified : <191025.1724>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
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

#ifndef __DCCPACKET_H
#define __DCCPACKET_H

#include <stdint.h>

/** Maximum number of payload bytes. */
#define DCC_PACKET_MAX_PAYLOAD (6)

/// Stores a DCC packet in memory. Used to send data from the packet generation
/// (usually the command station refresh loop flows) to the DCC track driver.
typedef struct dcc_packet
{
    /// Specifies the meaning of the command byte for packets to send.
    struct pkt_t
    {
        /// Always 0.
        uint8_t is_pkt : 1;
        /// 0: DCC packet, 1: motorola packet.
        uint8_t is_marklin : 1;

        /// typically for DCC packets:
        /// 1: do NOT append an EC byte to the end of the packet.
        uint8_t skip_ec : 1;
        /// 1: send long preamble instead of packet. 0: send normal preamble
        /// and pkt.
        uint8_t send_long_preamble : 1;
        /// 1: wait for service mode ack and report it back to the host.
        uint8_t sense_ack : 1;
        /// The packet will be sent 1 + rept_count times to the wire. default:
        /// 0.
        uint8_t rept_count : 2;
        /// reserved for future use.
        uint8_t reserved : 1;
    } pkt;

    /// Specifies the meaning of the command byte for meta-commands to send.
    struct cmd_t
    {
        /// Always 1.
        uint8_t is_pkt : 1;
        /// Command identifier.
        uint8_t cmd : 7;
    } cmd;
    
    union
    {
        uint8_t header_raw_data;
        struct pkt_t packet_header;
        struct cmd_t command_header;
    };
    /** Specifies the number of used payload bytes. */
    uint8_t dlc;
    /** Packet payload bytes. */
    uint8_t payload[DCC_PACKET_MAX_PAYLOAD];

    /** An opaque key used by the hardware driver to attribute feedback
     * information to the source of the packet. This key will be sent back in
     * the dcc::Feedback structure. If the key is non-zero it is guaranteed
     * that some feedback (maybe empty) will be sent back after the packet is
     * transmitted to the track. */
    uintptr_t feedback_key;
} DCCPacket;

/** 1/2 bit times in units of cycles (5 nanoseconds) */
#define ONEBitTime 11600
#define ZEROBitTime 20000
/** Number of preamble one bits */
#define PreambleLength 14
#endif // __DCCPACKET_H

