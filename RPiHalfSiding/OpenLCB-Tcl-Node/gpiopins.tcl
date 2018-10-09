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
#  Last Modified : <181009.1359>
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
    snit::integer GPIOPinNo -min 0 -max 63
    snit::type GPIOPin {
        typeconstructor {
            wiringPiSetupGpio
        }
        #*** Pin instances
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
            set bcmpinno [$self cget -pinnumber]
            switch [$self cget -pinmode] {
                in {
                    pinMode $bcmpinno $::INPUT
                    switch [$self cget -pinpullmode] {
                        up {
                            pullUpDnControl $bcmpinno $::PUD_UP
                        }
                        down {
                            pullUpDnControl $bcmpinno $::PUD_DOWN
                        }
                        tri {
                            pullUpDnControl $bcmpinno $::PUD_OFF
                        }
                    }
                }
                out {pinMode $bcmpinno $::OUTPUT}
                high {
                    pinMode $bcmpinno $::OUTPUT
                    digitalWrite $bcmpinno $::HIGH
                }
                low {
                    pinMode $bcmpinno $::OUTPUT
                    digitalWrite $bcmpinno $::LOW
                }
                default {
                }
            }
        }
        method read {} {
            return [digitalRead [$self cget -pinnumber]]
        }
        method write {value} {
            digitalWrite [$self cget -pinnumber] $value
        }
        
    }
}




package provide gpiopins 1.0

