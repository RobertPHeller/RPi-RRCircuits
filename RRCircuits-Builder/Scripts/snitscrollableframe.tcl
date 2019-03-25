##############################################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Thu Jul 11 10:02:32 2013
#  Last Modified : <160223.1307>
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


snit::widgetadaptor ScrollableFrame {
    component uframe
    delegate option -xscrollcommand to hull
    delegate option -yscrollcommand to hull
    delegate option -xscrollincrement to hull
    delegate option -yscrollincrement to hull
    delegate option -height to hull
    delegate option -width to hull
    delegate method xview to hull
    delegate method yview to hull
    option -areawidth -type {snit::integer -min 0} -default 0 \
          -configuremethod _configure_sizes
    option -areaheight -type {snit::integer -min 0} -default 0 \
          -configuremethod _configure_sizes
    option -constrainedwidth -type snit::boolean -default no \
          -configuremethod _configure_sizes
    option -constrainedheight -type snit::boolean -default no \
          -configuremethod _configure_sizes
    option -style -default ScrollableFrame -configuremethod _configure_style
    method _configure_style {o v} {
        set options($o) $v
        catch {$uframe configure -style $v}
    }
    typeconstructor {
        ttk::style layout $type [ttk::style layout TFrame]
        ttk::style configure $type -highlightthickness 0 -borderwidth 0 \
              -relief flat
        bind $type <Configure> [mytypemethod _resize %W %w %h]
        bind $type <<ThemeChanged>> [mytypemethod _themeChanged %W]
    }
    typemethod _resize {w width height} {
        $w _resize_ $width $height
    }
    typemethod _themeChanged {w} {
        $w _themeChanged_
    }
    constructor {args} {
        installhull using canvas
        install uframe using ttk::frame $win.uframe
        set options(-areawidth) [from args -areawidth]
        set options(-areaheight) [from args -areaheight]
        $hull create window 0 0 -anchor nw -window $uframe -tags win \
              -width $options(-areawidth) -height $options(-areaheight)
        $self configurelist $args
        $self _themeChanged_
        bind $uframe <Configure> [mymethod _frameConfigure %w %h]
        bindtags $win [list $win $type [winfo toplevel $win] all]
    }
    method _themeChanged_ {} {
        $hull configure \
              -highlightthickness [ttk::style lookup $options(-style) \
                                   -highlightthickness {} 0] \
              -borderwidth [ttk::style lookup $options(-style) \
                             -borderwidth {} 0] \
              -relief [ttk::style lookup $options(-style) \
                       -relief {} flat]
    }
    method _configure_sizes {o v} {
        #puts stderr "*** $self _configure_sizes $o $v"        
        set options($o) $v
        set cw $options(-constrainedwidth)
        set modcw no
        set w  $options(-areawidth)
        set modw no
        set ch $options(-constrainedheight)
        set modch no
        set h  $options(-areaheight)
        set modh no
        switch $o {
            -constrainedwidth {set modcw yes}
            -areawidth {set modw yes}
            -constrainedheight {set modch yes}
            -areaheight {set modh yes}
        }
        set upd 0
        if { $modcw || (!$cw && $modw) } {
            if { $cw } {
                set w [winfo width $win]
            }
            set upd 1
        }
        if { $modch || (!$ch && $modh) } {
            if { $ch } {
                set h [winfo height $win]
            }
            set upd 1
        }
        #puts stderr "*** $self _configure_sizes: upd = $upd"
        if { $upd } {
            $hull itemconfigure win -width $w -height $h
        }
        return $v
    }
    method getframe {} {return $uframe}
    method see { widget {vert top} {horz left} {xOffset 0} {yOffset 0}} {
        set x0  [winfo x $widget]
        set y0  [winfo y $widget]
        set x1  [expr {$x0+[winfo width  $widget]}]
        set y1  [expr {$y0+[winfo height $widget]}]
        set xb0 [$hull canvasx 0]
        set yb0 [$hull canvasy 0]
        set xb1 [$hull canvasx [winfo width  $path]]
        set yb1 [$hull canvasy [winfo height $path]]
        set dx  0
        set dy  0
        
        if { [string equal $horz "left"] } {
            if { $x1 > $xb1 } {
                set dx [expr {$x1-$xb1}]
            }
            if { $x0 < $xb0+$dx } {
                set dx [expr {$x0-$xb0}]
            }
        } elseif { [string equal $horz "right"] } {
            if { $x0 < $xb0 } {
                set dx [expr {$x0-$xb0}]
            }
            if { $x1 > $xb1+$dx } {
                set dx [expr {$x1-$xb1}]
            }
        }

        if { [string equal $vert "top"] } {
            if { $y1 > $yb1 } {
                set dy [expr {$y1-$yb1}]
            }
            if { $y0 < $yb0+$dy } {
                set dy [expr {$y0-$yb0}]
            }
        } elseif { [string equal $vert "bottom"] } {
            if { $y0 < $yb0 } {
                set dy [expr {$y0-$yb0}]
            }
            if { $y1 > $yb1+$dy } {
                set dy [expr {$y1-$yb1}]
            }
        }

        if {($dx + $xOffset) != 0} {
            set x [expr {($xb0+$dx+$xOffset)/[winfo width $uframe]}]
            $hull xview moveto $x
        }
        if {($dy + $yOffset) != 0} {
            set y [expr {($yb0+$dy+$yOffset)/[winfo height $uframe]}]
            $hull yview moveto $y
        }
    }
    method _resize_ {width height} {
        #puts stderr "*** $self _resize_ $width $height"
        if {$options(-constrainedwidth)} {
            #puts stderr "*** $self _resize_: updating  win -width to [winfo width $win]"
            $hull itemconfigure win -width [winfo width $win]
        }
        if {$options(-constrainedheight)} {
            #puts stderr "*** $self _resize_: updating  win -height to [winfo height $win]"
            $hull itemconfigure win -height [winfo height $win]
        }
        #puts stderr "*** $self _resize_: win size is [$hull itemcget win -width] [$hull itemcget win -height]"
    }
    method _frameConfigure {width height} {
        #puts stderr "*** $self _frameConfigure $width $height"
        #puts stderr "*** $self _frameConfigure: uframe is [winfo width $uframe] [winfo height $uframe]"
        #puts stderr "*** $self _frameConfigure: win is [winfo width $win] [winfo height $win]"
        if {[winfo height $uframe] < [winfo height $win]} {
            set height [winfo height $win]
        }
        if {[winfo width $uframe] < [winfo width $win]} {
            set width [winfo width $win]
        }
        #puts stderr "*** $self _frameConfigure: scrollregion is [list 0 0 $width $height]"
        $hull configure -scrollregion [list 0 0 $width $height]
    }
}

package provide ScrollableFrame 1.0
