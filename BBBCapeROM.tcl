#!/usr/bin/tclsh
#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Sun Oct 21 09:00:39 2018
#  Last Modified : <181021.1211>
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

snit::enum CurrentNames -values {vdd_3V3_Used vdd_5V_Used sys_5V_Used 
    DC_Supplied}
snit::enum Direction -values {input output bidir}
snit::enum SlewRates -values {fast slow}
snit::enum PullModes -values {up down}
snit::type BeagleBoneCapeROM {
    pragma -hastypeinfo no -hastypedestroy no -hasinstances no
    typevariable header [binary format H2H2H2H2 AA 55 33 EE]
    typevariable revision [binary format A2 A1]
    typevariable bnameFMT {A32}
    typevariable bname
    typevariable versionFMT {A4}
    typevariable version
    typevariable manufactFMT {A16}
    typevariable manufact
    typevariable partnoFMT {A16}
    typevariable partno
    typevariable numberOfPinsFMT {S1}
    typevariable numberOfPins
    typevariable serialFMT {A2A2A4A4}
    typevariable serial
    typevariable pinUsageFMT {B16}
    typevariable pinUsage -array {}
    typevariable currentUsageFMT {S1}
    typevariable currents -array {}
    typevariable romcontents
    typeconstructor {
        $type setBname ""
        $type setVersion ""
        $type setManufact "Deepwoods Software"
        $type setPartno ""
        $type setNumberOfPins 0
        $type setSerial 0 0 "" 0
        for {set ipin 0} {$ipin < 74} {incr ipin} {
            $type setPinUsage $ipin no
        }
        foreach cur [CurrentNames cget -values] {
            $type setCurrent $cur 0
        }
    }
    typemethod setBname {name} {
        set bname [binary format $bnameFMT $name]
    }
    typemethod setVersion {version_} {
        set version [binary format $versionFMT $version_]
    }
    typemethod setManufact {manufact_} {
        set manufact [binary format $manufactFMT $manufact_]
    }
    typemethod setPartno {partno_} {
        set partno [binary format $partnoFMT $partno_]
    }
    typemethod setNumberOfPins {numberOfPins_} {
        set numberOfPins [binary format $numberOfPinsFMT $numberOfPins_]
    }
    typemethod setSerial {weekno yearno asscode bnum} {
        set serial [binary format $serialFMT [format %02d $weekno] \
                    [format %02d $yearno] $asscode [format %04d $bnum]]
    }
    typemethod setPinUsage {ip used {dir input} {slew fast} 
        {rxenable no} {pullmode down} {pullenabled yes} 
        {muxmode 0}} {
        Direction validate $dir
        SlewRates validate $slew
        PullModes validate $pullmode
        set bits {}
        if {!$used} {
            set bits 0000000000000000
        } else {
            append bits 1
            switch $dir {
                output {
                    append bits 10
                }
                input {
                    append bits 01
                }
                bidir {
                    append bits 11
                }
            }
            append bits 000000
            switch $slew {
                fast {
                    append bits 0
                }
                slow {
                    append bits 1
                }
            }
            if {$rxenable} {
                append bits 1
            } else {
                append bits 0
            }
            switch $pullmode {
                down {append bits 0}
                up   {append bits 1}
            }
            if {$pullenabled} {
                append bits 0
            } else {
                append bits 1
            }
            append bits [expr {($muxmode >> 2) & 1}]
            append bits [expr {($muxmode >> 1) & 1}]
            append bits [expr {($muxmode >> 0) & 1}]
        }
        set pinUsage($ip) [binary format $pinUsageFMT $bits]
    }
    typemethod setCurrent {currentname value} {
        CurrentNames validate $currentname
        set currents($currentname) [binary format $currentUsageFMT $value]
    }
    typemethod generateRom {{otherdata {}}} {
        set result $header
        append result $revision
        append result $bname
        append result $version
        append result $manufact
        append result $partno
        append result $numberOfPins
        append result $serial
        for {set ip 0} {$ip < 74} {incr ip} {
            append result $pinUsage($ip)
        }
        append result $currents(vdd_3V3_Used)
        append result $currents(vdd_5V_Used)
        append result $currents(sys_5V_Used)
        append result $currents(DC_Supplied)
        if {$otherdata ne {}} {
            append result $otherdata
        }
        while {[string length $result] < 32768} {
            append result [binary format c 0]
        }
        return $result
    }
    typemethod ReadSpecStream {fp} {
        $type setBname [gets $fp]
        $type setVersion [gets $fp]
        $type setManufact [gets $fp]
        $type setPartno [gets $fp]
        if {[scan [gets $fp] "%d" confpincount] < 1} {
            error "Bad configurable pin count!"
        }
        if {$confpincount < 0 || $confpincount > 74} {
            error "Configurable pin count out of range: $confpincount"
        }
        if {[scan [gets $fp] "%d" nonconfpincount] < 1} {
            error "Bad nonconfigurable pin count!"
        }
        if {$nonconfpincount < 0 || $nonconfpincount > 18} {
            error "Non-Configurable pin count out of range: $nonconfpincount"
        }
        $type setNumberOfPins [expr {$confpincount + $nonconfpincount}]
        for {set ipin 0} {$ipin < $confpincount} {incr ipin} {
            if {[scan [gets $fp] {%d %s %s %s %s %s %s %d} pinno \
                 used dir slew rxen pullmode pullen muxmode] < 8} {
                error "Format error on pin usage line"
            }
            $type setPinUsage $pinno $used $dir $slew $rxen \
                  $pullmode $pullen $muxmode
        }
        if {[scan [gets $fp] {%d %d %s %d} weekno yearno asscode bnum] < 4} {
            error "Format error on serial number line"
        }
        $type setSerial $weekno $yearno $asscode $bnum
        $type setCurrent vdd_3V3_Used [gets $fp]
        $type setCurrent vdd_5V_Used [gets $fp]
        $type setCurrent sys_5V_Used [gets $fp]
        $type setCurrent DC_Supplied [gets $fp]
        set rest [binary decode hex [read $fp]]
        set rom [$type generateRom $rest]
        return $rom
    }
    typemethod dumphexrom {rom fp} {
        set hexRom [binary encode hex $rom]
        for {set address 0} {$address < [string length $rom]} {incr address 16} {
            puts -nonewline $fp [format "%04x: " $address]
            set first [expr {$address * 2}]
            set last [expr {((16 * 2) + $first)-1}]
            if {$last >= [string length $hexRom]} {set last end}
            set hexline [string range $hexRom $first $last]
            puts $fp $hexline
        }
    }
    typemethod main {prog av} {
        set inputfile [from av -specfile -]
        set outputfile [from av -romfile BBBCapeROM.bin]
        set hexfile [from av -hexfile -]
        if {$inputfile eq {-}} {
            set infp stdin
        } else {
            set infp [open $inputfile r]
        }
        set romfp [open $outputfile w]
        fconfigure $romfp -encoding binary -translation binary
        if {$hexfile eq {-}} {
            set hexfp stdout
        } else {
            set hexfp [open $hexfile w]
        }
        set rom [$type ReadSpecStream $infp]
        if {$infp ne "stdin"} {close $infp}
        puts -nonewline $romfp $rom
        close $romfp
        $type dumphexrom $rom $hexfp
        if {$hexfp ne "stdout"} {close $hexfp}
    }
}


global argc
global argv
global argv0

#puts stderr "*** info exists argc: [info exists argc]"
#puts stderr "*** info exists argv: [info exists argv]"
#puts stderr "*** info exists argv0: [info exists argv0]"
 
BeagleBoneCapeROM main $argv0 $argv

                
