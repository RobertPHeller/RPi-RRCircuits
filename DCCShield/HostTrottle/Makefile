##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon Feb 5 14:36:02 2018
#  Last Modified : <180205.1459>
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
#    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
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


SNITLIB = /usr/share/tcl/tcllib1.16/snit
IMGLIB  = /home/heller/Deepwoods/ModelRRSystem/trunk/BuildData/Img-1.4-Linux64.zip
TCLKITRUNTIME = /usr/local/share/tclkits/tclkit-8.5.8-linux-x86_64
TCLKIT = /usr/local/bin/tclkit
MRRDIR = /home/heller/Deepwoods/ModelRRSystem/trunk/Scripts/Common
EXTRA_SCRIPTS = $(MRRDIR)/gettext.tcl $(MRRDIR)/snitStdMenuBar.tcl \
		$(MRRDIR)/snitHTMLHelp.tcl $(MRRDIR)/snitLFs.tcl \
		$(MRRDIR)/snitmainframe.tcl $(MRRDIR)/snitpagesmanager.tcl \
		$(MRRDIR)/snitdialog.tcl $(MRRDIR)/snitbuttonbox.tcl \
		$(MRRDIR)/snitCabWidgets.tcl $(MRRDIR)/IconsAndImages.tcl \
		$(MRRDIR)/DynamicHelp.tcl $(MRRDIR)/unknown.xpm \
		$(MRRDIR)/snitscrollw.tcl
SDX=/home/heller/Deepwoods/ModelRRSystem/trunk/BuildScripts/sdx.kit

AddKitDir=/home/heller/Deepwoods/ModelRRSystem/trunk/BuildScripts/AddKitDir.kit
AddKitFile=/home/heller/Deepwoods/ModelRRSystem/trunk/BuildScripts/AddKitFile.kit
MakePkgIndex=/home/heller/Deepwoods/ModelRRSystem/trunk/BuildScripts/MakePkgIndex.kit

all: ArduinoThrottle

ArduinoThrottle: ArduinoThrottle.src Host.tcl $(EXTRA_SCRIPTS)
	-rm -f  ArduinoThrottle.kit
	-rm -rf ArduinoThrottle.vfs
	$(TCLKIT) $(SDX) qwrap ArduinoThrottle.src
	$(TCLKIT) $(SDX) unwrap ArduinoThrottle.kit
	-rm -f  ArduinoThrottle.kit
	$(TCLKIT) $(AddKitDir) ArduinoThrottle lib $(SNITLIB)
	unzip -qq $(IMGLIB) -d ArduinoThrottle.vfs/lib/
	$(TCLKIT) $(AddKitFile) ArduinoThrottle lib/Common Host.tcl $(EXTRA_SCRIPTS)
	$(TCLKIT) $(MakePkgIndex) ArduinoThrottle Common
	$(TCLKIT) $(SDX) wrap ArduinoThrottle -runtime $(TCLKITRUNTIME)
	-rm -rf ArduinoThrottle.vfs

