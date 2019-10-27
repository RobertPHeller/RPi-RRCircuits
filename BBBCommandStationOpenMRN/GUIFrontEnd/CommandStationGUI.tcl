#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Sat Oct 26 10:09:51 2019
#  Last Modified : <191027.1712>
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

# $Id$

##
#
# @mainpage BBB Command Station GUI Front End
# @anchor toc
# @htmlonly
# <div class="contents">
# <div class="textblock"><ol type="1">
# <li><a class="el" href="help.html">Help</a></li>
# <li><a class="el" href="Version.html">Version</a></li>
# <li><a class="el" href="Copying.html">Copying</a><ol type="a">
# <li><a class="el" href="Copying.html#Warranty">Warranty</a></li>
# </ol></li>
# </ol></div></div>
# @endhtmlonly
# @latexonly
# @endlatexonly
#
# @defgroup CommandStationGUI BBB Command Station GUI Front End
# GUI Front end for the BeagleBone Black Command Station
#
# @section SYNOPSIS SYNOPSIS
# @section DESCRIPTION DESCRIPTION
# @section OPTIONS OPTIONS
# @section PARAMETERS PARAMETERS
# @section FILES FILES
# @section AUTHOR AUTHOR
# Robert Heller \<heller\@deepsoft.com\>
#
# @page mainGUI Main GUI

set argv0 [file join [file dirname [info nameofexecutable]] [file rootname [file tail [info script]]]]

package require Tk
package require tile
package require snit
package require CMDMainWindow
package require ScrollWindow
package require ROText
#package require ScrollableFrame
package require snitStdMenuBar
package require ButtonBox
#package require ScrollTabNotebook
package require HTMLHelp 2.0
package require LabelFrames
package require Dialog
package require Version

global ImageDir 
set ImageDir [file join [file dirname [file dirname [info script]]] \
              Scripts]
global HelpDir
set HelpDir [file join [file dirname [file dirname [file dirname \
                                                    [info script]]]] Help]

#image create photo banner -file [file join $ImageDir banner.gif]

image create photo DeepwoodsBanner -file [file join $ImageDir DeepwoodsBanner.gif]

snit::type Color {
    pragma -hastypeinfo no -hastypedestroy no -hasinstances no
    typemethod validate {value} {
        if {[catch {winfo rgb . $value}]} {
            return -code error -errorcode INVALID  "invalid color: $value"
        }
        return $value
    }
}

snit::integer FunctionNumber -min 0 -max 28
snit::enum Orientation -values {horizontal vertical}

snit::widget FunctionLight {
    typecomponent lightbm
    component number
    option    -number   -default 0 -type FunctionNumber \
          -configuremethod _setnumber -cgetmethod _getnumber
    variable  number_
    method _setnumber {o v} {set number_ $v}
    method _getnumber {o} {return $number_}
    component light
    option    -oncolor  -default white -type Color
    option    -offcolor -default black -type Color
    option    -orient   -default horizontal -type Orientation \
          -readonly yes
    method set {v} {
        snit::boolean validate $v
        if {$v} {
            $light configure -foreground $options(-oncolor)
        } else {
            $light configure -foreground $options(-offcolor)
        }
    }
    typeconstructor {
        set lightbm [image create bitmap lightbm -data {#define dot_width 16
#define dot_height 16
static unsigned char dot_bits[] = {
   0x00, 0x00, 0x80, 0x01, 0xc0, 0x03, 0xe0, 0x07, 0xf0, 0x0f, 0xf8, 0x1f,
   0xfc, 0x3f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfc, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f,
   0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00};
}]
    }
    constructor {args} {
        install number using ttk::label $win.number \
              -textvariable [myvar number_]
        install light using ttk::label $win.light \
              -image $lightbm
        $self configurelist $args
        $self set off
        switch $options(-orient) {
            horizontal {
                pack $number -side left
                pack $light  -side right
            }
            vertical {
                pack $number -side top
                pack $light  -side bottom
            }
        }
    }
}



snit::widget DescribeLoco {
    option -commandstationsocket
    component address
    variable address_
    component steps
    variable steps_
    component name
    variable name_
    component description
    variable description_
    component direction
    variable direction_
    component speedmph
    variable speedmph_
    component functions
    variable  functionsLights_ [list]
    component controller
    variable controller_
    component consistlist
    variable consistlist_
    constructor {args} {
        install address using LabelEntry $win.address \
              -label [_m "Label|Address: "] -editable no \
              -textvariable [myvar address_]
        pack $address -fill x
        install steps using LabelEntry $win.steps \
              -label [_m "Label|Steps: "] -editable no \
              -textvariable [myvar steps_]
        pack $steps -fill x
        install name using LabelEntry $win.name \
              -label [_m "Label|Name: "] -editable no \
              -textvariable [myvar name_]
        pack $name -fill x
        install description using LabelEntry $win.description \
              -label [_m "Label|Description: "] -editable no \
              -textvariable [myvar description_]
        pack $description -fill x
        set speeddir [LabelFrame $win.speeddir \
                      -text [_m "Label|Speed and Dir: "]]
        pack $speeddir -fill x
        set speeddirFrame [$speeddir getframe]
        install speedmph using ttk::entry $speeddirFrame.speedmph \
              -state readonly -textvariable [myvar speedmph_]
        pack $speedmph -side left -expand yes -fill x
        install direction using ttk::entry $speeddirFrame.direction \
              -width 1 -state readonly \
              -textvariable [myvar direction_]
        pack $direction -side right
        install functions using ttk::labelframe $win.functions \
              -labelanchor nw -text [_m "Label|Functions"]
        pack $functions -fill x
        for {set i 0} {$i <= 28} {incr i} {
            set fnlight [FunctionLight $functions.fn$i -number $i]
            if {$i == 0} {
                grid $fnlight -row 0 -column 0 -sticky news
            } else {
                set r [expr {($i-1)/14}]
                set c [expr {(($i-1)%14)+1}]
                grid $fnlight -row $r -column $c -sticky news
            }
            lappend functionsLights_ $fnlight
        }
        install controller using LabelEntry $win.controller \
              -label [_m "Label|Controller: "] -editable no \
              -textvariable [myvar controller_]
        pack $controller -fill x
        install consistlist using LabelEntry $win.consistlist \
              -label [_m "Label|Consist: "] -editable no \
              -textvariable [myvar consistlist_]
        pack $consistlist -fill x
        pack [frame $win.filler] -expand yes -fill both
        $self configurelist $args
    }
    method AnswerCallback {line} {
                #puts stderr "*** $self AnswerCallback $line"
        #puts stderr "*** $self AnswerCallback: llength of line: [llength $line]"
        #puts stderr "*** $self AnswerCallback: llength of line(5) is [llength [lindex $line 5]]"
        if {[llength $line] == 9 &&
            [llength [lindex $line 6]] == 29} {
            lassign $line address_ steps_ name_ description_ \
                  direction_ speedmph_ fn controller_ consistlist_
            foreach f $fn fl $functionsLights_ {
                $fl set $f
            }
            return {}
        } else {
            return false
        }
    }
}

snit::widget ListOfLocos {
    option -commandstationsocket
    component listbox
    component buttons
    
    component addlocodialog
    component   address 
    variable    address_ 3
    component   steps
    variable    steps_ 28
    component   name
    variable    name_ {}
    component   description
    variable    description_ {}
    method _createAddLocoDialog {} {
        if {[info exists addlocodialog] && $addlocodialog ne {} &&
            [winfo exists $addlocodialog]} {return $addlocodialog}
        install addlocodialog using Dialog $win.addlocodialog \
              -cancel 1 -default 0 -modal local \
              -parent $win -side bottom \
              -title [_ "Add Locomotive"] -transient yes
        $addlocodialog add add -text {Add Locomotive} \
              -command [mymethod _Add]
        $addlocodialog add cancel -text {Cancel} \
              -command [mymethod _Cancel]
        set frame [$addlocodialog getframe]
        install address using LabelSpinBox $frame.address \
              -label [_m "Label|Address: "] \
              -textvariable [myvar address_] -range {1 10000 1}
        pack $address -fill x
        install steps using LabelComboBox $frame.steps \
              -label [_m "Label|Steps: "] \
              -textvariable [myvar steps_] -values {28 128} 
        pack $steps -fill x
        install name using LabelEntry $frame.name \
              -label [_m "Label|Name: "] \
              -textvariable [myvar name_]
        pack $name -fill x
        install description using LabelEntry $frame.description \
              -label [_m "Label|Description: "] \
              -textvariable [myvar description_]
        pack $description -fill x
        return $addlocodialog
    }
    constructor {args} {
        install listbox using ttk::treeview $win.listbox \
              -columns [list address] \
              -displaycolumns [list address] \
              -selectmode browse -show {}
        $listbox column address -width 100
        pack $listbox -expand yes -fill both
        install buttons using ButtonBox $win.buttons \
              -orient horizontal
        pack $buttons -expand yes -fill x
            $buttons add ttk::button describe \
              -text [_m "Label|Describe"] \
              -command [mymethod _describe]
        $buttons add ttk::button delete -text [_m "Label|Delete"] \
              -command [mymethod _delete]
        $buttons add ttk::button add -text [_m "Label|Add"] \
              -command [mymethod _add]
        $self configurelist $args
        $self Refresh
    }
    method AnswerCallback {line} {
        $listbox delete [$listbox children {}]
        foreach l $line {
            $listbox insert {} end -id $l -values [list $l] -text $l
        }
        return {}
    }
    method Refresh {} {
        catch {puts $options(-commandstationsocket) "list locomotives"}
    }
    method _describe {} {
        set selected [$listbox selection]
        if {[llength $selected] == 0} {return}
        catch {puts $options(-commandstationsocket) [format {describe locomotive %d} [lindex $selected 0]]}
    }
    method _add {} {
        [$self _createAddLocoDialog] draw
    }
    proc quoteString {s} {
        if {[string first "\"" $s] >= 0} {
            return "'$s'"
        } else {
            return "\"$s\""
        }
    }
    method _Add {} {
        $addlocodialog withdraw
        catch {
            puts $options(-commandstationsocket) \
                  [format {define locomotive %d steps %d %s %s} \
                   $address_ $steps_ [quoteString $name_] \
                   [quoteString $description_]]
        }
        $addlocodialog enddialog {}
    }
    method _Cancel {} {
        $addlocodialog withdraw
        $addlocodialog enddialog {}
    }
    method _delete {} {
        set selected [$listbox selection]
        if {[llength $selected] == 0} {return}
        catch {puts $options(-commandstationsocket) [format {undefine locomotive %d} [lindex $selected 0]]}
    }
    method AllLocomotives {} {
        return [$listbox children {}]
    }
}

snit::widget Status {
    option -commandstationsocket
    component mainCurrent 
    variable mainCurrent_ 000.000
    component progCurrent
    variable progCurrent_ 000.000
    component temperature
    variable temperature_ 000.000
    constructor {args} {
        $self configurelist $args
        set mc [LabelFrame $win.mc -text [_m "Label|Mains current: "]]
        pack $mc -fill x
        set mcFrame [$mc getframe]
        install mainCurrent using ttk::label $mcFrame.mainCurrent \
              -textvariable [myvar mainCurrent_] \
              -justify right -anchor e
        pack $mainCurrent -side left -expand yes -fill x
        pack [ttk::label $mcFrame.a -text A] -side right
        set pc [LabelFrame $win.pc -text [_m "Label|Prog current: "]]
        pack $pc -fill x
        set pcFrame [$pc getframe]
        install progCurrent using ttk::label $pcFrame.progCurrent \
              -textvariable [myvar progCurrent_] \
              -justify right -anchor e
        pack $progCurrent -side left -expand yes -fill x
        pack [ttk::label $pcFrame.a -text A] -side right
        set temp [LabelFrame $win.temp -text [_m "Label|Heat Sink Temp: "]]
        pack $temp -fill x
        set tempFrame [$temp getframe]
        install temperature using ttk::label $tempFrame.temperature \
              -textvariable [myvar temperature_] \
              -justify right -anchor e
        pack $temperature -side left -expand yes -fill x
        pack [ttk::label $tempFrame.c -text C] -side right
        pack [frame $win.filler] -expand yes -fill both
        $self _refresh
    }
    method _refresh {} {
        catch {puts $options(-commandstationsocket) {status dummy}}
        after 1000 [mymethod _refresh]
    }
    method AnswerCallback {line} {
        lassign $line mainCurrent_ progCurrent_ temperature_
    }
}

            


snit::type CommandStationGUI {
    pragma -hastypeinfo no -hastypedestroy no -hasinstances no
    typecomponent Main_
    typecomponent describe
    typecomponent list
    typecomponent status
    typecomponent nowhere
    
    typevariable socket_
    typevariable saveFp_ {}
    typevariable locoCount_ 0
    typevariable menu_ {
        "[_m {Menu|&File}]" {file:menu} {file} 0 {
            {command "[_m {Menu|File|&Load saved locomotives}]" {file:load} "[_ {Load Locomotives}]" {Ctrl l} -command "[mytypemethod _load]"}
            {command "[_m {Menu|File|&Save locomotives}]" {file:save} "[_ {Save Locomotives}]" {Ctrl s} -command "[mytypemethod _save]"}
            {command "[_m {Menu|File|&Exit}]" {file:exit} "[_ {Exit}]" {Ctrl q} -command "[mytypemethod _exit]"}
        } "[_m {Menu|&Edit}]" {edit} {edit} 0 {
            {command "[_m {Menu|Edit|Cu&t}]" {edit:cut edit:havesel} "[_ {Cut selection to the paste buffer}]" {Ctrl x} -command {StdMenuBar EditCut} -state disabled}
            {command "[_m {Menu|Edit|&Copy}]" {edit:copy edit:havesel} "[_ {Copy selection to the paste buffer}]" {Ctrl c} -command {StdMenuBar EditCopy} -state disabled}
            {command "[_m {Menu|Edit|&Paste}]" {edit:paste} "[_ {Paste selection from the paste buffer}]" {Ctrl c} -command {StdMenuBar EditPaste}}
            {command "[_m {Menu|Edit|C&lear}]" {edit:clear edit:havesel} "[_ {Clear selection}]" {} -command {StdMenuBar EditClear} -state disabled}
            {command "[_m {Menu|Edit|&Delete}]" {edit:delete edit:havesel} "[_ {Delete selection}]" {Ctrl d}  -command {StdMenuBar EditClear} -state disabled}
            {separator}
            {command "[_m {Menu|Edit|Select All}]" {edit:selectall} "[_ {Select everything}]" {} -command {StdMenuBar EditSelectAll}}
            {command "[_m {Menu|Edit|De-select All}]" {edit:deselectall edit:havesel} "[_ {Select nothing}]" {} -command {StdMenuBar EditSelectNone} -state disabled}
        } "[_m {Menu|&Help}]" {help} {help} 0 {
            {command "[_m {Menu|Help|On &Help...}]" {help:help} "[_ {Help on help}]" {} -command {HTMLHelp help Help}}
            {command "[_m {Menu|Help|On &Version}]" {help:help} "[_ {Version}]" {} -command {HTMLHelp help Version}}
            {command "[_m {Menu|Help|Warranty}]" {help:help} "[_ {Warranty}]" {} -command {HTMLHelp help Warranty}}
            {command "[_m {Menu|Help|Copying}]" {help:help} "[_ {Copying}]" {} -command {HTMLHelp help Copying}}
            {command "[_m {Menu|Help|Reference node for a CTI Acela network}]" {help:help} {} {} -command {HTMLHelp help "Command Station Reference"}}
        }
    }
    typeconstructor {
        global argc
        global argv0
        global argv
        set port [from argv -port 9900]
        set host [from argv -host snoopy]
        set socket_ [socket $host $port]
        #set socket_ stdout
        fconfigure $socket_ -blocking 0 -buffering line -translation lf
        fileevent $socket_ readable [mytypemethod _readSocket]
        set Main_ [cmdmainwindow .main -menu [subst $menu_]]
        pack $Main_ -expand yes -fill both
        set upper [$Main_ getframe]
        set describe [DescribeLoco $upper.describe -commandstationsocket $socket_]
        $upper add $describe -weight 1
        set list [ListOfLocos $upper.list -commandstationsocket $socket_]
        $upper add $list -weight 1
        set status [Status $upper.status -commandstationsocket $socket_]
        $upper add $status -weight 1
        HTMLHelp setDefaults "$::HelpDir" "index.html#toc"
        set nowhere [frame .nowhere]
        $Main_ showit
    }
    typemethod lockscreen {} {
        focus .nowhere
        grab set .nowhere
    }
    typemethod unlockscreen {} {
        grab  release .nowhere
    }
    typemethod _exit {} {
        ::exit
    }
    typemethod _load {} {
        set filename [tk_getOpenFile -defaultextension .locos \
                      -filetypes { {{Locolist Files} {.locos}  }
                      {{All Files}      *         } } \
                        -initialdir [pwd] \
                        -initialfile station.locos \
                        -parent . \
                        -title "Select file to save loco list into" \
                        ]
        if {$filename eq ""} {return}
        if {[catch {open $filename r} loadFp]} {
            tk_messageBox -type ok -icon error \
                  -message [_ "Could not open %s, because %s" \
                            $filename $loadFp]
            return
        }
        while {[gets $loadFp line] >= 0} {
            puts $socket_ $line
        }
        close $loadFp
    }
    typemethod _save {} {
        set filename [tk_getSaveFile -defaultextension .locos \
                      -filetypes { {{Locolist Files} {.locos}  }
                      {{All Files}      *         } } \
                        -initialdir [pwd] \
                        -initialfile station.locos \
                        -parent . \
                        -title "Select file to save loco list into" \
                        ]
        if {$filename eq ""} {return}
        if {[catch {open $filename w} saveFp_]} {
            tk_messageBox -type ok -icon error \
                  -message [_ "Could not open %s, because %s" \
                            $filename $saveFp_]
            set saveFp_ {}
            return
        }
        $type lockscreen
        set locos [$list AllLocomotives]
        #puts stderr "*** $type _save: locos = $locos"
        set locoCount_ [llength $locos]
        #puts stderr "*** $type _save: locoCount_ = $locoCount_"
        foreach l $locos {
            #puts stderr "*** $type _save: l = $l"
            puts $socket_ [format {describe locomotive %d} $l]
            tkwait variable [mytypevar locoCount_]
            #puts stderr "*** $type _save: locoCount_ = $locoCount_"
        }
        close $saveFp_
        set saveFp_ {}
        $type unlockscreen
    }
    proc quoteString {s} {
        if {[string first "\"" $s] >= 0} {
            return "'$s'"
        } else {
            return "\"$s\""
        }
    }
    typemethod save_loco {line} {
        puts stderr "*** $type save_loco: line = $line"
        if {[llength $line] == 9} {
            lassign $line address_ steps_ name_ description_ \
                  direction_ speedmph_ fn controller_ consistlist_
            puts $saveFp_ [format {define locomotive %d steps %d %s %s} \
                           $address_ $steps_ [quoteString $name_] \
                           [quoteString $description_]]
        }
        incr locoCount_ -1
        if {$locoCount_ == 0} {incr doneFlag_}
    }
    typemethod _readSocket {} {
        if {[gets $socket_ line] < 0} {
            if {[eof $socket_]} {
                close $socket_
                ::exit
            }
        } else {
            set line [regsub {^> } $line {}]
            if {$line eq ""} {return}
            if {[regexp {^#([^#]+)#[[:space:]]+(.*)$} $line => key result] > 0} {
                switch $key {
                    status {
                        $status AnswerCallback $result
                    }
                    list {
                        $list AnswerCallback $result
                    }
                    describe {
                        if {$saveFp_ ne {}} {
                            $type save_loco $result
                        } else {
                            $describe AnswerCallback $result
                        }
                    }
                    define -
                    undefine {
                        $list Refresh
                    }
                    default {
                        $Main_ log insert end "$result
                    }
                }
            } else {
                $Main_ log insert end "$line\n"
            }
            $Main_ log see end
        }
    }
}

