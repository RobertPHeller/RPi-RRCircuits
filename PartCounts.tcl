#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Thu Sep 8 13:54:06 2022
#  Last Modified : <220908.1405>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
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
#*****************************************************************************


package require snit
package require csv

snit::type PartCounts {
    typeconstructor {
        set total 0
        set SMDtotal 0
        set THTtotal 0
        set unique 0
        set SMDunique 0
        set THTunique 0
        set l1 [string trim [gets stdin]]
        set headings [::csv::split $l1]
        set QIndex [lsearch $headings "Qty"]
        set Tindex [lsearch $headings "Type"]
        while {[gets stdin line] >= 0} {
            set row [::csv::split $line]
            incr unique
            switch [lindex $row $Tindex] {
                SMD {
                    incr SMDunique
                    incr SMDtotal [lindex $row $QIndex]
                    incr total [lindex $row $QIndex]
                }
                thru-hole {
                    incr THTunique
                    incr THTtotal [lindex $row $QIndex]
                    incr total [lindex $row $QIndex]
                }
            }
        }
        puts [format "Total Parts: %4d\nTotal unique Parts: %4d" $total $unique]
        puts [format "Total SMD Parts: %4d\nTotal unique SMD Parts: %4d" \
              $SMDtotal $SMDunique]
        puts [format "Total THT Parts: %4d\nTotal unique THT Parts: %4d" \
              $THTtotal $THTunique]
    }
}
   
        
