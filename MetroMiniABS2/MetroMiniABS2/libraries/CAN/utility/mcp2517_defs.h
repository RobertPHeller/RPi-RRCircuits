// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed May 30 13:52:31 2018
//  Last Modified : <180603.1646>
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

#ifndef __MCP2517_DEFS_H
#define __MCP2517_DEFS_H

/** @name SPI Commands
 * @{
 */
#define SPI_RESET 0x00
#define SPI_READ  0x30
#define SPI_WRITE 0x20
#define SPI_READ_CRC 0xb0
#define SPI_WRITE_CRC 0xa0
#define SPI_WRITE_SAFE 0xc0
/**@}*/

/** @name Register addresses MCP2517
 *
 * Memory Map (page 7 of the MCP2517FD data sheet):
 *                 111111111122222222223333333333
 *       0123456789012345678901234567890123456789
 *       MSB                                  LSB
 *       Address                          Address   
 *       <-----------------32 bit--------------->
 *  0x003|MSB      |         |         |     LSB|0x000
 *       ----------------------------------------
 *       |                                      |
 *       |    CAN FD Controller Module SFR      |
 *       |          (752 BYTE)                  |
 *  0x2EF|                                      |0x02EC
 *       ----------------------------------------
 *  0x2F3|     Unimplemented                    |0x2F0
 *       |      (272 BYTE)                      |
 *  0x3FF|                                      |0x3FC
 *       ----------------------------------------
 *  0x403|                                      |0x400
 *       |    RAM                               |
 *  0xBFF|    (2K BYTE)                         |0xBFC
 *       ----------------------------------------
 *  0xC03|   Unimplemented                      |0xC00
 *       |     (512 BYTE)                       |
 *  0xDFF|                                      |0xDFC
 *       ----------------------------------------
 *  0xE03|                                      |0xE00
 *       |    MCP2517FD SFR                     |
 *       |     (20 BYTE)                        |
 *  0xE13|                                      |0xE10
 *       ----------------------------------------
 *  0xE17|                                      |0xE14
 *       |     Reserved                         |
 *       |      (492 BYTE)                      |
 *  0xFFF|                                      |0xFFC
 *       ----------------------------------------
 * 
 * All registers are 32 bits long (4 bytes).
 *
 * 
 * @{
 */
#define OSC         0xE00
#define IOCON       0xE04
#define CRC         0xE08
#define ECCCON      0xE0C
#define ECCSTAT     0xE10

#define C1CON       0x000
#define C1NBTCFG    0x004
#define C1DBTCFG    0x008
#define C1TDC       0x00C
#define C1TBC       0x010
#define C1TSCON     0x014
#define C1VEC       0x018
#define C1INT       0x01C
#define C1RXIF      0x020
#define C1TXIF      0x024
#define C1RXOVIF    0x028
#define C1TXATIF    0x02C
#define C1TXREQ     0x030
#define C1TREC      0x034
#define C1BDIAG0    0x038
#define C1BDIAG1    0x03C
#define C1TEFCON    0x040
#define C1TEFSTA    0x044
#define C1TEFUA     0x048
#define C1TXQCON    0x050
#define C1TXQSTA    0x054
#define C1TXQUA     0x058
#define C1FIFOCON1  0x05C
#define C1FIFOSTA1  0x060
#define C1FIFOUA1   0x064
#define C1FIFOCON2  0x068
#define C1FIFOSTA2  0x06C
#define C1FIFOUA2   0x070
#define C1FIFOCON3  0x074
#define C1FIFOSTA3  0x078
#define C1FIFOUA3   0x07C
#define C1FIFOCON4  0x080
#define C1FIFOSTA4  0x084
#define C1FIFOUA4   0x088
#define C1FIFOCON5  0x08C
#define C1FIFOSTA5  0x090
#define C1FIFOUA5   0x094
#define C1FIFOCON6  0x098
#define C1FIFOSTA6  0x09C
#define C1FIFOUA6   0x0A0
#define C1FIFOCON7  0x0A4
#define C1FIFOSTA7  0x0A8
#define C1FIFOUA7   0x0AC
#define C1FIFOCON8  0x0B0
#define C1FIFOSTA8  0x0B4
#define C1FIFOUA8   0x0B8
#define C1FIFOCON9  0x0BC
#define C1FIFOSTA9  0x0C0
#define C1FIFOUA9   0x0C4
#define C1FIFOCON10 0x0C8
#define C1FIFOSTA10 0x0CC
#define C1FIFOUA10  0x0D0
#define C1FIFOCON11 0x0D4
#define C1FIFOSTA11 0x0D8
#define C1FIFOUA11  0x0DC
#define C1FIFOCON12 0x0E0
#define C1FIFOSTA12 0x0E4
#define C1FIFOUA12  0x0E8
#define C1FIFOCON13 0x0EC
#define C1FIFOSTA13 0x0F0
#define C1FIFOUA13  0x0F4
#define C1FIFOCON14 0x0F8
#define C1FIFOSTA14 0x0FC
#define C1FIFOUA14  0x100
#define C1FIFOCON15 0x104
#define C1FIFOSTA15 0x108
#define C1FIFOUA15  0x10C
#define C1FIFOCON16 0x110
#define C1FIFOSTA16 0x114
#define C1FIFOUA16  0x118
#define C1FIFOCON17 0x11C
#define C1FIFOSTA17 0x120
#define C1FIFOUA17  0x124
#define C1FIFOCON18 0x128
#define C1FIFOSTA18 0x12C
#define C1FIFOUA18  0x130
#define C1FIFOCON19 0x134
#define C1FIFOSTA19 0x138
#define C1FIFOUA19  0x13C
#define C1FIFOCON20 0x140
#define C1FIFOSTA20 0x144
#define C1FIFOUA20  0x148
#define C1FIFOCON21 0x14C
#define C1FIFOSTA21 0x150
#define C1FIFOUA21  0x154
#define C1FIFOCON22 0x158
#define C1FIFOSTA22 0x15C
#define C1FIFOUA22  0x160
#define C1FIFOCON23 0x164
#define C1FIFOSTA23 0x168
#define C1FIFOUA23  0x16C
#define C1FIFOCON24 0x170
#define C1FIFOSTA24 0x174
#define C1FIFOUA24  0x178
#define C1FIFOCON25 0x17C
#define C1FIFOSTA25 0x180
#define C1FIFOUA25  0x184
#define C1FIFOCON26 0x188
#define C1FIFOSTA26 0x18C
#define C1FIFOUA26  0x190
#define C1FIFOCON27 0x194
#define C1FIFOSTA27 0x198
#define C1FIFOUA27  0x19C
#define C1FIFOCON28 0x1A0
#define C1FIFOSTA28 0x1A4
#define C1FIFOUA28  0x1A8
#define C1FIFOCON29 0x1AC
#define C1FIFOSTA29 0x1B0
#define C1FIFOUA29  0x1B4
#define C1FIFOCON30 0x1B8
#define C1FIFOSTA30 0x1BC
#define C1FIFOUA30  0x1C0
#define C1FIFOCON31 0x1C4
#define C1FIFOSTA31 0x1C8
#define C1FIFOUA31  0x1CC
#define C1FLTCON0   0x1D0
#define C1FLTCON1   0x1D4
#define C1FLTCON2   0x1D8
#define C1FLTCON3   0x1DC
#define C1FLTCON4   0x1E0
#define C1FLTCON5   0x1E4
#define C1FLTCON6   0x1E8
#define C1FLTCON7   0x1EC
#define C1FLTOBJ0   0x1F0
#define C1MASK0     0x1F4
#define C1FLTOBJ1   0x1F8
#define C1MASK1     0x1FC
#define C1FLTOBJ2   0x200
#define C1MASK2     0x204
#define C1FLTOBJ3   0x208
#define C1MASK3     0x20C
#define C1FLTOBJ4   0x210
#define C1MASK4     0x214
#define C1FLTOBJ5   0x218
#define C1MASK5     0x21C
#define C1FLTOBJ6   0x220
#define C1MASK6     0x224
#define C1FLTOBJ7   0x228
#define C1MASK7     0x22C
#define C1FLTOBJ8   0x230
#define C1MASK8     0x234
#define C1FLTOBJ9   0x238
#define C1MASK9     0x23C
#define C1FLTOBJ10  0x240
#define C1MASK10    0x244
#define C1FLTOBJ11  0x248
#define C1MASK11    0x24C
#define C1FLTOBJ12  0x250
#define C1MASK12    0x254
#define C1FLTOBJ13  0x258
#define C1MASK13    0x25C
#define C1FLTOBJ14  0x260
#define C1MASK14    0x264
#define C1FLTOBJ15  0x268
#define C1MASK15    0x26C
#define C1FLTOBJ16  0x270
#define C1MASK16    0x274
#define C1FLTOBJ17  0x278
#define C1MASK17    0x27C
#define C1FLTOBJ18  0x280
#define C1MASK18    0x284
#define C1FLTOBJ19  0x288
#define C1MASK19    0x28C
#define C1FLTOBJ20  0x290
#define C1MASK20    0x294
#define C1FLTOBJ21  0x298
#define C1MASK21    0x29C
#define C1FLTOBJ22  0x2A0
#define C1MASK22    0x2A4
#define C1FLTOBJ23  0x2A8
#define C1MASK23    0x2AC
#define C1FLTOBJ24  0x2B0
#define C1MASK24    0x2B4
#define C1FLTOBJ25  0x2B8
#define C1MASK25    0x2BC
#define C1FLTOBJ26  0x2C0
#define C1MASK26    0x2C4
#define C1FLTOBJ27  0x2C8
#define C1MASK27    0x2CC
#define C1FLTOBJ28  0x2D0
#define C1MASK28    0x2D4
#define C1FLTOBJ29  0x2D8
#define C1MASK29    0x2DC
#define C1FLTOBJ30  0x2E0
#define C1MASK30    0x2E4
#define C1FLTOBJ31  0x2E8
#define C1MASK31    0x2EC

/**@}*/

/** @name Bit definiations for the registers.
 * @{
 */

/** OSC -- Oscillator Control Register bits */
#define OSC_SCLKRDY 12
#define OSC_OSCRDY  10
#define OSC_PLLRDY   8
#define OSC_CLKODIV  5
#define OSC_SCLKDIV  4
#define OSC_OSCDIS   2
#define OSC_PLLEN    0

/** IOCON -- Input/Output Control Register bits */
#define IOCON_INTOD   30
#define IOCON_SOF     29
#define IOCON_TXCANOD 28
#define IOCON_PM1     25
#define IOCON_PM0     24
#define IOCON_GPIO1   17
#define IOCON_GPIO0   16
#define IOCON_LAT1     9
#define IOCON_LAT0     8
#define IOCON_XSTBYEN  6
#define IOCON_TRIS1    1
#define IOCON_TRIS0    0

/** CRC -- CRC Register bits */
#define CRC_FERRIE    25
#define CRC_CRCERRIE  24
#define CRC_FERRIF    17
#define CRC_CRCERRIF  16
#define CRC_CRC        0
#define CRC_CRCMASK  0x0FFFF

/** ECCCON -- ECC Control Register */
#define ECCCON_PARITY 8
#define ECCCON_PARITYMASK 0x3F
#define ECCCON_DEDIE 2
#define ECCCON_SESIE 1
#define ECCCON_ECCEN 0

/** ECCSTAT -- ECC Status Register */
#define ECCSTAT_ERRADDR 16
#define ECCSTAT_ERRADDRMASK 0x0FFF
#define ECCSTAT_DEDIF 2
#define ECCSTAT_SECIF 1

/** CiNBTCFG -- Nominal Bit Time Configuration Register */
#define CiNBTCFG_BRP 24
#define CiNBTCFG_BRP_M 0x0ff
#define CiNBTCFG_TSEG1 16
#define CiNBTCFG_TSEG1_M 0x7f
#define CiNBTCFG_TSEG2 8
#define CiNBTCFG_TSEG3_M 0x7f
#define CiNBTCFG_SJW 0
#define CiNBTCFG_SJW_M 0x7f

/** CiDBTCFG -- Data Bit Time Configuration Register */
#define CiDBTCFG_BRP 24
#define CiDBTCFG_BRP_M 0x0ff
#define CiDBTCFG_TSEG1 16
#define CiDBTCFG_TSEG1_M 0x1f
#define CiDBTCFG_TSEG2 8
#define CiDBTCFG_TSEG3_M 0x0f
#define CiDBTCFG_SJW 0
#define CiDBTCFG_SJW_M 0x0f

/** CiINT Interrupt Register */
#define CiINT_IVMIE    31
#define CiINT_WAKIE    30
#define CiINT_CERRIE   29
#define CiINT_SERRIE   28
#define CiINT_RXOVIE   27
#define CiINT_TXATIE   26
#define CiINT_SPICRCIE 25
#define CiINT_ECCIE    24
#define CiINT_TEFIE    20
#define CiINT_MODIE    19
#define CiINT_TBCIE    18
#define CiINT_RXIE     17
#define CiINT_TXIE     16
#define CiINT_IVMIF    15
#define CiINT_WAKIF    14
#define CiINT_CERRIF   13
#define CiINT_SERRIF   12
#define CiINT_RXOVIF   11
#define CiINT_TXATIF   10
#define CiINT_SPICRCIF  9
#define CiINT_ECCIF     8
#define CiINT_TEFIF     4
#define CiINT_MODIF     3
#define CiINT_TBCIF     2
#define CiINT_RXIF      1
#define CiINT_TXIF      0

/** CiCON CAN Control Register */

#define CiCON_TXBWS    28
#define CiCON_TXBWS_M  0x0F
#define CiCON_ABAT     27
#define CiCON_REQOP    14
#define CiCON_REQOP_M  0x07
#define CiCON_OPMOD    21
#define CiCON_OPMOD_M  0x07
#define CiCON_TXQEN    20
#define CiCON_STEF     19
#define CiCON_SERR2LOM 18
#define CiCON_ESIGM    17
#define CiCON_RTXAT    16
#define CiCON_BRSDIS   12
#define CiCON_BUSY     11
#define CiCON_WFT       9
#define CiCON_WFT_M    0x03
#define CiCON_WAKFIL    8
#define CiCON_PXEDIS    6
#define CiCON_ISOCRCEN  5
#define CiCON_DNCNT     0
#define CiCON_DNCNT_M   0x1F

/** CiRXIF -- Receive Interrupt Status Register */
#define CiRXIF_RFIF     1
#define CiRXIF_RFIF_M   0x7FFFFFFF

/** CiTEFSTA -- Transmit Event FIFO Status Register */
#define CiTEFSTA_TEFOVIF 3
#define CiTEFSTA_TEFFIF  2
#define CiTEFSTA_TEFHIF  1
#define CiTEFSTA_TEFNEIF 0

/** @} */

#endif  // __MCP2517_DEFS_H


