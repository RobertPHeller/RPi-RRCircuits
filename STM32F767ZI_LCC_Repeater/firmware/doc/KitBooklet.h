// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Jun 3 13:45:54 2024
//  Last Modified : <240607.1601>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
/// @copyright
///    Copyright (C) 2024  Robert Heller D/B/A Deepwoods Software
///			51 Locke Hill Road
///			Wendell, MA 01379-9728
///
///    This program is free software; you can redistribute it and/or modify
///    it under the terms of the GNU General Public License as published by
///    the Free Software Foundation; either version 2 of the License, or
///    (at your option) any later version.
///
///    This program is distributed in the hope that it will be useful,
///    but WITHOUT ANY WARRANTY; without even the implied warranty of
///    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///    GNU General Public License for more details.
///
///    You should have received a copy of the GNU General Public License
///    along with this program; if not, write to the Free Software
///    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
/// @file KitBooklet.h
/// @author Robert Heller
/// @date Mon Jun 3 13:45:54 2024
///
/// @page 00kitContents Kit Contents
/// This kit contains:
/// -# This booklet.
/// -# The router PCB.
/// @image html STM32F767ZI_LCC_Repeater_thumb.png
/// @image latex STM32F767ZI_LCC_Repeater_thumb.png height=1in
/// -# 2 2x19 socket headers
/// @image html ssq-101-01-f-d_SPL.jpg
/// @image latex ssq-101-01-f-d_SPL.jpg height=1in
/// -# 2 RJ45 connectors
/// @image html WR-MJ_SPL.jpg
/// @image latex WR-MJ_SPL.jpg height=1in
/// -# 2 2-position 2.54 pitch screw terminals
/// @image html ITP_538-39773-0002_2.jpg
/// @image latex ITP_538-39773-0002_2.jpg height=1in
/// -# 2 2x19 2.54 pitch pin headers
/// @image html MFG_TSW-119-09-T-D_tmb64x64.jpg
/// @image latex MFG_TSW-119-09-T-D_tmb64x64.jpg height=1in
/// -# 1 NUCLEO-F767ZI
/// @image html NUCLEO-F767ZI.jpg
/// @image latex NUCLEO-F767ZI.jpg height=1in
/// @page 01assembly Assembly
///
/// Assembly is straight forward.  The SMD parts are already soldered,
/// so only the through hole part need to be soldered to the board. 
/// Start with the shortest parts and work towards the tallest. Their are
/// two 2x2 and two 2x19 headers.  The two 2x2 headers are for the 
/// termination headers and the two 2x19 headers will go in the NUCLEO-F767ZI.
/// NUCLEO-F767ZI has holes for extended ST Morpho connectors -- only the base 
/// ST Morpho is used, at the end towards the ST-Link. Also be sure to face the
/// wire openings of the screw terminals out from the board.
/// 
/// @image html STM32F767ZI_LCC_Repeater.png
/// @image latex STM32F767ZI_LCC_Repeater.png "3D top view of the PCB"
/// 
/// Caution: before applying power to the repeater board, including connecting
/// to a LCC network, be sure that jumper JP1 ("PWR-EXT") next to the uUSB 
/// connector) is OFF and JP3 ("Power Source") has its jumper on the leftmost 
/// pair of pins (labeled E5V).
///
/// @page 02wiring General Wiring Notes
/// 
/// There are two RJ45 connectors for the LCC network segments.  One connector
/// for each segment.  The board includes termination resistors for both 
/// segments.
///
/// There are terminals for LCC network power input available. The repeater is
/// powered from the LCC network power bus, There are diodes protecting the 
/// power buses.
/// 
//////////////////////////////////////////////////////////////////////////////

