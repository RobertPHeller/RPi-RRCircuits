#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon Oct 8 23:36:30 2018
#  Last Modified : <181009.1502>
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
package require gpiopins
package require mcp23017gpiopins
package require Debouncer
package require Poller

namespace eval gpiopin {
    snit::type InputPin {
        option -pinconstructor -default {} -readonly yes
        component pin -inherit yes
        component debouncer -inherit yes
        poller::Poller
        constructor {args} {
            set options(-pinconstructor) [from args -pinconstructor {}]
            if {[$self cget -pinconstructor] eq {}} {
                error [format "Missing required -pinconstructor option"]
            }
            install pin using [$self cget -pinconstructor] %AUTO% \
                  -pinnumber [from args -pinnumber 0] \
                  -pinmode   in \
                  -pinpullmode [from args -pinpullmode up] \
                  -description [from args -description {}]
            install debouncer using debouncer::QuiesceDebouncer %AUTO%
            $self configurelist $args
            $self start_polling
        }
        method set_state {new_value} {
            $debouncer override $new_value
        }
        method get_state {} {return [$debouncer current_state]}
        method poll_33hz {} {
            if {[$debouncer update_state [expr {[$pin read] == 1}]]} {
                #...
            }
        }
    }
    snit::type MotorPin {
        option -pinconstructor -default {} -readonly yes
        component pin -inherit yes
        variable state unknown
        constructor {args} {
            set options(-pinconstructor) [from args -pinconstructor {}]
            if {[$self cget -pinconstructor] eq {}} {
                error [format "Missing required -pinconstructor option"]
            }
            install pin using [$self cget -pinconstructor] %AUTO% \
                  -pinnumber [from args -pinnumber 0] \
                  -pinmode   out \
                  -pinpullmode [from args -pinpullmode up] \
                  -description [from args -description {}]
            $self configurelist $args
        }
        method set_state {value} {
            if {$value} {
                $pin write 1
            } else {
                $pin write 0
            }
            set state $value
        }
        method get_state {} { return $state }
    }
    snit::type LEDPin {
        option -pinconstructor -default {} -readonly yes
        component pin -inherit yes
        variable state 0
        constructor {args} {
            set options(-pinconstructor) [from args -pinconstructor {}]
            if {[$self cget -pinconstructor] eq {}} {
                error [format "Missing required -pinconstructor option"]
            }
            install pin using [$self cget -pinconstructor] %AUTO% \
                  -pinnumber [from args -pinnumber 0] \
                  -pinmode   low \
                  -description [from args -description {}]
            install debouncer using debouncer::QuiesceDebouncer %AUTO%
            $self configurelist $args
            set state 0
        }
        method set_state {value} {
            if {$value} {
                $pin write 1
            } else {
                $pin write 0
            }
            set state $value
        }
        method get_state {} { return $state }
    }
}

            
        
package provide GpioPin 1.0

