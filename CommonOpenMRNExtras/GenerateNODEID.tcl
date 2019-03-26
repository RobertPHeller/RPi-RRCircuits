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
#  Created       : Fri Aug 3 10:35:43 2018
#  Last Modified : <180803.1104>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
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


if {[catch {open NODEID.txt r} fpid]} {
    error "NODEID.txt missing!"
}

if {[scan [gets $fpid] "%x %x %x %x %x %x" n1 n2 n3 n4 n5 n6] < 6} {
    error "NODEID.txt format error"
}
close $fpid

incr n6
if {$n6 > 255} {
    set n6 0
    incr n5
    if {$n5 > 255} {
        set n5 0
        incr n4
        if {$n4 > 255} {
            set n4 0
            incr n3
            if {$n3 > 255} {
                set n3 0
                incr n2
                if {$n2 > 255} {
                    set n2 0
                    incr n1
                    if {$n1 > 255} {
                        error "Out of node ids!"
                    }
                }
            }
        }
    }
}


if {[catch {open NODEID.txt w} fpid]} {
    error "Cannot create new NODEID.txt!"
}

puts $fpid [format {%02X %02X %02X %02X %02X %02X} $n1 $n2 $n3 $n4 $n5 $n6]
close $fpid

if {[catch {open NODEID.hxx w} nfp]} {
    error "Cannot create new NODEID.hxx!"
}


puts $nfp [format {extern const openlcb::NodeID NODE_ID = 0x%02X%02X%02X%02X%02X%02XULL; // %02X %02X %02X %02X %02X %02X} \
      $n1 $n2 $n3 $n4 $n5 $n6 $n1 $n2 $n3 $n4 $n5 $n6]
close $nfp
