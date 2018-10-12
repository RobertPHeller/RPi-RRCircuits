#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Fri Oct 12 07:49:15 2018
#  Last Modified : <181012.0923>
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
package require Poller

namespace eval masts {}

snit::macro masts::definePin {name} {
    component ${name}Pin
    option -${name} -readonly yes -default {} -cgetmethod _get${name}Pin
    method _get${name}Pin {option} "return \$${name}Pin"
}

namespace eval masts {
    snit::enum pointSignalAspects -values {unknown stop 
        approachlimited approach clear}
    snit::enum frogSignalAspects -values {unknown stop approach clear}
    snit::type MastPoints {
        poller::Poller
        variable aspect unknown
        configuration::ConfigOptions _config
        method _config {option value} {
            set options($option) $value
            $self configure \
                  -stopevent [lcc::EventID create %AUTO% \
                              -eventidstring [[lindex \
                                               [$value \
                                                getElementsByTagName \
                                                stop] 0] data]] \
                  -approachlimitedevent [lcc::EventID create %AUTO% \
                              -eventidstring [[lindex \
                                               [$value \
                                                getElementsByTagName \
                                                approachlimited] 0] data]] \
                  -approachevent [lcc::EventID create %AUTO% \
                              -eventidstring [[lindex \
                                               [$value \
                                                getElementsByTagName \
                                                approach] 0] data]] \
                  -clearevent [lcc::EventID create %AUTO% \
                              -eventidstring [[lindex \
                                               [$value \
                                                getElementsByTagName \
                                                clear] 0] data]]
        }
        option -stopevent -type lcc::EventID_or_null -default {}
        option -approachlimitedevent -type lcc::EventID_or_null -default {}
        option -approachevent -type lcc::EventID_or_null -default {}
        option -clearevent -type lcc::EventID_or_null -default {}
        option -eventsendcallback -default {}
        masts::definePin os
        masts::definePin points
        option -point_state -readonly yes -type snit::boolean -default false
        masts::definePin nextocc
        masts::definePin highgreen
        masts::definePin highyellow
        masts::definePin highred
        masts::definePin lowyellow
        masts::definePin lowred
        constructor {args} {
            ::log::log debug "$type create $self $args"
            ::log::log debug [list $self info args _getosPin is [$self info args _getosPin]]
            ::log::log debug [list $self info body _getosPin is [$self info body _getosPin]]
            set osPin [from args -os]
            set pointsPin [from args -points]
            set nextoccPin [from args -nextocc]
            set highgreenPin [from args -highgreen]
            set highyellowPin [from args -highyellow]
            set highredPin [from args -highred]
            set lowyellowPin [from args -lowyellow]
            set lowredPin [from args -lowred]
            $self configurelist $args
            ::log::log debug "$type create $self: methods are [$self info methods]"
            ::log::log debug "$type create $self: options are [$self info options]"
            $self start_polling
            ::log::log debug "$type create $self: [$self configure]"
        }
        method eval {} {
            ::log::log debug "*** $self eval"
            set result false
            if {[$osPin get_state] eq "false"} {
                if {$aspect ne "stop"} {set result true}
                set aspect stop
            } elseif {[$pointsPin get_state] eq [$self cget -point_state]} {
                if {$aspect ne "approachlimited"} {set result true}
                set aspect approachlimited
            } elseif {[$nextoccPin get_state] eq "false"} {
                if {$aspect ne "approach"} {set result true}
                set aspect approach
            } else {
                if {$aspect ne "clear"} {set result true}
                set aspect clear
            }
            ::log::log debug "*** $self eval: aspect is $aspect"
            ::log::log debug "*** $self eval: result is $result"
            switch $aspect {
                stop {
                    $highgreenPin set_state off
                    $highyellowPin set_state off
                    $highredPin set_state on
                    $lowyellowPin set_state off
                    $lowredPin set_state on
                }
                approachlimited {
                    $highgreenPin set_state off
                    $highyellowPin set_state off
                    $highredPin set_state on
                    $lowyellowPin set_state on
                    $lowredPin set_state off
                }
                approach {
                    $highgreenPin set_state off
                    $highyellowPin set_state on
                    $highredPin set_state off
                    $lowyellowPin set_state off
                    $lowredPin set_state on
                }
                clear {
                    $highgreenPin set_state on
                    $highyellowPin set_state off
                    $highredPin set_state off
                    $lowyellowPin set_state off
                    $lowredPin set_state on
                }
            }
            return $result
        }
        method poll_33hz {} {
            ::log::log debug "*** $self poll_33hz"
            if {[$self eval]} {
                set callback [$self cget -eventsendcallback]
                if {$callback eq {}} {return}
                switch $aspect {
                    stop {
                        set event [$self cget -stopevent]
                    }
                    approachlimited {
                        set event [$self cget -approachlimitedevent]
                    }
                    approach {
                        set event [$self cget -approachevent]
                    }
                    clear {
                        set event [$self cget -clearevent]
                    }
                }
                if {$event eq {}} {return}
                uplevel #0 $callback send-event-report $event
            }
        }
    }
    snit::type MastFrog {
        poller::Poller
        variable currentaspect unknown
        configuration::ConfigOptions _config
        method _config {option value} {
            set options($option) $value
            $self configure \
                  -stopevent [lcc::EventID create %AUTO% \
                              -eventidstring [[lindex \
                                               [$value \
                                                getElementsByTagName \
                                                stop] 0] data]] \
                  -approachevent [lcc::EventID create %AUTO% \
                              -eventidstring [[lindex \
                                               [$value \
                                                getElementsByTagName \
                                                approach] 0] data]] \
                  -clearevent [lcc::EventID create %AUTO% \
                              -eventidstring [[lindex \
                                               [$value \
                                                getElementsByTagName \
                                                clear] 0] data]]
        }
        option -stopevent -type lcc::EventID_or_null -default {}
        option -approachevent -type lcc::EventID_or_null -default {}
        option -clearevent -type lcc::EventID_or_null -default {}
        option -eventsendcallback -default {}
        masts::definePin os
        masts::definePin points
        option -point_state -readonly yes -type snit::boolean -default false
        masts::definePin nextocc
        masts::definePin green
        masts::definePin yellow
        masts::definePin red
        constructor {args} {
            ::log::log debug "$type create $self $args"
            set osPin [from args -os]
            set pointsPin [from args -points]
            set nextoccPin [from args -nextocc]
            set greenPin [from args -green]
            set yellowPin [from args -yellow]
            set redPin [from args -red]
            $self configurelist $args
            $self start_polling
            ::log::log debug "$type create $self: [$self configure]"
        }
        method eval {} {
            set result false
            if {[$osPin get_state] eq "false" ||
                [$pointsPin get_state] eq [$self cget -point_state]} {
                if {$aspect ne "stop"} {set result true}
                set aspect stop
            } elseif {[$nextoccPin get_state] eq "false"} {
                if {$aspect ne "approach"} {set result true}
                set aspect approach
            } else {
                if {$aspect ne "clear"} {set result true}
                set aspect clear
            }
            ::log::log debug "*** $self eval: aspect is $aspect"
            ::log::log debug "*** $self eval: result is $result"
            switch $aspect {
                stop {
                    $greenPin set_state off
                    $yellowPin set_state off
                    $redPin set_state on
                }
                approach {
                    $greenPin set_state off
                    $yellowPin set_state on
                    $redPin set_state off
                }
                clear {
                    $greenPin set_state on
                    $yellowPin set_state off
                    $redPin set_state off
                }
            }
            return $result
        }
                
        method poll_33hz {} {
            if {[$self eval]} {
                set callback [$self cget -eventsendcallback]
                if {$callback eq {}} {return}
                switch $aspect {
                    stop {
                        set event [$self cget -stopevent]
                    }
                    approach {
                        set event [$self cget -approachevent]
                    }
                    clear {
                        set event [$self cget -clearevent]
                    }
                }
                if {$event eq {}} {return}
                uplevel #0 $callback send-event-report $event
            }
        }
    }
}

package provide Masts 1.0

