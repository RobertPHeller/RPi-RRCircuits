#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon Oct 8 20:50:02 2018
#  Last Modified : <181008.2103>
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

package require Tclwiringpi;#  require the Tclwiringpi package
package require snit;#     require the SNIT OO framework

namespace eval gpiopins {
    snit::enum PinModes -values {disabled in out high low}
    snit::enum PullModes  -values {up down tri}
    snit::type GPIOPinNo {
        pragma  -hastypeinfo false -hastypedestroy false -hasinstances false
        
        typevariable BCMPins -array {
            0 17
            1 18
            2 27
            3 22
            4 23
            5 24
            6 25
            7 4
            21 5
            22 6
            23 13
            24 19
            25 26
            26 12
            27 16
            28 20
            29 21
        }
        typevariable GPIOPins -array {
            17 0
            18 1
            27 2
            22 3
            23 4
            24 5
            25 6
            4 7
            5 21
            6 22
            13 23
            19 24
            26 25
            12 26
            16 27
            20 28
            21 29
        }
        typemethod validate {pinno} {
            if {[info exists BCMPins($pinno)]} {
                return $pinno
            } else {
                error [format "Not a GPIO pin number: %s" $pinno]
            }
        }
        typemethod AllPins {} {
            return [lsort -integer [array names BCMPins]]
        }
        typemethod BCMPinNo {gpiopinno} {
            if {[info exists BCMPins($gpiopinno)]} {
                return $BCMPins($gpiopinno)
            } else {
                error [format "Not a GPIO pin number: %s" $gpiopinno]
            }
        }
        typemethod GPIOPinNo {bcmpinno} {
            if {[info exists GPIOPins($bcmpinno)]} {
                return $GPIOPins($bcmpinno)
            } else {
                error [format "Not a BCM pin number: %s" $bcmpinno]
            }
        }
    }
    snit::type GPIOPin {
        typevariable  GPIOCMD;#           gpio command
        typeconstructor {
            set GPIOCMD [auto_execok "gpio"]
            exec $GPIOCMD unexportall;# flush all existing exported pins
            wiringPiSetupSys
        }
        #*** Pin instances
        variable oldPin 0;# The saved value of the pin (input mode only)
        option -pinnumber -readonly yes -type GPIOPinNo -default 0
        option -pinmode -readonly yes -type PinModes -default disabled
        option -pinpullmode -readonly yes -type PullModes -default up
        option -description -readonly yes -default {}
        constructor {args} {
            # Construct an instance for a GPIO pin
            #
            # @param ... Options:
            # @arg -pinnumber The pin number
            # @arg -pinmode   The pin's mode
            # @arg -pinpullmode The pin's pull mode
            # @arg -description Description of the pin.
            # @par
            
            $self configurelist $args
            set bcmpinno [GPIOPinNo BCMPinNo [$self cget -pinnumber]]
            if {[$self cget -pinmode] eq "disabled"} {
                exec "$GPIOCMD" unexport $bcmpinno
            } else {
                exec "$GPIOCMD" mode   [$self cget -pinnumber] [$self cget -pinpullmode]
                exec "$GPIOCMD" export $bcmpinno [$self cget -pinmode]
            }
        }
        method read {} {
            return [digitalRead [GPIOPinNo BCMPinNo [$self cget -pinnumber]]]
        }
        method write {value} {
            digitalWrite [GPIOPinNo BCMPinNo [$self cget -pinnumber]] $value
        }
        
    }
}




package provide gpiopins 1.0

