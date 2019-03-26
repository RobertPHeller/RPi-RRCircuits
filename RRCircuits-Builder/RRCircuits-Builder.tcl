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
#  Last Modified : <190326.0031>
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
package require Dialog
package require ScrollWindow

set argv0 [file join  [file dirname [info nameofexecutable]] RRCircuits-Builder]

global HelpDir
set HelpDir [file join [file dirname [file dirname \
                                      [file dirname \
                                       [info script]]]] Help]


snit::widgetadaptor OptionsFrame {
    option -builder -readonly yes -default {} -cgetmethod builder_cget
    method parent_cget {option} {return $builder}
    component builder
    component programselect
    component targetselect
    component numberoftargets
    component firstNID
    constructor {args} {
        set builder [from args -builder]
        if {$builder eq {}} {
            error "Missing -builder option!"
        }
        installhull using ttk::labelframe -labelanchor nw \
              -text "Build Options:"
        install programselect using LabelComboBox $win.programselect \
              -label "Program to build: " \
              -values [$builder ProgramDirectories] \
              -editable no
        $programselect set [lindex [$programselect cget -values] 0]
        pack $programselect -fill x
        install targetselect using LabelComboBox $win.targetselect \
              -label "Target machine type: " \
              -values [$builder TargetDirectories] \
              -editable no
        $targetselect set [lindex [$targetselect cget -values] 0]
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



snit::widgetadaptor DeployDialog {
    delegate option -parent to hull
    component builder
    option -builder -readonly yes -default {} -cgetmethod builder_cget
    method parent_cget {option} {return $builder}
    component programList
    variable user {pi}
    variable host {rasppi}
    variable directory {bin}
    constructor {args} {
        set builder [from args -builder]
        if {$builder eq {}} {
            error "Missing -builder option!"
        }
        installhull using Dialog -separator 0 \
              -modal local -parent . -place center \
              -side bottom -title "Deploy nodes" \
              -transient 1 -anchor e
        $hull add deploy -text Deploy -command [mymethod _Deploy]
        $hull add cancel -text Cancel -command [mymethod _Cancel]
        wm protocol [winfo toplevel $win] WM_DELETE_WINDOW [mymethod _Cancel]
        set frame [$hull getframe]
        set sw [ScrolledWindow $frame.sw -scrollbar both -auto both]
        pack $sw -fill both -expand yes
        install programList using ttk::treeview $sw.programList \
              -columns program -displaycolumns program \
              -selectmode browse -show headings
        $programList heading program -text "Node Program"
        $sw setwidget $programList
        set lf [LabelFrame $frame.userhost -text "SSH Target: "]
        pack $lf -fill x -expand yes
        set lfframe [$lf getframe]
        set uentry [ttk::entry $lfframe.uentry \
                    -textvariable [myvar user] -justify right]
        pack $uentry -side left
        pack [ttk::label $lfframe.at -text {@}] -side left
        set hentry [ttk::entry $lfframe.hentry \
                    -textvariable [myvar host] -justify left]
        pack $hentry -side left
        pack [ttk::label $lfframe.colon -text {:}] -side left
        set dentry [ttk::entry $lfframe.dentry \
                    -textvariable [myvar directory] -justify left]
        pack $dentry -fill x -expand yes -side left
        $self configurelist $args
    }
    method draw {args} {
        $self configurelist $args 
        $programList delete [$programList children {}]
        foreach p [$builder FindAllDeployableNodes] {
            $programList insert {} end -id $p \
                  -value [file tail $p]
        }
        return [$hull draw]
    }
    method _Deploy {} {
        $hull withdraw
        set selected [$programList selection]
        if {$selected ne {}} {
            set result [list $selected]
            set sshdest ${host}:
            if {$user ne {}} {
                set sshdest ${user}@$sshdest
            }
            if {$directory ne {}} {
                append sshdest ${directory}/
            }
            lappend result $sshdest
        } else {
            set result {}
        }
        return [$hull enddialog $result]
    }
    method _Cancel {} {
        $hull withdraw
        return [$hull enddialog {}]
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
    typecomponent deployDialog
    
    typevariable BaseDirectory {}
    typevariable OPENMRNPATH {}
    typevariable _ProgramDirectories -array {
        {16 LED Driver Output} 16DriverOutputOpenMRN
        {16 PWM Led Driver} 16PWMLedDriverOpenMRN
        {Quad Occupancy Detector} QuadOCDectOpenMRN
        {Quad StallMotor W/Sense} QuadSMCSenseOpenMRN
    }
    typemethod ProgramDirectories {} {
        return [array names _ProgramDirectories]
    }
    typevariable _TargetDirectories -array {
        {Raspberry Pi} rpi.linux.armv7a
        {Beagle Bone Black} bbb.linux.armv7a
        {Pocket Beagle} pb.linux.armv7a
    }
    typemethod ProgramFromName {name} {
        if {[info exists _ProgramDirectories($name)]} {
            return $_ProgramDirectories($name)
        } else {
            return {}
        }
    }
    typemethod TargetDirectories {} {
        return [array names _TargetDirectories]
    }
    typemethod TargetFromName {name} {
        if {[info exists _TargetDirectories($name)]} {
            return $_TargetDirectories($name)
        } else {
            return {}
        }
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
                          -builder $type]
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
        $main toolbar addbutton tools deploy -text "Deploy" \
              -command [mytypemethod _Deploy]
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
        set topprogfile [file join $BaseDirectory $_ProgramDirectories([from opts -program])]
        set targetdir   [file join $topprogfile targets $_TargetDirectories([from opts -target])]
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
    typemethod FindAllDeployableNodes {} {
        set result [list]
        foreach p [array names _ProgramDirectories] {
            set ppath $_ProgramDirectories($p)
            set progTargetPath [file join $BaseDirectory \
                                $ppath targets]
            foreach t [array names _TargetDirectories] {
                set tpath $_TargetDirectories($t)
                set t1 [string toupper [lindex [split $tpath {.}] 0]]
                set dir [file join $progTargetPath $tpath]
                puts stderr "*** $type FindAllDeployableNodes: dir = $dir"
                foreach exe [glob -nocomplain \
                              -directory $dir \
                             ${ppath}_${t1}_??_??_??_??_??_??] {
                    lappend result $exe
                }
            }
        }
        return $result
    }
    typemethod _Deploy {} {
        if {[info exists deployDialog] &&
            [winfo exists $deployDialog]} {
        } else {
            set deployDialog [DeployDialog $main.deployDialog -builder $type]
        }
        set deployList [$deployDialog draw -parent $main]
        if {$deployList eq {}} {return}
        set command [linsert $deployList 0 scp]
        $buildlog insert end "$command\n"
        lappend command 2>@1
        set _logfp [open "|$command" r]
        set _eof 0
        fileevent $_logfp readable [mytypemethod _fp2log]
        vwait [mytypevar _eof]
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

