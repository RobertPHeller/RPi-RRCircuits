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
#  Last Modified : <240424.2001>
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

SNITLIB = /usr/share/tcltk/tcllib1.21/snit
CSVLIB  = /usr/share/tcltk/tcllib1.21/csv
STRUCTLIB  = /usr/share/tcltk/tcllib1.21/struct

KiCadBOM2PCBWayBOM.kit : KiCadBOM2PCBWayBOM.tcl \
		csvmatrix.tcl
	-rm KiCadBOM2PCBWayBOM.kit
	-rm -rf KiCadBOM2PCBWayBOM.vfs
	$(SDX) qwrap KiCadBOM2PCBWayBOM.tcl
	$(SDX) unwrap KiCadBOM2PCBWayBOM.kit
	-rm KiCadBOM2PCBWayBOM.kit
	ln -s $(SNITLIB) KiCadBOM2PCBWayBOM.vfs/lib/
	ln -s $(CSVLIB) KiCadBOM2PCBWayBOM.vfs/lib/
	ln -s $(STRUCTLIB) KiCadBOM2PCBWayBOM.vfs/lib/
	mkdir KiCadBOM2PCBWayBOM.vfs/lib/Support
	cp csvmatrix.tcl  KiCadBOM2PCBWayBOM.vfs/lib/Support/
	echo "pkg_mkIndex KiCadBOM2PCBWayBOM.vfs/lib/Support/ *.tcl"| $(TCLSH) 
	$(SDX) wrap KiCadBOM2PCBWayBOM.kit
	#-rm -rf KiCadBOM2PCBWayBOM.vfs

clean:
	-rm KiCadBOM2PCBWayBOM.kit
	-rm -rf KiCadBOM2PCBWayBOM.vfs
