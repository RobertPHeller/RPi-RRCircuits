##############################################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon Apr 13 08:31:19 2015
#  Last Modified : <211220.1517>
#
#  Description	
#
#  Notes
#
#  History
#	
##############################################################################
#
#    Copyright (C) 2015  Robert Heller D/B/A Deepwoods Software
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


package require snit


package require csv
package require struct::matrix

snit::type csvmatrix {
    component matrix
    typevariable gensym 0
    variable headings
    option -alternate -readonly yes -default no
    option -sourcechanel -readonly yes -default {}
    option -headings -readonly yes -default {}
    delegate method * to matrix
    constructor {args} {
        $self configurelist $args
        if {$options(-sourcechanel) ne {}} {
            set fp $options(-sourcechanel)
            set l1 [string trim [gets $fp]]
            #puts stderr "*** $type create $self: l1 = '$l1'"
            while {[string length $l1] == 0} {
                set l1 [string trim [gets $fp]]
                #puts stderr "*** $type create $self: l1 = '$l1'"
            }
            set headings [::csv::split $l1]
            incr gensym
            set name [format "csvmatrix%04d" $gensym]
            install matrix using ::struct::matrix $name
            $matrix add columns [llength $headings]
            if {[$self cget -alternate]} {
                ::csv::read2matrix -alternate $fp $matrix
            } else {
                ::csv::read2matrix $fp $matrix
            }
            #puts stderr "*** $type create $self: headings = $headings"
        } elseif {[llength $options(-headings)] > 0} {
            set headings $options(-headings)
            incr gensym
            set name [format "csvmatrix%04d" $gensym]
            install matrix using ::struct::matrix $name
            $matrix add columns [llength $headings]
        } else {
            error "One of -sourcechanel or -headings must be specified!"
        }
    }
    method foreveryrow {col cmd} {
        for {set i 0} {$i < [$matrix rows]} {incr i} {
            set cell [$matrix get cell $col $i]
            set cell [uplevel  "$cmd \{$cell\}"]
            $matrix set cell $col $i $cell
        }
    }
    method addcol {heading} {
        $matrix add columns 1
        lappend headings $heading
    }
    method headings {} {return $headings}
    method headingcol {heading} {return [lsearch $headings $heading]}
    method savetochannel {fp} {
        puts $fp [::csv::join $headings]
        ::csv::writematrix $matrix $fp
    }
    method savetocsv {filename} {
        if {[catch {open "$filename" w} fp]} {
            error "Could not open $filename for output: $fp"
        }
        $self savetochannel $fp
        close $fp
    }
}

package provide csvmatrix 1.0
