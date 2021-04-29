#!/usr/bin/tclsh
#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Wed Apr 28 10:13:03 2021
#  Last Modified : <210429.0818>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2021  Robert Heller D/B/A Deepwoods Software
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
#*****************************************************************************


package require snit

snit::type AddAllReadmes {
    pragma -hastypeinfo    no
    pragma -hastypedestroy no
    pragma -hasinstances   no
    
    typevariable _ExceptDirs {Adafruit-METRO-328-PCB hats-master 
        KiCad-Schematic-Symbol-Libraries max7219-master RPi_Hat.pretty 
        RPi_Hat_Template CommonOpenMRNExtras ABSWithSiding SMCSenseHAT-EEProm}
    typevariable LinkFormat {1. [%s](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/%s)}
    proc generateOneLink {dirname} {
        puts stdout [format $LinkFormat $dirname $dirname]
    }
    proc blockquoteP1 {readmefile} {
        set fp [open $readmefile r]
        gets $fp line
        puts stdout {}
        while {[gets $fp line] >= 0} {
            if {[string trim $line] ne ""} {break}
        }
        puts stdout "    > $line"
        while {[gets $fp line] >= 0} {
            if {[string trim $line] eq ""} {break}
            puts stdout "    > $line"
        }
        close $fp
    }
    typevariable _HeaderPattern {^## Availble Projects:}
    typevariable _SkipPattern {^1. \[[^\]]}
    typemethod Main {} {
        while {[gets stdin line] >= 0} {
            puts $line
            if {[regexp $_HeaderPattern $line] > 0} {
                break
            }
        }
        while {[gets stdin line] >= 0} {
            if {[string trim $line] eq ""} {continue}
            if {[regexp $_SkipPattern $line] > 0} {continue}
            break
        }
        foreach d [lsort -dictionary [glob -nocomplain "*/README.md"]] {
            set dirname [file tail [file dirname $d]]
            if {[lsearch -exact $_ExceptDirs $dirname] >= 0} {continue}
            puts stdout ""
            generateOneLink $dirname
            blockquoteP1 $d
        }
        puts stdout ""
        while {[gets stdin line] >= 0} {
            puts $line
        }
    }
}

AddAllReadmes Main
            
        
