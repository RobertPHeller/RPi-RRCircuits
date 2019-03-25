#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Sat Feb 27 15:54:05 2016
#  Last Modified : <161015.1159>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2016  Robert Heller D/B/A Deepwoods Software
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

package require gettext
package require Tk
package require tile
package require snit

## @addtogroup TclCommon
# @{


snit::widget ScrollTabNotebook {
    ## @brief Tabbed Notebook with scrollable tabs.
    # This is a Tabbed Notebook widget, with scrollable tabs.  It implements
    # left and right arrows, as needed, to shift the tab row to the left or
    # right to allow for more tabs than will fit in the available space.
    #
    # Options:
    # @arg -style The style to use. The default is ScrollTabNotebook.
    # @arg -width The width in pixels.
    # @arg -height The height in pixels.
    # @arg -cursor The cursor to use.
    # @arg -takefocus Can the widget take focus?
    # @par
    
    widgetclass ScrollTabNotebook
    hulltype ttk::frame
    
    option -style -default ScrollTabNotebook
    delegate option -width to hull
    delegate option -height to hull
    delegate option -cursor to hull
    delegate option -takefocus to hull
    
    component tabrow
    ## @privatesection Row containing the tabs.        
    component left
    ## Left arrow button.
    component tabs
    ## Scrolling tab frame (canvas).
    component right
    ## Right arrow button.

    variable select
    ## The currently selected page.
    variable _clientRow 1
    ## Grid row for client pages (1 is bottom).
    variable _tabrow 0
    ## Grid row for tabs (0 is top).
    variable _hpage 0
    ## Height of tabrow.
    variable _wpage 0
    ## Width of tabrow.
    variable _textid
    ## Scratch text id.
    variable realized 0
    ## Flag to indicate if the widget is realized.
    variable pages {}
    ## The list of available pages.
    variable pages_opts -array {}
    ## The options for the available pages.
    variable base 0
    ## Leftmost visible tab.
    variable dbg
    ## Darker tab background.
    variable lbg
    ## Lighter tab background.
    
    typevariable _tabsides
    ## Tabside type checker.
    typevariable _radiustype
    ## Tab radius and bevelsize type checker.
    typevariable _paddingtype
    ## Tab padding type checker.
    typevariable _warrow 12
    ## Width of an arrow button.
    
    typevariable _left {
#define left_width 16
#define left_height 16
static unsigned char left_bits[] = {
   0x00, 0x70, 0x00, 0x7e, 0x00, 0x7e, 0x80, 0x7f, 0xf0, 0x7f, 0xfe, 0x7f,
   0xfe, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xf0, 0x7f,
   0x80, 0x7f, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x70};
    }
    ## Bitmap for the left button.
    typevariable _right {
#define right_width 16
#define right_height 16
static unsigned char right_bits[] = {
   0x08, 0x00, 0x78, 0x00, 0x78, 0x00, 0xf8, 0x01, 0xf8, 0x0f, 0xf8, 0x7f,
   0xf8, 0x7f, 0xf8, 0xff, 0xf8, 0xff, 0xf8, 0x7f, 0xf8, 0x7f, 0xf8, 0x0f,
   0xf8, 0x01, 0x78, 0x00, 0x78, 0x00, 0x08, 0x00};
    }
    ## Bitmap for the right button.
    typeconstructor {
        ttk::style layout ScrollTabNotebook [ttk::style layout TNotebook]
        # Notebook.client -sticky nswe
        ttk::style layout ScrollTabNotebook.Tab [ttk::style layout \
                                                 TNotebook.Tab]
        # Notebook.tab 
        #      -sticky nswe 
        #      -children {
        #           Notebook.padding 
        #               -side top 
        #               -sticky nswe 
        #               -children {
        #                   Notebook.focus 
        #                       -side top 
        #                       -sticky nswe 
        #                       -children {
        #                            Notebook.label 
        #                                -side top 
        #                                -sticky {}}}}
        
        ttk::style configure ScrollTabNotebook \
              -background [ttk::style lookup TNotebook -background]
        ttk::style configure ScrollTabNotebook.Tab \
              -background [ttk::style lookup TNotebook.Tab -background]
        #ttk::style map ScrollTabNotebook.Tab -background \
        #      [list selected [ttk::style lookup TNotebook.Tab \
        #                      -background selected]]
        ttk::style configure ScrollTabNotebook.Tab \
              -arcradius 2 \
              -bevelsize 0 \
              -pady {0 6} \
              -borderwidth [ttk::style lookup TNotebook.Tab -borderwidth]
        
        ttk::style configure ScrollTabNotebook.leftarrow \
              -relief flat -padding {0 0} -shiftrelief 0 \
              -background [ttk::style lookup TNotebook.Tab -background]
        ttk::style configure ScrollTabNotebook.rightarrow \
              -relief flat -padding {0 0} -shiftrelief 0 \
              -background [ttk::style lookup TNotebook.Tab -background]
        ttk::style layout ScrollTabNotebook.leftarrow [ttk::style layout TButton]
        ttk::style layout ScrollTabNotebook.rightarrow [ttk::style layout TButton]
        #puts stderr "*** $type typeconstructor: [ttk::style configure ScrollTabNotebook.leftarrow]"
        #puts stderr "*** $type typeconstructor: [ttk::style configure ScrollTabNotebook.rightarrow]"
        
        bind ScrollTabNotebook <<ThemeChanged>> [mytypemethod _themeChanged %W]
        bind ScrollTabNotebook <Configure> [mytypemethod _Configure %W %w %h]
    
        set _tabsides [snit::enum %AUTO% -values {top bottom}]
        set _radiustype [snit::integer %AUTO% -min 0 -max 8]
        set _paddingtype [snit::listtype %AUTO% -minlen 2 -maxlen 2 -type [snit::integer %AUTO% -min 0]]
        
    }



    constructor {args} {
        ## @publicsection @brief Constructor: create a ScrollTabNotebook.
        #
        # @param name Pathname of the widget.
        # @param ... Options:
        # @arg -style Widget style.
        # @arg -width The width of the widget.
        # @arg -height The height of the widget.
        # @arg -cursor The cursor to use.
        # @arg -takefocus Can the widget take focus?
        # @par
        
        set options(-style) [from args -style]
        set tablayout [ttk::style layout ${options(-style)}.Tab]
        set tabchildren [lrange [from tablayout -children {}] 1 end]
        set tabposition [from tabchildren -side top]
        $_tabsides validate $tabposition
        #puts stderr "*** $type create $self: _tabsides is $_tabsides"
        if {$tabposition eq {top}} {
            set _clientRow 1
            set _tabrow 0
        } else {
            set _clientRow 0
            set _tabrow 1
        }
        #puts stderr "*** $type create $self: _clientRow = $_clientRow, _tabrow = $_tabrow"
        $self _compute_height
        
        install tabrow using frame $win.tabrow -borderwidth 0
        grid $tabrow -row $_tabrow -column 0 -sticky news
        install left using ttk::button $tabrow.leftarrow \
              -image [image create bitmap -data $_left] \
              -command [mymethod _xview -1] \
              -style ${options(-style)}.leftarrow
        grid columnconfigure $tabrow 0 -weight 0
        install tabs using canvas $tabrow.tabs \
              -background [ttk::style lookup $options(-style) -background] \
              -height [expr {$_hpage + 4}] -borderwidth 0
        grid $tabs -row 0 -column 1 -sticky news
        grid columnconfigure $tabrow 1 -weight 1
        install right using ttk::button $tabrow.rightarrow \
              -image [image create bitmap -data $_right] \
              -command [mymethod _xview 1] \
              -style ${options(-style)}.rightarrow
        grid columnconfigure $tabrow 2 -weight 0
        grid columnconfigure $win 0 -weight 1
        set select {}
        $self configurelist $args
        foreach {dbg lbg} [get3dcolor $win [ttk::style lookup ${options(-style)}.Tab -background]] {break}
        #puts stderr "*** $type create $self: arrow button heights: [winfo reqheight $left] [winfo reqheight $right]"
        #puts stderr "*** $type create $self: tabs canvas height: [winfo reqheight $tabs]"
        
    }
    method compute_size {} {
        ## (Re-)compute the size of the widget.
        #
        
        set wmax 0
        set hmax 0
        update idletasks
        foreach page $pages {
            set w [winfo reqwidth $page]
            set h [winfo reqheight $page]
            set wmax [expr {($w > $wmax)?$w:$wmax}]
            set hmax [expr {($h > $hmax)?$h:$hmax}]
        }
        $win configure -width $wmax -height $hmax
        $tabs configure -width [expr {$wmax - $_warrow}]
        $self _draw_arrows
        $self _redraw
    }
    method add {window args} {
        ## @brief Add a window to the end of the page list.
        # Adds a new window (page) to the list of managed pages.
        #
        # @param window The window to add.
        # @param ... Tab options:
        # @arg -state The state of the  tab (NOT IMPLEMENTED - state is always 
        #             normal).
        # @arg -sticky The stickyness (as in grid configure ... -sticky).
        # @arg -padding  The padding (as in grid configure ... -padx and -pady).
        # @arg -text  The text of the tab.
        # @arg -image  The image of the tab.
        # @arg -compound The compound of the tab (see the -compound option of
        #                labels and buttons).
        # @arg -underline The underline of the tab label (NOT IMPLEMENTED,
        #                the -underline option is ignored).
        # @par
        
        return [eval [list $self insert end $window] $args]
    }
    
    method insert {pos window args} {
        ## @brief Insert a window at the specified position.
        # Inserts a new window (page) to the list of managed pages at the 
        # specified position.
        #
        # @param pos The insert position.  
        # @param window The window to insert.
        # @param ... Tab options:
        # @arg -state The state of the  tab (NOT IMPLEMENTED - state is always 
        #             normal).
        # @arg -sticky The stickyness (as in grid configure ... -sticky).
        # @arg -padding  The padding (as in grid configure ... -padx and -pady).
        # @arg -text  The text of the tab.
        # @arg -image  The image of the tab.
        # @arg -compound The compound of the tab (see the -compound option of
        #                labels and buttons).
        # @arg -underline The underline of the tab label (NOT IMPLEMENTED,
        #                the -underline option is ignored).
        # @par
        
        
        
        #puts stderr "*** $self insert $pos $window $args"
        set index [$self index $pos]
        if {$index == [llength $pages]} {set index end}
        set pindex [lsearch -exact $pages $window]
        if {$pindex < 0} {
            set pages [linsert $pages $index $window]
            set pages_opts($window,-state) [from args -state normal]
            set pages_opts($window,-sticky) [from args -sticky [ttk::style lookup ${options(-style)}.Tab -sticky]]
            set pages_opts($window,-padding) [from args -padding {0 0}]
            set pages_opts($window,-text) [from args -text {}]
            set pages_opts($window,-image) [from args -image {}]
            set pages_opts($window,-compound) [from args -compound center]
            set pages_opts($window,-underline) [from args -underline {}]
            set pages_opts($window,-width) 0
        } else {
            set pages [linsert [lreplace $pages $pindex $pindex] $index $window]
            set pages_opts($window,-state) [from args -state $pages_opts($window,-state)]
            set pages_opts($window,-sticky) [from args -sticky $pages_opts($window,-sticky)]
            set pages_opts($window,-padding) [from args -padding $pages_opts($window,-padding)]
            set pages_opts($window,-text) [from args -text $pages_opts($window,-text)]
            set pages_opts($window,-image) [from args -image $pages_opts($window,-image)]
            set pages_opts($window,-compound) [from args -compound $pages_opts($window,-compound)]
            set pages_opts($window,-underline) [from args -underline $pages_opts($window,-underline)]
        }
        
        $self _compute_height
        $self _compute_width
        $self _draw_page $window 1
        if {[llength $pages] == 1} {$self _select $window}
        $self _redraw
    }
    method tab {tabid args} {
        ## Updates the tab options for tabid.
        #
        # @param tabid The tab index.
        # @param ... Tab options:
        # @arg -state The state of the  tab (NOT IMPLEMENTED - state is always 
        #             normal).
        # @arg -sticky The stickyness (as in grid configure ... -sticky).
        # @arg -padding  The padding (as in grid configure ... -padx and -pady).
        # @arg -text  The text of the tab.
        # @arg -image  The image of the tab.
        # @arg -compound The compound of the tab (see the -compound option of
        #                labels and buttons).
        # @arg -underline The underline of the tab label (NOT IMPLEMENTED,
        #                the -underline option is ignored).
        # @par
        
        
        set index [$self index $tabid]
        if {$index >= [llength $pages]} {
            set window [lindex $pages end]
        } else {
            set window [lindex $pages $index]
        }
        #puts stderr "*** $self tab: window is $window, args is \{$args\}"
        #puts stderr "*** $self tab: pages_opts($window,*) is [array get pages_opts($window,*)]"
        switch [llength $args] {
            0 {
                set result [list]
                foreach o {-state -sticky -padding -text -image -compound -underline} {
                    lappend $o $pages_opts($window,$o)
                }
                return $result
            }
            1 {
                set o [lindex $args 0]
                if {[info exists pages_opts($window,$o)]} {
                    return $pages_opts($window,$o)
                } else {
                    error [_ "No such tab option: %s" $o]
                }
            }
            default {
                set pages_opts($window,-state) [from args -state $pages_opts($window,-state)]
                set pages_opts($window,-sticky) [from args -sticky $pages_opts($window,-sticky)]
                set pages_opts($window,-padding) [from args -padding $pages_opts($window,-padding)]
                set pages_opts($window,-text) [from args -text $pages_opts($window,-text)]
                set pages_opts($window,-image) [from args -image $pages_opts($window,-image)]
                set pages_opts($window,-compound) [from args -compound $pages_opts($window,-compound)]
                set pages_opts($window,-underline) [from args -underline $pages_opts($window,-underline)]
                $self _compute_height
                $self _compute_width
                $self _draw_page $window 1
                $self _redraw
            }
        }
    }
    
    method tabs {} {
        ## Return all tabs
        # @return All managed windows
        
        #puts stderr "*** $self tabs: pages is \{$pages\}"
        return $pages
    }
        
    method forget {tabid} {
        ## Removes the tab specified by tabid,  unmaps  and  unmanages  the
        # associated window.
        #
        # @param tabid The tab to remove.
        #
        
        set index [$self index $tabid]
        if {$tabid eq {end}} {
            set index end
        }
        if {$index < 0} {return}
        set newselect [lsearch $pages $select]
        $tabs delete p:[lindex $pages $index]
        set pages [lreplace $pages $index $index]
        $self _compute_width
        if {[lsearch $pages $select] < 0} {
            catch {grid forget $select}
            set select {}
            #if {$newselect >= [llength $pages]} {incr newselect -1}
            #if {[llength $pages] > 0} {$self select $newselect}
        }
        if {$index < $base} {
            incr base -1
        }
        $self _redraw
    }
    method select {{tabid {}}} {
        ## @brief Selects the specified tab.
        # The associated slave window will  be
        # displayed,  and the previously-selected window (if different) is
        # unmapped.  If tabid is omitted, returns the widget name  of  the
        # currently selected pane.
        #
        # @param tabid The tab to select.
        # @return If tabid is ommited, return the currently selected pane.
        
        if {$tabid eq {}} {
            return $select
        }
        set index [$self index $tabid]
        if {$tabid eq {end}} {
            set index end
        }
        set newpage [lindex $pages $index]
        if {$newpage eq $select} {
            return $select
        } else {
            $self _select $newpage
            return $select
        }
    }
    method see {tabid} {
        ## Make the specified tabid visible.
        #
        # @param tabid The tabid to make visible.
        #
        set index [$self index $tabid]
        if {$tabid eq {end}} {
            set index end
        }
        if {$index < $base} {
            set base $index
            $self _redraw
        } else {
            set page [lindex $pages $index]
            set w [expr {[winfo $win width] - 1}]
            set fpage [expr {[$self _get_x_page $index] + $pages_opts($page,-width) + 6}]
            set idx $base
            while { $idx < $index && $fpage > $w } {
                set fpage [expr {$fpage - $pages_opts([lindex $pages $idx],-width)}]
                incr idx
            }
            if { $idx != $base } {
                set base $idx
                $self _redraw
            }
        }
    }
    method index {tabid} {
        ## Returns  the numeric index of the tab specified by tabid, or the 
        # total number of tabs if tabid is the string "end".
        #
        # @param tabid The tabid to get the index of.
        # @return The numeric index of the tab specified by tabid.
        
        if {[string is integer -strict $tabid]} {
            if {$tabid >= 0 && $tabid < [llength $pages]} {
                return $tabid
            } else {
                error [_ "Out of range: %d" $tabid]
            }
        } elseif {$tabid eq {end}} {
            return [llength $pages]
        } elseif {$tabid eq {current}} {
            set index [lsearch $pages $select]
            return $index
        } elseif {[string index $tabid 0] eq {.}} {
            set pindex [lsearch $pages $tabid]
            if {$pindex >= 0} {
                return $pindex
            } else {
                error [_ "Not a managed window: %s" $tabid]
            }
        } elseif {[string index $tabid 0] eq {@}} {
            if {[regexp {^@([[:digit:.-]]),([[:digit:.-]])$} $tabid => x y] > 0} {
                set matches [$tabs find closest $x $y]
                foreach m $matches {
                    set tags [$tabs itemcget $m -tags]
                    foreach t $tags {
                        set pindex [lsearch $pages $t]
                        if {$pindex >= 0} {
                            return $pindex
                        }
                    }
                }
                error [_ "Cannot find tab at %f,%f" $x $y]
            } else {
                error [_ "Syntax error: %s" $tabid]
            }
        } else {
            error [_ "Syntax error: %s" $tabid]
        }
    }
    method _test_page {page} {
        ## @privatesection Test to see if page is managed.
        #
        # @param page The page to test.
        # @return The position of the page.
        
        if { [set pos [lsearch -exact $pages $page]] < 0} {
            error [_ "Page %s does not exist" $page]
        }
        return $pos
    }
        
    method _compute_width {} {
        ## @brief Recompute tab width.
        
        set wmax 0
        set wtot 0
        set hmax $_hpage
        set font [ttk::style lookup ${options(-style)}.Tab -font]
        if {![info exists _textid]} {
            set _textid [$tabs create text 0 -100 -font $font -anchor nw]
        }
        set id $_textid
        $tabs itemconfigure $id -font $font
        foreach page $pages {
            $tabs itemconfigure $id -text $pages_opts($page,-text)
            foreach {x1 y1 x2 y2} [$tabs bbox $id] {break}
            set x2 [expr {$x2 - 6}]
            set wtext [expr {$x2 - $x1 + 20}]
            set htext [expr {$y2 - $y1}]
            if {[set img $pages_opts($page,-image)] ne ""} {
                set wimg [image width $img]
                set himg [image height $img]
            } else {
                set wimg 0
                set himg 0
            }
            set compound $pages_opts($page,-compound)
            if {$compound eq "none"} {
                if {$pages_opts($page,-image) ne {}} {
                    set compound image
                } else {
                    set compound text
                }
            } elseif {$pages_opts($page,-image) eq {}} {
                set compound text
            } elseif {$pages_opts($page,-text) eq {}} {
                set compound image
            }
            switch $compound {
                text {
                    set wtab $wtext
                    set htab $htext
                }
                image {
                    set wtab $wimg
                    set htab $himg
                }
                top -
                bottom {
                    if {$wtext > $wimg} {
                        set wtab $wtext
                    } else {
                        set wtab $wimg
                    }
                    set htab [expr {$htext + $himg + 4}]
                }
                center {
                    if {$wtext > $wimg} {
                        set wtab $wtext
                    } else {
                        set wtab $wimg
                    }
                    if {$htext > $himg} {
                        set htab $htext
                    } else {
                        set htab $himg
                    }
                }
                left -
                right {
                    set wtab [expr {$wtext + $wimg + 4}]
                    if {$htext > $himg} {
                        set htab $htext
                    } else {
                        set htab $himg
                    }
                }
            }
            set wmax [expr {($wtab > $wmax) ? $wtab : $wmax}]
            incr wtot $wtab
            set pages_opts($page,-width) $wtab
            set hmax [expr {($htab > $hmax) ? $htab : $hmax}]
        }
        set _hpage $hmax
        set _wpage $wtot
    }
    method _compute_height {} {
        ## @brief Recompute tab height.
                
        set font [ttk::style lookup ${options(-style)}.Tab -font]
        set pady0 [lindex [ttk::style lookup ${options(-style)}.Tab -pady] \
                   0]
        set pady1 [lindex [ttk::style lookup ${options(-style)}.Tab -pady] \
                   1]
        set metrics [font metrics $font -linespace]
        set hmax 0
        foreach page $pages {
            set img $pages_opts($page,-image)
            set text $pages_opts($page,-text)
            set len [llength [split $text "\n"]]
            if {$len < 1} {set len 1}
            set compound $pages_opts($page,-compound)
            if {$compound eq "none"} {
                if {$pages_opts($page,-image) ne {}} {
                    set compound image
                } else {
                    set compound text
                }
            } elseif {$pages_opts($page,-image) eq {}} {
                set compound text
            } elseif {$pages_opts($page,-text) eq {}} {
                set compound image
            }
            switch $compound {
                text {
                    set htab [expr {$len * $metrics}]
                }
                image {
                    set htab [image height $img]
                }
                top -
                bottom {
                    set htab [expr {($len * $metrics) + [image height $img] + 4}]
                }
                center {
                    set htab [expr {(($len * $metrics)>[image height $img])?(($len * $metrics)+4):([image height $img] + 4)}]
                }
                left -
                right {
                    set ht [expr {$len * $metrics}]
                    set hi [image height $img]
                    if {$ht > $hi} {
                        set htab $ht
                    } else {
                        set htab $hi
                    }
                }
            }
            if {$htab > $hmax} {
                set hmax $htab
            }
        }
        set _hpage [expr {$hmax + $pady0 + $pady1}]
        if {[info exists tabs] && [winfo exists $tabs]} {
            $tabs configure -height $_hpage
        }
    }
    
    method _get_x_page {pos} {
        ## Get X position of the page at pos.
        #
        # @param pos The page position.
        # @return The x position of the tab.
        
        set x 0
        if {$pos < $base} {
            foreach page [lrange $pages $pos [expr {$base - 1}]] {
                incr x [expr {-$pages_opts($page,-width)}]
            }
        } elseif {$pos > $base} {
            foreach page [lrange $pages $base [expr {$pos - 1}]] {
                incr x $pages_opts($page,-width)
            }
        }
        return $x
    }
    method _xview {inc} {
        ## Shift the tabs to the left or right.
        #
        # @param inc The shift increment, negative to the left, positive to 
        # the right.
        
        #puts stderr "*** $self _xview $inc"
        if {$inc == -1} {
            set lbase [expr {$base - 1}]
            set dx $pages_opts([lindex $pages $base],-width)
        } else {
            set dx [expr {-$pages_opts([lindex $pages $base],-width)}]
            set lbase [expr {$base + 1}]
        }
        if {$lbase >= 0 && $lbase < [llength $pages]} {
            set base $lbase
            $tabs move page $dx 0
            $self _draw_arrows
        }
    }
    
    method _highlight {flag page} {
        ## Turn highlighting on or off for the specified tab.
        #
        # @param flag Flag to indicate turning highlighting on or off.
        # @param page The page whose tab to turn highlighting on or off.
        
        if {$pages_opts($page,-state) eq "disabled"} {return}
        switch $flag {
            on {
                $tabs itemconfigure "${page}:poly" \
                      -fill [ttk::style lookup ${options(-style)}.Tab \
                             -background active]
                $tabs itemconfigure "${page}:text" \
                      -fill [ttk::style lookup ${options(-style)}.Tab \
                             -foreground active]
            }
            off {
                $tabs itemconfigure "${page}:poly" \
                      -fill [ttk::style lookup ${options(-style)}.Tab \
                             -background]
                $tabs itemconfigure "${page}:text" \
                      -fill [ttk::style lookup ${options(-style)}.Tab \
                             -foreground]
            }
        }
    }
    method _select {page} {
        ## Make the specified page the selected page.
        #
        # @param page The page to select.
        #
        
        #puts stderr "*** $self _select $page"
        
        if {$page eq ""} {return}
        if {$pages_opts($page,-state) ne "normal"} {return}
        set oldsel $select
        #puts stderr "*** $self _select: oldsel = $oldsel"
        if {$page eq $oldsel} {return}
        if {$oldsel ne ""} {
            grid forget $oldsel
            set select ""
            $self _draw_page $oldsel 0
        }
        set select $page
        #puts stderr "*** $self _select: select = $select"
        if {$select ne ""} {
            $self _draw_page $select 0
            set padx [lindex $pages_opts($select,-padding) 0]
            set pady [lindex $pages_opts($select,-padding) 1]
            set sticky $pages_opts($select,-sticky)
            grid $select -column 0 -in $win -row $_clientRow \
                  -padx $padx -pady $pady -sticky $sticky
        }
    }
    method _redraw {} {
        ## Redraw the tabs and all.
        #
        
        if {!$realized} {return}
        set tablayout [ttk::style layout ${options(-style)}.Tab]
        set tabchildren [lrange [from tablayout -children {}] 1 end]
        set tabposition [from tabchildren -side top]
        $_tabsides validate $tabposition
        if {$tabposition eq {top}} {
            set _clientRow 1
            set _tabrow 0
        } else {
            set _clientRow 0
            set _tabrow 1
        }
        if {$select ne ""} {
            grid forget $select
        }
        grid forget $tabrow
        grid $tabrow -row $_tabrow -column 0 -sticky news
        if {$select ne ""} {
            set padx [lindex $pages_opts($select,-padding) 0]
            set pady [lindex $pages_opts($select,-padding) 1]
            set sticky $pages_opts($select,-sticky)
            grid $select -column 0 -in $win -row $_clientRow -padx $padx \
                  -pady $pady -sticky $sticky
        }
        $self _compute_height
        foreach page $pages {
            $self _draw_page $page 0
        }
        $self _draw_arrows
    }
    method _draw_page {page create} {
        ## Draw a tab for the specified page, creating a new tab if asked or
        # moving an old one otherwise.
        #
        # @param page The page whose tab we will draw.
        # @param create Flag indicating if the tab needs to be created.
        #
        
        set pos [lsearch -exact $pages $page]
        set bg  [ttk::style lookup ${options(-style)}.Tab -background]
        set fgt $lbg
        set fgb $dbg
        set h $_hpage
        set xd [$self _get_x_page $pos]
        set xf [expr {$xd + $pages_opts($page,-width)}]
        set pady0 [lindex [ttk::style lookup ${options(-style)}.Tab -pady] \
                   0]
        # Set the initial text offsets -- a few pixels down, centered 
        # left-to-right
        set textOffsetY [expr {$pady0 + 3}]
        set textOffsetX 9

        # Coordinates of the tab corners are:
        #     c3        c4
        #
        # c2                c5
        #
        # c1                c6
        #
        # where
        # c1 = $xd,	  $h
        # c2 = $xd+$xBevel,	           $arcRadius+2
        # c3 = $xd+$xBevel+$arcRadius, $arcRadius
        # c4 = $xf+1-$xBevel,          $arcRadius
        # c5 = $xf+$arcRadius-$xBevel, $arcRadius+2
        # c6 = $xf+$arcRadius,         $h
        
        set top		2
        set arcRadius	[ttk::style lookup ${options(-style)}.Tab -arcradius]
        set xBevel	[ttk::style lookup ${options(-style)}.Tab -bevelsize]

        if { $select ne $page } {
            if { $pos == 0 } {
                # The leftmost page is a special case -- it is drawn with its
                # tab a little indented.  To achieve this, we incr xd.  We also
                # decr textOffsetX, so that the text doesn't move left/right.
                incr xd 2
                incr textOffsetX -2
            }
        } else {
            # The selected page's text is raised higher than the others
            incr top -2
        }

        # Precompute some coord values that we use a lot
        set topPlusRadius	[expr {$top + $arcRadius}]
        set rightPlusRadius	[expr {$xf + $arcRadius}]
        set leftPlusRadius	[expr {$xd + $arcRadius}]

        # Sven
        set tablayout [ttk::style layout ${options(-style)}.Tab]
        set tabchildren [lrange [from tablayout -children {}] 1 end]
        set side [from tabchildren -side top]
        set tabsOnBottom [string equal $side "bottom"]

        set h1 [expr {[winfo height $tabs]}]
        set bd [ttk::style lookup ${options(-style)}.Tab -borderwidth]
        if {$bd < 1} { set bd 1 }

        if { $tabsOnBottom } {
            # adjust to keep bottom edge in view
            incr h1 -1
            set top [expr {$top * -1}]
            set topPlusRadius [expr {$topPlusRadius * -1}]
            # Hrm... the canvas has an issue with drawing diagonal segments
            # of lines from the bottom to the top, so we have to draw this line
            # backwards (ie, lt is actually the bottom, drawn from right to left)
            set lt  [list \
                     $rightPlusRadius			[expr {$h1-$h-1}] \
                     [expr {$rightPlusRadius - $xBevel}]	[expr {$h1 + $topPlusRadius}] \
                     [expr {$xf - $xBevel}]			[expr {$h1 + $top}] \
                     [expr {$leftPlusRadius + $xBevel}]	[expr {$h1 + $top}] \
                     ]
            set lb  [list \
                     [expr {$leftPlusRadius + $xBevel}]	[expr {$h1 + $top}] \
                     [expr {$xd + $xBevel}]			[expr {$h1 + $topPlusRadius}] \
                     $xd					[expr {$h1-$h-1}] \
                     ]
            # Because we have to do this funky reverse order thing, we have to
            # swap the top/bottom colors too.
            set tmp $fgt
            set fgt $fgb
            set fgb $tmp
        } else {
            set lt [list \
                    $xd					$h \
                    [expr {$xd + $xBevel}]			$topPlusRadius \
                    [expr {$leftPlusRadius + $xBevel}]	$top \
                    [expr {$xf + 1 - $xBevel}]		$top \
                    ]
            set lb [list \
                    [expr {$xf + 1 - $xBevel}] 		[expr {$top + 1}] \
                    [expr {$rightPlusRadius - $xBevel}]	$topPlusRadius \
                    $rightPlusRadius			$h \
                    ]
        }

        set compound $pages_opts($page,-compound)
        if {$compound eq "none"} {
            if {$pages_opts($page,-image) ne {}} {
                set compound image
            } else {
                set compound text
            }
        } elseif {$pages_opts($page,-image) eq {}} {
            set compound text
        } elseif {$pages_opts($page,-text) eq {}} {
            set compound image
        }
        set img $pages_opts($page,-image)
        set font [ttk::style lookup ${options(-style)}.Tab -font]
        $tabs itemconfigure $_textid -text $pages_opts($page,-text) \
              -font $font -anchor nw
        foreach {x1 y1 x2 y2} [$tabs bbox $_textid] {break}
        set htext [expr {$y2 - $y1}]
        set x2 [expr {$x2 - 6}]
        set wtext [expr {$x2 - $x1 + 20}]
        switch $compound {
            text {
                set ytext $top
                if { $tabsOnBottom } {
                    # The "+ 2" below moves the text closer to the bottom of 
                    # the tab, so it doesn't look so cramped.  I should be 
                    # able to achieve the same goal by changing the anchor of 
                    # the text and using this formula: 
                    # ytext = $top + $h1 - $textOffsetY
                    # but that doesn't quite work (I think the linespace from 
                    # the text gets in the way)
                    incr ytext [expr {$h1 - $h + 2}]
                }
                incr ytext $textOffsetY
                set xtext [expr {$xd + $textOffsetX}]
            }
            image {
                set yimage $top
                set ximage [expr {$xd + $textOffsetX}]
            }
            center {
                set ytext $top
                if { $tabsOnBottom } {
                    # The "+ 2" below moves the text closer to the bottom of 
                    # the tab, so it doesn't look so cramped.  I should be 
                    # able to achieve the same goal by changing the anchor of 
                    # the text and using this formula: 
                    # ytext = $top + $h1 - $textOffsetY
                    # but that doesn't quite work (I think the linespace from 
                    # the text gets in the way)
                    incr ytext [expr {$h1 - $h + 2}]
                }
                incr ytext $textOffsetY
                set xtext [expr {$xd + $textOffsetX}]
                set yimage $top
                set ximage [expr {$xd + $textOffsetX}]
                if {$wtext > [image width $img]} {
                    set ximage [expr {$ximage + (($wtext - [image width $img]) / 2.0)}]
                } elseif {[image width $img] > $wtext} {
                    set xtext [expr {$xtext + (([image width $img] - $wtext)/2.0)}]
                }
                if {$htext > [image height $img]} {
                    set yimage [expr {$yimage + (($htext - [image height $img]) / 2.0)}]
                } elseif {[image height $img] > $htext} {
                    set ytext [expr {$ytext + (([image height $img] - $htext)/2.0)}]
                }
            }
            bottom {
                set ytext $top
                if { $tabsOnBottom } {
                    # The "+ 2" below moves the text closer to the bottom of 
                    # the tab, so it doesn't look so cramped.  I should be 
                    # able to achieve the same goal by changing the anchor of 
                    # the text and using this formula: 
                    # ytext = $top + $h1 - $textOffsetY
                    # but that doesn't quite work (I think the linespace from 
                    # the text gets in the way)
                    incr ytext [expr {$h1 - $h + 2}]
                }
                incr ytext $textOffsetY
                set xtext [expr {$xd + $textOffsetX}]
                set yimage [expr {$ytext + $htext}]
                set ximage [expr {$xd + $textOffsetX}]
                if {$wtext > [image width $img]} {
                    set ximage [expr {$ximage + (($wtext - [image width $img]) / 2.0)}]
                } elseif {[image width $img] > $wtext} {
                    set xtext [expr {$xtext + (([image width $img] - $wtext)/2.0)}]
                }
            }
            top {
                set ximage [expr {$xd + $textOffsetX}]
                set yimage $top
                set ytext [expr {$yimage + [image height $img]}]
                if { $tabsOnBottom } {
                    # The "+ 2" below moves the text closer to the bottom of 
                    # the tab, so it doesn't look so cramped.  I should be 
                    # able to achieve the same goal by changing the anchor of 
                    # the text and using this formula: 
                    # ytext = $top + $h1 - $textOffsetY
                    # but that doesn't quite work (I think the linespace from 
                    # the text gets in the way)
                    incr ytext [expr {$h1 - $h + 2}]
                }
                set xtext [expr {$xd + $textOffsetX}]
                if {$wtext > [image width $img]} {
                    set ximage [expr {$ximage + (($wtext - [image width $img]) / 2.0)}]
                } elseif {[image width $img] > $wtext} {
                    set xtext [expr {$xtext + (([image width $img] - $wtext)/2.0)}]
                }
            }
            left {
                set ximage [expr {$xd + $textOffsetX}]
                set yimage $top
                set ytext $top
                if { $tabsOnBottom } {
                    # The "+ 2" below moves the text closer to the bottom of 
                    # the tab, so it doesn't look so cramped.  I should be 
                    # able to achieve the same goal by changing the anchor of 
                    # the text and using this formula: 
                    # ytext = $top + $h1 - $textOffsetY
                    # but that doesn't quite work (I think the linespace from 
                    # the text gets in the way)
                    incr ytext [expr {$h1 - $h + 2}]
                }
                set xtext [expr {$xd + $textOffsetX + [image width $img]}]
                if {$htext > [image height $img]} {
                    set yimage [expr {$yimage + (($htext - [image height $img]) / 2.0)}]
                } elseif {[image height $img] > $htext} {
                    set ytext [expr {$ytext + (([image height $img] - $htext)/2.0)}]
                }
            }
            right {
                set ytext $top
                if { $tabsOnBottom } {
                    # The "+ 2" below moves the text closer to the bottom of 
                    # the tab, so it doesn't look so cramped.  I should be 
                    # able to achieve the same goal by changing the anchor of 
                    # the text and using this formula: 
                    # ytext = $top + $h1 - $textOffsetY
                    # but that doesn't quite work (I think the linespace from 
                    # the text gets in the way)
                    incr ytext [expr {$h1 - $h + 2}]
                }
                set xtext [expr {$xd + $textOffsetX}]
                set ximage [expr {$xd + $textOffsetX + $wtext}]
                set yimage $top
                if {$htext > [image height $img]} {
                    set yimage [expr {$yimage + (($htext - [image height $img]) / 2.0)}]
                } elseif {[image height $img] > $htext} {
                    set ytext [expr {$ytext + (([image height $img] - $htext)/2.0)}]
                }
            }
        }
        if {$page eq $select} {
            set fg [ttk::style lookup ${options(-style)}.Tab -foreground]
        } else {
            if {$pages_opts($page,-state) eq "normal"} {
                set fg [ttk::style lookup ${options(-style)}.Tab -foreground]
            } else {
                set fg [ttk::style lookup ${options(-style)}.Tab -disabledforeground]
            }
        }
        
        if {$create} {
            $tabs create polygon [concat $lt $lb] \
                  -tags [list page p:$page $page:poly] \
                  -outline $bg -fill $bg
            $tabs create line $lt \
                  -tags [list page p:$page $page:top top] \
                  -fill $fgt -width $bd
            $tabs create line $lb \
                  -tags [list page p:$page $page:bot bot] \
                  -fill $fgb  -width $bd
            if {$compound ne "text"} {
                $tabs create image $ximage $yimage \
                      -image $img \
                      -anchor nw \
                      -tags [list page p:$page $page:img]
            }
            if {$compound ne "image"} {
                $tabs create text $xtext $ytext \
                      -text $pages_opts($page,-text) \
                      -font $font \
                      -fill $fg \
                      -anchor nw \
                      -tags [list page p:$page $page:text]
                if {$pages_opts($page,-underline) ne {}} {
                    $tabs itemconfigure $page:text \
                          -underline $pages_opts($page,-underline)
                }
            }
            $tabs bind p:$page <ButtonPress-1> [mymethod _select $page]
            $tabs bind p:$page <Enter> [mymethod _highlight on $page]
            $tabs bind p:$page <Leave> [mymethod _highlight off $page]
        } else {
            if {$compound eq "image"} {
                $tabs delete $page:text
            } elseif {$compound ne "text"} {
                $tabs coords $page:img $ximage $yimage
                $tabs itemconfigure $page:img -image $img
            }
            if {$compound eq "text"} {
                $tabs coords $page:text $xtext $ytext
                $tabs delete $page:img
            } elseif {$compound ne "image"} {
                $tabs coords $page:text $xtext $ytext
                $tabs itemconfigure $page:text \
                      -text $pages_opts($page,-text) \
                      -font $font \
                      -fg $fg
                if {$pages_opts($page,-underline) ne {}} {
                    $tabs itemconfigure $page:text \
                          -underline $pages_opts($page,-underline)
                }
            }
        }
        
        $tabs coords $page:poly [concat $lt $lb]
        $tabs coords $page:top $lt
        $tabs coords $page:bot $lb
        $tabs itemconfigure $page:poly -fill $bg -outline $bg
        $tabs itemconfigure $page:top -fill $fgt -width $bd
        $tabs itemconfigure $page:bot -fill $fgb -width $bd
            
        if {$page eq $select} {
            $tabs raise p:$page
        } elseif {$pos == 0} {
            if {$select eq ""} {
                $tabs raise p:$page
            } else {
                $tabs lower p:$page p:$select
            }
        } else {
            set pred [lindex $pages [expr {$pos - 1}]]
            if {$select ne $pred || $pos == 1} {
                $tabs lower p:$page p:$pred
            } else {
                $tabs lower p:$page p:[lindex $pages [expr {$pos - 2}]]
            }
        }
    }
    method _draw_arrows {} {
        ## Draw the arrow buttons if needed.
        
        #puts stderr "*** $self _draw_arrows"
        set w [expr {[winfo width $tabs] - 1}]
        set h [expr {$_hpage -1}]
        set nbpages [llength $pages]
        set xl 0
        set xr [expr {$w - $_warrow}]
        #puts stderr "*** $self _draw_arrows: w = $w, h = $h, nbpages = $nbpages, xl = $xl, xr = $xr"
        #puts stderr "*** $self _draw_arrows: base = $base"
        if {$base > 0} {
            grid $left -row 0 -column 0
        } else {
            grid forget $left
        }
        #puts stderr "*** $self _draw_arrows: _wpage = $_wpage, \[$self _get_x_page 0\] = [$self _get_x_page 0]"
        if {$base < $nbpages-1 && $_wpage + [$self _get_x_page 0] > $w} {
            grid $right -row 0 -column 2
        } else {
            grid forget $right
        }
    }
    method _resize {} {
        ## Resize the widget.
        
        if {!$realized} {
            if {[set width [$self cget -width]] == 0 ||
                [set height [$self cget -height]] == 0 } {
                $self compute_size
            }
            set realized 1
        }
        $self _redraw
    }
    typemethod _themeChanged {w} {
        ## @brief Theme Changed typemethod.
        #
        # @param w The widget the theme changed for.
        
        $w _themeChanged_
    }
    method _themeChanged_ {} {
        ## @brief Theme Changed method.
        #
        
        $self _compute_width 
        $self _redraw
    }
    typemethod _Configure {widget width height} {
        ## @brief Configure typemethod.
        #
        # @param widget The widget the Configure event happened for.
        # @param width The new width.
        # @param height The new height.
        #
        
        $widget _resize
    }
    proc get3dcolor {path bgcolor} {
        ## Compute 3D colors.
        #
        # @param path Window path.
        # @param bgcolor Background color to use as a base.
        # @return Two RGB colors, one darker, one lighter.
        
        foreach val [winfo rgb $path $bgcolor] {
            lappend dark [expr {60*$val/100}]
            set tmp1 [expr {14*$val/10}]
            if { $tmp1 > 65535 } {
                set tmp1 65535
            }
            set tmp2 [expr {(65535+$val)/2}]
            lappend light [expr {($tmp1 > $tmp2) ? $tmp1:$tmp2}]
        }
        return [list [eval format "#%04x%04x%04x" $dark] [eval format "#%04x%04x%04x" $light]]
    }
}

## @}

package provide ScrollTabNotebook 1.0

