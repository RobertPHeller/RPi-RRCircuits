#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Sun Feb 4 14:04:17 2018
#  Last Modified : <180204.1523>
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


snit::enum speedsteps -values {S14 S28 S128}

snit::integer address -min 0 -max 9999
snit::integer muaddress -min 0 -max 99
snit::integer speed128 -min -127 -max 127
snit::integer speed28 -min -28 -max 28
snit::integer speed14 -min -13 -max 13

snit::type MultiUnit {
    variable units [list]
    constructor {args} {
    }
    method addUnit {a} {
        address validate $a
        if {[lsearch -exact $units $a] >= 0} {return}
        lappend units $a
    }
    method member {a} {
        address validate $a
        if {[lsearch -exact $units $a] >= 0} {
            return yes
        } else {
            return no
        }
    }
    method deleteUnit {a} {
        address validate $a
        set idx [lsearch -exact $units $a]
        if {$idx < 0} {return}
        if {$idx == 0} {
            set units [lrange $units 1 end]
        } else {
            set units [lreplace $units $idx $idx]
        }
    }
    method unitCount {} {return [llength $units]}
    method get {i} {
        if {$i < 0 || $i >= [llength $units]} {
            return -1
        }
        return [lindex $units $i]
    }
}

snit::type DoubleHeader {
    variable address1
    variable address2
    constructor {a1 a2 args} {
        address validate $a1
        address validate $a2
        set address1 $a1
        set address2 $a2
    }
    method member {a} {
        address validate $a
        if {$a == $a1 || $a == $a2} {
            return yes
        } else {
            return no
        }
    }
    method get1 {} {return $a1}
    method get2 {} {return $a2}
}

snit::type DCCHostController {
    variable ttyfd
    option -port -default /dev/ttyACM0 -readonly yes
    variable doubleHeaders [list]
    variable multipleUnits -array {}
    variable commandQueue [list]
    variable isready no
    constructor {args} {
        $self configurelist $args
        if {[catch {open $options(-port) r+} ttyfd]} {
            error "Could not open $options(-port): $ttyfd"
            unset ttyfd
        }
        fconfigure $ttyfd -buffering line -translation [list auto cr] \
              -mode 115200,n,8,2 -blocking yes
        fileevent $ttyfd readable [mymethod _response]
        puts $ttyfd {}
    }
    method _response {} {
        if {[gets $ttyfd response] < 0} {
            close $ttyfd
            return
        }
        if {$response eq "Ready"} {
            set isready yes
            if {[llength $commandQueue] > 0} {
                puts $ttyfd [lindex $commandQueue 0]
                set commandQueue [lrange $commandQueue 1 end]
                set isready no
            }
        }
    }
    method _queueCommand {command} {
        lappend commandQueue $command
        if {$isready} {
            puts $ttyfd [lindex $commandQueue 0]
            set commandQueue [lrange $commandQueue 1 end]
            set isready no
        }
    }
    method makeDoubleHeader {a1 a2} {
        address validate $a1
        address validate $a2
        set dh1 [$self _lookupDoubleHeader $a1]
        if {$dh1 ne {}} {
            error "$a1 is already part of a double header"
            return
        }
        set dh2 [$self _lookupDoubleHeader $a2]
        if {$dh2 ne {}} {
            error "$a2 is already part of a double header"
            return
        }
        lappend doubleHeaders [DoubleHeader create %AUTO% $a1 $a2]
    }
    method breakDoubleHeader {a1 a2} {
        address validate $a1
        address validate $a2
        set dh1 [$self _lookupDoubleHeader $a1]
        if {$dh1 eq {}} {
            error "$a1 is not part of a double header"
            return
        }
        set dh2 [$self _lookupDoubleHeader $a2]
        if {$dh2 eq {}} {
            error "$a2 is not part of a double header"
            return
        }
        if {$dh1 ne $dh2} {
            error "$a1 and $a2 are parts of different double headers"
            return
        }
        set idx [lsearch -exact $doubleHeaders $dh1]
        if {$idx == 0} {
            set doubleHeaders [lrange $doubleHeaders 1 end]
        } elseif {$idx > 1} {
            set doubleHeaders [lreplace $doubleHeaders $idx $idx]
        }
        $dh1 destory
    }
    method _lookupDoubleHeader {address} {
        foreach dh $doubleHeaders {
            if {[$dh member $address]} {
                return $dh
            }
        }
        return {}
    }
    method _lookupMultipleUnit {address} {
        address validate $address
        foreach muaddr [array names multipleUnits] {
            if {[$multipleUnits($muaddr) member $address]} {
                return $muaddr
            }
        }
        return -1
    }
    method addtomu {muaddress newaddress} {
        muaddress validate $muaddress
        address validate $newaddress
        if {[$self _lookupMultipleUnit $newaddress] >= 0} {
            error "$newaddress is already part of a MU"
            return
        }
        if {![info exists multipleUnits($muaddress)]} {
            set multipleUnits($muaddress) [MultiUnit create %AUTO%]
        }
        $multipleUnits($muaddress) addUnit $newaddress
    }
    method setSpeed {address speed steps} {
        speedsteps validate $steps
        address validate $address
        set dh [$self _lookupDoubleHeader $address]
        if {$dh ne {}} {
            return [$self _setSpeedDH $dh $speed $steps]
        }
        if {[info exists multipleUnits($address)]} {
            return [$self _setSpeedMU $multipleUnits($address) $speed $steps]
        }
        set muaddr [$self _lookupMultipleUnit $address]
        if {$muaddr >= 0} {
            return [$self _setSpeedMU $multipleUnits($muaddr) $speed $steps]
        }
        $self _setSpeed $address $speed $steps
    }
    method _setSpeed {address speed steps} {
        switch $steps {
            S14 {
                speed14 validate $speed
                return [$self _queueCommand [format {SET SPEED %d %d 14} $address $speed]]
            }
            S28 {
                speed28 validate $speed
                return [$self _queueCommand [format {SET SPEED %d %d 28} $address $speed]]
            }
            S128 {
                speed128 validate $speed
                return [$self _queueCommand [format {SET SPEED %d %d 128} $address $speed]]
            }
        }
    }
    method _setSpeedDH {dh speed steps} {
        $self _setSpeed [$dh get1] $speed $steps
        $self _setSpeed [$dh get2] $speed $steps
    }
    method _setSpeedMU {mu speed steps} {
        set muc [$mu unitCount]
        for {set i 0} {$i < $muc} {incr i} {
            $self _setSpeed [$mu get $i] $speed $steps
        }
    }
}

