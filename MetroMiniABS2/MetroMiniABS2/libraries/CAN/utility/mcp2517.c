/* -*- C -*- ****************************************************************
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : Robert Heller
 *  Created       : Sun Jun 3 10:11:20 2018
 *  Last Modified : <180605.1503>
 *
 *  Description	
 *
 *  Notes
 *
 *  History
 *	
 ****************************************************************************
 *
 *    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
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



/* Bit timings
 * 
 * Fosc  = 20Mhz
 * BRP   =  20 = 0x13 (+1)
 * TQ    =   1us
 * TSEG1 =   4 = 0x03 (+1)
 * TSEG2 =   3 = 0x02 (+1)
 * SJW   =   1 = 0x00 (+1)
 * SynSeg              = 1
 * Prop Seg+Phase1 Seg = 4
 * Phase2 Seg          = 3
 *                   -----
 *                       8
 * Bus speed = 1 / ((Total # of TQ) * TQ)
 *             1 / (8 * TQ) = 125 Khz
 */

#include "mcp2517_private.h"

#ifdef	SUPPORT_FOR_MCP2517__

#ifndef	MCP2517_CLKOUT_PRESCALER
#error	MCP2517_CLKOUT_PRESCALER not defined!  #error MCP2517_CLKOUT_PRESCALER not defined!
#elif MCP2517_CLKOUT_PRESCALER == 0
	#define	CLKOUT_PRESCALER_	0x0
#elif MCP2517_CLKOUT_PRESCALER == 1
	#define	CLKOUT_PRESCALER_	0x4
#elif MCP2517_CLKOUT_PRESCALER == 2
	#define	CLKOUT_PRESCALER_	0x5
#elif MCP2517_CLKOUT_PRESCALER == 4
	#define	CLKOUT_PRESCALER_	0x6
#elif MCP2517_CLKOUT_PRESCALER == 8
	#define	CLKOUT_PRESCALER_	0x7
#else
	#error	invaild value of MCP2517_CLKOUT_PRESCALER
#endif

// -------------------------------------------------------------------------
void mcp2517_write_register( uint16_t adress, uint32_t data )
{
    RESET(MCP2517_CS);
    
    spi_putc(SPI_WRITE| ((adress >> 8) & 0x0F));
    spi_putc(adress & 0x0ff);
    spi_putc(data & 0x0ff);
    spi_putc((data >> 8) & 0x0ff);
    spi_putc((data >> 16) & 0x0ff);
    spi_putc((data >> 24) & 0x0ff);
    SET(MCP2515_CS);
}

// -------------------------------------

uint32_t mcp2517_read_register(uint16_t adress)
{
    uint32_t data;
    uint8_t  buffer[4];
    
    RESET(MCP2517_CS);
    
    spi_putc(SPI_READ| ((adress >> 8) & 0x0F));
    spi_putc(adress & 0x0ff);
    buffer[0] = spi_putc(0xff);
    buffer[1] = spi_putc(0xff);
    buffer[2] = spi_putc(0xff);
    buffer[3] = spi_putc(0xff);
    SET(MCP2517_CS);
    
    data = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
    return data;
}

/** CRC table for the CRC-16. The poly is 0x8005 (x^16 + x^15 + x^2 + 1) */

static uint16_t const crc16_table[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

static inline uint16_t crc16_byte(uint16_t crc, const uint8_t data)
{
    return (crc >> 8) ^ crc16_table[(crc ^ data) & 0xff];
}


/**
 * crc16 - compute the CRC-16 for the data buffer
 * @crc:        previous CRC value
 * @buffer:     data pointer
 * @len:        number of bytes in the buffer
 *
 * Returns the updated CRC value.
 */
static uint16_t crc16(uint16_t crc, uint8_t const *buffer, uint8_t len)
{
    while (len--)
        crc = crc16_byte(crc, *buffer++);
    return crc;
}




int  mcp2517_read_crc(uint16_t adress,uint8_t *buffer,uint8_t len)
{
    uint8_t i, byte;
    if ((len & 0x3) != 0) {return -2;}
    uint16_t crc = 0xffff, read_crc;
    
    RESET(MCP2517_CS);
    byte = SPI_READ_CRC| ((adress >> 8) & 0x0F);
    crc = crc16_byte(crc,byte);
    spi_putc(byte);
    byte = adress & 0x0ff;
    crc = crc16_byte(crc,byte);
    spi_putc(byte);
    spi_putc(len);
    crc = crc16_byte(crc,len);
    for (i = 0; i < len; i++) {
        buffer[i] = spi_putc(0xff);
    }
    crc = crc16(crc,buffer,len);
    read_crc |= (spi_putc(0xff) << 8);
    read_crc  = spi_putc(0xff);
    SET(MCP2517_CS);
    if (crc != read_crc) return -1;
    else return len;
}


void  mcp2517_write_crc(uint16_t adress,uint8_t *buffer,uint8_t len)
{
    uint8_t i, byte;
    if ((len & 0x3) != 0) {return -2;}
    uint16_t crc = 0xffff;
    
    RESET(MCP2517_CS);
    byte = SPI_WRITE_CRC| ((adress >> 8) & 0x0F);
    crc = crc16_byte(crc,byte);
    spi_putc(byte);
    byte = adress & 0x0ff;
    crc = crc16_byte(crc,byte);
    spi_putc(byte);
    crc = crc16_byte(crc,len);
    spi_putc(len);
    for (i = 0; i < len; i++) {
        spi_putc(buffer[i]);
    }
    crc = crc16(crc,buffer,len);
    spi_putc((crc>>8)&0x0ff);
    spi_putc(crc&0x0ff);
    SET(MCP2517_CS);
}

void  mcp2517_write_safe(uint16_t adress,uint32_t word)
{
    uint8_t  i, byte, buffer[4];
    uint16_t crc = 0xffff;
    
    buffer[0] = word & 0x0ff;
    buffer[1] = (word >> 8) & 0x0ff;
    buffer[2] = (word >> 16) & 0x0ff;
    buffer[3] = (word >> 24) & 0x0ff;
    
    RESET(MCP2517_CS);
    byte = SPI_WRITE_SAFE| ((adress >> 8) & 0x0F);
    crc = crc16_byte(crc,byte);
    byte = adress & 0x0ff;
    crc = crc16_byte(crc,byte);
    spi_putc(byte);
    for (i = 0; i < 4; i++) {
        spi_putc(buffer[i]);
    }
    crc = crc16(crc,buffer,4);
    spi_putc((crc>>8)&0x0ff);
    spi_putc(crc&0x0ff);
    SET(MCP2517_CS);
}
// -------------------------------------------------------------------------

#if 0

// I don't know how all of these numbers were derived and don't know the mcp2517 equiv.
// I am only going to support 125 Kbps for now.

const uint8_t _mcp2515_cnf[8][3] PROGMEM = {
	// 10 kbps
	{	0x04,
		0xb6,
		0xe7
	},
	// 20 kbps
	{	0x04,
		0xb6,
		0xd3
	},
	// 50 kbps
	{	0x04,
		0xb6,
		0xc7
	},
	// 100 kbps
	{	0x04,
		0xb6,
		0xc3
	},
	// 125 kbps
	{	(1<<PHSEG21),					// CNF3
		(1<<BTLMODE)|(1<<PHSEG11),		// CNF2
		(1<<BRP2)|(1<<BRP1)|(1<<BRP0)	// CNF1
	},
	// 250 kbps
	{	0x03,
		0xac,
		0x81
	},
	// 500 kbps
	{	0x03,
		0xac,
		0x80
	},
	// 1 Mbps
	{	(1<<PHSEG21),
		(1<<BTLMODE)|(1<<PHSEG11),
		0
	}
};
#endif

const uint32_t _mcp2517_cnf[8] PROGMEM = {
	// 10 kbps
	// ((50-1)<<CiDBTCFG_BRP) | ((14-1)<<CiDBTCFG_TSEG1) | ((5-1)<<CiDBTCFG_TSEG2) | ((4-1)<<CiDBTCFG_SJW)
	0x310d0403,
	// 20 kbps
	// ((25-1)<<CiDBTCFG_BRP) | ((14-1)<<CiDBTCFG_TSEG1) | ((5-1)<<CiDBTCFG_TSEG2) | ((4-1)<<CiDBTCFG_SJW)
	0x180d0403,
	// 50 kbps
	// ((10-1)<<CiDBTCFG_BRP) | ((14-1)<<CiDBTCFG_TSEG1) | ((5-1)<<CiDBTCFG_TSEG2) | ((4-1)<<CiDBTCFG_SJW)
	0x090d0403,
	// 100 kbps
	// ((5-1)<<CiDBTCFG_BRP) | ((14-1)<<CiDBTCFG_TSEG1) | ((5-1)<<CiDBTCFG_TSEG2) | ((4-1)<<CiDBTCFG_SJW)
	0x040d0403,
	// 125 kbps
	// ((10-1)<<CiDBTCFG_BRP) | ((4-1)<<CiDBTCFG_TSEG1) | ((4-1)<<CiDBTCFG_TSEG2) | ((1-1)<<CiDBTCFG_SJW)
	0x09030300,
	// 250 kbps
	// ((2-1)<<CiDBTCFG_BRP) |  ((8-1)<<CiDBTCFG_TSEG1) | ((4-1)<<CiDBTCFG_TSEG2) | ((3-1)<<CiDBTCFG_SJW)
	0x01070302,
	// 500 kbps
	// ((1-1)<<CiDBTCFG_BRP) |  ((8-1)<<CiDBTCFG_TSEG1) | ((4-1)<<CiDBTCFG_TSEG2) | ((3-1)<<CiDBTCFG_SJW)
	0x00070302,
	// 1 Mbps1 Mbps
	// ((1-1)<<CiDBTCFG_BRP) |  ((4-1)<<CiDBTCFG_TSEG1) | ((2-1)<<CiDBTCFG_TSEG2) | ((1-1)<<CiDBTCFG_SJW)
	0x00030100
};
    
bool mcp2517_init(uint8_t bitrate)
{
    uint32_t dataword;
    
    if (bitrate >= 8) return false; // 
    
    SET(MCP2517_CS);
    SET_OUTPUT(MCP2517_CS);
    
    // Activate pins for the SPI Interface
    RESET(P_SCK);
    RESET(P_MOSI);
    RESET(P_MISO);
    
    SET_OUTPUT(P_SCK);
    SET_OUTPUT(P_MOSI);
    SET_INPUT(P_MISO);
    
    // SPI Initialization
    mcp251x_spi_init();
    
    // MCP2517 software reset
    RESET(MCP2517_CS);
    spi_putc(SPI_RESET);
    SET(MCP2517_CS);
    
    // Wait for MCP2517 to start up.
    _delay_ms(0.1);
    
    // OSC init.
    dataword = 0;
    mcp2517_write_register(OSC,dataword); // Wake up and enter config mode
    dataword |= CLKOUT_PRESCALER_ << OSC_CLKODIV;
    mcp2517_write_register(OSC,dataword);
    
    // GPIO Pins
    mcp2517_write_register(IOCON,(1<<IOCON_PM1)|(1<<IOCON_PM0));
    
    // C1CON register
    dataword = 0;
    dataword |= 1 << CiCON_ISOCRCEN; // Not sure about this..
    dataword |= 0 << CiCON_STEF;     // Disable Store in Transmit Event FIFO
    dataword |= 1 << CiCON_TXQEN;    // Enable Transmit Queue
    dataword |= CiCON_OPMOD_CONFIG << CiCON_REQOP;
    mcp2517_write_register(C1CON,dataword);
          
    // Bit timing configuration
    dataword = _mcp2517_cnf[bitrate];
    mcp2517_write_register(C1NBTCFG,dataword);
    mcp2517_write_register(C1DBTCFG,dataword);
    
    // TEF Not used.
    // TEF Configuration - 12 messages, time stamping enabled
    //dataword  = (12-1) << CiTEFCON_FSIZE;  // Guess
    //dataword |= 1 << CiTEFCON_TEFTSEN; // ???
    //mcp2517_write_register(C1TEFCON,dataword);
    
    // TXQ Configuration -- 8 messages, 32 byte payload, high priority
    dataword  = 1 << CiTXQCON_TXPRI;
    dataword |= (8-1) << CiTXQCON_FSIZE;
    dataword |= CiTXQCON_PLSIZE_32;
    mcp2517_write_register(C1TXQCON,dataword);
    
    // FIFO 1: Transmit FIFO: 5 messages, 64 byte maximum payload, low priority
    dataword  = 1 << CiFIFOCONm_TXEN; // Transmit FIFO
    dataword |= (5-1) << CiFIFOCONm_FSIZE;
    dataword |= CiTXQCON_PLSIZE_64 << CiFIFOCONm_PLSIZE;
    dataword |= 0 << CiFIFOCONm_TXPRI;
    mcp2517_write_register(C1FIFOCON1,dataword);

    // FIFO 2: Receive FIFO: 16 messages, 64 byte maximum payload, time stamping enabled, FIFO not empty Interrupt Enable.
    dataword  = 0 << CiFIFOCONm_TXEN; // Receive FIFO
    dataword |= (16-1) << CiFIFOCONm_FSIZE;
    dataword |= CiTXQCON_PLSIZE_64 << CiFIFOCONm_PLSIZE;
    dataword |= 1 << CiFIFOCONm_RXTSEN;
    dataword |= 1 << CiFIFOCONm_TFNRFNIE;
    mcp2517_write_register(C1FIFOCON2,dataword);
    
    // Enable ECC
    dataword  = 1 << ECCCON_ECCEN;
    mcp2517_write_register(ECCCON,dataword);
    
    // RAM Usage:     TEF           TXQ          TXFIFO       RXFIFO
    uint16_t rsizeB = /*(12*12) +*/ (8*(8+32)) + (5*(8+64)) + (16*(12+64));
    uint16_t rsizeL = (rsizeB+3)/4;
    uint16_t iw, ramaddress;
    
    // Check for over allocation...
    if (rsizeB > 2048) {
        // Opps over allocated RAM in the mcp2517...
    }
    // Initialize RAM to all 1's.
    ramaddress = RAMSTART;
    dataword   = 0xffffffff;
    for (iw = 0; iw < rsizeL; iw++) {
        mcp2517_write_register(ramaddress,dataword);
        ramaddress += 4;
    }
    
    // Time Stamp Control
    mcp2517_write_register(C1TSCON,0); // Stop and clear counter
    dataword  = 0 << CiTSCON_TSRES; // TS at SOF for FD frames
    dataword |= 0 << CiTSCON_TSEOF; // TS at SOF for clasical frames
    dataword |= 1 << CiTSCON_TBCEN; // Enable TS
    dataword |= ((80-1)<<CiTSCON_TBCPRE); // Counter at 4us (20Mhz / 80).
    mcp2517_write_register(C1TSCON,dataword);
    
    // Disable all filters
    mcp2517_write_register(C1FLTCON0,0);
    mcp2517_write_register(C1FLTCON1,0);
    mcp2517_write_register(C1FLTCON2,0);
    mcp2517_write_register(C1FLTCON3,0);
    mcp2517_write_register(C1FLTCON4,0);
    mcp2517_write_register(C1FLTCON5,0);
    mcp2517_write_register(C1FLTCON7,0);
    // Now enable filter 0, with a "wild card" mask (should match all 
    // messages).  Feed the messages (all of them) to FIFO 2.
    mcp2517_write_register(C1MASK0,0);
    mcp2517_write_register(C1FLTOBJ0,0);
    dataword = (1<<CiFLTCONm_FLTEN0)|(CiFLTCONm_BP2<<CiFLTCONm_F0BP);
    mcp2517_write_register(C1FLTCON0,dataword);
    
    // Enable interrupts.
    mcp2517_write_register(C1INT,MCP2517_INTERRUPTS);
    
#if defined(MCP2517_INT)
    SET_INPUT(MCP2517_INT);
    SET(MCP2517_INT);
    // Set up interrupt handler?
#endif
    
    bool error = false;
    // Check bit timing registers...
    if (mcp2517_read_register(C1NBTCFG) != dataword) error = true;
    if (mcp2517_read_register(C1DBTCFG) != dataword) error = true;
    
    if (error) {
        return false;
    } else {
        // Enter normal operating mode
        dataword = mcp2517_read_register(C1CON);
        dataword &= ~(CiCON_REQOP_M<<CiCON_REQOP);
        dataword |= CiCON_OPMOD_NORMALCANFD<<CiCON_REQOP;
        mcp2517_write_register(C1CON,dataword);
        
        // Wait for the mcp2517 to come up.
        while (((mcp2517_read_register(C1CON) >> CiCON_OPMOD) & CiCON_OPMOD_M) != CiCON_OPMOD_NORMALCANFD) ;
        return true;
    }
}

#endif
