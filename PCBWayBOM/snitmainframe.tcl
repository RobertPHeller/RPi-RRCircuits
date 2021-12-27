##############################################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon May 13 14:24:47 2013
#  Last Modified : <130528.1211>
#
#  Description	
#
#  Notes
#
#  History
#	
##############################################################################
#
#  Copyright (c) 2013 Deepwoods Software.
# 
#  All Rights Reserved.
# 
#  This  document  may  not, in  whole  or in  part, be  copied,  photocopied,
#  reproduced,  translated,  or  reduced to any  electronic  medium or machine
#  readable form without prior written consent from Deepwoods Software.
#
##############################################################################

package require Tk
package require snit
package require tile
package require DynamicHelp 1.0

snit::widget MainFrame {
    widgetclass MainFrame
    hulltype ttk::frame
    
    delegate option -width to hull
    delegate option -height to hull
    option -textvariable -default ""
    option -menu -default "" -readonly yes
    option -separator -type {snit::enum -values {none top bottom both}} -default both -readonly yes
    option -menubarfont -default ""
    option -menuentryfont -default ""
    delegate option -statusbarfont to statusLabel as -font
    delegate option -progressmax to progress as -maximum
    delegate option -progressvar to progress as -variable
    option -sizegrip -default yes -readonly yes -type snit::boolean
    
    component userframe
    component topframe
    component botframe
    component status
    component   label
    component   indframe
    component   prgframe
    component   progress
    
    variable top
    variable ntoolbar 0
    variable nindic 0
    variable menuid -array {}
    variable tags -array {}
    variable tagstate -array {}
    variable menutags -array {}
    variable mbfnt {}
    variable mefnt {}
    constructor {args} {
        set top  [winfo parent $win]
        if { [winfo toplevel $win] ne $top } {
            destroy $win
            return -code error "parent must be a toplevel"
        }
        install userframe using ttk::frame $win.frame
        install topframe  using ttk::frame $win.topf
        install botframe  using ttk::frame $win.botf
        
        pack $topframe -fill x
        grid columnconfigure $topframe 0 -weight 1
        set options(-separator) [from args -separator]
        if {$options(-separator) eq "both" || $options(-separator) eq "top"} {
            pack [ttk::separator $win.sep -orient horizontal] -fill x
        }
        if {$options(-separator) eq "both" || $options(-separator) eq "bottom"} {
            pack [ttk::separator $botframe.sep -orient horizontal] -fill x
        }
        # --- status bar -----------------------------------------------------
        install status using ttk::frame $win.status
        set options(-textvariable) [from args -textvariable]
        install label using ttk::label $status.label \
              -textvariable $options(-textvariable)
        install indframe using ttk::frame $status.indf
        install prgframe using ttk::frame $status.prgf
        place $label    -anchor w -x 0 -rely 0.5
        place $indframe -anchor ne -relx 1 -y 0 -relheight 1
        pack  $prgframe -in $indframe -side left -padx 2
        $status configure -height [winfo reqheight $label]
        install progress using ttk::progressbar $status.prg -orient horizontal
        pack $status    -in $botframe -fill x -pady 2
        pack $botframe  -side bottom -fill x
        pack $userframe -fill both -expand yes
        set options(-menu) [from args -menu]
        if { [llength $options(-menu)] } {
            $self _create_menubar $options(-menu)
        }
        $self configurelist $args
        if {$options(-sizegrip)} {
            pack [ttk::sizegrip $botframe.sizegrip] -side right
        }

    }
    method _create_menubar {descmenu} {
        foreach {v x} {mbfnt -menubarfont mefnt -menuentryfont} {
            if {$options($x) ne ""} {
                set $v [list -font $x]
            } else {
                set $v ""
            }
        }
        set menubar [eval [list menu $top.menubar -tearoff 0] $mbfnt]
        $top configure -menu $menubar
        set count 0
        foreach {name _tags _menuid tearoff entries} $descmenu {
            set opt  [_parse_name $name]
            if {[string length $_menuid] &&
                ![info exists menuid($_menuid)] } {
                # menu has identifier
                # we use it for its pathname, to enable special menu entries
                # (help, system, ...)
                set menu $menubar.$_menuid
            } else {
                set menu $menubar.menu$count
            }
            eval [list $menubar add cascade] $opt [list -menu $menu]
            eval [list menu $menu -tearoff $tearoff] $mefnt
            foreach tag $_tags {
                lappend tags($tag) $menubar $count
                if { ![info exists tagstate($tag)] } {
                    set tagstate($tag) 1
                }
            }
            set menutags([list $menubar $count]) $_tags 
            if { [string length $_menuid] } {
                # menu has identifier 
                set menuid($_menuid) $menu
            }
            $self _create_entries $menu $entries
            incr count
        }
    }
    method _create_entries {menu entries } {
        set count      [$menu cget -tearoff]
        set registered 0
        foreach entry $entries {
            set len  [llength $entry]
            set _type [lindex $entry 0]
            if { [string equal $_type "separator"] } {
                $menu add separator
                incr count
                continue
            }
            # entry name and tags
            set opt  [_parse_name [lindex $entry 1]]
            set _tags [lindex $entry 2]
            foreach tag $_tags {
                lappend tags($tag) $menu $count
                # ericm@scriptics:  Add a tagstate tracker
                if { ![info exists tagstate($tag)] } {
                    set tagstate($tag) 1
                }
            }
            # ericm@scriptics:  Add mapping from menu items to tags
            set menutags([list $menu $count]) $_tags
            if {[string equal $_type "cascade"] || [string equal $_type "cascad"]} {
                set _menuid  [lindex $entry 3]
                set tearoff [lindex $entry 4]
                set submenu $menu.menu$count
                if {$len > 6} {
                    set cascadeopts [lrange $entry 5 end-1]
                } else {
                    set cascadeopts {}
                }
                eval [list $menu add cascade] $opt [list -menu $submenu] $cascadeopts
                eval [list menu $submenu -tearoff $tearoff]
                if { [string length $_menuid] } {
                    # menu has identifier
                    set menuid($_menuid) $submenu
                }
                $self _create_entries $submenu [lindex $entry end]
                incr count
                continue
            }

            # entry help description
            set desc [lindex $entry 3]
            if { [string length $desc] } {
                if { !$registered } {
                    DynamicHelp register $menu menu $options(-textvariable)
                    set registered 1
                }
                DynamicHelp register $menu menuentry $count $desc
            }

            # entry accelerator
            set accel [_parse_accelerator [lindex $entry 4]]
            if { [llength $accel] } {
                lappend opt -accelerator [lindex $accel 0]
                bind $top [lindex $accel 1] [list $menu invoke $count]
            }

            # user options
            set useropt [lrange $entry 5 end]
            if { [string equal $_type "command"] ||
                [string equal $_type "radiobutton"] ||
                [string equal $_type "checkbutton"] } {
                eval [list $menu add $_type] $opt $useropt
            } else {
                return -code error "invalid menu type \"$_type\""
            }
            incr count
        }
    }
    
    proc _parse_name {menuname} {
        set idx [string first "&" $menuname]
        if { $idx == -1 } {
            return [list -label $menuname]
        } else {
            set beg [string range $menuname 0 [expr {$idx-1}]]
            set end [string range $menuname [expr {$idx+1}] end]
            append beg $end
            return [list -label $beg -underline $idx]
        }
    }
    proc _parse_accelerator { desc } {
        if { [llength $desc] == 1 } {
            set seq None
            set key [string tolower [lindex $desc 0]]
            # If the key is an F key (ie, F1, F2, etc), it has to be capitalized
            if {[regexp {^f([1-9]|([12][0-9]|3[0-5]))$} $key]} {
                set key [string toupper $key]
            }
        } elseif { [llength $desc] == 2 } {
            set seq [lindex $desc 0]
            set key [string tolower [lindex $desc 1]]
            # If the key is an F key (ie, F1, F2, etc), it has to be capitalized
            if {[regexp {^f([1-9]|([12][0-9]|3[0-5]))$} $key]} {
                set key [string toupper $key]
            }
        } else {
            return {}
        }
        switch -- $seq {
            None {
                set accel "[string toupper $key]"
                set event "<Key-$key>"
            }
            Ctrl {
                set accel "Ctrl+[string toupper $key]"
                set event "<Control-Key-$key>"
            }
            Alt {
                set accel "Alt+[string toupper $key]"
                set event "<Alt-Key-$key>"
            }
            CtrlAlt {
                set accel "Ctrl+Alt+[string toupper $key]"
                set event "<Control-Alt-Key-$key>"
            }
            default {
                return -code error "invalid accelerator code $seq"
            }
        }
        return [list $accel $event]
    }

    method getframe {} {return $userframe}
    method addtoolbar {} {
        global   tcl_platform
        
        set index $ntoolbar
        set toolframe $topframe.f$index
        set toolbar   $topframe.tb$index
        ttk::frame $toolframe -padding 1
        set toolbar [ttk::frame $toolbar -padding 2]
        pack $toolbar -in $toolframe -anchor w -expand yes -fill x
        incr ntoolbar
        grid $toolframe -column 0 -row $index -sticky ew
        return $toolbar
    }
    method gettoolbar {index} {
        return $topframe.tb$index
    }
    method addindicator { args } {
        set statusbarfont [$label cget -font]
        if { $statusbarfont ne "" } {
            set sbfnt [list -font $statusbarfont]
        } else {
            set sbfnt ""
        }
        set index $nindic
        set indic $indframe.f$index
        eval [list ttk::label $indic] $args -relief sunken -takefocus 0 $sbfnt
        pack $indic -side left -anchor w -padx 2 -fill y -expand 1
        incr nindic
        return $indic
    }
    method getindicator { index } {
        return $indframe.f$index
    }
    method getmenu { _menuid } {
        if { [info exists menuid($_menuid)] } {
            return $menuid($_menuid)
        }
        return ""
    }
    method {menu activate} {_menuid index} {
        if {![info exists menuid($_menuid)] } {return}
        set menu $menuid($_menuid)
        return [eval [list $menu activate $index]]
    }
    method {menu add} {_menuid entrytype args} {
        if {![info exists menuid($_menuid)] } {return}
        set menu $menuid($_menuid)
        set dynhelp [from args -dynamichelp]
        set res [eval [list $menu add $entrytype] $args]
        if {[string length "$dynhelp"]} {
            DynamicHelp add $menu -index [$menu index end] \
                  -variable $options(-textvariable) \
                  -text "$dynhelp"
        }
        return $res
    }
    method {menu delete} {_menuid index1 args} {
        if {![info exists menuid($_menuid)] } {return}
        set menu $menuid($_menuid)
        return [eval [list $menu delete $index1] [list $args]]
    }
    
    method {menu entrycget} {menuid index option} {
        if {![info exists menuid($_menuid)] } {return}
        set menu $menuid($_menuid)
        return [eval [list $menu entrycget $index $option]]
    }
    
    method {menu entryconfigure} {_menuid index args} {
        if {![info exists menuid($_menuid)] } {return}
        set menu $menuid($_menuid)
        set dynhelp [from args -dynamichelp]
        if {[string length "$dynhelp"]} {
            DynamicHelp add $menu -index [$menu index end] \
                  -variable $options(-textvariable) \
                  -text "$dynhelp"
        }
        return [eval [list $menu entryconfigure $index] $args]
    }
    method {menu sethelpvar} {menuid} {
        if {![info exists menuid($_menuid)] } {return}
        set menu $menuid($_menuid)
        DynamicHelp add $menu -variable $options(-textvariable)
    }
    
    method {menu index} {menuid index} {
        if {![info exists menuid($_menuid)] } {return}
        set menu $menuid($_menuid)
        return [eval [list $menu index $index]]
    }
    
    method {menu insert} {menuid index entrytype args} {
        if {![info exists menuid($_menuid)] } {return}
        set menu $menuid($_menuid)
        set dynhelp [from args -dynamichelp]
        set index [$menu index $index]
        set res [eval [list $menu insert $index $entrytype] $args]
        if {[string length "$dynhelp"]} {
            DynamicHelp  add $menu -index $index \
                  -variable $options(-textvariable) \
                  -text "$dynhelp"
        }
        return $res
    }
    
    method {menu invoke} {menuid index} {
        if {![info exists menuid($_menuid)] } {return}
        set menu $menuid($_menuid)
        return [eval [list $menu invoke $index]]
    }
    
    method {menu type} {menuid index} {
        if {![info exists menuid($_menuid)] } {return}
        set menu $menuid($_menuid)
        return [eval [list $menu type $index]]
    }
    
    method setmenustate { tag state } {
        # We need a more sophisticated state system.
        # The original model was this:  each menu item has a list of tags;
        # whenever any one of those tags changed state, the menu item did too.
        # This makes it hard to have items that are enabled only when both tagA and
        # tagB are.  The new model therefore only sets the menustate to enabled
        # when ALL of its tags are enabled.
        
        # First see if this is a real tag
        if { [info exists tagstate($tag)] } {
            if { ![string equal $state "disabled"] } {
                set tagstate($tag) 1
            } else {
                set tagstate($tag) 0
            }
            foreach {menu entry} $tags($tag) {
                set expression "1"
                foreach menutag $menutags([list $menu $entry]) {
                    append expression " && $tagstate($menutag)"
                }
                if { [expr $expression] } {
                    set state normal
                } else {
                    set state disabled
                }
                $menu entryconfigure $entry -state $state
            }
        }
        return
    }
    method showtoolbar { index bool } {
        set toolframe $topframe.f$index
        if { [winfo exists $toolframe] } {
            if { !$bool && [llength [grid info $toolframe]] } {
                grid forget $toolframe
                $topframe configure -height 1
            } elseif { $bool && ![llength [grid info $toolframe]] } {
                grid $toolframe -column 0 -row $index -sticky ew
            }
        }
    }
    method showstatusbar { name } {
        if { [string equal $name "none"] } {
            pack forget $status
        } else {
            pack $status -fill x -in $botframe -fill x -pady 2
            switch -- $name {
                status {
                    catch {pack forget $progress}
                }
                progression {
                    pack $progress -in $prgframe
                }
            }
        }
    }
}

package provide MainFrame 1.0
