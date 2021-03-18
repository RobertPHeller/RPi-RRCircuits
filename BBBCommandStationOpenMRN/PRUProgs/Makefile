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
#  Last Modified : <210318.1138>
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

COMMON:=$(abspath $(lastword $(MAKEFILE_LIST)/..))
PRU_CGT:=/usr/share/ti/cgt-pru
PRU_SUPPORT:=/usr/lib/ti/pru-software-support-package
MODEL:=$(shell cat /proc/device-tree/model | sed 's/ /_/g' | tr -d '\000')
$(warning MODEL=$(MODEL),TARGET=$(TARGET),COMMON=$(COMMON))

# Variables defined:
#   CHIP - target system-on-chip (am57xx, am335x)
#   CHIP_REV - target system-on-chip with rev (am572x_2_0, am335x)
#   PROC - cpu type (arm, pru, c6x, etc.)
#   PRUN - identifier for target PRU core
#   PRU_DIR - control directory for the PRU
#   CC -
#   LD - 
#   LDFLAGS - 
#   CFLAGS -
#
# Depends on udev rules: https://github.com/beagleboard/customizations/blob/master/etc/udev/rules.d/86-remoteproc-noroot.rules
# Check which model
CHIP=am335x
CHIP_REV=am335x
PRU_DIR_OPS=/dev/remoteproc/pruss-core0
PRUN_OPS=0
PROC=pru
EXE=.out
PRU_DIR_PROG=/dev/remoteproc/pruss-core1
PRUN_PROG=1

STACK_SIZE=0x100
HEAP_SIZE=0x100
GEN_DIR=gen
LINKER_COMMAND_FILE=./AM335x_PRU.cmd
PROGS=$(GEN_DIR)/MainTrackDCC.out $(GEN_DIR)/ProgTrackDCC.out $(GEN_DIR)/blink.out
CC=clpru -fe
CFLAGS=--include_path=$(COMMON) \
       --include_path=$(PRU_SUPPORT)/include \
       --include_path=$(PRU_SUPPORT)/include/$(CHIP_REV) \
       --include_path=$(PRU_CGT)/include\
       -v3 -O2 --display_error_number --endian=little \
       --hardware_mac=on --obj_directory=$(GEN_DIR) \
       --pp_directory=$(GEN_DIR) --asm_directory=$(GEN_DIR) \
       -ppd -ppa --asm_listing --c_src_interlist
HEADERS=dccpacket.h
SOURCE=CommonDCCPacketTransmit.c
LD=lnkpru -o
LDFLAGS=--reread_libs --warn_sections --stack_size=$(STACK_SIZE) \
	--heap_size=$(HEAP_SIZE) -i$(PRU_CGT)/lib \
	-i$(PRU_CGT)/include $(LINKER_COMMAND_FILE) \
	--library=libc.a --library=$(PRU_SUPPORT)/lib/rpmsg_lib.lib



all: $(PROGS)

$(GEN_DIR)/blink.out: $(GEN_DIR)/blink.object
	$(LD) $@ $^ $(LDFLAGS) -m $(basename $@).map


$(GEN_DIR)/MainTrackDCC.out:  $(GEN_DIR)/MainTrackDCC.object
	$(LD) $@ $^ $(LDFLAGS) -m $(basename $@).map
		

$(GEN_DIR)/ProgTrackDCC.out:  $(GEN_DIR)/ProgTrackDCC.object
	$(LD) $@ $^ $(LDFLAGS) -m $(basename $@).map

$(GEN_DIR)/blink.object: blink.c
	@mkdir -p $(GEN_DIR)
	$(CC) $@ $^ $(CFLAGS)

$(GEN_DIR)/MainTrackDCC.object: $(SOURCE) $(HEADERS) resource_table_0.h
	@mkdir -p $(GEN_DIR)
	$(CC) $@ $< -DMAINDCC $(CFLAGS) 
	@mv $(GEN_DIR)/$(basename $<).asm $(basename $@).asm
	@mv $(GEN_DIR)/$(basename $<).lst $(basename $@).lst
	@mv $(GEN_DIR)/$(basename $<).pp $(basename $@).pp
		

$(GEN_DIR)/ProgTrackDCC.object: $(SOURCE) $(HEADERS) resource_table_1.h
	@mkdir -p $(GEN_DIR)
	$(CC) $@ $< -DPROGDCC $(CFLAGS)
	@mv $(GEN_DIR)/$(basename $<).asm $(basename $@).asm
	@mv $(GEN_DIR)/$(basename $<).lst $(basename $@).lst
	@mv $(GEN_DIR)/$(basename $<).pp $(basename $@).pp

clean:
	rm -rf $(GEN_DIR)		

 
