#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon Oct 8 23:02:20 2018
#  Last Modified : <181009.1426>
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

package require snit

namespace eval debouncer {
    snit::integer Options -min 0 -max 255
    snit::type QuiesceDebouncer {
        option -waitcount -default 3 -type ::debouncer::Options
        variable count_ 0
        variable currentState_ 0
        constructor {args} {
            $self configurelist $args
        }
        method initialize {state} {
            if {$state} {
                set currentState_ 1
            } else {
                set currentState_ 0
            }
            set count_ 0
        }
        method override {new_state} {
            $self initialize $new_state
        }
        method current_state {} {
            return $currentState_
        }
        method update_state {state} {
            if {$state} {
                set new_state 1
            } else {
                set new_state 0
            }
            if {$new_state == $currentState_} {
                set count_ 0
                return false
            }
            incr count_
            if {$count_ == $waitCount_} {
                set currentState_ $new_state
                set count_ 0
                return true
            }
            return false
        }
            
    }
}

package provide Debouncer 1.0

