#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Tue Oct 9 16:30:48 2018
#  Last Modified : <181012.2024>
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
package require LCC


namespace eval producer {
    snit::type Producer {
        option -eventsendcallback -default {}
        option -eventon -type lcc::EventID_or_null -default {}
        option -eventoff -type lcc::EventID_or_null -default {}
        option -parent -default {} -readonly yes
        constructor {args} {
            $self configurelist $args
            if {[$self cget -parent] eq {}} {
                error [format "Missing required -parent option!"]
            }
        }
        method send_event {value} {
            set callback [$self cget -eventsendcallback]
            if {$callback eq {}} {return}
            if {$value == 0} {
                set event [$self cget -eventoff]
            } else {
                set event [$self cget -eventon]
            }
            if {$event eq {}} {return}
            uplevel #0 $callback send-event-report $event
        }
        method handle_identify_global {} {
            set callback [$self cget -eventsendcallback]
            if {$callback eq {}} {return}
            set v [[$self cget -parent] get_state]
            if {$v == 1} {
                set myvalidevent [$self cget -eventon]
                if {$myvalidevent ne {}} {
                    uplevel #0 $callback producer-valid $myvalidevent
                }
                set myinvalidevent [$self cget -eventoff]
                if {$myinvalidevent ne {}} {
                    uplevel #0 $callback producer-invalid $myinvalidevent
                }
            } else {
                set myvalidevent [$self cget -eventoff]
                if {$myvalidevent ne {}} {
                    uplevel #0 $callback producer-valid $myvalidevent
                }
                set myinvalidevent [$self cget -eventon]
                if {$myinvalidevent ne {}} {
                    uplevel #0 $callback producer-invalid $myinvalidevent
                }
            }
        }
        method handle_identify_producer {event} {
            set callback [$self cget -eventsendcallback]
            if {$callback eq {}} {return}
            set v [[$self cget -parent] get_state]
            if {$v == 1} {
                set myvalidevent [$self cget -eventon]
                set myinvalidevent [$self cget -eventoff]
            } else {
                set myvalidevent [$self cget -eventoff]
                set myinvalidevent [$self cget -eventon]
            }
            if {$myvalidevent ne {}} {
                if {$event eq "*" || [$event match $myvalidevent]} {
                    uplevel #0 $callback producer-valid $myvalidevent
                }
            }
            if {$myinvalidevent ne {}} {
                if {$event eq "*" || [$event match $myinvalidevent]} {
                    uplevel #0 $callback producer-invalid $myinvalidevent
                }
            }
        }
    }
}

            
                
                



package provide Producer 1.0
