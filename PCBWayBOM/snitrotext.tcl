##############################################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Fri May 24 15:07:12 2013
#  Last Modified : <130611.0918>
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

catch {
# Standard Motif bindings:

bind ROText <1> {
    tk::TextButton1 %W %x %y
    %W tag remove sel 0.0 end
}
bind ROText <B1-Motion> {
    set tk::Priv(x) %x
    set tk::Priv(y) %y
    tk::TextSelectTo %W %x %y
}
bind ROText <Double-1> {
    set tk::Priv(selectMode) word
    tk::TextSelectTo %W %x %y
    catch {%W mark set insert sel.last}
    catch {%W mark set anchor sel.first}
}
bind ROText <Triple-1> {
    set tk::Priv(selectMode) line
    tk::TextSelectTo %W %x %y
    catch {%W mark set insert sel.last}
    catch {%W mark set anchor sel.first}
}
bind ROText <Shift-1> {
    tk::TextResetAnchor %W @%x,%y
    set tk::Priv(selectMode) char
    tk::TextSelectTo %W %x %y
}
bind ROText <Double-Shift-1>	{
    set tk::Priv(selectMode) word
    tk::TextSelectTo %W %x %y 1
}
bind ROText <Triple-Shift-1>	{
    set tk::Priv(selectMode) line
    tk::TextSelectTo %W %x %y
}
bind ROText <B1-Leave> {
    set tk::Priv(x) %x
    set tk::Priv(y) %y
    tk::TextAutoScan %W
}
bind ROText <B1-Enter> {
    tk::CancelRepeat
}
bind ROText <ButtonRelease-1> {
    tk::CancelRepeat
}
bind ROText <Control-1> {
    %W mark set insert @%x,%y
}
bind ROText <Left> {
    tk::TextSetCursor %W insert-1c
}
bind ROText <Right> {
    tk::TextSetCursor %W insert+1c
}
bind ROText <Up> {
    tk::TextSetCursor %W [tk::TextUpDownLine %W -1]
}
bind ROText <Down> {
    tk::TextSetCursor %W [tk::TextUpDownLine %W 1]
}
bind ROText <Shift-Left> {
    tk::TextKeySelect %W [%W index {insert - 1c}]
}
bind ROText <Shift-Right> {
    tk::TextKeySelect %W [%W index {insert + 1c}]
}
bind ROText <Shift-Up> {
    tk::TextKeySelect %W [tk::TextUpDownLine %W -1]
}
bind ROText <Shift-Down> {
    tk::TextKeySelect %W [tk::TextUpDownLine %W 1]
}
bind ROText <Control-Left> {
    tk::TextSetCursor %W [tk::TextPrevPos %W insert tcl_startOfPreviousWord]
}
bind ROText <Control-Right> {
    tk::TextSetCursor %W [tk::TextNextWord %W insert]
}
bind ROText <Control-Up> {
    tk::TextSetCursor %W [tk::TextPrevPara %W insert]
}
bind ROText <Control-Down> {
    tk::TextSetCursor %W [tk::TextNextPara %W insert]
}
bind ROText <Shift-Control-Left> {
    tk::TextKeySelect %W [tk::TextPrevPos %W insert tcl_startOfPreviousWord]
}
bind ROText <Shift-Control-Right> {
    tk::TextKeySelect %W [tk::TextNextWord %W insert]
}
bind ROText <Shift-Control-Up> {
    tk::TextKeySelect %W [tk::TextPrevPara %W insert]
}
bind ROText <Shift-Control-Down> {
    tk::TextKeySelect %W [tk::TextNextPara %W insert]
}
bind ROText <Prior> {
    tk::TextSetCursor %W [tk::TextScrollPages %W -1]
}
bind ROText <Shift-Prior> {
    tk::TextKeySelect %W [tk::TextScrollPages %W -1]
}
bind ROText <Next> {
    tk::TextSetCursor %W [tk::TextScrollPages %W 1]
}
bind ROText <Shift-Next> {
    tk::TextKeySelect %W [tk::TextScrollPages %W 1]
}
bind ROText <Control-Prior> {
    %W xview scroll -1 page
}
bind ROText <Control-Next> {
    %W xview scroll 1 page
}

bind ROText <Home> {
    tk::TextSetCursor %W {insert linestart}
}
bind ROText <Shift-Home> {
    tk::TextKeySelect %W {insert linestart}
}
bind ROText <End> {
    tk::TextSetCursor %W {insert lineend}
}
bind ROText <Shift-End> {
    tk::TextKeySelect %W {insert lineend}
}
bind ROText <Control-Home> {
    tk::TextSetCursor %W 1.0
}
bind ROText <Control-Shift-Home> {
    tk::TextKeySelect %W 1.0
}
bind ROText <Control-End> {
    tk::TextSetCursor %W {end - 1 char}
}
bind ROText <Control-Shift-End> {
    tk::TextKeySelect %W {end - 1 char}
}

bind ROText <Control-Tab> {
    focus [tk_focusNext %W]
}
bind ROText <Control-Shift-Tab> {
    focus [tk_focusPrev %W]
}
bind ROText <Select> {
    %W mark set anchor insert
}
bind ROText <<Copy>> {
    tk_textCopy %W
}
# Additional emacs-like bindings:

bind ROText <Control-a> {
    if {!$tk_strictMotif} {
	tk::TextSetCursor %W {insert linestart}
    }
}
bind ROText <Control-b> {
    if {!$tk_strictMotif} {
	tk::TextSetCursor %W insert-1c
    }
}
bind ROText <Control-e> {
    if {!$tk_strictMotif} {
	tk::TextSetCursor %W {insert lineend}
    }
}
bind ROText <Control-f> {
    if {!$tk_strictMotif} {
	tk::TextSetCursor %W insert+1c
    }
}
bind ROText <Control-n> {
    if {!$tk_strictMotif} {
	tk::TextSetCursor %W [tk::TextUpDownLine %W 1]
    }
}
bind ROText <Control-p> {
    if {!$tk_strictMotif} {
	tk::TextSetCursor %W [tk::TextUpDownLine %W -1]
    }
}
if {[string compare $tcl_platform(platform) "windows"]} {
bind ROText <Control-v> {
    if {!$tk_strictMotif} {
	tk::TextScrollPages %W 1
    }
}
}

bind ROText <Meta-b> {
    if {!$tk_strictMotif} {
	tk::TextSetCursor %W [tk::TextPrevPos %W insert tcl_startOfPreviousWord]
    }
}
bind ROText <Meta-f> {
    if {!$tk_strictMotif} {
	tk::TextSetCursor %W [tk::TextNextWord %W insert]
    }
}
bind ROText <Meta-less> {
    if {!$tk_strictMotif} {
	tk::TextSetCursor %W 1.0
    }
}
bind ROText <Meta-greater> {
    if {!$tk_strictMotif} {
	tk::TextSetCursor %W end-1c
    }
}
# Macintosh only bindings:

# if text black & highlight black -> text white, other text the same
if {[string equal $tcl_platform(platform) "macintosh"]} {
bind ROText <FocusIn> {
    %W tag configure sel -borderwidth 0
    %W configure -selectbackground systemHighlight -selectforeground systemHighlightText
}
bind ROText <FocusOut> {
    %W tag configure sel -borderwidth 1
    %W configure -selectbackground white -selectforeground black
}
bind ROText <Option-Left> {
    tk::TextSetCursor %W [tk::TextPrevPos %W insert tcl_startOfPreviousWord]
}
bind ROText <Option-Right> {
    tk::TextSetCursor %W [tk::TextNextWord %W insert]
}
bind ROText <Option-Up> {
    tk::TextSetCursor %W [tk::TextPrevPara %W insert]
}
bind ROText <Option-Down> {
    tk::TextSetCursor %W [tk::TextNextPara %W insert]
}
bind ROText <Shift-Option-Left> {
    tk::TextKeySelect %W [tk::TextPrevPos %W insert tcl_startOfPreviousWord]
}
bind ROText <Shift-Option-Right> {
    tk::TextKeySelect %W [tk::TextNextWord %W insert]
}
bind ROText <Shift-Option-Up> {
    tk::TextKeySelect %W [tk::TextPrevPara %W insert]
}
bind ROText <Shift-Option-Down> {
    tk::TextKeySelect %W [tk::TextNextPara %W insert]
}

# End of Mac only bindings
}

}

snit::widgetadaptor ROText {
    delegate method * to hull
    delegate option * to hull
    option -style -default ROText
    method _themeUpdated {} {
        #puts stderr "*** $self _themeUpdated"
        foreach o {-background -borderwidth -font -foreground 
            -highlightbackground -highlightcolor -highlightthickness -relief 
            -insertbackground -selectbackground -insertborderwidth 
            -selectborderwidth -selectforeground -padx -pady} {
            if {![catch {ttk::style lookup $options(-style) $o} ov]} {
                #if {"$ov" eq ""} {continue}
                #puts stderr "*** $self _themeUpdated: $o $ov"
                catch {$hull configure $o $ov}
            }
        }
    }
    constructor {args} {
        installhull using text
        $self configurelist $args
        set indx [lsearch [bindtags $win] Text]
        bindtags $win [lreplace [bindtags $win] $indx $indx ROText]
        bind $win <<ThemeChanged>> [mymethod _themeUpdated]
        $self _themeUpdated
    }
}

package provide ROText 1.0

