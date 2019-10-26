##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Sat Oct 26 09:48:55 2019
#  Last Modified : <191026.1421>
#
#  Description	
#
#  Notes
#
#  History
#	
#  $Log$
#
##############################################################################
#
#    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
#			51 Locke Hill Road
#			Wendell, MA 01379-9728
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# 
#
##############################################################################

## Target / system dependent configs.  Edit as needed
#TCLRUNTIME=/usr/local/share/tclkits/tclkit-linux-armv7l
TCLRUNTIME=/usr/local/share/tclkits/tclkit-8.5.8-linux-x86_64
TCLKIT=/usr/local/bin/tclkit
SDX=/usr/local/bin/sdx
#TCLLIB=/usr/share/tcltk/tcllib1.16
TCLLIB=/usr/share/tcl/tcllib1.16
SNITLIB=$(TCLLIB)/snit
#IMGLIB=/usr/lib/tcltk/arm-linux-gnueabihf/Img1.4.2
IMGLIB=/usr/lib64/tcl8.5/Img1.4
MRRSCRIPTLIB=/home/heller/Deepwoods/ModelRRSystem/trunk/Scripts/Common
MRRBUILDSCRIPTS=/home/heller/Deepwoods/ModelRRSystem/trunk/BuildScripts
ADDKITDIR=$(MRRBUILDSCRIPTS)/AddKitDir.kit
ADDKITFILE=$(MRRBUILDSCRIPTS)/AddKitFile.kit
MAKEPKGINDEX=$(MRRBUILDSCRIPTS)/MakePkgIndex.kit
EXEEXT=

## No need to edit beyond this point.

PROG = CommandStationGUI
SOURCE = $(PROG).tcl
SCRIPTS = mainwindow.tcl $(MRRSCRIPTLIB)/snitscrollw.tcl \
	  $(MRRSCRIPTLIB)/snitrotext.tcl \
	  $(MRRSCRIPTLIB)/snitmainframe.tcl \
	  $(MRRSCRIPTLIB)/snitStdMenuBar.tcl \
	  $(MRRSCRIPTLIB)/splash.tcl \
	  $(MRRSCRIPTLIB)/gettext.tcl \
	  $(MRRSCRIPTLIB)/DynamicHelp.tcl \
	  $(MRRSCRIPTLIB)/snitbuttonbox.tcl \
	  $(MRRSCRIPTLIB)/snitHTMLHelp.tcl \
	  $(MRRSCRIPTLIB)/snitdialog.tcl \
	  $(MRRSCRIPTLIB)/IconsAndImages.tcl
	  
IMAGES = $(wildcard $(MRRSCRIPTLIB)/*.gif) \
	 $(wildcard $(MRRSCRIPTLIB)/*.png) \
	 $(wildcard $(MRRSCRIPTLIB)/*.xbm) \
	 $(wildcard $(MRRSCRIPTLIB)/*.xpm)

all: $(PROG)$(EXEEXT)

$(PROG)$(EXEEXT): $(SOURCE) $(SCRIPTS) $(IMAGES) Makefile
	rm -f $(PROG).kit
	rm -rf $(PROG).vfs 
	$(TCLKIT) $(SDX) qwrap $(SOURCE)
	$(TCLKIT) $(SDX) unwrap $(PROG).kit
	rm -f $(PROG).kit
	$(TCLKIT) $(ADDKITDIR) $(PROG) lib $(SNITLIB)
	$(TCLKIT) $(ADDKITDIR) $(PROG) lib $(IMGLIB)
	$(TCLKIT) $(ADDKITFILE) $(PROG) lib/Scripts \
			$(SCRIPTS) $(IMAGES)
	$(TCLKIT) $(MAKEPKGINDEX) $(PROG) Scripts
	$(TCLKIT) $(SDX) wrap $(PROG)$(EXEEXT) -runtime $(TCLRUNTIME)
	rm -rf $(PROG).vfs
