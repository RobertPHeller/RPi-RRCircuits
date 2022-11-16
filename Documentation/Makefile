##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Wed May 31 19:27:54 2017
#  Last Modified : <221116.1153>
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
#    Copyright (C) 2017  Robert Heller D/B/A Deepwoods Software
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

TEXS=usermanual.tex ../SMCSenseHAT/SMCSenseHAT.tex \
		    ../QuadSSSQuadIn/QuadSSSQuadIn.tex \
		    ../MCP23017Hat/MCP23017.tex \
		    ../OctalLEDDriver/OctalLEDDriver.tex \
		    ../CANHat2/CANHat2.tex \
		    ../ButtonHat/ButtonHat.tex \
		    ../LEDDriverHat/LEDDriverHat.tex \
		    ../QuadSignalDriverCK/QuadSignalDriverCK.tex \
		    ../QuadSignalDriverCA/QuadSignalDriverCA.tex \
		    ../SignalHAT/SignalHAT.tex \
		    ../DualUncouplerHAT/DualUncouplerHAT.tex \
		    ../MCP23008Hat/MCP23008Hat.tex \
		    ../CANHat/CANHat.tex \
		    ../QuadUncoupler/QuadUncoupler.tex \
		    ../QuadSMC/QuadSMC.tex \
		    ../QuadSMCSenseHat/QuadSMCSenseHat.tex \
		    ../QuadSMCSenseCape/QuadSMCSenseCape.tex \
		    ../PocketBeagleQuadSMCSense/PocketBeagleQuadSMCSense.tex \
		    ../ESP32QuadSMCSense/ESP32QuadSMCSense.tex \
		    ../LCCCANCape/LCCCANCape.tex \
		    ../ESP32-PWMHalfSiding/ESP32-PWMHalfSiding.tex \
		    ../ESP32-PWMHalfSidingSMD/ESP32-PWMHalfSidingSMD.tex \
		    ../ESP32-S3-MultiFunction/ESP32-S3-MultiFunction.tex \
		    ../PocketBeagleCommandStation/PocketBeagleCommandStation.tex

FIGURES= ../SMCSenseHAT/SMCSenseHAT3DTop.png \
	 ../SMCSenseHAT/SMCSenseHAT.png \
	 ../SMCSenseHAT/SMCSenseHAT.pdf \
	 ../QuadSSSQuadIn/QuadSSSQuadIn3DTop.png \
	 ../QuadSSSQuadIn/QuadSSSQuadIn.png \
	 ../QuadSSSQuadIn/QuadSSSQuadIn.pdf \
	 ../MCP23017Hat/MCP230173DTop.png \
	 ../MCP23017Hat/MCP23017.png \
	 ../MCP23017Hat/MCP23017.pdf \
	 ../OctalLEDDriver/OctalLEDDriver3DTop.png \
	 ../OctalLEDDriver/OctalLEDDriver.png \
	 ../OctalLEDDriver/OctalLEDDriver.pdf \
	 ../QuadSMCSenseHat/QuadSMCSenseHat3DTop.png \
	 ../QuadSMCSenseHat/QuadSMCSenseHat.pdf \
	 ../QuadSMCSenseHat/QuadSMCSenseHat.png \
	 ../QuadSMCSenseCape/QuadSMCSenseCape3DTop.png \
	 ../QuadSMCSenseCape/QuadSMCSenseCape-1.pdf \
	 ../QuadSMCSenseCape/QuadSMCSenseCape-2.pdf \
	 ../QuadSMCSenseCape/QuadSMCSenseCape.png \
	 ../LCCCANCape/LCCCANCape-1.pdf \
	 ../LCCCANCape/LCCCANCape.png \
	 ../LCCCANCape/LCCCANCape3DTop.png \
	 ../PocketBeagleQuadSMCSense/PocketBeagleQuadSMCSense3DTop.png \
	 ../PocketBeagleQuadSMCSense/PocketBeagleQuadSMCSense-1.pdf \
	 ../PocketBeagleQuadSMCSense/PocketBeagleQuadSMCSense-2.pdf \
	 ../PocketBeagleQuadSMCSense/PocketBeagleQuadSMCSense-3.pdf \
	 ../PocketBeagleQuadSMCSense/PocketBeagleQuadSMCSense.png \
	 ../ESP32QuadSMCSense/ESP32QuadSMCSense3DTop.png \
	 ../ESP32QuadSMCSense/ESP32QuadSMCSense-1.pdf \
	 ../ESP32QuadSMCSense/ESP32QuadSMCSense-2.pdf \
	 ../ESP32QuadSMCSense/ESP32QuadSMCSense-3.pdf \
	 ../ESP32QuadSMCSense/ESP32QuadSMCSense.png \
	 ../ESP32-PWMHalfSiding/*.png \
	 ../ESP32-PWMHalfSiding/ESP32-PWMHalfSiding-1.pdf \
	 ../ESP32-PWMHalfSiding/ESP32-PWMHalfSiding-2.pdf \
	 ../ESP32-PWMHalfSiding/ESP32-PWMHalfSiding-3.pdf \
	 ../ESP32-PWMHalfSiding/ESP32-PWMHalfSiding-4.pdf \
	 ../ESP32-PWMHalfSiding/ESP32-PWMHalfSiding-5.pdf \
	 ../ESP32-PWMHalfSiding/ESP32-PWMHalfSiding-6.pdf \
	 ../ESP32-PWMHalfSidingSMD/ESP32-PWMHalfSidingSMD-1.pdf \
	 ../ESP32-PWMHalfSidingSMD/ESP32-PWMHalfSidingSMD-2.pdf \
	 ../ESP32-PWMHalfSidingSMD/ESP32-PWMHalfSidingSMD-3.pdf \
	 ../ESP32-PWMHalfSidingSMD/ESP32-PWMHalfSidingSMD-4.pdf \
	 ../ESP32-PWMHalfSidingSMD/ESP32-PWMHalfSidingSMD-5.pdf \
	 ../ESP32-PWMHalfSidingSMD/ESP32-PWMHalfSidingSMD-6.pdf \
	 ../ESP32-PWMHalfSidingSMD/*.png \
	 ../ESP32-S3-MultiFunction/ESP32-S3-MultiFunction-1.pdf \
	 ../ESP32-S3-MultiFunction/ESP32-S3-MultiFunction-2.pdf \
	 ../ESP32-S3-MultiFunction/ESP32-S3-MultiFunction-3.pdf \
	 ../ESP32-S3-MultiFunction/ESP32-S3-MultiFunction-4.pdf \
	 ../ESP32-S3-MultiFunction/ESP32-S3-MultiFunction-5.pdf \
	 ../ESP32-S3-MultiFunction/ESP32-S3-MultiFunction-6.pdf \
	 ../ESP32-S3-MultiFunction/ESP32-S3-MultiFunction-7.pdf \
	 ../ESP32-S3-MultiFunction/ESP32-S3-MultiFunction-8.pdf \
	 ../ESP32-S3-MultiFunction/*.png \
	 ../PocketBeagleCommandStation/PocketBeagleCommandStation-[1234567].pdf \
	 ../PocketBeagleCommandStation/PocketBeagleCommandStation.png
	 

usermanual.pdf : $(TEXS) $(FIGURES)
	for f in $(TEXS) $(FIGURES);  do \
	  if test -f "`basename $$f`"; then continue; else ln -s $$f ./; fi; \
	done
	$(LATEX) usermanual
	-$(BIBTEX) usermanual
	$(LATEX) usermanual
	-$(MAKEINDEX) usermanual.inx
	$(LATEX) usermanual
	$(LATEX) usermanual
	mv usermanual.pdf usermanual-orig.pdf
	$(GS) -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -dPDFSETTINGS=/prepress -dEmbedAllFonts=true -sOutputFile=usermanual.pdf -f usermanual-orig.pdf

	
clean:
	rm -f usermanual.pdf *.aux *.log *.inx *.bbl *.blg *.idx *.lof *.lot \
	*.toc
	
	
