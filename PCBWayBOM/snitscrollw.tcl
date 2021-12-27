##############################################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon May 13 20:08:01 2013
#  Last Modified : <130523.1000>
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
package require tile
package require snit

snit::widget ScrolledWindow {
    widgetclass ScrolledWindow
    hulltype ttk::frame
    
    option -scrollbar \
          -type {snit::enum -values {none both vertical horizontal}} \
          -default both \
          -configuremethod _configScrolling
    option -auto \
          -type {snit::enum -values {none both vertical horizontal}} \
          -default both \
          -configuremethod _configScrolling
    option -sides \
          -type {snit::enum -values {ne en nw wn se es sw ws}} \
          -default se \
          -configuremethod _configScrolling
    option -size -type {snit::integer -min 0} -default 0 -readonly yes
    option -ipad -type {snit::integer -min 0} -default 1 -readonly yes
    option -managed -type snit::boolean -default yes -readonly yes
    delegate option * to hull except {-class -style}
    
    component hscroll
    component vscroll
    
    variable realized no
    variable hsb -array {}
    variable vsb -array {}
    variable widget {}
    variable changed 0
    variable hlock
    variable vlock
    
    constructor {args} {
        install hscroll using ttk::scrollbar $win.hscroll \
              -takefocus 0 -orient horizontal
        install vscroll using ttk::scrollbar $win.vscroll \
              -takefocus 0 -orient vertical
        $self configurelist $args
        
        if {$options(-managed)} {
            set hsb(packed) $hsb(present)
            set vsb(packed) $vsb(present)
        } else {
            set hsb(packed) no
            set vsb(packed) no
        }
        if {$hsb(packed)} {
            grid $hscroll -column 1 -row $hsb(row) \
                  -sticky ew -ipady $options(-ipad)
        }
        if {$vsb(packed)} {
            grid $vscroll -column $vsb(column) -row 1 \
                  -sticky ns -ipadx $options(-ipad)
        }
        
        grid columnconfigure $win 1 -weight 1
        grid rowconfigure    $win 1 -weight 1
        
        bind $win <Configure> [mymethod _realize]
    }
    method getframe {} {return $win}
    method setwidget {wid} {
        if {[info exists $widget] && [winfo exist $widget] &&
            $wid ne $widget} {
            grid remove $widget
            $widget configure -xscrollcommand "" -yscrollcommand ""
        }
        set widget $wid
        grid $widget -in $win -row 1 -column 1 -sticky news
        $hscroll configure -command [list $widget xview]
        $vscroll configure -command [list $widget yview]
        $widget configure \
              -xscrollcommand [mymethod _set_hscroll] \
              -yscrollcommand [mymethod _set_vscroll]
    }
    method _configScrolling {option value} {
        if {[info exists options($option)] && $options($option) ne $value} {
            incr changed
        }
        set options($option) $value
        if {[info exists options(-scrollbar)] &&
            [info exists options(-auto)] &&
            [info exists options(-sides)] &&
            [info exists options(-ipad)]} {
            $self _setdata $options(-scrollbar) $options(-auto) $options(-sides)
            if {$changed > 0} {
                foreach {vmin vmax} [$hscroll get] {break}
                set hsb(packed) \
                      [expr {$hsb(present) && (!$hsb(auto) || ($vmin != 0 || $vmax != 1))}]
                foreach {vmin vmax} [$vscroll get] {break}
                set vsb(packed) \
                      [expr {$vsb(present) && (!$vsb(auto) || ($vmin != 0 || $vmax != 1))}]
                if {$hsb(packed)} {
                    grid $hscroll -column 1 -row $hsb(row) \
                          -sticky ew -ipady $options(-ipad)
                } else {
                    if {![info exists hlock]} {
                        set hsb(packed) 0
                        grid remove $hscroll
                    }
                }
                if {$vsb(packed)} {
                    grid $vscroll -column $vsb(column) -row 1 \
                          -sticky ns -ipadx $options(-ipad)
                } else {
                    if {![info exists vlock]} {
                        set vsb(packed) 0
                        grid remove $vscroll
                    }
                }
                set changed 0
            }
        }
    }
    method _set_hscroll {vmin vmax} {
        if {$realized && $hsb(present)} {
            if {$hsb(auto)} {
                if {$hsb(packed) && $vmin == 0 && $vmax == 1} {
                    if {![info exists hlock]} {
                        set hsb(packed) 0
                        grid remove $hscroll
                    }
                } elseif {!$hsb(packed) && ($vmin != 0 || $vmax != 1)} {
                    set hsb(packed) 1
                    grid $hscroll -column 1 -row $hsb(row) \
                          -sticky ew -ipady $options(-ipad)
                    set hlock 1
                    update idletasks
                    unset hlock
                }
            }
            $hscroll set $vmin $vmax
        }
    }
    method _set_vscroll {vmin vmax} {
        if {$realized && $vsb(present)} {
            if {$vsb(auto)} {
                if {$vsb(packed) && $vmin == 0 && $vmax == 1} {
                    if {![info exists vlock]} {
                        set vsb(packed) 0
                        grid remove $vscroll
                    }
                } elseif {!$vsb(packed) && ($vmin != 0 || $vmax != 1)} {
                    set vsb(packed) 1
                    grid $vscroll -column $vsb(column) -row 1 \
                          -sticky ns -ipadx $options(-ipad)
                    set vlock 1
                    update idletasks
                    unset vlock
                }
            }
            $vscroll set $vmin $vmax
        }
    }
    method _setdata {scrollbar auto sides} {
        set sb    [lsearch {none horizontal vertical both} $scrollbar]
        set auto  [lsearch {none horizontal vertical both} $auto]
        
        set hsb(present)  [expr {($sb & 1) != 0}]
        set hsb(auto)	  [expr {($auto & 1) != 0}]
        set hsb(row)	  [expr {[string match *n* $sides] ? 0 : 2}]

        set vsb(present)  [expr {($sb & 2) != 0}]
        set vsb(auto)	  [expr {($auto & 2) != 0}]
        set vsb(column)   [expr {[string match *w* $sides] ? 0 : 2}]
    }
    method _realize {} {
        bind $win <Configure> {}
        set realized yes
    }
}

package provide ScrollWindow 1.0
