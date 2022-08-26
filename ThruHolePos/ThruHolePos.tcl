#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Fri Aug 26 09:05:12 2022
#  Last Modified : <220826.1342>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2022  Robert Heller D/B/A Deepwoods Software
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
package require readlisp

snit::type ThruHolePos {
    variable Ref_ ""
    variable Val_ ""
    variable Package_ ""
    variable PosX_ 0.0
    variable PosY_ 0.0
    variable Rot_ 0.0
    variable Side_ ""
    typevariable footprints_ [list]
    constructor {Ref Val Package PosX PosY Rot Side args} {
        set Ref_ $Ref
        set Val_ $Val
        set Package_ $Package
        set PosX_ $PosX
        snit::double validate $PosX_
        set PosY_ [expr {0 - $PosY}]
        snit::double validate $PosY_
        set Rot_ $Rot
        snit::double validate $Rot_
        set Side_ $Side
        #$self configurelist $args
        lappend footprints_ $self
    }
    #C1        22pf               C_0402                               124.4600   -37.1475   90.0000  top
    #                                                                                                   11111111111
    #         11111111112222222222333333333344444444445555555555666666666677777777778888888888999999999900000000001
    #12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
    method write {{fp stdout}} {
        puts $fp [format \
                  {%-10.10s %-17.17s %-34.34s %9.4f %9.4f %9.4f %s} \
                  $Ref_ $Val_ $Package_ $PosX_ $PosY_ $Rot_ $Side_]
    }
    typemethod WriteHeader {{fp stdout}} {
        puts $fp [format \
                  {### Module positions - created on %s ###} \
                  [clock format [clock seconds] \
                   -format {%a %b %d %H:%M:%S %Y}]]
        puts $fp {### Printed by ThruHolePos ###}
        puts $fp {## Unit = mm, Angle = deg.}
        puts $fp {## Side : top}
        puts $fp {# Ref     Val                Package                                  PosX       PosY       Rot  Side}
    }
    typeconstructor {
        global argv
        global argv0
        set sourcefile [from argv -sourcefile "-"]
        set outputfile [from argv -outputfile "-"]
        if {$sourcefile eq "-"} {
            set in stdin
        } else {
            set in [open $sourcefile r]
        }
        set string [read $in]
        if {$sourcefile ne "-"} {
            close $in
        }
        set sourceList [lisp::read $string]
        foreach e $sourceList {
            #puts stderr "ThruHolePos: \[lindex \$e 0\] is [lindex $e 0]"
            if {[lindex $e 0] eq "module"} {
                $type processModule $e
            }
        }
        #puts stderr "*** ThruHolePos: footprints_ is $footprints_"
        if {$outputfile eq "-"} {
            set out stdout
        } else {
            set out [open $outputfile w]
        }
        $type WriteHeader $out
        foreach f $footprints_ {
            $f write $out
        }
        if {$outputfile ne "-"} {
            close $out
        }
    }
    typemethod processModule {module} {
        set p [split [lindex $module 1] :]
        if {[llength $p] > 1} {set p [lindex $p 1]}
        set package $p
        set reference {}
        set posx 0
        set posy 0
        set rotation 0
        set value {}
        set attrs {}
        foreach e $module {
            if {[llength $e] > 1} {
                switch [lindex $e 0] {
                    at {
                        set posx [lindex $e 1]
                        set posy [lindex $e 2]
                        if {[llength $e] < 4} {
                            set rotation 0
                        } else {
                            set rotation [lindex $e 3]
                        }
                    }
                    fp_text {
                        switch [lindex $e 1] {
                            reference {
                                set reference [lindex $e 2]
                            }
                            value {
                                set value [lindex $e 2]
                            }
                            default {}
                        }
                    }
                    attr {
                        set attrs [lrange $e 1 end]
                    }
                    default {}
                }
            }
        }
        if {[llength $attrs] > 0} {return}
        $type create %AUTO% $reference $value $package $posx $posy $rotation top
    }        
}



