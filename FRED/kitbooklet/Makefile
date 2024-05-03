##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Fri May 3 11:19:07 2024
#  Last Modified : <240503.1124>
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
#    Copyright (C) 2024  Robert Heller D/B/A Deepwoods Software
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


PDFLATEX=/usr/bin/pdflatex
PDFBOOK2=/usr/bin/pdfbook2

SRCS = kitbooklet.tex ../FRED_Board3D_Bottom.png ../FRED_Board3D_Top.png

kitbooklet-book.pdf: $(SRCS)
	-rm *.aux *.log *.toc kitbooklet.pdf
	$(PDFLATEX) kitbooklet.tex
	$(PDFLATEX) kitbooklet.tex
	$(PDFLATEX) kitbooklet.tex
	$(PDFLATEX) kitbooklet.tex
	$(PDFBOOK2) kitbooklet.pdf
