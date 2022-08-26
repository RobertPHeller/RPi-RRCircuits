##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Fri Aug 26 08:41:52 2022
#  Last Modified : <220826.1124>
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
#    Copyright (C) 2022  Robert Heller D/B/A Deepwoods Software
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

ThruHolePos.kit : ThruHolePos.tcl readlisp.tcl
	-rm ThruHolePos.kit
	-rm -rf ThruHolePos.vfs
	$(SDX) qwrap ThruHolePos.tcl
	$(SDX) unwrap ThruHolePos.kit
	-rm ThruHolePos.kit
	ln -s $(SNITLIB) ThruHolePos.vfs/lib/
	mkdir ThruHolePos.vfs/lib/support
	cp readlisp.tcl ThruHolePos.vfs/lib/support/
	echo "pkg_mkIndex ThruHolePos.vfs/lib/support/ *.tcl"| $(TCLSH)
	$(SDX) wrap ThruHolePos.kit
	#-rm -rf ThruHolePos.vfs
	

clean:
	-rm ThruHolePos.kit
	-rm -rf ThruHolePos.vfs
