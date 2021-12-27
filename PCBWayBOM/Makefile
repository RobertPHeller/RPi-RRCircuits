##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Sat Apr 3 12:04:06 2021
#  Last Modified : <211220.1447>
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
#    Copyright (C) 2021  Robert Heller D/B/A Deepwoods Software
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


SDX = /usr/local/bin/sdx
TCLKIT = /usr/local/bin/tclkit
TCLSH = /usr/bin/tclsh

SNITLIB = /usr/share/tcltk/tcllib1.19/snit
CSVLIB  = /usr/share/tcltk/tcllib1.19/csv
STRUCTLIB  = /usr/share/tcltk/tcllib1.19/struct
IMGLIB = /usr/lib/tcltk/x86_64-linux-gnu/Img1.4.7

SOURCES = csvmatrix.tcl DynamicHelp.tcl gray50.xbm IconsAndImages.tcl \
	  info.xbm questhead.xbm snitbuttonbox.tcl snitdialog.tcl \
	  snitHTMLHelp.tcl snitmainframe.tcl snitrotext.tcl \
	  snitscrollableframe.tcl snitscrollw.tcl


PCBWayBOM.kit : PCBWayBOM.tcl \
		$(SOURCES)
	-rm PCBWayBOM.kit
	-rm -rf PCBWayBOM.vfs
	$(SDX) qwrap PCBWayBOM.tcl
	$(SDX) unwrap PCBWayBOM.kit
	-rm PCBWayBOM.kit
	ln -s $(SNITLIB) PCBWayBOM.vfs/lib/
	ln -s $(CSVLIB) PCBWayBOM.vfs/lib/
	ln -s $(STRUCTLIB) PCBWayBOM.vfs/lib/
	ln -s $(IMGLIB) PCBWayBOM.vfs/lib/
	mkdir PCBWayBOM.vfs/lib/Support
	cp $(SOURCES)  PCBWayBOM.vfs/lib/Support/
	echo "pkg_mkIndex PCBWayBOM.vfs/lib/Support/ *.tcl"| $(TCLSH) 
	$(SDX) wrap PCBWayBOM.kit
	#-rm -rf PCBWayBOM.vfs

clean:
	-rm PCBWayBOM.kit
	-rm -rf PCBWayBOM.vfs
