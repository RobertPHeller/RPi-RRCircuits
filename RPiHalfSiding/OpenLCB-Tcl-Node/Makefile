##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon Oct 8 19:47:57 2018
#  Last Modified : <181008.2119>
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

# These variables need to be customized...
SNITLIB = /usr/share/tcltk/tcllib1.16/snit
TCLLOGLIB = /usr/share/tcltk/tcllib1.16/log
URILIB=/usr/share/tcltk/tcllib1.16/uri
IMG_ZIP=/home/heller/Deepwoods/ModelRRSystem/trunk/BuildData/Img-1.4-Linuxarmv7l.zip
TCLXML31=/home/heller/Deepwoods/ModelRRSystem/trunk/BuildData/Tclxml3.1-GenericPureTcl.zip
TCLKITRUNTIME = /usr/local/share/tclkits/tclkit-linux-armv7l
TCLKIT = /usr/local/bin/tclkit
SDX=/home/heller/Deepwoods/ModelRRSystem/trunk/BuildScripts/sdx.kit
AddKitDir=/home/heller/Deepwoods/ModelRRSystem/trunk/BuildScripts/AddKitDir.kit
AddKitFile=/home/heller/Deepwoods/ModelRRSystem/trunk/BuildScripts/AddKitFile.kit
MakePkgIndex=/home/heller/Deepwoods/ModelRRSystem/trunk/BuildScripts/MakePkgIndex.kit
LCCLib=/home/heller/Deepwoods/ModelRRSystem/trunk/Scripts/LCC/
COMMONLib=/home/heller/Deepwoods/ModelRRSystem/trunk/Scripts/Common/
COMMONBLib=/home/heller/Deepwoods/ModelRRSystem/BUILDS/Linuxarmv7l/Scripts/Common/
SOCKETCAN_FILES = /home/heller/Deepwoods/ModelRRSystem/BUILDS/Linuxarmv7l/C++/TclSocketCAN/.libs/libtclsocketcan.so \
		/home/heller/Deepwoods/ModelRRSystem/BUILDS/Linuxarmv7l/C++/TclSocketCAN/pkgIndex.tcl
WiringPiSupportFiles = /home/heller/Deepwoods/ModelRRSystem/BUILDS/Linuxarmv7l/C++/wiringPi/.libs/libtclwiringpi.so \
			/home/heller/Deepwoods/ModelRRSystem/BUILDS/Linuxarmv7l/C++/wiringPi/pkgIndex.tcl
UNZIP = /usr/bin/unzip
### End of configuration vars

LCCLibFiles = $(LCCLib)/ConfigDialogs.tcl \
	      $(LCCLib)/ConfigurationEditor.tcl \
	      $(LCCLib)/eventDialogs.tcl \
	      $(LCCLib)/GenerateEventID.tcl \
	      $(LCCLib)/lcc.tcl \
	      $(LCCLib)/OpenLCB_Common.tcl

COMMONLibFiles = $(COMMONLib)/gettext.tcl \
		 $(COMMONBLib)/Version.tcl \
		 $(COMMONLib)/ParseXML.tcl \
		 $(COMMONLib)/snitStdMenuBar.tcl \
	         $(COMMONLib)/snitHTMLHelp.tcl \
	         $(COMMONLib)/snitdialog.tcl \
	         $(COMMONLib)/snitbuttonbox.tcl \
	         $(COMMONLib)/IconsAndImages.tcl \
		 $(COMMONLib)/gettext.tcl \
		 $(COMMONLib)/snitmainframe.tcl \
		 $(COMMONLib)/mainwindow.tcl \
		 $(COMMONLib)/DynamicHelp.tcl \
		 $(COMMONLib)/snitscrollw.tcl \
		 $(COMMONLib)/snitscrollableframe.tcl \
	         $(COMMONLib)/snitLFs.tcl \
	         $(COMMONLib)/snitrotext.tcl \
	         $(COMMONLib)/snitScrollNotebook.tcl \
		 $(COMMONLib)/questhead.xbm \
		 $(COMMONLib)/unknown.xpm

OpenLCB_HalfsidingScripts = $(LCCLibFiles) $(COMMONLibFiles) \
	gpiopins.tcl mcp23017gpiopins.tcl


all: OpenLCB_Halfsiding

OpenLCB_Halfsiding: OpenLCB_Halfsiding.tcl \
	$(OpenLCB_HalfsidingScripts) $(SOCKETCAN_FILES) \
	$(WiringPiSupportFiles)
	-rm -f OpenLCB_Halfsiding.kit
	-rm -rf OpenLCB_Halfsiding.vfs
	$(TCLKIT) $(SDX) qwrap OpenLCB_Halfsiding.tcl
	$(TCLKIT) $(SDX) unwrap OpenLCB_Halfsiding.kit
	-rm -f OpenLCB_Halfsiding.kit
	$(TCLKIT) $(AddKitDir) OpenLCB_Halfsiding lib $(SNITLIB) 
	$(TCLKIT) $(AddKitDir) OpenLCB_Halfsiding lib $(TCLLOGLIB)
	$(TCLKIT) $(AddKitDir) OpenLCB_Halfsiding lib $(URILIB)
	$(TCLKIT) $(AddKitFile) OpenLCB_Halfsiding \
		lib/Common $(OpenLCB_HalfsidingScripts)
	$(UNZIP) -qq -d OpenLCB_Halfsiding.vfs/lib \
		$(TCLXML31)
	$(UNZIP) -qq -d OpenLCB_Halfsiding.vfs/lib \
		$(IMG_ZIP)
	$(TCLKIT) $(MakePkgIndex) OpenLCB_Halfsiding \
		Common
	$(TCLKIT) $(AddKitFile) OpenLCB_Halfsiding \
		lib/TclSocketCAN $(SOCKETCAN_FILES)
	$(TCLKIT) $(AddKitFile) OpenLCB_Halfsiding \
		lib/WiringPi $(WiringPiSupportFiles)
	$(TCLKIT) $(SDX) wrap OpenLCB_Halfsiding -runtime $(TCLKITRUNTIME)
	-rm -rf OpenLCB_Halfsiding.vfs
	
