#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Fri Oct 12 09:59:44 2018
#  Last Modified : <181012.1136>
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
package require HWInit
package require Config
package require Poller

namespace eval absslaves {
    snit::integer SlaveID -min 0 -max 255
    snit::type ABSSlave {
        typevariable slaves [list]
        typevariable configuration
        typevariable eventsendcallback
        typevariable serialpath
        typevariable serialFp
        poller::TypePoller
        typevariable slaveIndex 64
        typemethod poll_33hz {} {
            for {set islave 0} {
                if {$slaveIndex >= [length $slaves]} {
                    set slaveIndex 0
                }
                set currentslave [lindex $slaves $slaveIndex]
                incr slaveIndex
                if {[$currentslave Process $serialFp]} {
                    break
                }
            }
        }
        typemethod Initialize {args} {
            set configuration [from args -configuration]
            if {[llength $configuration] > 64} {
                 ::log::log error "Too many slaves!"
                 ::exit 99
             }
             set eventsendcallback [from args -eventsendcallback]
             set serialpath [from args -serialpath]
             if {[catch {open $serialpath r+} serialFp]} {
                 ::log::log error "Could not open serial port ($serialpath) because: $serialFp"
                 ::exit 99
             }
             catch {fconfigure $serialFp -mode 115200,n,8,2}
             foreach slaveconf $configuration {
                 $type create %AUTO% \
                      -configuration $slaveconf \
                      -eventsendcallback $eventsendcallback
             }
             $type start_polling
             return $type
         }
         configuration::ConfigOptions _config
         method _config {option value} {
             set options($option) $value
             $self configure \
                   -enabled [[lindex [$value getElementsByTagName \
                                      enabled] 0] data] \
                   -nodeid  [[lindex [$value getElementsByTagName \
                                      nodeid] 0] data] \
                   -occupied [lcc::EventID create %AUTO% \
                              -eventidstring [[lindex \
                                               [$value \
                                                getElementsByTagName \
                                                occupied] 0] data]] \
                  -clear [lcc::EventID create %AUTO% \
                          -eventidstring [[lindex \
                                           [$value \
                                            getElementsByTagName \
                                            clear] 0] data]] \
                  -stopeast [lcc::EventID create %AUTO% \
                             -eventidstring [[lindex \
                                              [$value \
                                               getElementsByTagName \
                                               stopeast] 0] data]] \
                  -approacheast [lcc::EventID create %AUTO% \
                                 -eventidstring [[lindex \
                                                  [$value \
                                                   getElementsByTagName \
                                                   approacheast] 0] data]] \
                  -cleareast [lcc::EventID create %AUTO% \
                              -eventidstring [[lindex \
                                               [$value \
                                                getElementsByTagName \
                                                cleareast] 0] data]] \
                  -stopwest [lcc::EventID create %AUTO% \
                             -eventidstring [[lindex \
                                              [$value \
                                               getElementsByTagName \
                                               stopwest] 0] data]] \
                  -approachwest [lcc::EventID create %AUTO% \
                                 -eventidstring [[lindex \
                                                  [$value \
                                                   getElementsByTagName \
                                                   approachwest] 0] data]] \
                  -clearwest [lcc::EventID create %AUTO% \
                              -eventidstring [[lindex \
                                               [$value \
                                                getElementsByTagName \
                                                clearwest] 0] data]]
        } 
        option -enabled -default false -type snit::boolean
        option -nodeid  -default 255 -type absslaves::SlaveID
        option -occupied -default {} -type lcc::EventID_or_null
        option -clear -default {} -type lcc::EventID_or_null
        option -stopeast -default {} -type lcc::EventID_or_null
        option -approacheast -default {} -type lcc::EventID_or_null
        option -cleareast -default {} -type lcc::EventID_or_null
        option -stopwest -default {} -type lcc::EventID_or_null
        option -approachwest -default {} -type lcc::EventID_or_null
        option -clearwest -default {} -type lcc::EventID_or_null
        option -eventsendcallback -default {}
        variable occ 255
        variable west_aspect unknown
        variable east_aspect unknown
        constructor {args} {
            $self configurelist $args
            ::log::log debug "$self configure => [$self configure]"
        }
        variable timeout
        method _timeout {} {incr timeout -1}
        method _dataarrived {} {incr timeout 1}
        method Process {fp} {
            if {![$self cget -enabled]} {return false}
            puts $fp [format {:g%d;} [$self cget -nodeid]]
            set timeout 0
            fileevent $fp readable [mymethod _dataarrived]
            set id [after 10 [mymethod _timeout]]
            vwait [myvar timeout]
            cancel $id
            fileevent $fp readable {}
            if {$timeout < 0} {
                return false
            }
            set message [gets $fp]
            scan $message {:R%d%cE%cW%c;} id o e w
            if {$id != [$self cget -nodeid]} {return false}
            set callback [$self cget -eventsendcallback]
            if {$o != $occ} {
                set occ $o
                set event {}
                switch [format %c $occ] {
                    O {
                        set event [$self cget -occupied]
                    }
                    C {
                        set event [$self cget -clear]
                    }
                }
                if {$event ne {} && $callback ne {}} {
                    uplevel #0 $callback send-event-report $event
                }
            }
            set event {}
            switch [format %c $e] {
                S {
                    if {east_aspect ne "stop"} {
                        set east_aspect stop
                        set event [$self cget -stopeast]
                    }
                }
                A {
                    if {east_aspect ne "approach"} {
                        set east_aspect approach
                        set event [$self cget -approacheast]
                    }
                }
                C {
                    if {east_aspect ne "clear"} {
                        set east_aspect clear
                        set event [$self cget -cleareast]
                    }
                }
            }
            if {$event ne {} && $callback ne {}} {
                uplevel #0 $callback send-event-report $event
            }
            set event {}
            switch [format %c $w] {
                S {
                    if {west_aspect ne "stop"} {
                        set west_aspect stop
                        set event [$self cget -stopwest]
                    }
                }
                A {
                    if {west_aspect ne "approach"} {
                        set west_aspect approach
                        set event [$self cget -approachwest]
                    }
                }
                C {
                    if {west_aspect ne "clear"} {
                        set west_aspect clear
                        set event [$self cget -clearwest]
                    }
                }
            }
            if {$event ne {} && $callback ne {}} {
                uplevel #0 $callback send-event-report $event
            }
            return true
        }
    }
}




package provide ABSSlaveBus 1.0
