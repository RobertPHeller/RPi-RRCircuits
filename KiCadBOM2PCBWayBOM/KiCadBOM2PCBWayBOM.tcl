#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Fri Apr 16 08:44:50 2021
#  Last Modified : <210417.0702>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2021  Robert Heller D/B/A Deepwoods Software
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
package require csvmatrix

snit::type KiCadBOM2PCBWayBOM {
    component matrix
    delegate method * to matrix
    option -sourcefile -readonly yes -default project.csv
    delegate option * to matrix except -sourcechanel
    typevariable CollatedComponentsPattern {^"Collated Components:"}
    constructor {args} {
        puts stderr "*** $type create $self $args"
        set options(-sourcefile) [from args -sourcefile project.csv]
        puts stderr "*** $type create $self: options(-sourcefile) is '$options(-sourcefile)'"
        if {[catch {open $options(-sourcefile) r} fp]} {
            error "Could not open source file: $options(-sourcefile), because $fp"
            return
        }
        while {[gets $fp line] >= 0} {
            set line [string trim $line]
            #puts stderr "*** $type create $self: line is '$line'"
            if {[regexp $CollatedComponentsPattern $line] > 0} {
                gets $fp line
                break
            }
        }
        #puts stderr "*** $type create $self: line is '$line'"
        install matrix using csvmatrix %%AUTO%% -sourcechanel $fp -alternate yes
        close $fp
        $self configurelist $args
        set LibPart_index [$self headingcol "LibPart"]
        set Footprint_index [$self headingcol "Footprint"]
        set MouserPN_index [$self headingcol "Mouser Part Number"]
        set Manufacturer_Name_index [$self headingcol "Manufacturer_Name"]
        set Manufacturer_Part_Number_index [$self headingcol "Manufacturer_Part_Number"]
        for {set i 0} {$i < [$matrix rows]} {incr i} {
            $matrix set cell $LibPart_index $i [_processLibPart [$matrix get cell $LibPart_index $i]]
            $matrix set cell $Footprint_index $i [_processFootprint [$matrix get cell $Footprint_index $i]]
            set mpn [$matrix get cell $Manufacturer_Part_Number_index $i]
            #puts stderr "*** $type create $self: mpn is '$mpn'"
            if {$mpn ne ""} {continue}
            set mouserPN [$matrix get cell $MouserPN_index $i]
            lassign [_processMouserPN $mouserPN] manName manPN
            $matrix set cell $Manufacturer_Name_index $i $manName
            $matrix set cell $Manufacturer_Part_Number_index $i $manPN
        }
        for {set i [expr {[$matrix rows]-1}]} {$i >= 0} {incr i -1} {
            set footprint [$matrix get cell $Footprint_index $i]
            if {[_nonSMDFootprint $footprint]} {
                $matrix delete row $i
            }
        }
        set item 0
        set ItemCol [$self headingcol "Item"]
        for {set i 0} {$i < [$matrix rows]} {incr i} {
            incr item
            $matrix set cell $ItemCol $i $item
        }
            
    }
    proc _processLibPart {libpart} {
        if {[regexp {:([^:]*)$} $libpart => baseparttype] > 0} {
            return $baseparttype
        } else {
            return $libpart
        }
    }
    proc _processFootprint {footprint} {
        if {[regexp {:([^:]*)$} $footprint => basefootprint] > 0} {
            set basefootprint [regsub {^[CRD]_} $basefootprint {}]
            set basefootprint [regsub {^LED_} $basefootprint {}]
            return $basefootprint
        } else {
            return $footprint
        }
    }
    proc _processMouserPN {mouserPN} {
        #puts stderr "*** _processMouserPN $mouserPN"
        if {[regexp {^([[:digit:]]*)-(.*)$} $mouserPN => manufactureNumber manufacturePN] > 0} {
            #puts stderr "*** _processMouserPN: manufactureNumber is $manufactureNumber"
            #puts stderr "*** _processMouserPN: manufacturePN is $manufacturePN"
            set manufactureName [_lookupMan $manufactureNumber]
            if {$manufactureName eq ""} {puts stderr "*** _processMouserPN: $mouserPN"}
            return [list $manufactureName $manufacturePN]
        } else {
            return [list {} {}]
        }
    }
    typevariable _MouserManufaturers -array {
        791 Walsin
        647 Nichicon
        963 {Taiyo Yuden}
        603 Yageo
        710 {Wurth Elektronik}
        187 {Samsung Electro-Mechanics}
        755 {ROHM Semiconductor}
        610 {Central Semiconductor}
        621 {Diodes Incorporated}
        833 {Micro Commercial Components (MCC)}
        757 Toshiba
        652 Bourns
        530 {Bel Fuse}
        651 {Phoenix Contact}
        71 Vishay
        660 {KOA Speer}
        279 {TE Connectivity / Holsworthy}
        667 Panasonic
        926 {Texas Instruments}
        595 {Texas Instruments}
        511  STMicroelectronics
        896 {Mill-Max}
        583 Rectron
        865 {Torex Semiconductor}
        863 {ON Semiconductor}
        942 {Infineon / IR}
        512 {ON Semiconductor / Fairchild}
        579 {Microchip Technology}
    }
    proc _lookupMan {mannum} {
        if {[info exists _MouserManufaturers($mannum)]} {
            return $_MouserManufaturers($mannum)
        } else {
            return {}
        }
    }
    typevariable thoughholeFPs {Fiducial TerminalBlock Pin_Header RJ45_8N 
        25mmFanMount MountingHole  MFR500 8964300490000000}
    proc _nonSMDFootprint {foot} {
        foreach thoughholeFP $thoughholeFPs {
            if {[regexp "^$thoughholeFP" $foot] > 0} {return yes}
        }
        return no
    }
    typevariable pcbwayHeadings {"Item #" "Designator" "Qty" "Manufacturer" 
        "Mfg Part #" "Description" "Value" "Package/Footprint" "Type" 
        "Your Instructions / Notes"}
    typevariable KiCad2pcbwayMap -array {
        Item "Item #"
        Qty Qty
        "Reference(s)" Designator
        Manufacturer_Name Manufacturer
        Manufacturer_Part_Number "Mfg Part #"
        Description Description
        Value Value
        Footprint "Package/Footprint"
    }
        
    method CreatePCBWayBOM {outfile} {
        set pcbwayBOM [csvmatrix %%AUTO%% -headings $pcbwayHeadings]
        for {set i 0} {$i < [$matrix rows]} {incr i} {
            $pcbwayBOM add row
            foreach c1 [array names KiCad2pcbwayMap] {
                $pcbwayBOM set cell [$pcbwayBOM headingcol $KiCad2pcbwayMap($c1)] $i \
                      [$matrix get cell [$matrix headingcol $c1] $i]
                $pcbwayBOM set cell [$pcbwayBOM headingcol Type] $i SMD
            }
        }
        $pcbwayBOM savetocsv $outfile
    }
    
            
}


set BOM [KiCadBOM2PCBWayBOM create %%AUTO%% -sourcefile PocketBeagleCommandStation_SMD.csv]

puts [$BOM headings]
$BOM CreatePCBWayBOM PocketBeagleCommandStation_SMD_PCBWay.csv
