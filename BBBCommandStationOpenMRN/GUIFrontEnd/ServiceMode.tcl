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
#  Last Modified : <191031.1301>
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

package require Tk
package require tile
package require snit
package require LabelFrames
package require ScrollableFrame
package require ScrollWindow

snit::integer CVAddress -min 1 -max 1024


snit::widgetadaptor CVByte {
    option -bytenumber -default 1 -type CVAddress -readonly yes
    option -label      -default "" -readonly yes
    delegate option * to hull except {-label -from -to -editable}
    delegate method * to hull 
    constructor {args} {
        installhull using LabelSpinBox -from 0 -to 255
        $self configurelist $args
        if {$options(-label) ne ""} {
            $hull configure -label $options(-label)
        } else {
            $hull configure -label [_m "Label|CV number %d" $options(-bytenumber)]
        }
    }
}

snit::widgetadaptor CVWord {
    option -bytenumber -default 1 -type CVAddress -readonly yes
    option -label      -default "" -readonly yes
    delegate option * to hull except {-label -from -to -editable}
    delegate method * to hull
    constructor {args} {
        installhull using LabelSpinBox -from 0 -to 65535
        $self configurelist $args
        if {$options(-label) ne ""} {
            $hull configure -label $options(-label)
        } else {
            $hull configure -label [_m "Label|CV number %d" $options(-bytenumber)]
        }
    }
}


snit::widgetadaptor CVReadonly {
    option -bytenumber -default 1 -type CVAddress -readonly yes
    option -label      -default "" -readonly yes
    delegate option * to hull except {-label -editable}
    delegate method * to hull 
    constructor {args} {
        installhull using LabelEntry -editable no
        $self configurelist $args
        if {$options(-label) ne ""} {
            $hull configure -label $options(-label)
        } else {
            $hull configure -label [_m "Label|CV number %d" $options(-bytenumber)]
        }
    }
}

snit::listtype BitFields -minlen 8 -maxlen 8
snit::integer BitNumber -min 0 -max 7
snit::integer uint8_1 -min 0 -max 255

snit::widgetadaptor CVBitField {
    option -bytenumber -default 1 -type CVAddress -readonly yes
    option -label      -default "" -readonly yes
    option -fieldlables -type BitFields -readonly yes -default \
          {Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7}
    delegate option * to hull except {-text}
    delegate method * to hull except {getframe}
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
    method setbyte {byte} {
        uint8_1 validate $byte
        for {set ibit 0} {$ibit < 8} {incr ibit} {
            $self setbit [expr {($byte >> $ibit) & 0x01}]
        }
    }
    method getbyte {} {
        set result 0
        for {set ibit 0} {$ibit < 8} {incr ibit} {
            set result [expr {$result | ([$self getbit $ibit] << $ibit)}]
        }
        return $result
    }
    constructor {args} {
        installhull using LabelFrame
        $self configurelist $args
        if {$options(-label) ne ""} {
            $hull configure -text $options(-label)
        } else {
            $hull configure -text [_m "Label|CV number %d" $options(-bytenumber)]
        }
        set frame [$hull getframe]
        set ibit 0
        foreach bitlabel $options(-fieldlables) {
            pack [ttk::checkbutton $frame.bit$ibit -offvalue 0 \
                  -onvalue 1 -variable [myvar bits_($ibit)] \
                  -text $bitlabel] -side left
            incr ibit
        }
    }
}

snit::widget ServiceMode {
    hulltype tk::toplevel
    option -commandstationsocket
    typevariable CV_Labels -array {}
    typevariable CV_WidgetConstructors -array {}
    typeconstructor {
        set CV_Labels(1) [_m "Label|Primary Address"]
        set CV_WidgetConstructors(1) \
              [list CVByte -bytenumber 1 -label $CV_Labels(1)]
        set CV_Labels(2) [_m "Label|Start Voltage"]
        set CV_WidgetConstructors(2) \
              [list CVByte -bytenumber 2 -label $CV_Labels(2)]
        set CV_Labels(3) [_m "Label|Acceleration Rate"]
        set CV_WidgetConstructors(3) \
              [list CVByte -bytenumber 3 -label $CV_Labels(3)]
        set CV_Labels(4) [_m "Label|Deceleration Rate"]
        set CV_WidgetConstructors(4) \
              [list CVByte -bytenumber 4 -label $CV_Labels(4)]
        set CV_Labels(5) [_m "Label|High Voltage"]
        set CV_WidgetConstructors(5) \
              [list CVByte -bytenumber 5 -label $CV_Labels(5)]
        set CV_Labels(6) [_m "Label|Mid Voltage"]
        set CV_WidgetConstructors(6) \
              [list CVByte -bytenumber 6 -label $CV_Labels(6)]
        set CV_Labels(7) [_m "Label|Manufacturer Version Number"]
        set CV_WidgetConstructors(7) \
              [list CVReadonly -bytenumber 7 -label $CV_Labels(7)]
        set CV_Labels(8) [_m "Label|Manufacturer ID"]
        set CV_WidgetConstructors(8) \
              [list CVReadonly -bytenumber 8 -label $CV_Labels(8)]
        set CV_Labels(9) [_m "Label|Total PWM Period"]
        set CV_WidgetConstructors(9) \
              [list CVByte -bytenumber 9 -label $CV_Labels(9)]
        set CV_Labels(10) [_m "Label|EMF Feedback Cutout"]
        set CV_WidgetConstructors(10) \
              [list CVByte -bytenumber 10 -label $CV_Labels(10)]
        set CV_Labels(11) [_m "Label|Packet time-out Value"]
        set CV_WidgetConstructors(11) \
              [list CVByte -bytenumber 11 -label $CV_Labels(11)]
        set CV_Labels(12) [_m "Label|Power Source Conversion"]
        set CV_WidgetConstructors(12) \
              [list CVByte -bytenumber 12 -label $CV_Labels(12)]
        set CV_Labels(13) [_m "Label|Alternate Mode Function Status"]
        set CV_WidgetConstructors(13) \
              [list CVByte -bytenumber 13 -label $CV_Labels(13)]
        set CV_Labels(14) [_m "Label|Alternate Mode Function 2 Status"]
        set CV_WidgetConstructors(14) \
              [list CVByte -bytenumber 14 -label $CV_Labels(14)]
        set CV_Labels(15) [_m "Label|Decoder Lock A"]
        set CV_WidgetConstructors(15) \
              [list CVByte -bytenumber 15 -label $CV_Labels(15)]
        set CV_Labels(16) [_m "Label|Decoder Lock B"]
        set CV_WidgetConstructors(16) \
              [list CVByte -bytenumber 16 -label $CV_Labels(16)]
        set CV_Labels(17) [_m "Label|Extended Address High"]
        set CV_Labels(18) [_m "Label|Extended Address Low"]
        set CV_WidgetConstructors(17) \
              [list CVWord -bytenumber 17 -label [_m "Label|Extended Address"]]
        set CV_Labels(19) [_m "Label|Consist Address"]
        set CV_WidgetConstructors(19) \
              [list CVByte -bytenumber 19 -label $CV_Labels(19)]
        set CV_Labels(21) [_m "Label|Consist Address Active for F1-F8"]
        set CV_WidgetConstructors(21) \
              [list CVByte -bytenumber 21 -label $CV_Labels(21)]
        set CV_Labels(22) [_m "Label|Consist Address Active for FL and F9-F12"]
        set CV_WidgetConstructors(22) \
              [list CVByte -bytenumber 22 -label $CV_Labels(22)]
        set CV_Labels(23) [_m "Label|Acceleration Adjustment"]
        set CV_WidgetConstructors(23) \
              [list CVByte -bytenumber 23 -label $CV_Labels(23)]
        set CV_Labels(24) [_m "Label|Deceleration Adjustment"]
        set CV_WidgetConstructors(24) \
              [list CVByte -bytenumber 24 -label $CV_Labels(24)]
        set CV_Labels(25) [_m "Label|Speed Table/Mid Range Cab Speed Step"]
        set CV_WidgetConstructors(25) \
              [list CVByte -bytenumber 25 -label $CV_Labels(25)]
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
        set CV_Labels(29) [_m "Label|Configurations Supported"]
        set CV_WidgetConstructors(29) \
              [list CVBitField -bytenumber 29 \
               -label $CV_Labels(29) \
               -fieldlables \
               [list [_m "Label|Locomotive Direction"] \
                [_m "Label|FL location"] \
                [_m "Label|Power Source Conversion"] \
                [_m "Label|Bi-Directional Communications"] \
                [_m "Label|Speed Table"] \
                [_m "Label|Entended Addressing"] \
                [_m "Label|Reserved"] \
                [_m "Label|Decoder type"]]]
        set CV_Labels(30) [_m "Label|ERROR Information"]
        set CV_WidgetConstructors(30) \
              [list CVByte -bytenumber 30 -label $CV_Labels(30)]
        set CV_Labels(31) [_m "Label|Index High Byte"]
        set CV_Labels(32) [_m "Label|Index Low Byte"]
        set CV_WidgetConstructors(31) \
              [list CVWord -bytenumber 31 -label [_m "Label|Index"]]
    }
    variable cvWidgets_ -array {}
    component scroll
    component scrollframe
    component buttons
    constructor {args} {
        wm withdraw $win
        wm transient $win [winfo toplevel [winfo parent $win]]
        wm protocol $win WM_DELETE_WINDOW [mymethod hide]
        install scroll using ScrolledWindow $win.scroll
        pack $scroll -expand yes -fill both
        install scrollframe using ScrollableFrame $win.scrollframe
        $scroll setwidget $scrollframe
        set frame [$scrollframe getframe]
        set irow 0
        foreach cv [lsort -integer [array names CV_WidgetConstructors]] {
            set constructorFun [lindex $CV_WidgetConstructors($cv) 0]
            set constructorArgs [lrange $CV_WidgetConstructors($cv) 1 end]
            set cvWidgets_($cv) \
                  [eval [list $constructorFun $frame.cv$cv] \
                   $constructorArgs]
            grid $cvWidgets_($cv) -column 0 -row $irow -sticky news
            incr irow
        }
        install buttons using ButtonBox $win.buttons \
              -orient horizontal
        pack $buttons -fill x
        $buttons add ttk::button close -text [_m "Label|Close"] \
              -command [mymethod hide]
        
    }
    method show {} {wm deiconify $win}
    method hide {} {wm withdraw $win}
}




package provide ServiceMode 1.0
