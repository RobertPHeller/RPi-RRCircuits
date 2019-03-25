##############################################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Wed Jul 10 16:07:16 2013
#  Last Modified : <140303.1344>
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
package require IconImage

## Ttk based Label<mumble> (LabelEntry, LabelSpinBox, LabelComboBox)

snit::macro editable {thecomp} {
    typevariable _editablecompname $thecomp
    option -editable -type snit::boolean \
          -configuremethod _configure_editable \
          -cgetmethod _cget_editable -default yes
    method _configure_editable {o v} {
        set thecomp [set $_editablecompname]
        #puts stderr "*** $self _configure_editable: _compname = $_compname, thecomp = $thecomp"
        if {$v} {
            $thecomp configure -state normal
        } else {
            $thecomp configure -state readonly
        }
        return $v
    }
    method _cget_editable {o} {
        set thecomp [set $_editablecompname]
        #puts stderr "*** $self _cget_editable: _compname = $_compname, thecomp = $thecomp"
        if {[$thecomp cget -state] eq "normal"} {
            return yes
        } else {
            return no
        }
    }
}


snit::widget LabelEntry {
    hulltype ttk::frame
    widgetclass LabelEntry
    option -style -default LabelEntry
    typeconstructor {
        ttk::style layout $type [ttk::style layout TLabelframe]
        ttk::style layout $type.Label [ttk::style layout TLabelframe.Label]
    }
    editable entry
    component label
    component entry
    delegate option * to entry except {-class -style}
    delegate option -label to label as -text
    delegate option -labelwidth to label as -width
    delegate option -labelimage to label as -image
    delegate option -labelcompound to label as -compound
    delegate option -labelanchor to label as -anchor
    delegate option -labelfont to label as -font
    delegate option -labeljustify to label as -justify
    delegate method * to entry except {cget configure xview}
    option -text -default {} \
          -configuremethod _configure_text \
          -cgetmethod _cget_text
    method _configure_text {o v} {
        #puts stderr "*** $self _configure_text $o $v"
        set state [$entry cget -state]
        $entry configure -state normal
        $entry delete 0 end
        $entry insert end $v
        $entry configure -state $state
        return $v
    }
    method _cget_text {o} {
        return [$entry get]
    }
    constructor {args} {
        install label using ttk::label $win.label
        pack $label -side left
        install entry using ttk::entry $win.entry
        pack $entry -side left -fill x -expand yes
        $self configurelist $args
    }
    method bind {sequence script} {
        bind $entry $sequence $script
    }
}

snit::listtype threenumbers  -minlen 0 -maxlen 3 -type snit::double

snit::widget LabelSpinBox {
    hulltype ttk::frame
    widgetclass LabelSpinBox
    option -style LabelSpinBox
    typeconstructor {
        ttk::style layout $type [ttk::style layout TLabelframe]
        ttk::style layout $type.Label [ttk::style layout TLabelframe.Label]
    }
    component label
    component spinbox
    editable spinbox
    option -text -default {} \
          -configuremethod _configure_text \
          -cgetmethod _cget_text
    method _configure_text {o v} {
        #puts stderr "*** $self _configure_text $o $v"
        set state [$spinbox cget -state]
        $spinbox configure -state normal
        $spinbox set $v
        $spinbox configure -state $state
        return $v
    }
    method _cget_text {o} {
        return [$spinbox get]
    }
    
    delegate option * to spinbox except {-class -style}
    delegate option -label to label as -text
    delegate option -labelwidth to label as -width
    delegate option -labelimage to label as -image
    delegate option -labelcompound to label as -compound
    delegate option -labelanchor to label as -anchor
    delegate option -labelfont to label as -font
    delegate option -labeljustify to label as -justify
    delegate method * to spinbox except {cget configure xview}
    option -range -default {} -type threenumbers \
          -configuremethod _configure_range \
          -cgetmethod _cget_range
    method _configure_range {o v} {
        if {[llength $v] == 0} {return}
        if {[llength $v] != 3} {
            error "Expected a list of three numbers, got $v for $o"
        }
        lassign $v from to increment
        $spinbox configure -from $from -to $to -increment $increment
    }
    method _cget_range {o} {
        return [list [$spinbox cget -from] [$spinbox cget -to] \
                [$spinbox cget -increment]]
    }
    constructor {args} {
        install label using ttk::label $win.label
        pack $label -side left
        install spinbox using spinbox $win.spinbox
        pack $spinbox -side left -fill x -expand yes
        $self configurelist $args
    }
    method bind {sequence script} {
        bind $spinbox $sequence $script
    }
}


snit::widget LabelComboBox {
    hulltype ttk::frame
    widgetclass LabelComboBox
    option -style LabelComboBox
    typeconstructor {
        ttk::style layout $type [ttk::style layout TLabelframe]
        ttk::style layout $type.Label [ttk::style layout TLabelframe.Label]
    }
    component label
    component combobox
    editable combobox
    option -text -default {} \
          -configuremethod _configure_text \
          -cgetmethod _cget_text
    method _configure_text {o v} {
        #puts stderr "*** $self _configure_text $o $v"
        set state [$combobox cget -state]
        $combobox configure -state normal
        $combobox set $v
        $combobox configure -state $state
        return $v
    }
    method _cget_text {o} {
        return [$combobox get]
    }
    option -modifycmd -default ""
    delegate option * to combobox except {-class -style}
    delegate option -label to label as -text
    delegate option -labelwidth to label as -width
    delegate option -labelimage to label as -image
    delegate option -labelcompound to label as -compound
    delegate option -labelanchor to label as -anchor
    delegate option -labelfont to label as -font
    delegate option -labeljustify to label as -justify
    delegate method * to combobox except {cget configure xview}
    constructor {args} {
        install label using ttk::label $win.label
        pack $label -side left
        install combobox using ttk::combobox $win.combobox
        pack $combobox -side left -fill x -expand yes
        $self configurelist $args
        bind $combobox <<ComboboxSelected>> [mymethod _selected]
    }
    method bind {sequence script} {
        bind $combobox $sequence $script
    }
    method _selected {} {
        if {"$options(-modifycmd)" ne ""} {
            uplevel #0 "$options(-modifycmd)"
        }
    }
}

snit::widget FileEntry {
    hulltype ttk::frame
    widgetclass FileEntry
    option -style FileEntry
    typevariable openfold
    typeconstructor {
        ttk::style layout $type [ttk::style layout TLabelframe]
        ttk::style layout $type.Label [ttk::style layout TLabelframe.Label]
        set openfold [IconImage image openfold]
    }
    component label
    component entry
    component browse
    delegate option * to entry except {-class -style}
    delegate option -label to label as -text
    delegate option -labelwidth to label as -width
    delegate option -labelimage to label as -image
    delegate option -labelcompound to label as -compound
    delegate option -labelanchor to label as -anchor
    delegate option -labelfont to label as -font
    delegate option -labeljustify to label as -justify
    delegate method * to entry except {cget configure xview}
    option -fileimage -configuremethod _configure_fileimage
    method _configure_fileimage {o v} {
        set options($o) $v
        catch {$browse configure -image $v}
    }
    option -filedialog -default open -type {snit::enum -values {open save directory}}
    option -defaultextension -default ""
    option -filetypes -type snit::listtype -default {}
    option -title -default ""
    option -text -default {} \
          -configuremethod _configure_text \
          -cgetmethod _cget_text
    method _configure_text {o v} {
        #puts stderr "*** $self _configure_text $o $v"
        set state [$entry cget -state]
        $entry configure -state normal
        $entry delete 0 end
        $entry insert end $v
        $entry configure -state $state
        return $v
    }
    method _cget_text {o} {
        return [$entry get]
    }
    constructor {args} {
        install label using ttk::label $win.label
        pack $label -side left
        install entry using ttk::entry $win.entry
        pack $entry -side left -fill x -expand yes
        set bimage [from args -fileimage $openfold]
        set options(-fileimage) $bimage
        install browse using ttk::button $win.browse -image $bimage \
              -command [mymethod _Browse]
        pack $browse -side right
        $self configurelist $args
    }
    method _Browse {} {
        set dialogType $options(-filedialog)
        set defaultextension $options(-defaultextension)
        set filetypes $options(-filetypes)
        set title $options(-title)
        set currentfile [$entry get]
        switch $dialogType {
            open {
                set newfile [tk_getOpenFile \
                             -defaultextension "$defaultextension" \
                             -filetypes "$filetypes" \
                             -title "$title" \
                             -initialdir [file dirname "$currentfile"] \
                             -initialfile "$currentfile" \
                             -parent $win]
                if {"$newfile" ne ""} {
                    $entry delete 0 end
                    $entry insert end $newfile
                }
            }
            save {
                set newfile [tk_getSaveFile \
                             -defaultextension "$defaultextension" \
                             -filetypes "$filetypes" \
                             -title "$title" \
                             -initialdir [file dirname "$currentfile"] \
                             -initialfile "$currentfile" \
                             -parent $win]
                if {"$newfile" ne ""} {
                    $entry delete 0 end
                    $entry insert end $newfile
                }
            }
            directory {
                set newdirectory [tk_chooseDirectory \
                             -title "$title" \
                             -initialdir "$currentfile" \
                             -parent $win]
                if {"$newdirectory" ne ""} {
                    $entry delete 0 end
                    $entry insert end $newdirectory
                }
            }
        }
    }
    method bind {sequence script} {
        bind $entry $sequence $script
    }
}

snit::widget LabelSelectColor {
    hulltype ttk::frame
    widgetclass LabelSelectColor
    option -style LabelSelectColor
    typevariable palette
    typeconstructor {
        ttk::style layout $type [ttk::style layout TLabelframe]
        ttk::style layout $type.Label [ttk::style layout TLabelframe.Label]
        set palette [IconImage image palette]
    }
    component label
    component entry
    component browse
    delegate option * to entry except {-class -style}
    delegate option -label to label as -text
    delegate option -labelwidth to label as -width
    delegate option -labelimage to label as -image
    delegate option -labelcompound to label as -compound
    delegate option -labelanchor to label as -anchor
    delegate option -labelfont to label as -font
    delegate option -labeljustify to label as -justify
    delegate method * to entry except {cget configure xview}
    option -title -default ""
    option -text -default {} \
          -configuremethod _configure_text \
          -cgetmethod _cget_text
    method _configure_text {o v} {
        #puts stderr "*** $self _configure_text $o $v"
        set state [$entry cget -state]
        $entry configure -state normal
        $entry delete 0 end
        $entry insert end $v
        $entry configure -state $state
        return $v
    }
    method _cget_text {o} {
        return [$entry get]
    }
    constructor {args} {
        install label using ttk::label $win.label
        pack $label -side left
        install entry using ttk::entry $win.entry
        pack $entry -side left -fill x -expand yes
        set bimage [from args -fileimage $palette]
        set options(-fileimage) $bimage
        install browse using ttk::button $win.browse -image $bimage \
              -command [mymethod _ColorPopup]
        pack $browse -side right
        $self configurelist $args
        if {[$entry get] eq ""} {$entry insert end white}
    }
    method _ColorPopup {} {
        set newcolor [tk_chooseColor -initialcolor "[$entry get]" \
                      -title $options(-title) -parent $win]
        if {"$newcolor" ne ""} {
            $entry delete 0 end
            $entry insert end "$newcolor"
        }
    }
}
    
    
snit::widget LabelFrame {
    hulltype ttk::frame
    widgetclass LabelFrame
    option -style LabelFrame
    typeconstructor {
        ttk::style layout $type [ttk::style layout TLabelframe]
        ttk::style layout $type.Label [ttk::style layout TLabelframe.Label]
    }
    component label
    component uframe
    delegate option -image to label
    delegate option -text to label
    delegate option -width to label
    delegate option -compound to label
    delegate option -anchor to label
    delegate option -font to label
    delegate option -justify to label
    delegate option -borderwidth to hull
    delegate option -relief to hull
    constructor {args} {
        install label using ttk::label $win.label
        pack $label -side left
        install uframe using ttk::frame $win.uframe
        pack $uframe -fill x -expand yes -side left
        $self configurelist $args
    }
    method getframe {} {return $uframe}
}



package provide LabelFrames 1.0
