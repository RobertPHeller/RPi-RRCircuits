/* -*- C -*- //////////////////////////////////////////////////////////////
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : Robert Heller
 *  Created       : Thr Mar 30 10:49:54 2023
 *  Last Modified : <240502.1411>
 *
 *  Description	
 *
 *  Notes
 *
 *  History
 *	
 ****************************************************************************/
/** @copyright
 *  Copyright (c) $YEAR$, Robert Heller
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are  permitted provided that the following conditions are met:
 * 
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 * 
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *  @file KitBooklet.h
 *  @author Robert Heller
 *  @date Thr Mar 30 10:49:54 2023
 * 
 * @page 00kitContents Kit Contents
 * This kit contains:
 * -# This booklet.
 * -# The router PCB.
 * @image html STM32F767ZI_LCC_PNET_Router_thumb.png
 * @image latex STM32F767ZI_LCC_PNET_Router_thumb.png height=1in
 * -# 2 2x19 socket headers
 * @image html ssq-101-01-f-d_SPL.jpg
 * @image latex ssq-101-01-f-d_SPL.jpg height=1in
 * -# 2 RJ45 connectors
 * @image html WR-MJ_SPL.jpg
 * @image latex WR-MJ_SPL.jpg height=1in
 * -# 2 RJ12 connectors
 * @image html 95501-2661_SPL.jpg
 * @image latex 95501-2661_SPL.jpg height=1in
 * -# 3 2-position 2.54 pitch screw terminals
 * @image html ITP_538-39773-0002_2.jpg
 * @image latex ITP_538-39773-0002_2.jpg height=1in
 * -# 2 2x2 2.54 pitch pin headers
 * @image html 87227-2.jpg
 * @image latex 87227-2.jpg height=1in
 * -# 2 2x19 2.54 pitch pin headers
 * @image html MFG_TSW-119-09-T-D_tmb64x64.jpg
 * @image latex MFG_TSW-119-09-T-D_tmb64x64.jpg height=1in
 * -# 1 NUCLEO-F767ZI
 * @image html NUCLEO-F767ZI.jpg
 * @image latex NUCLEO-F767ZI.jpg height=1in
 * @page 01assembly Assembly
 * 
 * Assembly is straight forward.  The SMD parts are already soldered,
 * so only the through hole part need to be soldered to the board. 
 * Start with the shortest parts and work towards the tallest. Their are
 * two 2x2 and two 2x19 headers.  The two 2x2 headers are for the 
 * termination headers and the two 2x19 headers will go in the NUCLEO-F767ZI.
 * NUCLEO-F767ZI has holes for extended ST Morpho connectors -- only the base 
 * ST Morpho is used, at the end towards the ST-Link. Also be sure to face the
 * wire openings of the screw terminals out from the board.
 * 
 * @image html STM32F767ZI_LCC_PNET_Router.png
 * @image latex STM32F767ZI_LCC_PNET_Router.png "3D top view of the PCB"
 * 
 * Caution: before applying power to the router board, including connecting
 * to a LCC network, be sure that jumper JP1 ("PWR-EXT") next to the uUSB 
 * connector) is OFF and JP3 ("Power Source") has its jumper on the leftmost 
 * pair of pins (labeled E5V).
 * 
 * @page 02wiring General Wiring Notes
 * @section termination Termination of the networks.
 * 
 * Each of the two CAN networks includes a termination jumper block.
 * Termination works the same for both.  You have a choise of no 
 * termination, simple termination, and center tapped termination,
 * as shown here:
 * 
 * @image html TerminationJumpers.png
 * @image latex  TerminationJumpers.png "Termination Jumpers" width=3in
 * 
 * @section connecting Connecting the networks,
 * 
 * There are two RJ45 connectors for the LCC network and two RJ12 6-6 
 * connectors for the PNET network.  Connecting to each of these 
 * networks is straight forward and this board connects to the two
 * networks like any other board or node on the respective networks.
 * 
 * There are terminals for LCC network power input and power output
 * available. The router is powered from the LCC network power bus,
 * which can be the local power input terminals or from a power input
 * somewhere else on the LCC network.
 * 
 * There is a power input terminal for the PNET network.  This terminal allows
 * for providing power to PNET boards that need power.
 * 
 *
 * @page 03initialConfig Initial Configuration
 * 
 * The first time the node is started, it is necessary to set the node id.
 * See \ref NodeIdConfig for information about this.
 ****************************************************************************/
