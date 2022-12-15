##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Wed Sep 21 11:39:10 2022
#  Last Modified : <220921.1251>
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

OPENMRNPATH=$(HOME)/openmrn
OPENMRNIDFPATH=../../OpenMRNIDF

CDI.xmlout : compile_cdi
	./compile_cdi >/dev/null
	
EXTRAFLAGS= -DSNIP_PROJECT_PAGE=\"https://www.deepsoft.com/\" \
            -DSNIP_HW_VERSION=\"1.0.0\" \
            -DSNIP_SW_VERSION=\"1.0\" \
            -DSNIP_PROJECT_NAME=\"Esp32MultiFunction\" \
            -DCDI_VERSION=\"0x0100\"


compile_cdi: config.hxx CompileCdiMain.cxx
	g++ -o $@ $(EXTRAFLAGS) -I. -I../main -I$(OPENMRNIDFPATH)/src -I$(OPENMRNPATH)/src -I$(OPENMRNPATH)/include $(CDIEXTRA)  --std=c++11 -MMD -MF $@.d $(CXXFLAGSEXTRA) CompileCdiMain.cxx
	
