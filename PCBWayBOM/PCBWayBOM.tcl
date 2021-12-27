#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon Dec 20 13:56:50 2021
#  Last Modified : <211220.1538>
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
package require Tk
package require tile
package require MainFrame
package require ScrollableFrame
package require ScrollWindow

snit::widgetadaptor MainWindow {
    typevariable _menu {
        "&File" {file:menu} {file} 0 {
            {command "&Open" {file:open} "Open..." {Ctrl o} \
                      -command "[mymethod LoadBOM]"}
            {command "&Save" {file:save} "Save" {Ctrl s} \
                      -command "[mymethod SaveBOM]"}
            {command "Save &As" {file:saveas} "Save as" {Ctrl a} \
                      -command "[mymethod SaveBOMas]"}
            {command "E&xit" {file:exit} "Exit the application" {Ctrl q} \
                      -command "[mymethod CarefulExit]"}
        }
    }
    component bom
    component bomframe
    variable status {}
    variable currentfile {}
    variable dirty no
    variable heading {}
    variable writecol -1
    constructor {args} {
        set menu [subst $_menu]
        installhull using MainFrame -menu $menu -textvariable [myvar status] \
              -width 800 -height 250
        $hull showstatusbar status
        set uframe [$hull getframe]
        set sw [ScrolledWindow $uframe.sw -scrollbar both -auto both]
        pack $sw -expand yes -fill both
        install bomframe using ScrollableFrame [$sw getframe].bomframe
        $sw setwidget $bomframe
        wm protocol [winfo toplevel $win] WM_DELETE_WINDOW [mymethod CarefulExit]
        wm title [winfo toplevel $win] "Edit BOM"
    }
    typevariable csv_files {
        {{CSV Files}   {.csv}      TEXT}
        {{All Files}        *          } }
    method LoadBOM {} {
        if {$currentfile ne {}} {
            set defaultfile $currentfile
        } else {
            set defaultfile pcbway.csv
        }
        set filename [tk_getOpenFile -filetypes $csv_files \
                      -initialdir [pwd] \
                      -initialfile $defaultfile \
                      -parent $win \
                      -title  "Address file to open"]
        if {$filename eq {}} {return}
        if {[catch {open $filename r} fp]} {
            tk_message -icon error -type ok \
                  -message [format "Could not open %s: %s" $filename $fp]
            return
        }
        set heading [gets $fp]
        install bom using csvmatrix %%AUTO%% -sourcechanel $fp
        close $fp
        set currentfile $filename
        set dirty no
        set f [$bomframe getframe]
        foreach ch [winfo children $f] {
            destroy $ch
        }
        set col 0
        set font [font create -family Courier -size -10]
        set widths {}
        set writecol -1
        foreach h [$bom headings] {
            grid [ttk::label $f.h$col -text $h -font $font] -column $col -row 0 -sticky nw
            switch $h {
                {Item #} {lappend widths 10}
                {*Designator} {lappend widths 20}
                {*Qty} {lappend widths 10}
                {Manufacturer} {lappend widths 20}
                {*Mfg Part #} {lappend widths 20}
                {Description / Value} {lappend widths 20}
                {*Package/Footprint} {lappend widths 20}
                {Mounting Type} {lappend widths 10} 
                {*Unit Price(5 sets)} {lappend widths 20} 
                {*Total} {lappend widths 20} 
                {*Delivery Time} {lappend widths 30} 
                {*Actual Purchase Mfg Part #} {lappend widths 20} 
                {*PCBWay Note} {lappend widths 30} 
                {Customer Reply} {lappend widths 30; set writecol $col} 
                {PCBWay Update} {lappend widths 30} 
            }
            incr col
        }
        for {set i 0} {$i < [$bom rows]} {incr i} {
            set row [expr {$i + 1}]
            set col 0
            foreach rv [$bom get row $i] w $widths {
                grid [ttk::entry $f.r${row}C$col -width $w] -column $col -row $row -sticky nw
                $f.r${row}C$col insert end $rv
                if {$col != $writecol} {
                    $f.r${row}C$col configure -state readonly
                } else {
                    bind $f.r${row}C$col <Return> [mymethod _update %W [expr {$row-1}]]
                }
                incr col
            }
        }
    }
    method _update {e row} {
        $bom set cell $writecol $row [$e get]
        set dirty yes
    }
    proc rows {crlist} {
        set result {}
        foreach cr $crlist {
            foreach {col row} $cr {break}
            lappend result $row
        }
        return $result
    }
    proc intersect {l1 l2} {
        set result {}
        foreach e $l1 {
            if {[lsearch $l2 $e] >= 0} {
                lappend result $e
            }
        }
        return $result
    }
    method SaveBOM {} {
        $self SaveBOMas $currentfile
    }
    method SaveBOMas {{savefile {}}} {
        if {![info exists bom]} {return}
        if {$currentfile ne {}} {
            set defaultfile $currentfile
        } else {
            set defaultfile pcbway.csv
        }
        if {$savefile eq {}} {
            set savefile [tk_getSaveFile -filetypes $csv_files \
                          -initialdir [pwd] \
                          -initialfile $defaultfile \
                          -parent $win \
                          -title  "BOM to save to"]
        }
        if {$savefile eq {}} {return}
        if {[catch {open $savefile w} fp]} {
            tk_message -icon error -type ok \
                  -message [format "Could not create %s: %s" $savefile $fp]
            return
        }
        puts $fp $heading
        $bom savetochannel $fp
        set currentfile $savefile
        set dirty no
    }
    method CarefulExit {{dontask no}} {
        if {$dirty} {
            set ans yes
            if {!$dontask} {
                set ans [tk_messageBox \
                         -icon question \
                         -type yesno \
                         -message {Data not saved  -- save and exit?}]
            }
            if {$ans eq "yes"} {
                $self SaveAddressFile
                set dontask yes
            }
        }
        if {$dontask} {
            set ans yes
        } else {
            set ans [tk_messageBox -icon question -type yesno \
                     -message {Really Exit?}]
        }
        switch -exact "$ans" {
            no {return}
            yes {
                exit
            }
        }
    }
}

pack [MainWindow .main] -fill both -expand yes
    
        
