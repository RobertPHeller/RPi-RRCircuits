#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Fri Aug 26 11:27:43 2022
#  Last Modified : <220826.1220>
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


namespace eval lisp {
    proc read {str {nextvar {}}} {
        #puts stderr "*** lisp::read $str $nextvar"
        set str [skipSpaces $str]
        if {[string length $str] == 0} {return {}}
        set c [string index $str 0]
        if {[string equal $c "("]} then {
            set result [readList [string range $str 1 [string length $str]] next]
        } elseif {[string equal $c ")"]} then {
            error "invalid syntax: $str"
        } else {
            set result [readAtom $str next]
        }
        if {$nextvar ne {}} {
            upvar $nextvar remainder
            set remainder $next
        }
        return $result
    }
    proc readList {str {nextvar {}}} {
        #puts stderr "*** lisp::readList $str $nextvar"
        set ret [list]
        while {1} {
            set str [skipSpaces $str]
            if {[string length $str] == 0} then {
                error unfinished parenthesis"
            }
            set c [string index $str 0]
            if {[string equal $c "("]} then {  # for editor
            } elseif {[string equal $c ")"]} then {
                break
            }
            set elm [read $str next]
            lappend ret $elm
            set str $next
        }
        if {$nextvar ne {}} {
            upvar $nextvar remainder
            set remainder [string range $str 1 end]
        }
        #puts stderr "*** lisp::readList returning $ret"
        return $ret
    }
    variable QUOTE "\""
    proc readAtom {str {nextvar {}}} {
        #puts stderr "*** lisp::readAtom $str $nextvar"
        variable QUOTE
        set next ""
        set i 0
        set quote -1
        while {$i < [string length $str]} {
            if {$quote >= 0} {
                if {[string index $str $i] ne $QUOTE} {
                    incr i
                    continue
                }
                set next [string range $str [expr {$i + 1}] end]
                set str [string range $str $quote [expr {$i - 1}]]
                break
            } elseif {[string index $str $i] eq $QUOTE} {
                set quote [expr {$i + 1}]
                incr i
                continue
            } elseif {[isDelimiter $str $i]} {
                set next [string range $str $i [string length $str]]
                set str [string range $str 0 [expr {$i - 1}]]
                break
            }
            incr i
        }
        set result $str
        if {$nextvar ne {}} {
            upvar $nextvar remainder
            set remainder $next
        }
        return $result
    }
    proc skipSpaces {str} {
        set i 0
        while {$i < [string length $str]} {
            if {![isSpace $str $i]} {
                break
            }
            incr i
        }
        return [string range $str $i [string length $str]]
    }
    proc isSpace {str i} {
        set c [string index $str $i]
        return [expr {[string equal $c "\t"]
                || [string equal $c "\n"]
                || [string equal $c "\r"]
                || [string equal $c " "]}]
    }
    proc isDelimiter {str i} {
        set c [string index $str $i]
        return [expr {[string equal $c "("]
                || [string equal $c ")"]
                || [isSpace $str $i]}]
    }
}

package provide readlisp 1.0
