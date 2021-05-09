#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Thu Oct 31 10:11:53 2019
#  Last Modified : <210509.0745>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
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

##
# @page servicemode Service Mode Screen
# The Service Mode Screen provides access to the service mode 
# functions (reading and writing CVs) using the programming track.
# @section cvs CV display
# The bulk of the Service Mode Screen is the display of CVs.  The
# CVs are broken up into groups under tabs.  The first group tab is 
# the Required CVs.  These are the CVs that are mandatory and required
# by all decoders.  The second through fifth tabs have the common, but
# optional CVs, broken up into 4 group.  Then there are two groups of
# less common CVs.  Finally there is a tab for custom CVs.  This last
# tab holds any additional CVs that are not defined elsewhere.
#
# The tabs contain these CVs:
#
# - Required CVs
# @par
# @image latex ServiceModeRequiredCVs.png "ServiceMode, Required CVs" width=5in
# @image html ServiceModeRequiredCVsSmall.png
#   - #1 Primary Address (short address)
#   - #2 Start Voltage
#   - #3 Acceleration Rate
#   - #4 Deceleration Rate
#   - #7 Manufacturer Version Number (readonly)
#   - #8 Manufacturer ID (readonly)
#   - #11 Packet time-out Value
#   - #29 Configuration Data (bit field)
# - Common CVs, Group 1
# @par
# @image latex ServiceModeCommonCVsGroup1.png "ServiceMode, Common CVs, Group 1" width=5in
# @image html ServiceModeCommonCVsGroup1Small.png
#   - #5 High Voltage
#   - #6 Mid Voltage
#   - #9 Total PWM Period
#   - #10 EMF Feedback Cutout
#   - #12 Power Source Conversion
#   - #13 Alternate Mode Function Status
#   - #14 Alternate Mode Function 2 Status
#   - #15 Decoder Lock A
#   - #16 Decoder Lock B
#   - #17 & #18 Extended Address (long address) (2 bytes)
# - Common CVs, Group 2
# @par
# @image latex ServiceModeCommonCVsGroup2.png "ServiceMode, Common CVs, Group 2" width=5in
# @image html ServiceModeCommonCVsGroup2Small.png
#   - #19 Consist Address
#   - #21 Consist Address Active for F1-F8
#   - #22 Consist Address Active for FL and F9-F12
#   - #23 Acceleration Adjustment
#   - #24 Deceleration Adjustment
#   - #25 Speed Table/Mid Range Cab Speed Step
#   - #27 Decoder Automatic Stopping Configuration (bit field)
#   - #28 Bi-Directional Communication Configuration
# - Common CVs, Group 3
# @par
# @image latex ServiceModeCommonCVsGroup3.png "ServiceMode, Common CVs, Group 3" width=5in
# @image html ServiceModeCommonCVsGroup3Small.png
#   - #30 ERROR Information
#   - #33 Forward Headlight FL(f) (bit field)
#   - #34 Reverse Headlight FL(r) (bit field)
#   - #35 Function 1 (bit field)
#   - #36 Function 2 (bit field)
#   - #37 Function 3 (bit field)
#   - #38 Function 4 (bit field)
#   - #39 Function 5 (bit field)
# - Common CVs, Group 4
# @par
# @image latex ServiceModeCommonCVsGroup4.png "ServiceMode, Common CVs, Group 4" width=5in
# @image html ServiceModeCommonCVsGroup4Small.png
#   - #40 Function 6 (bit field)
#   - #41 Function 7 (bit field)
#   - #42 Function 8 (bit field)
#   - #43 Function 9 (bit field)
#   - #44 Function 10 (bit field)
#   - #45 Function 11 (bit field)
#   - #46 Function 12 (bit field)
# - AdditionalCVs, Group 1
# @par
# @image latex ServiceModeAdditionalCVsGroup1.png "ServiceMode, Additional CVs, Group 1" width=5in
# @image html ServiceModeAdditionalCVsGroup1Small.png
#   - #65 Kick Start
#   - #66 Forward Trim
#   - #67 through #94 SpeedTable (28 bytes)
#   - #95 Reverse Trim
#   - #105 User Identifier #1
#   - #106 User Identifier #2
# - AdditionalCVs, Group 2
# @par
# @image latex ServiceModeAdditionalCVsGroup2.png "ServiceMode, Additional CVs, Group 2" width=5in
# @image html ServiceModeAdditionalCVsGroup2Small.png
#   - #892 Decoder Load
#   - #893 Dynamic Flags
#   - #894 Fuel/Coal
#   - #895 Water
#   - Index Page CVs (#31 and #32 - the page index, plus #257 through 
#     #512 - the page bytes)
# - Custom CVs
# This tab holds any custom defined CV intefaces defined. See 
# @ref definenewcv for help on creating custom CV intefaces.
# @section buttons Buttons
# There are four buttons along the bottom of the Service Mode Screen.
#
# -# Close 
#    Closes the Service Mode Screen.
# -# Load All 
#    Reads all of the CVs.
# -# Update All 
#    Writes all of the CVs.
# -# Add Custom CV 
#    Defines a CV not handled elsewhere. See @ref definenewcv.
# @section definenewcv Defining new CVs
# Custom CV interfaces can be created using the Add Custom CV Dialog 
# box, shown here:
# @par
# @image latex AddCustomCVDialog.png "Add Custom CV Dialog"
# @image html  AddCustomCVDialog.png
# @par
# A custom CV has a name, a type (Byte, Word, or Bit Field), a byte 
# number, and for bit fields, each bit has a name (label).  The 
# Custom CV interfaces are added to the Custom CV tab.

package require Tk
package require tile
package require snit
package require LabelFrames
package require ScrollableFrame
package require ScrollWindow
package require ScrollTabNotebook
package require Dialog

snit::integer CVAddress -min 1 -max 1024

snit::integer uint8_t -min 0 -max 255

snit::widget CVByte {
    option -bytenumber -default 1 -type CVAddress -readonly yes
    option -label      -default "" -readonly yes
    option -callback {}
    hulltype ttk::labelframe
    component spinbox 
    variable _spinvalue 0
    delegate method * to spinbox except {cget configure xview}
    constructor {args} {
        $self configurelist $args
        $hull configure -labelanchor nw
        if {$options(-label) ne ""} {
            $hull configure -text $options(-label)
        } else {
            $hull configure -text [_m "Label|CV number %d" $options(-bytenumber)]
        }
        install spinbox using spinbox $win.spinbox -from 0 -to 255 \
              -textvariable [myvar _spinvalue]
        grid $spinbox -row 0 -column 0 -columnspan 2 -sticky news
        grid [ttk::button $win.update -text [_m "Label|Update"] \
              -command [mymethod update]] -row 1 -column 0 \
              -sticky ns
        grid [ttk::button $win.load -text [_m "Label|Load"] \
              -command [mymethod load]] -row 1 -column 1 \
              -sticky ns
        grid columnconfigure $win 0 -weight 1 
        grid columnconfigure $win 1 -weight 1 
    }
    method set {value} {
        uint8_t validate $value
        set _spinvalue $value
    }
    method update {} {
        if {$options(-callback) ne {}} {
            uplevel #0 "$options(-callback) update 1 $win"
        }
    }
    method load {} {
        if {$options(-callback) ne {}} {
            uplevel #0 "$options(-callback) load 1 $win"
        }
    }
}

snit::integer uint16_t -min 0 -max 65535

snit::widget CVWord {
    option -bytenumber -default 1 -type CVAddress -readonly yes
    option -label      -default "" -readonly yes
    option -callback {}
    hulltype ttk::labelframe
    component spinbox
    variable _spinvalue 0
    delegate method * to spinbox except {cget configure xview}
    constructor {args} {
        $self configurelist $args
        $hull configure -labelanchor nw
        if {$options(-label) ne ""} {
            $hull configure -text $options(-label)
        } else {
            $hull configure -text [_m "Label|CV number %d" $options(-bytenumber)]
        }
        install spinbox using spinbox $win.spinbox -from 0 -to 65535 \
              -textvariable [myvar _spinvalue]
        grid $spinbox -row 0 -column 0 -columnspan 2 -sticky news
        grid [ttk::button $win.update -text [_m "Label|Update"] \
              -command [mymethod update]] -row 1 -column 0 \
              -sticky ns
        grid [ttk::button $win.load -text [_m "Label|Load"] \
              -command [mymethod load]] -row 1 -column 1 \
              -sticky ns
        grid columnconfigure $win 0 -weight 1 
        grid columnconfigure $win 1 -weight 1 
    }
    method set {value} {
        uint16_t validate $value
        set _spinvalue $value
    }
    method update {} {
        if {$options(-callback) ne {}} {
            uplevel #0 "$options(-callback) update 2 $win"
        }
    }
    method load {} {
        if {$options(-callback) ne {}} {
            uplevel #0 "$options(-callback) load 2 $win"
        }
    }
}


snit::widget CVReadonly {
    option -bytenumber -default 1 -type CVAddress -readonly yes
    option -label      -default "" -readonly yes
    option -callback {}
    hulltype ttk::labelframe
    component entry
    variable _entryvalue 0
    delegate method * to entry except {cget configure xview}
    constructor {args} {
        $self configurelist $args
        $hull configure -labelanchor nw
        if {$options(-label) ne ""} {
            $hull configure -text $options(-label)
        } else {
            $hull configure -text [_m "Label|CV number %d" $options(-bytenumber)]
        }
        install entry using ttk::entry $win.entry -state readonly \
              -textvariable _entryvalue
        grid $entry -row 0 -column 0 -columnspan 2 -sticky news
        grid [ttk::button $win.load -text [_m "Label|Load"] \
              -command [mymethod load]] -row 1 -column 1 \
              -sticky ns
        grid columnconfigure $win 0 -weight 1 
        grid columnconfigure $win 1 -weight 1 
    }
    method set {value} {
        uint8_t validate $value
        set _entryvalue $value
    }
    method load {} {
        if {$options(-callback) ne {}} {
            uplevel #0 "$options(-callback) load 1 $win"
        }
    }
    method update {} {}
}

snit::listtype BitFields -minlen 8 -maxlen 8
snit::integer BitNumber -min 0 -max 7
snit::integer FieldColumns -min 2 -max 8

snit::widget CVBitField {
    option -bytenumber -default 1 -type CVAddress -readonly yes
    option -label      -default "" -readonly yes
    option -callback {}
    option -fieldlables -type BitFields -readonly yes -default \
          {Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7}
    option -columns -default 2 -type FieldColumns -readonly yes
    variable bits_ -array {
        0 0
        1 0
        2 0
        3 0
        4 0
        5 0
        6 0
        7 0
    }
    method getbit {ibit} {
        BitNumber validate $ibit
        return $bits_($ibit)
    }
    method setbit {ibit value} {
        BitNumber validate $ibit
        snit::boolean validate $value
        if {$value} {
            set bits_($ibit) 1
        } else {
            set bits_($ibit) 0
        }
    }
    method set {byte} {
        uint8_t validate $byte
        for {set ibit 0} {$ibit < 8} {incr ibit} {
            $self setbit [expr {($byte >> $ibit) & 0x01}]
        }
    }
    method get {} {
        set result 0
        for {set ibit 0} {$ibit < 8} {incr ibit} {
            set result [expr {$result | ([$self getbit $ibit] << $ibit)}]
        }
        return $result
    }
    hulltype ttk::labelframe
    constructor {args} {
        $self configurelist $args
        $hull configure -labelanchor nw
        if {$options(-label) ne ""} {
            $hull configure -text $options(-label)
        } else {
            $hull configure -text [_m "Label|CV number %d" $options(-bytenumber)]
        }
        set frame $win
        set ibit 0
        set grow 0
        set gcol 0
        foreach bitlabel $options(-fieldlables) {
            grid [ttk::checkbutton $frame.bit$ibit -offvalue 0 \
                  -onvalue 1 -variable [myvar bits_($ibit)] \
                  -text $bitlabel] -row $grow \
                  -column $gcol -sticky news
            incr ibit
            incr gcol
            if {$gcol == $options(-columns)} {
                incr grow
                set gcol 0
            }
        }
        for {set icol 0} {$icol < $options(-columns)} {incr icol} {
            grid columnconfigure $frame $icol -weight 1
        }
        grid [ttk::button $frame.update -text [_m "Label|Update"] \
              -command [mymethod update]] -row $grow -column 0 \
              -columnspan [expr {$options(-columns) / 2}] \
              -sticky ns
        grid [ttk::button $frame.load -text [_m "Label|Load"] \
              -command [mymethod load]] -row $grow \
              -column [expr {$options(-columns) / 2}] \
              -columnspan [expr {$options(-columns) / 2}] \
              -sticky ns
    }
    method update {} {
        if {$options(-callback) ne {}} {
            uplevel #0 "$options(-callback) update 1 $win"
        }
    }
    method load {} {
        if {$options(-callback) ne {}} {
            uplevel #0 "$options(-callback) load 1 $win"
        }
    }
}

snit::widget SpeedTable {
    hulltype ttk::labelframe
    option -callback -configuremethod _propagateoption
    method _propagateoption {o v} {
        set options($o) $v
        foreach e [array names elements] {
            $elements($e) configure $o $v
        }
    }
    variable elements -array {}
    constructor {args} {
        $self configurelist $args
        $hull configure -labelanchor nw
        $hull configure -text [_m "Label|Speed table"]
        set grow 0
        set gcol 0
        for {set i 67} {$i <= 94} {incr i} {
            set element [expr {$i - 66}]
            set elements($element) [CVByte $win.element$element \
                                    -bytenumber $i \
                                    -label [_m "Label|Element %d" $element] \
                                    -callback $options(-callback)]
            grid $elements($element) -row $grow -column $gcol -sticky news
            incr gcol
            if {$gcol > 6} {
                incr grow
                set gcol 0
            }
        }
        for {set c 0} {$c < 7} {incr c} {
            grid columnconfigure $win $c -weight 1
        }
    }
    method update {} {
        foreach e [array names elements] {
            $elements($e) update
        }
    }
    method load {} {
        foreach e [array names elements] {
            $elements($e) load
        }
    }
}
        

snit::widget ServiceMode {
    hulltype tk::toplevel
    option -commandstationsocket
    typevariable CV_Labels -array {}
    typevariable CV_WidgetConstructors -array {}
    typevariable CV_Tab -array {}
    typeconstructor {
        set CV_Labels(1) [_m "Label|Primary Address"]
        set CV_WidgetConstructors(1) \
              [list CVByte -bytenumber 1 -label $CV_Labels(1)]
        set CV_Tab(1) requiredCVs
        set CV_Labels(2) [_m "Label|Start Voltage"]
        set CV_WidgetConstructors(2) \
              [list CVByte -bytenumber 2 -label $CV_Labels(2)]
        set CV_Tab(2) requiredCVs
        set CV_Labels(3) [_m "Label|Acceleration Rate"]
        set CV_WidgetConstructors(3) \
              [list CVByte -bytenumber 3 -label $CV_Labels(3)]
        set CV_Tab(3) requiredCVs
        set CV_Labels(4) [_m "Label|Deceleration Rate"]
        set CV_WidgetConstructors(4) \
              [list CVByte -bytenumber 4 -label $CV_Labels(4)]
        set CV_Tab(4) requiredCVs
        set CV_Labels(5) [_m "Label|High Voltage"]
        set CV_WidgetConstructors(5) \
              [list CVByte -bytenumber 5 -label $CV_Labels(5)]
        set CV_Tab(5) optCommonCVsGroup1
        set CV_Labels(6) [_m "Label|Mid Voltage"]
        set CV_WidgetConstructors(6) \
              [list CVByte -bytenumber 6 -label $CV_Labels(6)]
        set CV_Tab(6) optCommonCVsGroup1
        set CV_Labels(7) [_m "Label|Manufacturer Version Number"]
        set CV_WidgetConstructors(7) \
              [list CVReadonly -bytenumber 7 -label $CV_Labels(7)]
        set CV_Tab(7) requiredCVs
        set CV_Labels(8) [_m "Label|Manufacturer ID"]
        set CV_WidgetConstructors(8) \
              [list CVReadonly -bytenumber 8 -label $CV_Labels(8)]
        set CV_Tab(8) requiredCVs
        set CV_Labels(9) [_m "Label|Total PWM Period"]
        set CV_WidgetConstructors(9) \
              [list CVByte -bytenumber 9 -label $CV_Labels(9)]
        set CV_Tab(9) optCommonCVsGroup1
        set CV_Labels(10) [_m "Label|EMF Feedback Cutout"]
        set CV_WidgetConstructors(10) \
              [list CVByte -bytenumber 10 -label $CV_Labels(10)]
        set CV_Tab(10) optCommonCVsGroup1
        set CV_Labels(11) [_m "Label|Packet time-out Value"]
        set CV_WidgetConstructors(11) \
              [list CVByte -bytenumber 11 -label $CV_Labels(11)]
        set CV_Tab(11) requiredCVs
        set CV_Labels(12) [_m "Label|Power Source Conversion"]
        set CV_WidgetConstructors(12) \
              [list CVByte -bytenumber 12 -label $CV_Labels(12)]
        set CV_Tab(12) optCommonCVsGroup1
        set CV_Labels(13) [_m "Label|Alternate Mode Function Status"]
        set CV_WidgetConstructors(13) \
              [list CVByte -bytenumber 13 -label $CV_Labels(13)]
        set CV_Tab(13) optCommonCVsGroup1
        set CV_Labels(14) [_m "Label|Alternate Mode Function 2 Status"]
        set CV_WidgetConstructors(14) \
              [list CVByte -bytenumber 14 -label $CV_Labels(14)]
        set CV_Tab(14) optCommonCVsGroup1
        set CV_Labels(15) [_m "Label|Decoder Lock A"]
        set CV_WidgetConstructors(15) \
              [list CVByte -bytenumber 15 -label $CV_Labels(15)]
        set CV_Tab(15) optCommonCVsGroup1
        set CV_Labels(16) [_m "Label|Decoder Lock B"]
        set CV_WidgetConstructors(16) \
              [list CVByte -bytenumber 16 -label $CV_Labels(16)]
        set CV_Tab(16) optCommonCVsGroup1
        set CV_Labels(17) [_m "Label|Extended Address High"]
        set CV_Labels(18) [_m "Label|Extended Address Low"]
        set CV_WidgetConstructors(17) \
              [list CVWord -bytenumber 17 -label [_m "Label|Extended Address"]]
        set CV_Tab(17) optCommonCVsGroup1
        set CV_Labels(19) [_m "Label|Consist Address"]
        set CV_WidgetConstructors(19) \
              [list CVByte -bytenumber 19 -label $CV_Labels(19)]
        set CV_Tab(19) optCommonCVsGroup2
        set CV_Labels(21) [_m "Label|Consist Address Active for F1-F8"]
        set CV_WidgetConstructors(21) \
              [list CVByte -bytenumber 21 -label $CV_Labels(21)]
        set CV_Tab(21) optCommonCVsGroup2
        set CV_Labels(22) [_m "Label|Consist Address Active for FL and F9-F12"]
        set CV_WidgetConstructors(22) \
              [list CVByte -bytenumber 22 -label $CV_Labels(22)]
        set CV_Tab(22) optCommonCVsGroup2
        set CV_Labels(23) [_m "Label|Acceleration Adjustment"]
        set CV_WidgetConstructors(23) \
              [list CVByte -bytenumber 23 -label $CV_Labels(23)]
        set CV_Tab(23) optCommonCVsGroup2
        set CV_Labels(24) [_m "Label|Deceleration Adjustment"]
        set CV_WidgetConstructors(24) \
              [list CVByte -bytenumber 24 -label $CV_Labels(24)]
        set CV_Tab(24) optCommonCVsGroup2
        set CV_Labels(25) [_m "Label|Speed Table/Mid Range Cab Speed Step"]
        set CV_WidgetConstructors(25) \
              [list CVByte -bytenumber 25 -label $CV_Labels(25)]
        set CV_Tab(25) optCommonCVsGroup2
        set CV_Labels(27) [_m "Label|Decoder Automatic Stopping Configuration"]
        set CV_WidgetConstructors(27) \
              [list CVBitField -bytenumber 27 \
               -label $CV_Labels(27) \
               -fieldlables \
               [list [_m "Label|Right rail positive"] \
                [_m "Label|Left rail positive"] \
                [_m "Label|Signal Controlled Influence cutout"] \
                [_m "Label|Reserved"] \
                [_m "Label|Reverse DC"] \
                [_m "Label|Forward DC"] \
                [_m "Label|Reserved"] \
                [_m "Label|Reserved"]]]
        set CV_Tab(27) optCommonCVsGroup2
        set CV_Labels(28) [_m "Label|Bi-Directional Communication Configuration"]
        set CV_WidgetConstructors(28) \
              [list CVBitField -bytenumber 28 \
               -label $CV_Labels(28) \
               -fieldlables \
               [list [_m "Label|Unsolicited Decoder Initiated Transmission"] \
                [_m "Label|Initiated Broadcast Transmission using Asymmetrical DCC Signal"] \
                [_m "Label|Initiated Broadcast Transmission using Signal Controlled Influence Signal"] \
                [_m "Label|Reserved"] \
                [_m "Label|Reserved"] \
                [_m "Label|Reserved"] \
                [_m "Label|Reserved"] \
                [_m "Label|Reserved"]]]
        set CV_Tab(28) optCommonCVsGroup2
        set CV_Labels(29) [_m "Label|Configuration Data"]
        set CV_WidgetConstructors(29) \
              [list CVBitField -bytenumber 29 \
               -label $CV_Labels(29) \
               -fieldlables \
               [list [_m "Label|Locomotive Direction"] \
                [_m "Label|FL location"] \
                [_m "Label|Power Source Conversion"] \
                [_m "Label|Bi-Directional Communications"] \
                [_m "Label|Speed Table"] \
                [_m "Label|Extended Addressing"] \
                [_m "Label|Reserved"] \
                [_m "Label|Decoder type"]]]
        set CV_Tab(29) requiredCVs
        set CV_Labels(30) [_m "Label|ERROR Information"]
        set CV_WidgetConstructors(30) \
              [list CVByte -bytenumber 30 -label $CV_Labels(30)]
        set CV_Tab(30) optCommonCVsGroup3
        set CV_Labels(33) [_m "Label|Forward Headlight FL(f)"]
        set CV_WidgetConstructors(33) \
              [list CVBitField -bytenumber 33 \
               -label $CV_Labels(33) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 1"] \
                [_m "Label|Output 2"] \
                [_m "Label|Output 3"] \
                [_m "Label|Output 4"] \
                [_m "Label|Output 5"] \
                [_m "Label|Output 6"] \
                [_m "Label|Output 7"] \
                [_m "Label|Output 8"]]]
        set CV_Tab(33) optCommonCVsGroup3
        set CV_Labels(34) [_m "Label|Reverse Headlight FL(r)"]
        set CV_WidgetConstructors(34) \
              [list CVBitField -bytenumber 34 \
               -label $CV_Labels(34) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 1"] \
                [_m "Label|Output 2"] \
                [_m "Label|Output 3"] \
                [_m "Label|Output 4"] \
                [_m "Label|Output 5"] \
                [_m "Label|Output 6"] \
                [_m "Label|Output 7"] \
                [_m "Label|Output 8"]]]
        set CV_Tab(34) optCommonCVsGroup3
        set CV_Labels(35) [_m "Label|Function 1"]
        set CV_WidgetConstructors(35) \
              [list CVBitField -bytenumber 35 \
               -label $CV_Labels(35) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 1"] \
                [_m "Label|Output 2"] \
                [_m "Label|Output 3"] \
                [_m "Label|Output 4"] \
                [_m "Label|Output 5"] \
                [_m "Label|Output 6"] \
                [_m "Label|Output 7"] \
                [_m "Label|Output 8"]]]
        set CV_Tab(35) optCommonCVsGroup3
        set CV_Labels(36) [_m "Label|Function 2"]
        set CV_WidgetConstructors(36) \
              [list CVBitField -bytenumber 36 \
               -label $CV_Labels(36) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 1"] \
                [_m "Label|Output 2"] \
                [_m "Label|Output 3"] \
                [_m "Label|Output 4"] \
                [_m "Label|Output 5"] \
                [_m "Label|Output 6"] \
                [_m "Label|Output 7"] \
                [_m "Label|Output 8"]]]
        set CV_Tab(36) optCommonCVsGroup3
        set CV_Labels(37) [_m "Label|Function 3"]
        set CV_WidgetConstructors(37) \
              [list CVBitField -bytenumber 37 \
               -label $CV_Labels(37) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 1"] \
                [_m "Label|Output 2"] \
                [_m "Label|Output 3"] \
                [_m "Label|Output 4"] \
                [_m "Label|Output 5"] \
                [_m "Label|Output 6"] \
                [_m "Label|Output 7"] \
                [_m "Label|Output 8"]]]
        set CV_Tab(37) optCommonCVsGroup3
        set CV_Labels(38) [_m "Label|Function 4"]
        set CV_WidgetConstructors(38) \
              [list CVBitField -bytenumber 38 \
               -label $CV_Labels(38) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 4"] \
                [_m "Label|Output 5"] \
                [_m "Label|Output 6"] \
                [_m "Label|Output 7"] \
                [_m "Label|Output 8"] \
                [_m "Label|Output 9"] \
                [_m "Label|Output 10"] \
                [_m "Label|Output 11"]]]
        set CV_Tab(38) optCommonCVsGroup3
        set CV_Labels(39) [_m "Label|Function 5"]
        set CV_WidgetConstructors(39) \
              [list CVBitField -bytenumber 39 \
               -label $CV_Labels(39) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 4"] \
                [_m "Label|Output 5"] \
                [_m "Label|Output 6"] \
                [_m "Label|Output 7"] \
                [_m "Label|Output 8"] \
                [_m "Label|Output 9"] \
                [_m "Label|Output 10"] \
                [_m "Label|Output 11"]]]
        set CV_Tab(39) optCommonCVsGroup3
        set CV_Labels(40) [_m "Label|Function 6"]
        set CV_WidgetConstructors(40) \
              [list CVBitField -bytenumber 40 \
               -label $CV_Labels(40) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 4"] \
                [_m "Label|Output 5"] \
                [_m "Label|Output 6"] \
                [_m "Label|Output 7"] \
                [_m "Label|Output 8"] \
                [_m "Label|Output 9"] \
                [_m "Label|Output 10"] \
                [_m "Label|Output 11"]]]
        set CV_Tab(40) optCommonCVsGroup4
        set CV_Labels(41) [_m "Label|Function 7"]
        set CV_WidgetConstructors(41) \
              [list CVBitField -bytenumber 41 \
               -label $CV_Labels(41) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 4"] \
                [_m "Label|Output 5"] \
                [_m "Label|Output 6"] \
                [_m "Label|Output 7"] \
                [_m "Label|Output 8"] \
                [_m "Label|Output 9"] \
                [_m "Label|Output 10"] \
                [_m "Label|Output 11"]]]
        set CV_Tab(41) optCommonCVsGroup4
        set CV_Labels(42) [_m "Label|Function 8"]
        set CV_WidgetConstructors(42) \
              [list CVBitField -bytenumber 42 \
               -label $CV_Labels(42) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 4"] \
                [_m "Label|Output 5"] \
                [_m "Label|Output 6"] \
                [_m "Label|Output 7"] \
                [_m "Label|Output 8"] \
                [_m "Label|Output 9"] \
                [_m "Label|Output 10"] \
                [_m "Label|Output 11"]]]
        set CV_Tab(42) optCommonCVsGroup4
        set CV_Labels(43) [_m "Label|Function 9"]
        set CV_WidgetConstructors(43) \
              [list CVBitField -bytenumber 43 \
               -label $CV_Labels(43) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 7"] \
                [_m "Label|Output 8"] \
                [_m "Label|Output 9"] \
                [_m "Label|Output 10"] \
                [_m "Label|Output 11"] \
                [_m "Label|Output 12"] \
                [_m "Label|Output 13"] \
                [_m "Label|Output 14"]]]
        set CV_Tab(43) optCommonCVsGroup4
        set CV_Labels(44) [_m "Label|Function 10"]
        set CV_WidgetConstructors(44) \
              [list CVBitField -bytenumber 44 \
               -label $CV_Labels(44) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 7"] \
                [_m "Label|Output 8"] \
                [_m "Label|Output 9"] \
                [_m "Label|Output 10"] \
                [_m "Label|Output 11"] \
                [_m "Label|Output 12"] \
                [_m "Label|Output 13"] \
                [_m "Label|Output 14"]]]
        set CV_Tab(44) optCommonCVsGroup4
        set CV_Labels(45) [_m "Label|Function 11"]
        set CV_WidgetConstructors(45) \
              [list CVBitField -bytenumber 45 \
               -label $CV_Labels(45) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 7"] \
                [_m "Label|Output 8"] \
                [_m "Label|Output 9"] \
                [_m "Label|Output 10"] \
                [_m "Label|Output 11"] \
                [_m "Label|Output 12"] \
                [_m "Label|Output 13"] \
                [_m "Label|Output 14"]]]
        set CV_Tab(45) optCommonCVsGroup4
        set CV_Labels(46) [_m "Label|Function 12"]
        set CV_WidgetConstructors(46) \
              [list CVBitField -bytenumber 46 \
               -label $CV_Labels(46) \
               -columns 8 -fieldlables \
               [list [_m "Label|Output 7"] \
                [_m "Label|Output 8"] \
                [_m "Label|Output 9"] \
                [_m "Label|Output 10"] \
                [_m "Label|Output 11"] \
                [_m "Label|Output 12"] \
                [_m "Label|Output 13"] \
                [_m "Label|Output 14"]]]
        set CV_Tab(46) optCommonCVsGroup4
        set CV_Labels(65) [_m "Label|Kick Start"]
        set CV_WidgetConstructors(65) \
              [list CVByte -bytenumber 65 -label $CV_Labels(65)]
        set CV_Tab(65) additionalCVsGroup1
        set CV_Labels(66) [_m "Label|Forward Trim"]
        set CV_WidgetConstructors(66) \
              [list CVByte -bytenumber 66 -label $CV_Labels(66)]
        set CV_Tab(66) additionalCVsGroup1
        set CV_WidgetConstructors(67) [list SpeedTable]
        set CV_Tab(67) additionalCVsGroup1
        set CV_Labels(95) [_m "Label|Reverse Trim"]
        set CV_WidgetConstructors(95) \
              [list CVByte -bytenumber 95 -label $CV_Labels(95)]
        set CV_Tab(95) additionalCVsGroup1
        set CV_Labels(105) [_m "Label|User Identifier #1"]
        set CV_WidgetConstructors(105) \
              [list CVByte -bytenumber 105 -label $CV_Labels(105)]
        set CV_Tab(105) additionalCVsGroup1
        set CV_Labels(106) [_m "Label|User Identifier #2"]
        set CV_WidgetConstructors(106) \
              [list CVByte -bytenumber 106 -label $CV_Labels(106)]
        set CV_Tab(106) additionalCVsGroup1
        set CV_Labels(892) [_m "Label|Decoder Load"]
        set CV_WidgetConstructors(892) \
              [list CVByte -bytenumber 892 -label $CV_Labels(892)]
        set CV_Tab(892) additionalCVsGroup2
        set CV_Labels(893) [_m "Label|Dynamic Flags"]
        set CV_WidgetConstructors(893) \
              [list CVByte -bytenumber 893 -label $CV_Labels(893)]
        set CV_Tab(893) additionalCVsGroup2
        set CV_Labels(894) [_m "Label|Fuel/Coal"]
        set CV_WidgetConstructors(894) \
              [list CVByte -bytenumber 894 -label $CV_Labels(894)]
        set CV_Tab(894) additionalCVsGroup2
        set CV_Labels(895) [_m "Label|Water"]
        set CV_WidgetConstructors(895) \
              [list CVByte -bytenumber 895 -label $CV_Labels(895)]
        set CV_Tab(895) additionalCVsGroup2
    }
    variable cvWidgets_ -array {}
    #component scroll
    #component scrollframe
    component notebook
    component  requiredCVs
    component  optCommonCVsGroup1
    component  optCommonCVsGroup2
    component  optCommonCVsGroup3
    component  optCommonCVsGroup4
    component  additionalCVsGroup1
    component  additionalCVsGroup2
    component  customCVs
    component buttons
    component indexpage
    component  pageindex
    variable   pageindex_ 0
    component  pageoffset
    variable   pageoffset_ 0
    component  pagecv
    variable   pagecv_ 0
    component  indexpagebuttons
    component customCVDialog
    component  customCVLabel
    component  customCVType
    component  customCVNumber
    component  customCVBit0
    component  customCVBit1
    component  customCVBit2
    component  customCVBit3
    component  customCVBit4
    component  customCVBit5
    component  customCVBit6
    component  customCVBit7
    constructor {args} {
        wm withdraw $win
        wm transient $win [winfo toplevel [winfo parent $win]]
        wm protocol $win WM_DELETE_WINDOW [mymethod hide]
        wm title $win [_ "Configuration Variables (Service mode / Programming Track)"]
        #install scroll using ScrolledWindow $win.scroll \
        #      -scrollbar vertical -auto vertical
        #pack $scroll -expand yes -fill both
        #install scrollframe using ScrollableFrame $win.scrollframe \
        #      -constrainedwidth yes
        #$scroll setwidget $scrollframe
        #set frame [$scrollframe getframe]
        install notebook using ScrollTabNotebook $win.notebook
        pack $notebook -expand yes -fill both
        foreach c {requiredCVs optCommonCVsGroup1 optCommonCVsGroup2 optCommonCVsGroup3 optCommonCVsGroup4 additionalCVsGroup1 additionalCVsGroup2 customCVs} \
              l [list [_m "Label|Required CVs"] [_m "Label|Common CVs, Group 1"] [_m "Label|Common CVs, Group 2"] [_m "Label|Common CVs, Group 3"] [_m "Label|Common CVs, Group 4"] [_m "Label|AdditionalCVs, Group 1"] [_m "Label|AdditionalCVs, Group 2"] [_m "Label|Custom CVs"]] {
            install $c using ttk::frame $notebook.$c
            $notebook add [set $c] -text $l -sticky news
        }        
        #set minwidth 0
        foreach cv [lsort -integer [array names CV_WidgetConstructors]] {
            set constructorFun [lindex $CV_WidgetConstructors($cv) 0]
            set constructorArgs [lrange $CV_WidgetConstructors($cv) 1 end]
            set frame [set $CV_Tab($cv)]
            set cvWidgets_($cv) \
                  [eval [list $constructorFun $frame.cv$cv \
                         -callback [mymethod _CVcallback]] \
                   $constructorArgs]
            pack $cvWidgets_($cv) -fill x
            #update idle
            #set cvw [winfo reqwidth $cvWidgets_($cv)]
            #if {$cvw > $minwidth} {set minwidth $cvw}
            #puts stderr "*** $type create $self: minwidth = $minwidth"
        }
        install indexpage using ttk::labelframe $additionalCVsGroup2.indexpage \
              -labelanchor nw -text [_m "Label|Index page CVs"]
        pack $indexpage -fill x
        install pageindex using LabelSpinBox $indexpage.pageindex \
              -label [_m "Label|Page number"] -from 0 -to 65525 \
              -textvariable [myvar pageindex_]
        pack $pageindex -fill x
        install pageoffset  using LabelSpinBox $indexpage.pageoffset \
              -label [_m "Label|Page Offset"] -from 0 -to 255 \
              -textvariable [myvar pageoffset_]
        pack $pageoffset -fill x
        install pagecv using LabelSpinBox $indexpage.pagecv \
              -label [_m "Label|Value"] \
              -from 0 -to 255 -textvariable [myvar pagecv_]
        pack $pagecv -fill x
        install indexpagebuttons using ButtonBox  $indexpage.indexpagebuttons \
              -orient horizontal
        pack $indexpagebuttons -fill x
        $indexpagebuttons add ttk::button update -text [_m "Label|Update"] \
              -command [mymethod _updateIndexPage]
        $indexpagebuttons add ttk::button load   -text [_m "Label|Load"] \
              -command [mymethod _loadIndexPage]
        incr minwidth 20
        install buttons using ButtonBox $win.buttons \
              -orient horizontal
        pack $buttons -fill x
        $buttons add ttk::button close -text [_m "Label|Close"] \
              -command [mymethod hide]
        $buttons add ttk::button loadall  -text [_m "Label|Load All"] \
              -command [mymethod loadall]
        $buttons add ttk::button updateall  -text [_m "Label|Update All"] \
              -command [mymethod updateall]
        $buttons add ttk::button custom -text [_m "Label|Add Custom CV"] \
              -command [mymethod addCustomCV]
        #update idle
        #wm geometry $win [format {=%dx%d} $minwidth \
        #                  [winfo reqheight $win]]
        #wm minsize  $win $minwidth [winfo reqheight $win]
        $self configurelist $args
    }
    method show {} {
        $self _loadReq 
        wm deiconify $win
    }
    method hide {} {
        wm withdraw $win
    }
    method _updateIndexPage {} {
        puts $options(-commandstationsocket) [format {writeprogcvword %d %d} 31 $pageindex_]
        puts $options(-commandstationsocket) [format {writeprogcvbyte %d %d} [expr {256 + $pageoffset_}] $pagecv_]
    }
    method _loadIndexPage {} {
        puts $options(-commandstationsocket) [format {writeprogcvword %d %d} 31 $pageindex_]
        set _pendingLoads([expr {256 + $pageoffset_}]) $indexpage.pagecv
        puts $options(-commandstationsocket) [format {readcv %d} [expr {256 + $pageoffset_}]]
    }
    method _loadReq {} {
        foreach cv [lsort -integer [array names CV_WidgetConstructors]] {
            if {[winfo parent $cvWidgets_($cv)] eq $requiredCVs} {
                $cvWidgets_($cv) load
            }
        }
    }
    method loadall {} {
        foreach cv [lsort -integer [array names CV_WidgetConstructors]] {
            $cvWidgets_($cv) load
        }
    }
    method updateall {} {
        foreach cv [lsort -integer [array names CV_WidgetConstructors]] {
            $cvWidgets_($cv) update
        }
    }
    variable _pendingLoads -array {}
    method _CVcallback {mode size W} {
        puts stderr "*** $self _CVcallback: $mode $size [$W cget -bytenumber] [$W get]"
        switch $mode {
            load {
                switch $size {
                    1 {
                        set _pendingLoads([$W cget -bytenumber]) $W
                        puts $options(-commandstationsocket) [format {readcv %d} [$W cget -bytenumber]]
                    }
                    2 {
                        set _pendingLoads([$W cget -bytenumber]) $W
                        puts $options(-commandstationsocket) [format {readcvword %d} [$W cget -bytenumber]]
                    }
                }
            }
            update {
                switch $size {
                    1 {
                        puts $options(-commandstationsocket) [format {writeprogcvbyte %d %d} [$W cget -bytenumber] [$W get]]
                    }
                    2 {
                        puts $options(-commandstationsocket) [format {writeprogcvword %d %d} [$W cget -bytenumber] [$W get]]
                    }
                }
            }
        }
    }
    method AnswerCallback {result} {
        puts stderr "*** $self AnswerCallback $result"
        lassign $result mode bytenum value
        puts stderr "*** $self AnswerCallback: mode is $mode, bytenum is $bytenum, value is $value"
        if {$mode eq "load"} {
            if {[info exists _pendingLoads($bytenum)]} {
                if {$value >= 0} {
                    $_pendingLoads($bytenum) set $value
                } 
                unset _pendingLoads($bytenum)
                return true
            }
        }
        return false
    }
    method _createCustomCVDialog {} {
        if {[info exists customCVDialog] && [winfo exists $customCVDialog]} {
            return $customCVDialog
        }
        install customCVDialog using Dialog $win.customCVDialog \
              -cancel 1 -default 0 -modal local \
              -parent $win -side bottom \
              -title [_ "Add Custom CV"] -transient yes
        $customCVDialog add add -text {Add CV} -command [mymethod _AddCustomCV]
        $customCVDialog add cancel -text {Cancel} \
              -command [mymethod _CancelAddCustomCV]
        set frame [$customCVDialog getframe]
        install customCVLabel using LabelEntry $frame.customCVLabel \
              -label [_m "Name: "]
        pack $customCVLabel -fill x
        install customCVType using LabelComboBox $frame.customCVType \
              -label [_m "Type: "] \
              -values {Byte Word Bitfield} \
              -modifycmd [mymethod _changeCustomCVType] -editable no
        pack $customCVType -fill x
        $customCVType set Byte
        install customCVNumber using LabelSpinBox $frame.customCVNumber \
              -label [_m "Label|CV Number"] -range {1 1024 1}
        pack $customCVNumber -fill x
        $customCVNumber set 896
        for {set i 0} {$i < 8} {incr i} {
            install customCVBit$i using LabelEntry $frame.customCVBit$i \
                  -label [_m "Label|Bit %d label" $i] \
                  -state disabled
            pack [set customCVBit$i] -fill x
        }
        return $customCVDialog
    }
    variable _customCVIndex 1025
    method _AddCustomCV {} {
        $customCVDialog withdraw
        set cvlabel [$customCVLabel get]
        set cvtype [$customCVType get]
        set cvnumber [$customCVNumber get]
        puts stderr "*** $self _AddCustomCV: cvnumber is $cvnumber"
        set cvfieldlabels [list]
        if {$cvtype eq "Bitfield"} {
            for {set i 0} {$i < 8} {incr i} {
                set cvbitlabel [[set customCVBit$i] get]
                lappend cvfieldlabels $cvbitlabel
            }
        }
        switch $cvtype {
            Byte {
                set cvWidgets_($_customCVIndex) \
                      [CVByte $customCVs.cv$_customCVIndex \
                       -bytenumber $cvnumber -label $cvlabel \
                       -callback [mymethod _CVcallback]]
                pack $cvWidgets_($_customCVIndex) -fill x
                incr _customCVIndex
            }
            Word {
                set cvWidgets_($_customCVIndex) \
                      [CVWord $customCVs.cv$_customCVIndex \
                       -bytenumber $cvnumber -label $cvlabel \
                       -callback [mymethod _CVcallback]]
                pack $cvWidgets_($_customCVIndex) -fill x
                incr _customCVIndex
            }
            Bitfield {
                set cvWidgets_($_customCVIndex) \
                      [CVBitField $customCVs.cv$_customCVIndex \
                       -bytenumber $cvnumber -label $cvlabel \
                       -fieldlables $cvfieldlabels \
                       -callback [mymethod _CVcallback]]
                pack $cvWidgets_($_customCVIndex) -fill x
                incr _customCVIndex
            }
        }
        $customCVDialog enddialog {}
    }
    method _CancelAddCustomCV {} {
        $customCVDialog withdraw
        $customCVDialog enddialog {}
    }
    method _changeCustomCVType {} {
        if {[$customCVType get] eq "Bitfield"} {
            set state normal
        } else {
            set state disabled
        }
        for {set i 0} {$i < 8} {incr i} {
            [set customCVBit$i] configure -state $state
        }
        
    }
    method addCustomCV {} {
        [$self _createCustomCVDialog] draw
    }
}




package provide ServiceMode 1.0


