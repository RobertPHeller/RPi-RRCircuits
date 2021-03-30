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
#  Last Modified : <210330.1051>
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
# @mainpage Introduction
#
# This is the GUI Front end for the Beagleboard OpenLCB/OpenMRN/LCC/DCC
# command station.  This program provides a friendly User Interface to
# the command station.  The command station implements the OpenLCB 
# traction message protocol using "virtual" Train nodes to receive
# OpenLCB Traction messages from OpenLCB throttle nodes. These 
# OpenLCB Traction messages are then converted to DCC packets and
# sent to the rails.
#
# The GUI provides an interface to command station operating status,
# access to the table of "virtual" Train nodes (DCC locomotives 
# available for throttles to control), and access to the decoder CVs
# on the programming track.
#
# 
# @anchor toc
# @htmlonly
# <div class="contents">
# <div class="textblock"><ol type="1">
# <li><a class="el" href="CommandStationGUI.html">Man page</a></li>
# <li><a class="el" href="index.html">GUI Front End Reference</a></li>
# <li><a class="el" href="mainGUI.html">Main GUI</a><ol type="a">
# <li><a class="el" href="mainGUI.html#locomotivedescription">Locomotive Description</a></li>
# <li><a class="el" href="mainGUI.html#locomotivelist">Locomotive List</a></li>
# <li><a class="el" href="mainGUI.html#commandstationstatus">Command Station Status</a></li>
# <li><a class="el" href="mainGUI.html#logsection">Log Output</a></li>
# </ol></li>
# <li><a class="el" href="servicemode.html">Service Mode Screen</a><ol type="a">
# <li><a class="el" href="servicemode.html#cvs">CV display</a></li>
# <li><a class="el" href="servicemode.html#buttons">Buttons</a></li>
# <li><a class="el" href="servicemode.html#definenewcv">Defining new CVs</a></li>
# </ol></li>
# <li><a class="el" href="help.html">Help</a></li>
# <li><a class="el" href="Version.html">Version</a></li>
# <li><a class="el" href="Copying.html">Copying</a><ol type="a">
# <li><a class="el" href="Copying.html#Warranty">Warranty</a></li>
# </ol></li>
# </ol></div></div>
# @endhtmlonly
#
#
# @page CommandStationGUI Invoking the GUI Front end
# GUI Front end for the BeagleBone Black Command Station.  Complete
# user documentation is available as a PDF reference manual which
# can be printed or on screen by clicking on the Help toolbar button
# or under the Help menu.
#
# @section SYNOPSIS SYNOPSIS
#
# CommandStationGUI ?X11 options? ?-- options?
# 
# @section DESCRIPTION DESCRIPTION
#
# This is the GUI Front end for the Beagleboard OpenLCB/OpenMRN/LCC/DCC
# command station.  This program provides a friendly User Interface to
# the command station.  It connects to the OpenMRN Command Station via
# a Tcp/Ip connection.
#
# @section OPTIONS OPTIONS
#
# See the man page for wish for the X11 options.
#
# @arg -port The Tcp/Ip port to connect to.  Default is 9900.
# @arg -host The host name or IP address of the Beagleboard the 
# OpenMRN command station node is running on.  For Pocket Beagles
# operating as USB tethered node, this would be IP address 192.168.6.2
# or 192.168.7.2.
# @arg -help Display help text and exit.
# @par
#
# @section PARAMETERS PARAMETERS
#
# None.
#
# @section FILES FILES
#
# None.
#
# @section AUTHOR AUTHOR
# Robert Heller \<heller\@deepsoft.com\>
#
# @page mainGUI Main GUI
#
# The main GUI is shown here:
# @par
# @image  latex CommandStationMainGUI.png "Command Station Main GUI" width=5in
# @image html CommandStationMainGUISmall.png
# @par
# At the top is a menubar, with File, Edit, Programming Track, and 
# Help menus. Then there is a toolbar. Below the toolbar are four 
# sections:
# -# The locomotive description section.
# -# The locomotive list section.
# -# The command station status section.
# -# A log section.
# @section locomotivedescription Locomotive Description
# The locomotive description section contains a snapshot of a selected
# locomotive, including its address, speed steps (28 or 128), name,
# description, its speed and direction, its functions, its controlling
# throttle and its consist status.
# @section locomotivelist Locomotive List
# The locomotive list section lists the virtual Train nodes in DCC
# address order.  There are three buttons below the list to describe,
# delete, or add locomotives.
# @section commandstationstatus Command Station Status
# The command station status section displays the current status of
# of the command station. This includes the current in use on each of
# the outputs (mains and programming track), whether the output is
# enabled, and if it is over current and if thermal shutdown is 
# eminent.  It also displays the temperature of the heat sink, 
# whether the fan is on, and if the temperature alarm is on.
# @section logsection Log Output
# The log section at the bottom will contain any log messages produced
# by the command station.


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
package require ServiceMode
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
    component number
    option    -number   -default 0 -type FunctionNumber \
          -configuremethod _setnumber -cgetmethod _getnumber
    variable  number_
    method _setnumber {o v} {set number_ $v}
    method _getnumber {o} {return $number_}
    component light
    component onbm
    component offbm
    option    -oncolor  -default white -type Color \
          -configuremethod setbmforeground_
    option    -offcolor -default black -type Color \
          -configuremethod setbmforeground_
    method setbmforeground_ {o v} {
        set options($o) $v
        if {$o eq "-oncolor} {
            $onbm configure -foregound $v
        } else {
            $offbm configure -foregound $v
        }
    }
    option    -orient   -default horizontal -type Orientation \
          -readonly yes
    method set {v} {
        snit::boolean validate $v
        if {$v} {
            $light configure -image $onbm
        } else {
            $light configure -image $offbm
        }
    }
    typevariable dot_ {#define dot9x9_width 9
#define dot9x9_height 9
static unsigned char dot9x9_bits[] = {
   0x38, 0x00, 0x7c, 0x00, 0xfe, 0x00, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01,
   0xfe, 0x00, 0x7c, 0x00, 0x38, 0x00};
}
    constructor {args} {
        install number using ttk::label $win.number \
              -textvariable [myvar number_]
        set options(-oncolor) [from args -oncolor]
        set options(-offcolor) [from args -offcolor]
        set onbm [image create bitmap ${selfns}on -data $dot_ \
                -foreground $options(-oncolor)]
        set offbm [image create bitmap ${selfns}off -data $dot_ \
                -foreground $options(-offcolor)]
        install light using ttk::label $win.light \
              -image $offbm
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

snit::widget StatusLight {
    component label
    option    -label   -default 0 \
          -configuremethod _setlabel -cgetmethod _getlabel
    variable  label_
    method _setlabel {o v} {set label_ $v}
    method _getlabel {o} {return $label_}
    component light
    component onbm
    component offbm
    option    -oncolor  -default white -type Color \
          -configuremethod setbmforeground_
    option    -offcolor -default black -type Color \
          -configuremethod setbmforeground_
    method setbmforeground_ {o v} {
        set options($o) $v
        if {$o eq "-oncolor} {
            $onbm configure -foregound $v
        } else {
            $offbm configure -foregound $v
        }
    }
    option    -orient   -default horizontal -type Orientation \
          -readonly yes
    method set {v} {
        #puts stderr "*** $self set $v"
        snit::boolean validate $v
        if {$v} {
            $light configure -image $onbm
        } else {
            $light configure -image $offbm
        }
        #puts stderr "*** -: $light cget -image: [$light cget -image]"
        #puts stderr "*** -: [$light cget -image] cget -foreground: [[$light cget -image] cget -foreground]"
    }
    typevariable dot_ {#define dot9x9_width 9
#define dot9x9_height 9
static unsigned char dot9x9_bits[] = {
   0x38, 0x00, 0x7c, 0x00, 0xfe, 0x00, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01,
   0xfe, 0x00, 0x7c, 0x00, 0x38, 0x00};
}
    constructor {args} {
        install label using ttk::label $win.label \
              -textvariable [myvar label_]
        set options(-oncolor) [from args -oncolor]
        set options(-offcolor) [from args -offcolor]
        set onbm [image create bitmap ${selfns}on -data $dot_ \
                -foreground $options(-oncolor)]
        set offbm [image create bitmap ${selfns}off -data $dot_ \
                -foreground $options(-offcolor)]
        install light using ttk::label $win.light \
              -image $offbm
        $self configurelist $args
        $self set off
        switch $options(-orient) {
            horizontal {
                pack $label -side left
                pack $light  -side right
            }
            vertical {
                pack $label -side top
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
    component mainsenabled
    component mainsthermflag
    component mainsovercurrent
    component progenabled
    component progthermflag
    component progovercurrent
    component fanon
    component alarmon
    
    constructor {args} {
        $self configurelist $args
        set mc [ttk::labelframe $win.mc \
                -text [_m "Label|Mains: "] \
                -labelanchor nw]
        pack $mc -fill x
        set mcLF [LabelFrame $mc.current -text [_m "Label|Current: "]]
        pack $mcLF -fill x
        set mcFrame [$mcLF getframe]
        install mainCurrent using ttk::label $mcFrame.mainCurrent \
              -textvariable [myvar mainCurrent_] \
              -justify right -anchor e
        pack $mainCurrent -side left -expand yes -fill x
        pack [ttk::label $mcFrame.a -text A] -side right
        set statusFrame [ttk::frame $mc.status]
        pack $statusFrame -fill x
        install mainsenabled using StatusLight $statusFrame.en \
              -label [_m "Label|Enabled: "]
        pack $mainsenabled -side left
        install mainsthermflag using StatusLight $statusFrame.tf \
              -label [_m "Label|Thermal Flag: "] -oncolor red
        pack $mainsthermflag -side left
        install mainsovercurrent using StatusLight $statusFrame.oc \
              -label [_m "Label|Over Current: "] -oncolor red
        pack $mainsovercurrent -side left
        set pc [ttk::labelframe $win.pc \
                -text [_m "Label|Prog Track: "] \
                -labelanchor nw]
        pack $pc -fill x
        set pcLF [LabelFrame $pc.current -text [_m "Label|Current: "]]
        pack $pcLF -fill x
        set pcFrame [$pcLF getframe]
        install progCurrent using ttk::label $pcFrame.progCurrent \
              -textvariable [myvar progCurrent_] \
              -justify right -anchor e
        pack $progCurrent -side left -expand yes -fill x
        pack [ttk::label $pcFrame.a -text A] -side right
        set statusFrame [ttk::frame $pc.status]
        pack $statusFrame -fill x
        install progenabled using StatusLight $statusFrame.en \
              -label [_m "Label|Enabled: "]
        pack $progenabled -side left
        install progthermflag using StatusLight $statusFrame.tf \
              -label [_m "Label|Thermal Flag: "] -oncolor red
        pack $progthermflag -side left
        install progovercurrent using StatusLight $statusFrame.oc \
              -label [_m "Label|Over Current: "] -oncolor red
        pack $progovercurrent -side left
        set hsfan [ttk::labelframe $win.hsfan \
                   -text [_m "Label|Heat Sink: "] \
                   -labelanchor nw]
        pack $hsfan -fill x
        set temp [LabelFrame $hsfan.temp -text [_m "Label|Temperature: "]]
        pack $temp -fill x
        set tempFrame [$temp getframe]
        install temperature using ttk::label $tempFrame.temperature \
              -textvariable [myvar temperature_] \
              -justify right -anchor e
        pack $temperature -side left -expand yes -fill x
        pack [ttk::label $tempFrame.c -text C] -side right
        set statusFrame [ttk::frame $hsfan.status]
        pack $statusFrame -fill x
        install fanon using StatusLight $statusFrame.fanon \
              -label [_m "Label|Fan On: "]
        pack $fanon -side left
        install alarmon using StatusLight $statusFrame.alarmon \
              -label [_m "Label|Alarm: "] -oncolor red
        pack $alarmon -side left
        pack [frame $win.filler] -expand yes -fill both
        $self _refresh
    }
    method _refresh {} {
        catch {puts $options(-commandstationsocket) {status dummy}}
        after 1000 [mymethod _refresh]
    }
    method AnswerCallback {line} {
        #puts stderr "*** $self AnswerCallback $line"
        lassign $line mainCurrent_ progCurrent_ temperature_ \
              mainsEn mainsTF mainsOC \
              progEn progTF progOC \
              fanOn AlarmOn
        $mainsenabled set $mainsEn
        $mainsthermflag set $mainsTF
        $mainsovercurrent set $mainsOC
        $progenabled set $progEn
        $progthermflag set $progTF
        $progovercurrent set $progOC
        $fanon set $fanOn
        $alarmon set $AlarmOn
    }
}

            


snit::type CommandStationGUI {
    pragma -hastypeinfo no -hastypedestroy no -hasinstances no
    typecomponent Main_
    typecomponent describe
    typecomponent list
    typecomponent status
    typecomponent nowhere
    typecomponent servicemode
    
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
        } "[_m {Menu|&Programming Track}]" {prog} {prog} 0 {
            {command "[_m {Menu|Programming Track|Service Mode}]" {prog:service} "[_ {Open Service Mode Screen}]" {Ctrl p} -command "[mytypemethod _serviceMode]"}
        } "[_m {Menu|&Help}]" {help} {help} 0 {
            {command "[_m {Menu|Help|On &Help...}]" {help:help} "[_ {Help on help}]" {} -command {HTMLHelp help Help}}
            {command "[_m {Menu|Help|On &Version}]" {help:help} "[_ {Version}]" {} -command {HTMLHelp help Version}}
            {command "[_m {Menu|Help|Warranty}]" {help:help} "[_ {Warranty}]" {} -command {HTMLHelp help Warranty}}
            {command "[_m {Menu|Help|Copying}]" {help:help} "[_ {Copying}]" {} -command {HTMLHelp help Copying}}
            {command "[_m {Menu|Help|Reference}]" {help:help} {} {} -command {HTMLHelp help "GUI Front End Reference"}}
        }
    }
    typemethod usage {command} {
        puts stderr "Usage:"
        puts stderr [format {%s: ?X11 options? ?-- options?} $command]
        puts stderr {}
        puts stderr {Where options are:}
        puts stderr "\t-port portnumber"
        puts stderr "\t\tportnumber is the Tcp/Ip port to use.  Defaults to 9900."
        puts stderr "\t-host hostorip"
        puts stderr "\t\thostorip is the host or IP address of the beagleboard."
        puts stderr {}
    }
    typeconstructor {
        global argc
        global argv0
        global argv
        if {[lsearch -exact $argv -help] >= 0} {
            $type usage $argv0
            ::exit
        }
        set port [from argv -port 9900]
        set host [from argv -host lucy]
        if {[llength $argv] > 0} {
            puts stderr "$argv0: unknown options or parameters: $argv"
            $type usage $argv0
            ::exit 99
        }
        set socket_ [socket $host $port]
        #set socket_ stdout
        fconfigure $socket_ -blocking 0 -buffering line -translation lf
        fileevent $socket_ readable [mytypemethod _readSocket]
        wm protocol . WM_DELETE_WINDOW [mytypemethod _exit]
        wm title . [_ "Command Station GUI"]
        set Main_ [cmdmainwindow .main -menu [subst $menu_]]
        pack $Main_ -expand yes -fill both
        $Main_ toolbar show
        $Main_ toolbar addbutton exit -compound top \
              -text [_m "Label|Exit"] -image [IconImage image quit] \
              -command [mytypemethod _exit]
        $Main_ toolbar addbutton load -compound top \
              -text [_m "Label|Load"] -image [IconImage image folder-open] \
              -command [mytypemethod _load]
        $Main_ toolbar addbutton save -compound top \
              -text [_m "Label|Save"] -image [IconImage image folder] \
              -command [mytypemethod _save]
        $Main_ toolbar addbutton service -compound top \
              -text [_m "Label|Service"] \
              -image [IconImage image configuration] \
              -command [mytypemethod _serviceMode]
        $Main_ toolbar addbutton help -compound top \
              -text [_m "Label|Help"] -image [IconImage image help] \
              -command {HTMLHelp help "GUI Front End Reference"}
        set upper [$Main_ getframe]
        set describe [DescribeLoco $upper.describe -commandstationsocket $socket_]
        $upper add $describe -weight 1
        set list [ListOfLocos $upper.list -commandstationsocket $socket_]
        $upper add $list -weight 1
        set status [Status $upper.status -commandstationsocket $socket_]
        $upper add $status -weight 1
        HTMLHelp setDefaults "$::HelpDir" "index.html#toc"
        set nowhere [frame .nowhere]
        set servicemode [ServiceMode $Main_.servicemode \
                         -commandstationsocket $socket_]
        $Main_ showit
    }
    typemethod lockscreen {} {
        focus .nowhere
        grab set .nowhere
    }
    typemethod unlockscreen {} {
        grab  release .nowhere
    }
    typemethod _exit {{ask yes}} {
        if {$ask} {
            set answer [tk_messageBox -type yesno -icon question \
                        -message [_ "Do you really want to quit?"]]
            if {!$answer} {return}
        }
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
            update idle
            #puts stderr "*** $type _load: line is $line"
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
        #puts stderr "*** $type save_loco: line = $line"
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
            #puts stderr "*** $type _readSocket: line is $line"
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
                    servicemode {
                        if {![$servicemode AnswerCallback $result]} {
                            $Main_ log insert end "$result
                        }
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
    typemethod _serviceMode {} {
        $servicemode show
    }
}

