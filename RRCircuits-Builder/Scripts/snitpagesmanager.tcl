##############################################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Wed Feb 26 10:05:02 2014
#  Last Modified : <140228.1447>
#
#  Description	
#
#  Notes
#
#  History
#	
##############################################################################
#
#  Copyright (c) 2014 Deepwoods Software.
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

snit::widgetadaptor PagesManager {
    #widgetclass PagesManager
    delegate option -xscrollcommand to hull
    delegate option -yscrollcommand to hull
    delegate option -xscrollincrement to hull
    delegate option -yscrollincrement to hull
    delegate option -height to hull
    delegate option -width to hull
    delegate option -background to hull
    delegate option -cursor to hull
    delegate method xview to hull
    delegate method yview to hull
    option -style -default PagesManager
    typeconstructor {
        ttk::style configure $type -background ""
        bind $type <<ThemeChanged>> [mytypemethod _themeChanged %W]
    }
    typemethod _themeChanged {w} {
        $w _themeChanged_
    }
    variable select   ""
    variable pages    {}
    variable cpt      0
    variable realized 0
    constructor {args} {
        installhull using canvas
        bind $win <Configure> [mymethod _realize]
    }
    method _themeChanged_ {} {
        $hull configure -background [ttk::style lookup $options(-style) \
                                     -background {} ""]
    }
    method compute_size {} {
        #puts stderr "*** $self compute_size"
        set wmax 0
        set hmax 0
        update idletasks
        foreach page $pages {
            set w    [winfo reqwidth  $win.f$page]
            set h    [winfo reqheight $win.f$page]
            set wmax [expr {$w>$wmax ? $w : $wmax}]
            set hmax [expr {$h>$hmax ? $h : $hmax}]
        }
        #puts stderr "*** $self compute_size: wmax = $wmax, hmax = $hmax"
        $hull configure -width $wmax -height $hmax
    }
    method add {page} {
        if { [lsearch -exact $pages $page] != -1 } {
            return -code error "page \"$page\" already exists"
        }
        lappend pages $page
        ttk::frame $win.f$page -relief flat -borderwidth 0
        return $win.f$page
    }
    method delete {page} {
        set pos [$self _test_page $page]
        set pages [lreplace $pages $pos $pos]
        if { $select == $page } {
            set select ""
        }
        destroy $win.f$page
        $self _redraw
    }
    method raise {{page ""}} {
        #puts stderr "*** $self raise $page"
        if { $page != "" } {
            $self _test_page $page
            $self _select $page
        }
        return $select
    }
    method pages { {first ""} {last ""} } {
        if { ![string length $first] } {
            return $pages
        }
        if { ![string length $last] } {
            return [lindex $pages $first]
        } else {
            return [lrange $pages $first $last]
        }
    }
    method getframe { page } {
        set pos [$self _test_page $page]
        return $win.f$page
    }
    method _test_page { page } {
        #puts stderr "*** $self _test_page $page"
        if { [set pos [lsearch $pages $page]] == -1 } {
            return -code error "page \"$page\" does not exists"
        }
        return $pos
    }
    method _select { page } {
        #puts stderr "*** $self _select $page"
        set oldsel $select
        if { $page != $oldsel } {
            set select $page
            $self _draw_area
        }
    }
    method _redraw {} {
        if { !$realized } {
            return
        }
        $self _draw_area
    }
    method _draw_area {} {
        #puts stderr "*** $self _draw_area"
        set w   [winfo width  $win]
        set h   [winfo height $win]
        #puts stderr "*** $self _draw_area: w = $w, h = $h"
        set sel $select
        #puts stderr "*** $self _draw_area: sel = $sel"
        if { $sel != "" } {
            if { [llength [$hull find withtag window]] } {
                #puts stderr "*** $self _draw_area: \[$hull find withtag window\] = [$hull find withtag window]"
                $hull coords window 0 0
                $hull itemconfigure window    \
                      -width  $w \
                      -height $h \
                      -window $win.f$sel
            } else {
                $hull create window 0 0 \
                      -width  $w \
                      -height $h \
                      -tags   window \
                      -anchor nw \
                      -window $win.f$sel
            }
        } else {
            $hull delete window
        }
    }
    method _realize {} {
        #puts stderr "*** $self _realize"
        if {[set width  [$hull cget -width]]  == 0 ||
            [set height [$hull cget -height]] == 0 } {
            $self compute_size
        }
        set realized 1
        $self _draw_area
        bind $win <Configure> [mymethod _draw_area]
    }
}

package provide PagesManager 1.0
