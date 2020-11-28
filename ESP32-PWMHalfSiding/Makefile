##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Thu Nov 26 21:13:49 2020
#  Last Modified : <201128.1512>
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
#    Copyright (C) 2020  Robert Heller D/B/A Deepwoods Software
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


LATEX=/usr/bin/pdflatex
BIBTEX=/usr/bin/bibtex
MAKEINDEX=/usr/bin/makeindex
GS=/usr/bin/gs

TEXS=KitBooklet.tex ESP32-PWMHalfSiding.tex

FIGURES= ESP32-PWMHalfSiding.png \
	 ESP32-PWMHalfSiding3DTop.png \
	 ESP32-PWMHalfSiding-1.pdf \
	 ESP32-PWMHalfSiding-2.pdf \
	 ESP32-PWMHalfSiding-3.pdf \
	 ESP32-PWMHalfSiding-4.pdf \
	 ESP32-PWMHalfSiding-5.pdf \
	 ESP32-PWMHalfSiding-6.pdf \
	 ESP32-PWMHalfSiding.png \
	 ESP32-PWMHalfSidingTermination.png \
	 ESP32-PWMHalfSidingTurnoutMotorsFig.png \
	 ESP32-PWMHalfSidingTotoiseWiring.png \
	 ESP32-PWMHalfSidingOccupancyDetectors.png \
	 ESP32-PWMHalfSidingLampDivers.png \
	 ExampleSiding.png \
	 ESP32HalfSiding-CP1Wiring.png \
	 ABSTrack_Annotated.png ABSTrack_Wiring.png \
	 CP1-ID-Config.png  CP1-OC-Config.png \
	 CP1-Turnout-Config.png CP1-Mast1-Config.png \
	 CP1-OSVeto-Normal1.png CP1-OSVeto-Normal2.png CP1-OSVeto-Normal3.png \
	 CP1ME-Stop-Logic-Config1.png  CP1ME-Stop-Logic-Config3.png \
	 CP1ME-Stop-Logic-Config2.png  \
	 CP1ME-TakeSiding-Logic-Config1.png  \
	 CP1ME-TakeSiding-Logic-Config2.png \
	 CP1ME-Approach-Logic-Config1.png CP1ME-Approach-Logic-Config2.png \
	 CP1ME-Clear-Logic-Config1.png CP1ME-Clear-Logic-Config2.png
	 
	 
	 

KitBooklet.pdf : $(TEXS) $(FIGURES)
	$(LATEX) KitBooklet
	-$(BIBTEX) KitBooklet
	$(LATEX) KitBooklet
	-$(MAKEINDEX) KitBooklet
	$(LATEX) KitBooklet
	$(LATEX) KitBooklet
	mv KitBooklet.pdf KitBooklet-orig.pdf
	$(GS) -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -dPDFSETTINGS=/prepress -dEmbedAllFonts=true -sOutputFile=KitBooklet.pdf -f KitBooklet-orig.pdf
