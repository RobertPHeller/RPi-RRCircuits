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
 *  Last Modified : <230402.1216>
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
 * @page assembly Assembly
 * 
 * Assembly is straight forward.  The SMD parts are already soldered,
 * so only the through hole part need to be soldered to the board. 
 * Start with the shortest parts and work towards the tallest.  The
 * pin headers are in the form of a single 2x40 "break away" header.
 * This header can be cut with a pair of wire cutters.  You will need
 * two 2x2 and two 2x18 headers.  The two 2x2 headers are for the 
 * termination headers and the two 2x18 headers will go in the Pocket
 * Beagle.  Also be sure to face the wire openings of the screw 
 * terminals out from the board.
 * 
 * @image html LCC-PNET-ROUTER-3D.png
 * @image latex LCC-PNET-ROUTER-3D.png "3D top view of the PCB"
 * 
 * 
 * 
 * @page wiring General Wiring Notes
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
 * @section usbhost USB Host connector.
 * 
 * The USB-A connector is a USB Host connection.  Any standard USB 2.0
 * device could be connected, but main use would be for a USB to 
 * Ethernet or WiFi is the primary intent.  Be sure to check for driver
 * support.
 * 
 * @page download Downloadables and Software Support
 * 
 * @section initialsetup Initial Setup
 * The included microSD card is based on a Beagle Bone 10.3 iot image
 * and has LCC-PNET-Router, OpenMRN, and the rest of the packages
 * and utilities needed to rebuild the firmware.  The firmware has
 * been built and installed under @c /opt/LCC-PNET-Router/sbin and
 * a systemd system file has been installed under 
 * @c /etc/systemd/system (but not been enabled).  The 
 * @c /boot/uEnv.txt and @c /etc/network/interfaces files have been
 * edited to bring up both CAN IFs (@c can0 and @c can1).  Things are
 * mostly ready.  There are some local configuration that must be done
 * to makes this properly ready.
 *
 * The default login for the Beagle Bone 10.3 iot image is
 * 
 * - Username: debian
 * - Password: temppwd
 * 
 * With the included microSD card inserted in the PocketBeagle and a
 * USB cable connected between a desktop or laptop computer and the 
 * PocketBeagle, there should be a network connection set up:
 * 
 * | IP        Address  | Conn. Type | Operating System(s) |
 * | :----------------- | :--------- | :------------------ |
 * | 192.168.7.2        | USB        | Windows             |
 * | 192.168.6.2        | USB        | Mac OS X, Linux     |
 * 
 * You should be able to use a ssh client (Putty under MS-Windows, 
 * native slogin in a terminal window under Mac OS X and Linux) to 
 * connect:
 * 
 * @code
 * slogin debian@192.168.6.2
 * debian password:
 * @endcode
 * 
 * The password is @c temppwd.
 * 
 * Once logged in, you will want to edit the configuration file:
 * @c /etc/defaults/lccpnetrouter.  The important thing to edit is
 * the NID.  You can get a unique id range here:
 * https://registry.openlcb.org/requestuniqueidrange - you should not
 * use the default in the file, it is only there as a placeholder.
 * Once you have finished editing the file, you can enable the 
 * router to start at boot time automatically:
 * 
 * @code
 * sudo systemctl enable lccpnetrouter
 * @endcode
 * 
 * At this point you can safely shutdown the PocketBeagle with the
 * command:
 * 
 * @code
 * sudo /sbin/poweroff
 * @endcode
 * 
 * Once all of the LEDs go off, you can disconnect the PocketBeagle
 * from the laptop or desktop computer and plug it into the router
 * board.  Be sure to orient it properly -- the end with the USB 
 * connector should be towards to LCC connectors, as indicated on the
 * board. You can now connect it to the networks and power the networks
 * up.  Once the PocketBeagle boots up, the router will be up and ready
 * to be configured with a CDI configuration tool (such as JMRI).
 *
 * @section configuration Configuring the Router
 * 
 * The router has three configuration tabs, each with 32 instances:
 * 
 * -# PNET Triggers
 * -# PNET Controls
 * -# PNET Dimmers
 * 
 * They are all very similar.  For each instance there is a name, an
 * enable flag, two events, and a set of instance specific parameters.
 * 
 * One of the events will be produced when a matching PNET message is
 * received and the other event will be consumed and will cause a
 * PNET message to be sent.
 * 
 * There are 32 of each of these message types as the code is shipped.
 * 
 * 
 ****************************************************************************/
