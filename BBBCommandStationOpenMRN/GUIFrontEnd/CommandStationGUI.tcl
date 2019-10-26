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
#  Last Modified : <191026.1214>
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
package require MainWindow
package require ScrollWindow
package require ROText
#package require ScrollableFrame
package require snitStdMenuBar
package require ButtonBox
#package require ScrollTabNotebook
package require HTMLHelp 2.0

snit::type CommandStationGUI {
    pragma -hastypeinfo no -hastypedestroy no -hasinstances no
    typecomponent Main_
    typecomponent log
    typevariable socket_
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
        fconfigure $socket_ -blocking 0 -buffering line -translation lf
        fileevent $socket_ readable [mytypemethod _readSocket]
        set Main_ [mainwindow .main -menu [subst $menu_]]
        pack $Main_ -expand yes -fill both
        set log [ROText [$Main_ scrollwindow getframe].log]
        $Main_ scrollwindow setwidget $log
        $Main_ showit
    }
    typemethod _exit {} {
        ::exit
    }
    typemethod _load {} {
    }
    typemethod _save {} {
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
                if {[info exists $key]} {
                    set answer [[set $key] AnswerCallback $result]
                    if {$answer ne "false"} {
                        $log insert end "[$answer toString]\n"
                    } else {
                        $log insert end "$result\n"
                    }
                } else {
                    $log insert end "$result\n"
                }
            } else {
                $log insert end "$line\n"
            }
            $log see end
        }
    }
}

