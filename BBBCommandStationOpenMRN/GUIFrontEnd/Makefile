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
#  Last Modified : <191027.1034>
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

NODE:=$(shell uname -n)
ifeq ($(NODE),fruitloops)
TCLRUNTIME=/usr/local/share/tclkits/tclkit-linux-armv7l
TCLLIB=/usr/share/tcltk/tcllib1.16
IMGLIB=/usr/lib/tcltk/arm-linux-gnueabihf/Img1.4.2
else
TCLRUNTIME=/usr/local/share/tclkits/tclkit-8.5.8-linux-x86_64
TCLLIB=/usr/share/tcl/tcllib1.16
IMGLIB=/usr/lib64/tcl8.5/Img1.4
endif
target:=$(shell uname -m)-$(shell uname -s | tr 'A-Z' 'a-z')
TCLKIT=/usr/local/bin/tclkit
SDX=/usr/local/bin/sdx
SNITLIB=$(TCLLIB)/snit
MRRSCRIPTLIB=/home/heller/Deepwoods/ModelRRSystem/trunk/Scripts/Common
MRRBUILDSCRIPTS=/home/heller/Deepwoods/ModelRRSystem/trunk/BuildScripts
ADDKITDIR=$(MRRBUILDSCRIPTS)/AddKitDir.kit
ADDKITFILE=$(MRRBUILDSCRIPTS)/AddKitFile.kit
MAKEPKGINDEX=$(MRRBUILDSCRIPTS)/MakePkgIndex.kit
DOXYGEN=/usr/bin/doxygen
EXEEXT=

## No need to edit beyond this point.

VERSION=1.0
PROG = CommandStationGUI
SOURCE = $(PROG).tcl
SCRIPTS = mainwindow.tcl Version.tcl $(MRRSCRIPTLIB)/snitscrollw.tcl \
	  $(MRRSCRIPTLIB)/snitrotext.tcl \
	  $(MRRSCRIPTLIB)/snitmainframe.tcl \
	  $(MRRSCRIPTLIB)/snitStdMenuBar.tcl \
	  $(MRRSCRIPTLIB)/splash.tcl \
	  $(MRRSCRIPTLIB)/gettext.tcl \
	  $(MRRSCRIPTLIB)/DynamicHelp.tcl \
	  $(MRRSCRIPTLIB)/snitbuttonbox.tcl \
	  $(MRRSCRIPTLIB)/snitHTMLHelp.tcl \
	  $(MRRSCRIPTLIB)/snitdialog.tcl \
	  $(MRRSCRIPTLIB)/IconsAndImages.tcl \
	  $(MRRSCRIPTLIB)/snitLFs.tcl
	  
IMAGES = $(wildcard $(MRRSCRIPTLIB)/*.gif) \
	 $(wildcard $(MRRSCRIPTLIB)/*.png) \
	 $(wildcard $(MRRSCRIPTLIB)/*.xbm) \
	 $(wildcard $(MRRSCRIPTLIB)/*.xpm)

all: $(PROG)$(EXEEXT) html/index.html latex/refman.pdf

html/index.html latex/refman.pdf man/man1/$(PROG).1 : Doxyfile \
		$(SOURCE) Version.tcl
	-rm -rf html latex man
	$(DOXYGEN)
	make -C latex refman.pdf
	
$(PROG)$(EXEEXT): $(SOURCE) $(SCRIPTS) $(IMAGES) html/index.html Makefile
	-rm -f $(PROG).kit
	-rm -rf $(PROG).vfs 
	$(TCLKIT) $(SDX) qwrap $(SOURCE)
	$(TCLKIT) $(SDX) unwrap $(PROG).kit
	rm -f $(PROG).kit
	$(TCLKIT) $(ADDKITDIR) $(PROG) lib $(SNITLIB)
	$(TCLKIT) $(ADDKITDIR) $(PROG) lib $(IMGLIB)
	$(TCLKIT) $(ADDKITFILE) $(PROG) lib/Scripts \
			$(SCRIPTS) $(IMAGES)
	$(TCLKIT) $(MAKEPKGINDEX) $(PROG) Scripts
	$(TCLKIT) $(ADDKITFILE) $(PROG) Help html/*
	$(TCLKIT) $(SDX) wrap $(PROG)$(EXEEXT) -runtime $(TCLRUNTIME)
	rm -rf $(PROG).vfs

Version.tcl: FORCE
	echo '## @page Version Version' >Version.tcl
	echo "# Version is $(VERSION) on $(target)." >>Version.tcl
	echo "# Built on `date`, build system: `uname -n -m`" >>Version.tcl
	echo "#" >>Version.tcl
	echo "" >>Version.tcl
	echo "namespace eval Version {" >>Version.tcl
	echo "    variable VERSION {$(VERSION)}" >>Version.tcl
	echo "    variable target  {$(target)}"  >>Version.tcl
	echo "    variable build   {`date +%Y%m%d%H%M`-`uname -n`-`uname -m`}" >>Version.tcl
	echo "}" >>Version.tcl
	echo "package provide Version $(VERSION)" >>Version.tcl
	 
FORCE:
