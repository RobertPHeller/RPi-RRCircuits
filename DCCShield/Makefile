##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Wed Jan 31 13:20:49 2018
#  Last Modified : <180201.1737>
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


ARDUINO_DIR = /usr/local/arduino-1.0.5/
ARDMK_DIR   = /home/heller/Arduino-Makefile/
BOARD_TAG   = uno
USER_LIB_PATH := /home/heller/
ARDUINO_LIBS = CmdrArduino EEPROM Wire LiquidCrystal_I2C
BISON = /usr/bin/bison

OTHER_OBJS = build-uno/DCCCommands.tab.cpp.o

#DEBUG = yes

include $(ARDMK_DIR)/Arduino.mk

build-uno/DCCCommands.tab.cpp.o : parser/DCCCommands.tab.cpp
	@$(MKDIR) $(dir $@)
	$(CXX) -MMD -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

parser/DCCCommands.tab.cpp : DCCCommands.y
	@$(MKDIR) $(dir $@)
	$(BISON) $< -o $@


clean:: 
	$(REMOVE) parser/DCCCommands.tab.cpp


