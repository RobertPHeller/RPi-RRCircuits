#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Thu Oct 11 21:42:26 2018
#  Last Modified : <181011.2155>
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
package require LCC;#      require the OpenLCB code
package require OpenLCB_Common;# Common code
package require ParseXML;# require the XML parsing code (for the conf file)
package require gettext;#  require the localized message handler
package require log;#      require the logging package.
package require GpioPin
package require HWInit
package require Config

namespace eval configuredproducer {
    snit::type ConfiguredProducer {
        configuration::ConfigOptions _config
        method _config {option value} {
            set options($option) $value
            $self configure \
                  -eventon [lcc::EventID create %AUTO% \
                             -eventidstring [[lindex \
                                              [$value \
                                               getElementsByTagName \
                                               eventon] 0] data]] \
                  -eventoff [lcc::EventID create %AUTO% \
                               -eventidstring [[lindex \
                                                [$value \
                                                 getElementsByTagName \
                                                 eventoff] 0] data]] \
                  -waitcount [[lindex [$value getElementsByTagName debounce] 0] data]
        }
        component pin -inherit yes
        option -pin -readonly yes -default {}\
              -cgetmethod _getPin
        method _getPin {option} {return $pin}
        constructor {args} {
            ::log::log debug "$type create $self $args"
            set pin [from args -pin]
            gpiopin::InputPin validate $pin
            $self configurelist $args
            ::log::log debug "$type create $self: opts are: [$self configure]"
        }
    }
}
            
package provide ConfiguredProducer 1.0
