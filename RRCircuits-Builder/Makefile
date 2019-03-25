##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Sun Mar 24 20:59:52 2019
#  Last Modified : <190324.2133>
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


NATIVEKIT = /usr/local/bin/tclkit
SDX       = /usr/local/bin/sdx

RUNTIME_LinuxArmv7l = /usr/local/share/tclkits/tclkit-linux-armv7l

SNITLIB = /usr/share/tcltk/tcllib1.16/snit/
IMGLIB  = /usr/lib/tcltk/arm-linux-gnueabihf/Img1.4.2/

SCRIPTSSRCS := $(wildcard Scripts/*.tcl)
IMAGES      := $(wildcard Images/*.png) $(wildcard Images/*.gif) \
		$(wildcard Images/*.xpm) $(wildcard Images/*.xbm)
SOURCE = RRCircuits-Builder.tcl
TARGET = RRCircuits-Builder

$(TARGET) : $(SOURCE) $(SCRIPTSSRCS) $(IMAGES)
	-rm -f  $(TARGET).kit
	-rm -rf $(TARGET).vfs
	$(NATIVEKIT) $(SDX) qwrap $(SOURCE)
	$(NATIVEKIT) $(SDX) unwrap $(TARGET).kit
	-rm -f  $(TARGET).kit
	ln -s $(SNITLIB) $(TARGET).vfs/lib/
	ln -s $(IMGLIB)  $(TARGET).vfs/lib/
	mkdir $(TARGET).vfs/lib/Scripts/
	cp $(SCRIPTSSRCS) $(TARGET).vfs/lib/Scripts/
	cp $(IMAGES) $(TARGET).vfs/lib/Scripts/
	echo "pkg_mkIndex $(TARGET).vfs/lib/Scripts/ \*.tcl" | $(NATIVEKIT) 
	$(NATIVEKIT) $(SDX) wrap $(TARGET) -runtime $(RUNTIME_LinuxArmv7l)
	-rm -rf $(TARGET).vfs

