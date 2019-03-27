// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Mar 25 19:12:50 2019
//  Last Modified : <190326.2108>
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

#ifndef __RRCIRCUITS-BUILDER_H
#define __RRCIRCUITS-BUILDER_H

/** @mainpage Table Of Contents
 * @anchor toc
 * @htmlonly
 * <div class="contents">
 * <div class="textblock"><ol type="1">
 * <li><a class="el" href="reference.html">RRCircuits Builder Reference</a><ol type="1">
 * <li><a class="el" href="reference.html#before">Before you run the RRCircuits Builder</a></li>
 * <li><a class="el" href="reference.html#startup">Starting the Builder</a></li>
 * <li><a class="el" href="reference.html#buildingnodes">Building node programs</a></li>
 * <li><a class="el" href="reference.html#deploying">Deploying node programs</a></li></ol></li>
 * <li><a class="el" href="help.html">Help</a></li>
 * <li><a class="el" href="Version.html">Version</a></li>
 * <li><a class="el" href="Copying.html">Copying</a><ol type="a">
 * <li><a class="el" href="Copying.html#Warranty">Warranty</a></li>
 * </ol></li><!-- Copying -->
 * </ol></div></div><!-- Contents -->
 * @endhtmlonly

 * @endhtmlonly
 * @page reference RRCircuits Builder Reference
 * This program is a convience tool to allow build OpenMRN node 
 * programs that work with the Deepwoods Software RRCircuits 
 * circuit boards for Raspberry Pis, BeagleBone Black, and 
 * PocketBeagle boards allowing for the use of "embedded Linux"
 * as a platform for layout control.  Modelers can use these
 * small and inexpensive Linux computers with one (or in the case
 * the Raspberry Pi and BeagleBone Black, more than one) of the 
 * RRCircuits add-on boards to implement a distributed network, using 
 * either CAN (BeagleBone Black and PocketBeagle), wired Ethernet (
 * BeagleBone Black or Raspberry Pi), or WiFi (Raspberry Pi 3).
 * 
 * Deepwoods Software has provided OpenMRN node programs for these
 * embedded Linux machines that provide configurable OpenLCB nodes
 * for the citcuit boards.  The RRCircuits Builder will build these
 * programs for any of the selected platforms and will build them 
 * with the necessary unique NIDs.
 * 
 * @section before Before you run the RRCircuits Builder
 * 
 * Before running the RRCircuits Builder you need to install the
 * OpenMRN source tree.  The easiest way to do this is to clone it 
 * from Github into a openmrn directory directly under your home
 * directory.  You will also need the build-essential package (this
 * is the C/C++ compiler, binutils, plus development libraries and
 * tools (like make).  Don't worry, you won't have to do much at the
 * command line beyond using git to clone the OpenMRN source tree.
 * 
 * @section startup Starting the Builder
 * 
 * When you start up the Builder, there will be a large blank area
 * which will be where the build log will go.  To the right are two
 * sections if interest.  The upper part, labeled "Options:", is
 * where you will select which of the programs to build, for what
 * target (Raspberry Pi, BeagleBone Black, or PocketBeagle), the
 * number of nodes to build (each with a unique Node ID) and the
 * starting node id to use.
 * 
 * The lower part, labeled "Configuration:", details the detected
 * configuration, which includes the location of the programs,
 * the location of the OpenMRN source tree, the version of the 
 * compiler, and the version of the kernel your machine is running.
 * 
 * @section buildingnodes Building node programs
 * 
 * To build a program you select the program to build, the target,
 * the number of copies to build (each with a unique Node ID), and
 * the starting Node ID (less one) to use.  Then you just need to 
 * click on the Build button and wait for the compilation to complete.
 * 
 * @section deploying Deploying node programs
 * 
 * There is a Deploy button on the toolbar that can be used to copy
 * a node program to a selected target machine, using ssh.
 * 
 */
#endif // __RRCIRCUITS-BUILDER_H

