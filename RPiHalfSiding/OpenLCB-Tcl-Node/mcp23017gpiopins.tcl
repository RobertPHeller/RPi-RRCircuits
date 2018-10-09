#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon Oct 8 20:50:23 2018
#  Last Modified : <181009.1000>
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

namespace eval mcp23017gpiopins {
    snit::enum PinModes -values {in out high low}
    snit::enum PullModes  -values {up down tri}

    snit::integer GPIOPinNo -min 64
    snit::integer MCP23017Addr -min 0 -max 7
    snit::type MCP23017GPIOPin {
        # Instance options:
        # @arg -pinnumber The pin number
        # @arg -pinmode   The pin's mode
        # @arg -pinpullmode The pin's pullup mode
        # @arg -description Description of the pin.
        # @par
        #
        typevariable baseoffset 
        typevariable baseI2Caddress 0x20;# Base I2C address.
        typevariable I2CAddr 7;#  I2C address offset
        typemethod Init {{base 64} {I2CAddr 7}} {
            GPIOPinNo validate $base
            MCP23017Addr validate $I2CAddr
            mcp23017Setup $base [expr {$baseI2Caddress | $I2CAddr}]
        }
        #*** Pin instances
        variable oldPin 0;# The saved value of the pin (input mode only)
        option -pinnumber -readonly yes -type GPIOPinNo -default 64
        option -pinmode -readonly yes -type PinModes -default disabled
        option -pinpullmode -readonly yes -type PullModes -default up   
        option -description -readonly yes -default {}
        constructor {args} {
            # Construct an instance for a GPIO pin
            #
            # @param ... Options:
            # @arg -pinnumber The pin number
            # @arg -pinmode   The pin's mode
            # @arg -pinpullmode The pin's pullup mode
            # @arg -description Description of the pin.
            # @par
        
            $self configurelist $args
            set gpiopinno [$self cget -pinnumber]
            switch [$self cget -pinmode] {
                in {
                    pinMode $gpiopinno $::INPUT
                    switch [$self cget -pinpullmode] {
                        up {
                            pullUpDnControl $gpiopinno $::PUD_UP
                        }
                        down {
                            pullUpDnControl $gpiopinno $::PUD_DOWN
                        }
                        tri {
                            pullUpDnControl $gpiopinno $::PUD_OFF
                        }
                    }
                    pullUpDnControl $gpiopinno $::PUD_UP
                }
                out {
                    pinMode $gpiopinno $::OUTPUT
                } 
                high {
                    pinMode $gpiopinno $::OUTPUT
                    digitalWrite $gpiopinno $::HIGH
                }
                low  {
                    pinMode $gpiopinno $::OUTPUT
                    digitalWrite $gpiopinno $::LOW
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

package provide mcp23017gpiopins 1.0

