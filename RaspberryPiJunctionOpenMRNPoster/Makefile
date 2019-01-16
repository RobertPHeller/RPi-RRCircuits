##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Thu Apr 27 10:34:25 2017
#  Last Modified : <170715.1633>
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


FIGURES =  largeHeader.png PiJunction.png SMCSenseHAT-frame.png \
		QuadSSSQuadIn-frame.png MCP23017Hat-frame.png \
		LCC-Network-frame.png Raildriver-DCC-USB-frame.png

MAINSOURCE = source.tex

TARGETS = slides.pdf handout.pdf 

PDFLATEX = /usr/bin/pdflatex

%.pdf : %.tex
	$(PDFLATEX) $<
	latex_count=5 ; \
	  while egrep -s 'Rerun (LaTeX|to get cross-references right)' $*.log && [ $$latex_count -gt 0 ] ;\
	     do \
	       echo "Rerunning latex...." ;\
	       $(PDFLATEX) $< ;\
	       latex_count=`expr $$latex_count - 1` ;\
	     done

all: $(TARGETS)

slides.pdf: cleanslides slides.tex  $(FIGURES)

handout.pdf: cleanhandout handout.tex  $(FIGURES)

cleanslides:
	rm -f slides.ps slides.dvi slides.aux slides.toc slides.idx \
		slides.ind slides.ilg slides.log slides.out slides.pdf \
		slides.snm slides.nav
	
cleanhandout:
	rm -f handout.ps handout.dvi handout.aux handout.toc handout.idx \
		handout.ind handout.ilg handout.log handout.out handout.pdf
	
slides.tex: $(MAINSOURCE)
	<$(MAINSOURCE) sed 's/^\%\\documentclass\[ignorenonframetext\]{beamer}/\\documentclass\[ignorenonframetext]{beamer}/g'| \
	    sed 's/^\\documentclass\[letterpaper\]{article}/%\\documentclass[letterpaper]{article}/g' \
	    > slides.tex

handout.tex: $(MAINSOURCE)
	<$(MAINSOURCE) sed 's/^\\documentclass\[ignorenonframetext\]{beamer}/%\\documentclass\[ignorenonframetext]{beamer}/g'| \
	    sed 's/^\%\\documentclass\[letterpaper\]{article}/\\documentclass[letterpaper]{article}/g' \
	    > handout.tex

clean: cleanslides cleanhandout
	rm -f slides.tex handout.tex
