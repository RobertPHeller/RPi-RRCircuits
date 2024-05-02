##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Wed Mar 15 13:51:40 2023
#  Last Modified : <240502.1453>
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
#    Copyright (C) 2023  Robert Heller D/B/A Deepwoods Software
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


all: docs

SRCS =  ../main.cxx $(wildcard ../*.hxx) \
	$(wildcard ../PNETStack/include/*.hxx) \
	$(wildcard ../PNETStack/*.cxx) \
	$(wildcard ../targets/*/*.hxx) \
	Revision.h KitBooklet.h $(wildcard images/*)
	
Revision.h: ../targets/freertos.armv7m.st-stm32f767zi-nucleo/Revision.hxxout
	echo '/** @page Revision Build Revisions' > $@
	grep '^[[:space:]]*"[^"]*",$$' $< |sed 's/^[[:space:]]*"/ * - /g'|sed 's/",$$//g' >> $@
	echo '*/' >> $@
	
.PHONY:
docs: html latex kitbooklet.pdf UserManual.pdf

html/index.html UserManual.pdf  : $(SRCS) Makefile Doxyfile
	rm -rf html latex
	doxygen Doxyfile
	$(MAKE) -C latex
	mv latex/refman.pdf ./UserManual.pdf

latex2/refman.tex : ../main.cxx KitBooklet.h \
	../PNETStack/include/ConfiguredPCPNetControl.hxx  \
	../PNETStack/include/ConfiguredPCPNetDimmer.hxx \
	../PNETStack/include/ConfiguredPCPNetTrigger.hxx \
	../NodeIdConfigurationGroup.hxx \
	Makefile Doxyfile.kit
	rm -rf latex2
	doxygen Doxyfile.kit

kitbooklet.pdf: latex2/refman.tex
	sed -i~ '/chapter{Class Index}/,/input{classPCPNetTrigger}/d' latex2/refman.tex
	$(MAKE) -C latex2
	pdfbook2 --paper=letter latex2/refman.pdf
	mv latex2/refman-book.pdf kitbooklet.pdf
	
clean::
	rm -rf latex html warnings latex2 kitbooklet.pdf
	
veryclean:: clean
