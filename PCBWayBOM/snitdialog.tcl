##############################################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Fri May 24 09:50:47 2013
#  Last Modified : <130527.1137>
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
package require ButtonBox
package require IconImage

snit::widgetadaptor Dialog {
    
    option -style -default Dialog
    option -title -default {} -configuremethod _configTitle
    method _configTitle {option value} {
        set options($option) $value
        wm title $win $value
    }
    option -geometry -default {} -configuremethod _configGeometry \
          -validatemethod _validateGeometry
    method _configGeometry {option value} {
        set options($option) $value
        wm geometry $win $value
    }
    method _validateGeometry {option value} {
        if {[regexp {^=?([[:digit:]]+x[[:digit:]]+)?([+-][[:digit:]]+[+-][[:digit:]]+)?$} "$value"] < 1} {
            error "Malformed value: $value for $option"
        }
    }
    option -modal -default local -type {snit::enum -values {none local global}}
    option -bitmap -default {} -readonly yes
    option -image -default {} -readonly yes
    option -separator -default no -readonly yes -type snit::boolean
    option -cancel -default {}
    option -parent -default {} -type snit::window
    option -side -default bottom -readonly yes \
          -type {snit::enum -values {bottom left top right}}
    option -anchor -default c -readonly yes \
          -type {snit::enum -values {n e w s c}}
    option -class -default Dialog -readonly yes
    option -transient -default yes -readonly yes -type snit::boolean
    option -place -default center \
          -type {snit::enum -values {none center left right above below}}
    
    component bbox
    component frame
    component sep
    component label
    typeconstructor {
        ttk::style layout Dialog {
            Dialog.bbox -side bottom -sticky we
            Dialog.frame -sticky nswe
            Dialog.sep -side bottom -sticky we
            Dialog.label -side left -sticky nw
        }
        ttk::style configure Dialog \
              -borderwidth [ttk::style lookup "." -borderwidth] \
              -background [ttk::style lookup "." -background] \
              -relief raised \
              -framerelief flat \
              -frameborderwidth 0 \
              ;
        
        
        bind Dialog <<ThemeChanged>> [mytypemethod _ThemeChanged %W]
        bind Dialog <Escape>         [mytypemethod _Escape %W]
        bind Dialog <Return>         [mytypemethod _Return %W]
    }
    typemethod _ThemeChanged {w} {
        $w _ThemeChanged
    }
    typemethod _Escape {w} {
        return [$w  _Escape]
    }
    typemethod _Return {w} {
        return [$w _Return]
    }
    method _ThemeChanged {} {
        set background [ttk::style lookup $options(-style) -background]
        #puts stderr "*** $self _ThemeChanged: background = $background"
        #puts stderr "*** $self _ThemeChanged: hull is $hull"
        $hull configure \
              -borderwidth [ttk::style lookup $options(-style) -borderwidth] \
              -background $background \
              -relief [ttk::style lookup $options(-style) -relief]
        #puts stderr "*** $self _ThemeChanged: frame is $frame"
        $frame configure \
              -borderwidth [ttk::style lookup $options(-style) \
                            -frameborderwidth] \
              -relief [ttk::style lookup $options(-style) -framerelief]
    }
    method _Escape {} {
        return [$bbox invoke $options(-cancel)]
    }
    method _Return {} {
        return [$bbox invoke default]
    }
    ##delegate method add to bbox
    delegate method itemconfigure to bbox
    delegate method itemcget to bbox
    delegate method invoke to bbox
    delegate method setfocus to bbox
    delegate option -state to bbox
    delegate option -default to bbox
    
    variable realized no
    variable nbut 0
    variable result
    variable savedfocus
    variable savedgrab
    variable savedgrabopt
    
    constructor {args} {
        #puts stderr "*** $type create $self $args"
        set options(-style) [from args -style]
        set options(-class) [from args -class]
        installhull using tk::toplevel -class $options(-class) \
              -relief [ttk::style lookup $options(-style) -relief] \
              -borderwidth [ttk::style lookup $options(-style) -borderwidth]
        wm withdraw $win
        wm overrideredirect $win 1
        set options(-title) [from args -title]
        #puts stderr "*** $type create $self: options(-title) = '$options(-title)'"
        wm title $win $options(-title)
        set options(-parent) [from args -parent [winfo parent $win]]
        set options(-transient) [from args -transient]
        if {$options(-transient)} {
            wm transient $win [winfo toplevel $options(-parent)]
        }
        set options(-side) [from args -side]
        if {[lsearch {left right} $options(-side) ] >= 0} {
            set orient vertical
        } else {
            set orient horizontal
        }
        install bbox using ButtonBox $win.bbox -orient $orient
        install frame using ttk::frame $win.frame \
              -relief [ttk::style lookup $options(-style) -framerelief] \
              -borderwidth [ttk::style lookup $options(-style) \
                            -frameborderwidth]
        set background [ttk::style lookup $options(-style) -background]
        #puts stderr "*** $type create $self: background is $background"
        #puts stderr "*** $type create $self: hull is $hull"
        $hull configure -background $background
        #puts stderr "*** $type create $self: frame is $frame"
        #$frame configure -background $background
        set options(-bitmap) [from args -bitmap]
        set options(-image) [from args -image]
        if {$options(-image) ne ""} {
            #puts stderr "*** $type create $self: -image is $options(-image)"
            install label using ttk::label $win.label \
                  -image $options(-image)
        } elseif {$options(-bitmap) ne ""} {
            #puts stderr "*** $type create $self: -bitmap is $options(-bitmap)"
            install label using ttk::label $win.label \
                  -image [IconImage image $options(-bitmap) -filetype xbm \
                          -background $background]
            #puts stderr "*** $type create $self: label is $label"
            #puts stderr "*** $type create $self: winfo exists $label is [winfo exists $label]"
        }
        set options(-separator) [from args -separator]
        if {$options(-separator)} {
            #puts stderr "*** $type create $self: -separator is true"
            install sep using ttk::separator $win.sep -orient $orient \
                  -background $background
        }
        $self configurelist $args
    }
    method getframe {} {return $frame}
    method add {name args} {
        set cmd [list $bbox add ttk::button $name \
                 -command [from args -command \
                           [mymethod enddialog $nbut]]]
        set res [eval $cmd $args]
        incr nbut
        return $res
    }
    method enddialog {res} {
        set result $res
    }
    method draw {{focus ""} {overrideredirect no} {geometry ""}} {
        #puts stderr "*** $self draw $focus $overrideredirect $geometry"
        set parent $options(-parent)
        #puts stderr "*** $self draw: parent = $parent"
        #puts stderr "*** $self draw: realized = $realized"
        if { !$realized } {
            set realized yes
            if { [llength [winfo children $bbox]] } {
                set side $options(-side)
                if {[lsearch {left right} $options(-side) ] >= 0} {
                    set pad -padx
                    set fill y
                } else {
                    set pad -pady
                    set fill x
                }
                #puts stderr "*** $self draw: packing $bbox -side $side -padx 1m -pady 1m -anchor $options(-anchor)"
                pack $bbox -side $side -padx 1m -pady 1m \
                      -anchor $options(-anchor)
                if {[info exists sep] &&
                    [winfo exists $sep]} {
                    pack $sep -side $side -fill $fill $pad 2m
                }
            }
            #catch {puts stderr "*** $self draw: label is $label"}
            #catch {puts stderr "*** $self draw: winfo exists $label is [winfo exists $label]"}
            if {[info exists label] &&
                [winfo exists $label]} {
                #puts stderr "*** $self draw: label is $label"
                pack $label -side left -anchor n -padx 3m -pady 3m
            }
            #puts stderr "*** $self draw: packing $frame -padx 1m -pady 1m -fill both -expand yes"
            pack $frame -padx 1m -pady 1m -fill both -expand yes
        }
        #puts stderr "*** $self draw: realized"
        set geom $options(-geometry)
        if {$geometry eq "" && $geom eq ""} {
            set place $options(-place)
            #puts stderr "*** $self draw: place"
            if {$place ne "none"} {
                if {[winfo exists $parent]} {
                    _place $win 0 0 $place $parent
                } else {
                    _place $win 0 0 $place
                }
            }
        } else {
            if { $geom ne "" } {
                wm geometry $win $geom
            } else {
                wm geometry $win $geometry
            }
        }
        #puts stderr "*** $self draw: placed"
        update idletasks
        wm overrideredirect $win $overrideredirect
        wm deiconify $win
        #puts stderr "*** $self draw: deiconify done"
        if {![winfo exists $parent] ||
            ([wm state [winfo toplevel $parent]] ne "withdrawn")} {
            tkwait visibility $win
        }
        #puts stderr "*** $self draw: visibility done"
        set savedfocus [focus -displayof $win]
        focus $win
        if {[winfo exists $focus]} {
            focus -force $focus
        } else {
            $bbox setfocus default
        }
        #puts stderr "*** $self draw: visibility focus done"
        if {[set grab $options(-modal)] ne "none"} {
            set savedgrab [grab current]
            if {[winfo exists $savedgrab]} {
                set savedgrabopt [grab status $savedgrab]
            }
            if {$grab eq "global"} {
                grab -global $win
            } else {
                grab $win
            }
            if {[info exists result]} {unset result}
            tkwait variable [myvar result]
            if {[info exists result]} {
                set res $result
                unset result
            } else {
                set res -1
            }
            $self withdraw
            return $res
        }
        return ""
    }
    method withdraw {} {
        if {[info exists savedfocus] && [winfo exists $savedfocus]} {
            focus $savedfocus
        }
        if {[winfo exists $win]} {grab release $win}
        if {[info exists savedgrab] && [winfo exists $savedgrab]} {
            if {$savedgrabopt eq "global"} {
                grab -global $savedgrab
            } else {
                grab $savedgrab
            }
        }
        if {[winfo exists $win]} {wm withdraw $win}
    }
    destructor {
        catch {$self enddialog -1}
        catch {focus $savedfocus}
        catch {grab release $win}
    }
    proc _place { path w h args } { 
        update idletasks
        set reqw [winfo reqwidth  $path]
        set reqh [winfo reqheight $path]
        if { $w == 0 } {set w $reqw}
        if { $h == 0 } {set h $reqh}
        set arglen [llength $args]
        if { $arglen > 3 } {
            return -code error "Dialog::_place: bad number of arguments"
        }

        if { $arglen > 0 } {
            set where [lindex $args 0]
            set list  [list "at" "center" "left" "right" "above" "below"]
            set idx   [lsearch $list $where]
            if { $idx == -1 } {
                return -code error "Dialog::place: bad position: $where"
            }
            if { $idx == 0 } {
                set err [catch {
                         # purposely removed the {} around these expressions - [PT]
                         set x [expr int([lindex $args 1])]
                         set y [expr int([lindex $args 2])]
                     }]
                if { $err } {
                    return -code error "Dialog::_place: incorrect position"
                }
                if {$::tcl_platform(platform) == "windows"} {
                    # handle windows multi-screen. -100 != +-100
                    if {[string index [lindex $args 1] 0] != "-"} {
                        set x "+$x"
                    }
                    if {[string index [lindex $args 2] 0] != "-"} {
                        set y "+$y"
                    }
                } else {
                    if { $x >= 0 } {
                        set x "+$x"
                    }
                    if { $y >= 0 } {
                        set y "+$y"
                    }
                }
            } else {
                if { $arglen == 2 } {
                    set widget [lindex $args 1]
                    if { ![winfo exists $widget] } {
                        return -code error "Dialog::_place: \"$widget\" does not exist"
                    }
                } else {
                    set widget .
                }
                set sw [winfo screenwidth  $path]
                set sh [winfo screenheight $path]
                if { $idx == 1 } {
                    if { $arglen == 2 } {
                        # center to widget
                        set x0 [expr {[winfo rootx $widget] + ([winfo width  $widget] - $w)/2}]
                        set y0 [expr {[winfo rooty $widget] + ([winfo height $widget] - $h)/2}]
                    } else {
                        # center to screen
                        set x0 [expr {([winfo screenwidth  $path] - $w)/2 - [winfo vrootx $path]}]
                        set y0 [expr {([winfo screenheight $path] - $h)/2 - [winfo vrooty $path]}]
                    }
                    set x "+$x0"
                    set y "+$y0"
                    if {$::tcl_platform(platform) != "windows"} {
                        if { $x0+$w > $sw } {set x "-0"; set x0 [expr {$sw-$w}]}
                        if { $x0 < 0 }      {set x "+0"}
                        if { $y0+$h > $sh } {set y "-0"; set y0 [expr {$sh-$h}]}
                        if { $y0 < 0 }      {set y "+0"}
                    }
                } else {
                    set x0 [winfo rootx $widget]
                    set y0 [winfo rooty $widget]
                    set x1 [expr {$x0 + [winfo width  $widget]}]
                    set y1 [expr {$y0 + [winfo height $widget]}]
                    if { $idx == 2 || $idx == 3 } {
                        set y "+$y0"
                        if {$::tcl_platform(platform) != "windows"} {
                            if { $y0+$h > $sh } {set y "-0"; set y0 [expr {$sh-$h}]}
                            if { $y0 < 0 }      {set y "+0"}
                        }
                        if { $idx == 2 } {
                            # try left, then right if out, then 0 if out
                            if { $x0 >= $w } {
                                set x [expr {$x0-$sw}]
                            } elseif { $x1+$w <= $sw } {
                                set x "+$x1"
                            } else {
                                set x "+0"
                            }
                        } else {
                            # try right, then left if out, then 0 if out
                            if { $x1+$w <= $sw } {
                                set x "+$x1"
                            } elseif { $x0 >= $w } {
                                set x [expr {$x0-$sw}]
                            } else {
                                set x "-0"
                            }
                        }
                    } else {
                        set x "+$x0"
                        if {$::tcl_platform(platform) != "windows"} {
                            if { $x0+$w > $sw } {set x "-0"; set x0 [expr {$sw-$w}]}
                            if { $x0 < 0 }      {set x "+0"}
                        }
                        if { $idx == 4 } {
                            # try top, then bottom, then 0
                            if { $h <= $y0 } {
                                set y [expr {$y0-$sh}]
                            } elseif { $y1+$h <= $sh } {
                                set y "+$y1"
                            } else {
                                set y "+0"
                            }
                        } else {
                            # try bottom, then top, then 0
                            if { $y1+$h <= $sh } {
                                set y "+$y1"
                            } elseif { $h <= $y0 } {
                                set y [expr {$y0-$sh}]
                            } else {
                                set y "-0"
                            }
                        }
                    }
                }
            }
            
            ## If there's not a + or - in front of the number, we need to add one.
            if {[string is integer [string index $x 0]]} { set x +$x }
            if {[string is integer [string index $y 0]]} { set y +$y }

            wm geometry $path "${w}x${h}${x}${y}"
        } else {
            wm geometry $path "${w}x${h}"
        }
        update idletasks
    }
}

package provide Dialog 1.0
