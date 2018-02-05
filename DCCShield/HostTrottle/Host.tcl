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
#  Last Modified : <180205.1657>
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
snit::enum direction -values {forward reverse}

snit::integer address -min 0 -max 9999
snit::integer muaddress -min 1 -max 99
snit::integer speed128 -min -127 -max 127
snit::integer speed28 -min -28 -max 28
snit::integer speed14 -min -13 -max 13
snit::integer functions1 -min 0 -max 0x1f
snit::integer functions2 -min 0 -max 0x0f

snit::type MultiUnit {
    variable units [list]
    constructor {args} {
    }
    method addUnit {a dir} {
        address validate $a
        direction validate $dir
        if {[lsearch -exact $units $a] >= 0} {return}
        lappend units [list $a $dir]
    }
    method member {a} {
        address validate $a
        if {[lsearch -exact -index 0 $units $a] >= 0} {
            return yes
        } else {
            return no
        }
    }
    method deleteUnit {a} {
        address validate $a
        set idx [lsearch -exact -index 0 $units $a]
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
    method nextUnit {address} {
        address validate $address
        if {$address == 0} {
            return [lindex $units 0 0]
        }
        set i [lsearch -exact -index 0 $units $address]
        incr i
        if {$i == [llength $units]} {
            return 0
        } else {
            return [lindex $units $i 0]
        }
    }
    method previousUnit {address} {
        address validate $address
        if {$address == 0} {
            return [lindex $units end 0]
        }
        set i [lsearch -exact -index 0 $units $address]
        incr i -1
        if {$i < 0} {
            return 0
        } else {
            return [lindex $units $i 0]
        }
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

snit::type ArduinoThrottleHost {
    variable ttyfd
    option -port -default /dev/ttyACM0 -readonly yes
    option -lococharcallback -default {}
    option -functionstatecallback -default {}
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
    method getcvvalue {CV} {}
    method setcvvalue {CV Value} {}
    method setprogrammode {} {}
    method setnormalmode  {} {}
    method emergencystop  {} {
        $self emergencyStop
    }
    method getlocoinfo    {address} {
        address validate $address
        set dh [$self _lookupDoubleHeader $address]
        if {$dh ne {}} {
            $self _queueCommand [format {GET INFORMATION %d} [$dh get1]]
        } elseif {[info exists multipleUnits($address)]} {
            $self _queueCommand [format {GET INFORMATION %d} [$multipleUnits($address) get 0]]
        } elseif {[set muaddr [$self _lookupMultipleUnit $address]] >= 0} {
            $self _queueCommand [format {GET INFORMATION %d} [$multipleUnits($muaddr) get 0]]
        } else {
            $self _queueCommand [format {GET INFORMATION %d} $address]
        }
    }
    method setlocospeed   {address speedsteps direction speed} {
        if {$direction eq [_ "Reverse"]} {
            set speed [expr {$speed * -1}]
        }
        $self setSpeed $address $speed $speedsteps
    }
    method setlocofunctiong1 {address f0 f1 f2 f3 f4} {
        $self setFunctions1 $address [expr {($f0 << 4) | $f1 | ($f2 << 1) | ($f3 << 2) | ($f4 << 3)}]
    }
    method setlocofunctiong2a {address f5 f6 f7 f8} {
        $self setFunctions2 $address [expr {$f5 | ($f6 << 1) | ($f7 << 2) | ($f8 << 3)}]
    }
    method setlocofunctiong2b {address f9 f10 f11 f12} {
        $self setFunctions3 $address [expr {$f9 | ($f10 << 1) | ($f11 << 2) | ($f12 << 3)}]
    }
    
    method emergencyStop {args} {
        if {[llength $args] == 0} {
            $self _queueCommand "EMERGENCY STOP ALL"
            return
        }
        foreach address $args {
            address validate $address
            set dh [$self _lookupDoubleHeader $address]
            if {$dh ne {}} {
                $self _emergencyStopDH $dh
            } elseif {[info exists multipleUnits($address)]} {
                $self _emergencyStopMU $multipleUnits($address)
            } elseif {[set muaddr [$self _lookupMultipleUnit $address]] >= 0} {
                $self _emergencyStopMU $multipleUnits($muaddr)
            } else {
                $self _queueCommand [format "EMERGENCY STOP %d" $address]
            }
        }
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
    method setFunctions1 {address functions} {
        address validate $address
        functions1 validate $functions
        $self _queueCommand [format "SET FUNCTIONS1 %d %2x" $address $functions]
    }
    method setFunctions2 {address functions} {
        address validate $address
        functions2 validate $functions
        $self _queueCommand [format "SET FUNCTIONS2 %d %1x" $address $functions]
    }
    method setFunctions3 {address functions} {
        address validate $address
        functions2 validate $functions
        $self _queueCommand [format "SET FUNCTIONS3 %d %1x" $address $functions]
    }
    method establishDoubleHeader {a1 a2} {
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
    method dissolveDoubleHeader {a1 a2} {
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
    method addToMU {muaddress newaddress {direction forward}} {
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
    method removeFromMU {muaddress address} {
        muaddress validate $muaddress
        address validate $address
        set othermu [$self _lookupMultipleUnit $address]
        if {$othermu >= 0 && $othermu != $muaddress} {
            error "$address is part of another MU"
            return
        }
        if {![info exists multipleUnits($muaddress)]} {
            error "$muaddress does not exist"
            return
        }
        multipleUnits($muaddress) deleteUnit $address
        if {[$multipleUnits($muaddress) unitCount] == 0} {
            $multipleUnits($muaddress) destory
            unset multipleUnits($muaddress)
        }
    }
    method nextUnitMU {muaddress address} {
        muaddress validate $muaddress
        address validate $address
        set othermu [$self _lookupMultipleUnit $address]
        if {$othermu >= 0 && $othermu != $muaddress} {
            error "$address is part of another MU"
            return
        }
        if {![info exists multipleUnits($muaddress)]} {
            error "$muaddress does not exist"
            return
        }
        return $multipleUnits($muaddress) nextUnit $address
    }
    method previousUnitMU {muaddress address} {
        muaddress validate $muaddress
        address validate $address
        set othermu [$self _lookupMultipleUnit $address]
        if {$othermu >= 0 && $othermu != $muaddress} {
            error "$address is part of another MU"
            return
        }
        if {![info exists multipleUnits($muaddress)]} {
            error "$muaddress does not exist"
            return
        }
        return $multipleUnits($muaddress) previousUnit $address
    }
    method nextMU {muaddress} {
        muaddress validate $muaddress
        set mus [lsort -integer [array names multipleUnits]]
        if {$i == 0} {
            return [lindex $mus 0]
        }
        set i [lsearch -exact $mus $muaddress]
        incr i
        if {$i == [length $mus]} {
            return 0
        } else {
            return [lindex $mus $i]
        }
    }
    method previousMU {muaddress} {
        muaddress validate $muaddress
        set mus [lsort -integer [array names multipleUnits]]
        if {$i == 0} {
            return [lindex $mus end]
        }
        set i [lsearch -exact $mus $muaddress]
        incr i -1
        if {$i < 0} {
            return 0
        } else {
            return [lindex $mus $i]
        }
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
            lassign [$mu get $i] address direction
            if {$direction eq "reverse"} {
                set speed [expr {$speed * -1}]
            }
            $self _setSpeed $address $speed $steps
        }
    }
    method _emergencyStopDH {dh} {
        $self _queueCommand [format "EMERGENCY STOP %d" [$dh get1]]
        $self _queueCommand [format "EMERGENCY STOP %d" [$dh get2]]
    }
    method _emergencyStopMU {mu} {
        set muc [$mu unitCount]
        for {set i 0} {$i < $muc} {incr i} {
            $self _queueCommand [format "EMERGENCY STOP %d" [lindex [$mu get $i] 0]]
        }
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
    method _response {} {
        if {[gets $ttyfd response] < 0} {
            close $ttyfd
            return
        }
        puts stderr "*** _response: response is '$response'"
        if {[regexp {^Decoder Info ([[:digit:]]+),([[:digit:]]+),([[:digit:]-]+),([[:xdigit:]]+),([[:xdigit:]]+),([[:xdigit:]]+)$} $response -> address steps speed funct1 funct2 funct3] > 0} {
            if {$options(-lococharcallback) ne {}} {
                set cmd "$options(-lococharcallback) $address "
                if {$speed < 0} {
                    append cmd [_ "Reverse"]
                    set speed [expr {$speed * -1}]
                } else {
                    append cmd [_ "Forward"]
                }
                append cmd " "
                switch $steps {
                    14 {
                        append cmd S14
                    }
                    28 {
                        append cmd S28
                    }
                    128 {
                        append cmd S128
                    }
                    default {
                        append cmd S128
                    }
                }
                append cmd " "
                append cmd $speed
                uplevel #0 $cmd
            }
            if {$options(-functionstatecallback) ne {}} {
                set cmd "$options(-functionstatecallback) $address"
                set funct1 [scan $funct1 %x]
                append cmd " [expr {($funct1 >> 4) & 1}]"
                append cmd " [expr {($funct1 >> 0) & 1}]"
                append cmd " [expr {($funct1 >> 1) & 1}]"
                append cmd " [expr {($funct1 >> 2) & 1}]"
                append cmd " [expr {($funct1 >> 3) & 1}]"
                set funct2 [scan $funct2 %x]
                append cmd " [expr {($funct2 >> 0) & 1}]"
                append cmd " [expr {($funct2 >> 1) & 1}]"
                append cmd " [expr {($funct2 >> 2) & 1}]"
                append cmd " [expr {($funct2 >> 3) & 1}]"
                set funct3 [scan $funct3 %x]
                append cmd " [expr {($funct3 >> 0) & 1}]"
                append cmd " [expr {($funct3 >> 1) & 1}]"
                append cmd " [expr {($funct3 >> 2) & 1}]"
                append cmd " [expr {($funct3 >> 3) & 1}]"
                uplevel #0 $cmd
            }
        }
        if {$response eq "Ready"} {
            set isready yes
            if {[llength $commandQueue] > 0} {
                puts stderr "*** _response: commandQueue is $commandQueue"
                puts $ttyfd [lindex $commandQueue 0]
                set commandQueue [lrange $commandQueue 1 end]
                set isready no
            }
        }
        puts stderr "*** _response: (exit) isready is $isready"
    }
    method _queueCommand {command} {
        lappend commandQueue $command
        puts stderr "*** _queueCommand (entry) isready is $isready"
        if {$isready} {
            puts stderr "*** _queueCommand: commandQueue is $commandQueue"
            puts $ttyfd [lindex $commandQueue 0]
            set commandQueue [lrange $commandQueue 1 end]
            set isready no
        }
        puts stderr "*** _queueCommand (exit) isready is $isready"
    }
}

package provide ArduinoThrottleHost 1.0
