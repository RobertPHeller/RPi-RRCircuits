// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 31 07:37:17 2018
//  Last Modified : <180612.0845>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
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

#ifndef __MCP2517_PRIVATE_H
#define __MCP2517_PRIVATE_H

/**
 * @brief Defines only used inside the library
 * 
 * @author Robert Heller <heller@deepsoft.com>
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <stdbool.h>

#include "can.h"
#include "utils.h"

#include "can_private.h"
#include "spi.h"

#if defined(SUPPORT_MCP2517) && (SUPPORT_MCP2517 == 1)
	#if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega644__)
		#define	P_MOSI	B,5
		#define	P_MISO	B,6
		#define	P_SCK	B,7
		#define	SUPPORT_FOR_MCP2517__
	#elif defined(__AVR_ATmega8__)  || defined(__AVR_ATmega48__) || \
		  defined(__AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
		#define	P_MOSI	B,3
		#define	P_MISO	B,4
		#define	P_SCK	B,5
		#define	SUPPORT_FOR_MCP2517__
	#elif defined(__AVR_ATmega128__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		#define	P_MOSI	B,2
		#define	P_MISO	B,3
		#define	P_SCK	B,1
		#define	SUPPORT_FOR_MCP2517__
	#elif defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
		#define	P_MOSI	B,0
		#define	P_MISO	B,1
		#define	P_SCK	B,2
			
		#define	USE_SOFTWARE_SPI		1
		#define	SUPPORT_FOR_MCP2517__
	#else
		#error	choosen AVR-type is not supported yet!
	#endif
#endif

#ifdef  SUPPORT_FOR_MCP2517__

#define MCP2517_DISABLE_MASKS_AND_FILTERS 1
// ----------------------------------------------------------------------------
// load some default values

#ifndef	MCP2517_CLKOUT_PRESCALER
	#define	MCP2517_CLKOUT_PRESCALER	0
#endif
#ifndef	MCP2517_INTERRUPTS
	#define	MCP2517_INTERRUPTS			(1<<CiINT_RXIE)
#endif

// ----------------------------------------------------------------------------
// TODO: this file is imcompatible with the at90can
#include "mcp2517_defs.h"

#ifndef	MCP2517_CS
	#error	MCP2517_CS ist nicht definiert!
#endif

//#if defined(MCP2517_RX0BF) && !defined(MCP2517_RX1BF)
//	#warning	only MCP2517_RX0BF but not MCP2517_RX1BF defined!
//#elif !defined(MCP2517_RX0BF) && defined(MCP2517_RX1BF)
//	#warning	only MCP2517_RX1BF but not MCP2517_RX0BF defined!
//#elif defined(MCP2517_RX0BF) && defined(MCP2517_RX1BF)
//	#define	RXnBF_FUNKTION
//#endif

// -------------------------------------------------------------------------
/**
 * \brief	Write one 32-bit register location
 */
extern void mcp2517_write_register( uint16_t adress, uint32_t data );

// -------------------------------------------------------------------------
/**
 * \brief	Read one 32-bit register location
 */
extern uint32_t mcp2517_read_register(uint16_t adress);

//extern uint8_t mcp2517_read_status(uint8_t type);


/** \brief     Read a buffer of data
 * 
 * len must be a multiple of 4!
 * 
 */

extern int  mcp2517_read_data(uint16_t adress,uint8_t *buffer,uint8_t len);

/** \brief     Read a buffer of data with CRC
 * 
 * len must be a multiple of 4!
 * 
 */

extern int  mcp2517_read_crc(uint16_t adress,uint8_t *buffer,uint8_t len);

/** \brief    Write a buffer of data with CRC
 * 
 * len must be a multiple of 4!
 * 
 */

extern void  mcp2517_write_crc(uint16_t adress,uint8_t *buffer,uint8_t len);

/** \brief    Write a uint32_t of data with Safe CRC
 * 
 * 
 */

extern void  mcp2517_write_safe(uint16_t adress,uint32_t word);

// -------------------------------------------------------------------------
/**
 * \brief	Setzten/loeschen einzelner Bits
 *
 * Diese Funktionen laesst sich nur auf die Register BFPCTRL, 
 * TXRTSCTRL, CANCTRL, CNF1, CNF2, CNF3, CANINTE, CANINTF, EFLG,
 * TXB0CTRL, TXB1CTRL, TXB2CTRL, RXB0CTRL und RXB1CTRL anwenden.
 *
 * \see		Datenblatt des MCP2517, Registerbersichtstabelle
 */
extern void MCP2517_bit_modify(uint8_t adress, uint8_t mask, uint8_t data);

// -------------------------------------------------------------------------
//extern __attribute__ ((gnu_inline)) inline void mcp2517_change_operation_mode(uint8_t mode)
//{
//	mcp2517_bit_modify(CANCTRL, 0xe0, mode);
//	while ((mcp2517_read_register(CANSTAT) & 0xe0) != (mode & 0xe0))
//		;
//}

// -------------------------------------------------------------------------
/**
 * \brief	Liest bzw. schreibt eine CAN-Id zum MCP2517
 */
#if	SUPPORT_EXTENDED_CANID

extern void mcp2517_write_id( const uint32_t *id, uint8_t extended );

extern uint8_t mcp2517_read_id( uint32_t *id );

#else

extern void mcp2517_write_id( const uint16_t *id);

extern uint8_t mcp2517_read_id( uint16_t *id );

#endif	// USE_EXTENDED_CANID

extern void debugf(const char *__fmt,...);

#endif  // SUPPORT_FOR_MCP2517__


#endif // __MCP2517_PRIVATE_H


