#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Sun Mar 24 21:25:42 2019
#  Last Modified : <190325.1958>
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



package require snit
package require Tk
package require tile
package require MainWindow 1.0
package require ROText 1.0
package require LabelFrames
package require HTMLHelp 2.0

set argv0 [file join  [file dirname [info nameofexecutable]] RRCircuits-Builder]

global HelpDir
set HelpDir [file join [file dirname [file dirname \
                                      [file dirname \
                                       [info script]]]] Help]


snit::widgetadaptor OptionsFrame {
    option -parent
    component programselect
    component targetselect
    component numberoftargets
    component firstNID
    constructor {args} {
        installhull using ttk::labelframe -labelanchor nw \
              -text "Build Options:"
        install programselect using LabelComboBox $win.programselect \
              -label "Program to build: " \
              -values {{16 LED Driver Output}
            {16 PWM Led Driver} 
            {Quad Occupancy Detector}
            {Quad StallMotor W/Sense}} \
              -editable no
        $programselect set {16 LED Driver Output}
        pack $programselect -fill x
        install targetselect using LabelComboBox $win.targetselect \
              -label "Target machine type: " \
              -values {{Raspberry Pi} {Beagle Bone Black} 
            {Pocket Beagle}} \
              -editable no
        $targetselect set {Raspberry Pi}
        pack $targetselect -fill x
        install numberoftargets using LabelSpinBox $win.numberoftargets \
              -label "Number of target instances: " \
              -range {1 10 1}
        pack $numberoftargets -fill x
        install firstNID using LabelEntry $win.firstNID \
              -label "Starting NID (-1): " \
              -text  "05:01:01:01:22:80"
        pack $firstNID -fill x
        pack [frame $win.fill] -fill both -expand yes
        $self configurelist $args
    }
    method GetBuildOptions {} {
        set result [list]
        lappend result -program [$programselect get]
        lappend result -target  [$targetselect  get]
        lappend result -numberoftargets [$numberoftargets get]
        lappend result -firstnid [$firstNID get]
        return $result
    }
}

snit::type RRCircuits-Builder {
    # Set up for an ensemble command.
    pragma -hastypeinfo    no
    pragma -hastypedestroy no
    pragma -hasinstances   no
    
    typecomponent main
    typecomponent buildlog
    typecomponent optionsFrame
    
    typevariable BaseDirectory {}
    typevariable OPENMRNPATH {}
    typevariable ProgramDirectories -array {
        {16 LED Driver Output} 16DriverOutputOpenMRN
        {16 PWM Led Driver} 16PWMLedDriverOpenMRN
        {Quad Occupancy Detector} QuadOCDectOpenMRN
        {Quad StallMotor W/Sense} QuadSMCSenseOpenMRN
    }
    typevariable TargetDirectories -array {
        {Raspberry Pi} rpi.linux.armv7a
        {Beagle Bone Black} bbb.linux.armv7a
        {Pocket Beagle} pb.linux.armv7a
    }
    typevariable _eof
    typevariable _logfp
    typeconstructor {
        set BaseDirectory [file dirname [file dirname [file normalize [info nameofexecutable]]]]
        if {[info exists ::env(OPENMRNPATH)]} {
            set OPENMRNPATH $::env(OPENMRNPATH)
        } elseif {[file exists /opt/openmrn/src]} {
            set OPENMRNPATH /opt/openmrn
        } elseif {[file exists ~/openmrn/src]} {
            set OPENMRNPATH [file normalize ~/openmrn]
        } elseif {[file exists ../../../src]} {
            set OPENMRNPATH [file normalize ../../..]
        } else {
            tk_messageBox -default ok -icon error -type ok \
                  -message "OPENMRNPATH not found.  Please install OpenMRN in one of the standard places."
            $type _carefulExit yes
        }
        set main [mainwindow .main -scrolling yes \
                  -height 480 -width 640]
        pack $main -fill both -expand yes
        $main menu entryconfigure file "Exit" \
              -command [mytypemethod _carefulExit]
        set buildlog [ROText [$main scrollwindow getframe].buildlog]
        $main scrollwindow setwidget $buildlog
        $type _buildTools
        $main slideout add options
        set optionsFrame [OptionsFrame \
                          [$main slideout getframe options].options \
                          -parent $type]
        pack $optionsFrame -fill both -expand yes
        pack [$type _makeConfigInfoFrame \
              [$main slideout getframe options].configInfo] \
              -fill both -expand yes
        $main slideout show options
        HTMLHelp setDefaults "$::HelpDir" "index.html#toc"
        $main menu entryconfigure help "On Help..." -command {HTMLHelp help Help}
        $main menu delete help "On Keys..."
        $main menu delete help "Index..."
        $main menu delete help "Tutorial..."
        $main menu entryconfigure help "On Version" -command {HTMLHelp help Version}
        $main menu entryconfigure help "Warranty" -command {HTMLHelp help Warranty}
        $main menu entryconfigure help "Copying" -command {HTMLHelp help Copying}
        $main menu add help command \
              -label "Reference Manual" \
              -command {HTMLHelp help "RRCircuits Builder Reference"}
        $main showit
    }
    typemethod _makeConfigInfoFrame {w} {
        ttk::labelframe $w -labelanchor nw -text "Configuration:"
        pack [LabelEntry $w.basedir -label "BaseDirectory: " \
              -text $BaseDirectory -editable no] -fill x
        pack [LabelEntry $w.openmrn -label "OPENMRNPATH: " \
              -text $OPENMRNPATH -editable no] -fill x
        set gccver [lindex [split [exec gcc -v 2>@1] "\n"] end]
        pack [LabelEntry $w.gccversion -label "Compiler: " \
              -text $gccver -editable no] -fill x
        pack [LabelEntry $w.kversion -label "Kernel: " \
              -text [exec uname -s -n -r -m -o] -editable no] -fill x
        return $w
    }
    typemethod _buildTools {} {
        $main toolbar add tools
        $main toolbar addbutton tools sysinstall \
              -text "System Install" \
              -command [mytypemethod _SysInstall]
        $main toolbar addbutton tools build -text "Build" \
              -command [mytypemethod _Build]
        $main toolbar addbutton tools clearlog -text "Clear Log" \
              -command [mytypemethod _ClearLog]
        $main toolbar addbutton tools savelog -text "Save log" \
              -command [mytypemethod _SaveLog]
        $main toolbar show tools
    }
    typemethod _SysInstall {} {
        set _logfp [open "|sudo -A apt-get update" r]
        set _eof 0
        fileevent $_logfp readable [mytypemethod _fp2log]
        vwait [mytypevar _eof]
        set _logfp [open "|sudo -A apt-get dist-upgrade" r]
        set _eof 0
        fileevent $_logfp readable [mytypemethod _fp2log]
        vwait [mytypevar _eof]
        set _logfp [open "|sudo -A apt-get install build-essential tcl tcllib" r]
        set _eof 0
        fileevent $_logfp readable [mytypemethod _fp2log]
        vwait [mytypevar _eof]
    }
    typemethod _Build {} {
        set opts [$optionsFrame GetBuildOptions]
        $buildlog insert end "$opts"
        set count [from opts -numberoftargets]
        set topprogfile [file join $BaseDirectory $ProgramDirectories([from opts -program])]
        set targetdir   [file join $topprogfile targets $TargetDirectories([from opts -target])]
        if {[catch {open [file join $topprogfile NODEID.txt] w} nfp]} {
            tk_messageBox -default ok -icon error \
                  -message "Failed to open [file join $topprogfile NODEID.txt] for write: $nfp" \
                  -type ok
            return
        }
        puts $nfp [split [from opts -firstnid] :]
        close $nfp
        set command [list make -C]
        lappend command $targetdir
        lappend command NodeIDBin
        for {set i 0} {$i < $count} {incr i} {
            set _logfp [open "|$command" r]
            set _eof 0
            fileevent $_logfp readable [mytypemethod _fp2log]
            vwait [mytypevar _eof]
        }
    }
    typemethod _ClearLog {} {
        $buildlog delete 1.0 end
    }
    typemethod _SaveLog {} {
        set file [tk_getSaveFile -defaultextension .log \
                  -filetypes { {{Log files} {.log}  } {{All Files}  *      } } \
                  -initialdir . -initialfile build.log \
                  -title "File to save the log to"]
        if {$file eq {}} {return}
        if {[catch {open $file w} fp]} {
            tk_messageBox -default ok -icon error \
                  -message "Failed to open $file for write: $fp" \
                  -type ok
            return
        }
        puts $fp [$buildlog get 1.0 end-1c]
        close $fp
    }
    typemethod _fp2log {} {
        if {[gets $_logfp line] < 0} {
            incr _eof
            catch {close $_logfp}
            return
        } else {
            $buildlog insert end "$line\n"
            $buildlog see end
        }
    }
    typemethod _carefulExit {{dontask no}} {
        if {$dontask} {
            set answer yes
        } else {
            set answer "[tk_messageBox -icon question -type yesno -message {Really Exit}]"
        }
        switch -exact $answer {
            no {return}
            yes {
                exit
            }
        }
    }
}

