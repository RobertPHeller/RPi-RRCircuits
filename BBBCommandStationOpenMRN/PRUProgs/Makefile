##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Thu Oct 24 19:39:06 2019
#  Last Modified : <191119.0125>
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

PRU_CGT=/usr/share/ti/cgt-pru
PROGS=$(GEN_DIR)/MainTrackDCC.out $(GEN_DIR)/ProgTrackDCC.out $(GEN_DIR)/blink.out
INCLUDES=-I. -I/usr/lib/ti/pru-software-support-package/include/ -I/usr/lib/ti/pru-software-support-package/include/am335x/
HEADERS=dccpacket.h
SOURCE=CommonDCCPacketTransmit.c
LINKER_COMMAND_FILE=./AM335x_PRU.cmd
LIBS=--library=/usr/lib/ti/pru-software-support-package/lib/rpmsg_lib.lib
STACK_SIZE=0x100
HEAP_SIZE=0x100
GEN_DIR=gen

CFLAGS=-v3 -O2 --display_error_number --endian=little --hardware_mac=on --obj_directory=$(GEN_DIR) --pp_directory=$(GEN_DIR) -ppd -ppa
#Linker flags (Defined in 'PRU Optimizing C/C++ Compiler User's Guide)
LFLAGS=--reread_libs --warn_sections --stack_size=$(STACK_SIZE) --heap_size=$(HEAP_SIZE)


all: $(PROGS)

$(GEN_DIR)/blink.out: $(GEN_DIR)/blink.object
	$(PRU_CGT)/bin/clpru $(CFLAGS) -z -i$(PRU_CGT)/lib \
		-i$(PRU_CGT)/include $(LFLAGS) \
		-o  $(GEN_DIR)/blink.out \
		$(GEN_DIR)/blink.object \
		-m$(GEN_DIR)/blink.map \
		$(LINKER_COMMAND_FILE) --library=libc.a $(LIBS)


$(GEN_DIR)/MainTrackDCC.out:  $(GEN_DIR)/MainTrackDCC.object
	$(PRU_CGT)/bin/clpru $(CFLAGS) -z -i$(PRU_CGT)/lib \
		-i$(PRU_CGT)/include $(LFLAGS) \
		-o  $(GEN_DIR)/MainTrackDCC.out \
		$(GEN_DIR)/MainTrackDCC.object \
		-m$(GEN_DIR)/MainTrackDCC.map \
		$(LINKER_COMMAND_FILE) --library=libc.a $(LIBS)
		
		

$(GEN_DIR)/ProgTrackDCC.out:  $(GEN_DIR)/ProgTrackDCC.object
	$(PRU_CGT)/bin/clpru $(CFLAGS) -z -i$(PRU_CGT)/lib \
		-i$(PRU_CGT)/include $(LFLAGS) \
		-o  $(GEN_DIR)/ProgTrackDCC.out \
		$(GEN_DIR)/ProgTrackDCC.object \
		-m$(GEN_DIR)/ProgTrackDCC.map \
		$(LINKER_COMMAND_FILE) --library=libc.a $(LIBS)

$(GEN_DIR)/blink.object: blink.c
	@mkdir -p $(GEN_DIR)
	$(PRU_CGT)/bin/clpru --include_path=$(PRU_CGT)/include \
		$(INCLUDES) $(CFLAGS) -fe $@ $<

$(GEN_DIR)/MainTrackDCC.object: $(SOURCE) $(HEADERS) resource_table_0.h
	@mkdir -p $(GEN_DIR)
	$(PRU_CGT)/bin/clpru -DMAINDCC \
		--include_path=$(PRU_CGT)/include $(INCLUDES) \
		$(CFLAGS) -fe $@ $<
		

$(GEN_DIR)/ProgTrackDCC.object: $(SOURCE) $(HEADERS) resource_table_1.h
	@mkdir -p $(GEN_DIR)
	$(PRU_CGT)/bin/clpru -DPROGDCC \
		--include_path=$(PRU_CGT)/include $(INCLUDES) \
		$(CFLAGS) -fe $@ $<
		

 
