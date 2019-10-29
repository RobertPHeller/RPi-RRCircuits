##############################################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Thu May 16 13:48:41 2013
#  Last Modified : <191029.1530>
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
package require Img

snit::type IconImage {
    pragma -hastypeinfo no
    pragma -hastypedestroy no
    pragma -hasinstances   no
    
    typevariable icondir
    typevariable unknownimg
    typevariable imagemap -array {}
    typeconstructor {
        set icondir [file dirname [info script]]
        set unknownimg [image create photo -file [file join $icondir unknown.xpm]]
        snit::enum ftypes -values {any png xpm xbm}
    }
    typemethod insert {_self args} {
        #puts stderr "*** $type insert $_self $args"
        set _name [namespace tail $_self]
        #puts stderr "*** $type insert $_self: _name is $_name"
        set filetype [from args -filetype "any"]
        set _icondir [from args -icondir $icondir]
        #puts stderr "*** $type insert $_self: filetype is $filetype"
        ftypes validate $filetype
        #puts stderr "*** $type insert $_self: filetype validated."
        switch $filetype {
            xbm {
                set xbmfile [file join $_icondir $_name.xbm]
                set xpmfile {}
                set pngfile {}
                set giffile {}
            }
            xpm {
                set xbmfile {}
                set xpmfile [file join $_icondir $_name.xpm]
                set pngfile {}
                set giffile {}
            }
            png {
                set xbmfile {}
                set xpmfile {}
                set pngfile [file join $_icondir $_name.png]
                set giffile {}
            }
            gif {
                set xbmfile {}
                set xpmfile {}
                set pngfile {}
                set giffile [file join $_icondir $_name.gif]
            }
            any {
                set xbmfile [file join $_icondir $_name.xbm]
                set xpmfile [file join $_icondir $_name.xpm]
                set pngfile [file join $_icondir $_name.png]
                set giffile [file join $_icondir $_name.gif]
            }
        }
        #puts stderr "*** $type insert $_self: pngfile file is $pngfile"
        #puts stderr "*** $type insert $_self: xpmfile file is $xpmfile"
        #puts stderr "*** $type insert $_self: xbmfile file is $xbmfile"
        if {$pngfile ne {} && [file exists $pngfile]} {
            set imagemap($_name) [image create photo -file $pngfile]
        } elseif {$xpmfile ne {} && [file exists $xpmfile]} {
            set imagemap($_name) [image create photo -file $xpmfile]
        } elseif {$xbmfile ne {} && [file exists $xbmfile]} {
            set imagemap($_name) [image create bitmap -file $xbmfile \
                                 -background [from args -background {}] \
                                 -foreground [from args -foreground black]]
        } elseif {$giffile ne {} && [file exists $giffile]} {
            set imagemap($_name) [image create photo -file $giffile]
        } else {
            set imagemap($_name) $unknownimg
        }
        #puts stderr "*** $type insert $_self: imagemap($_name) is $imagemap($_name)"
        
    }
    typemethod image {name args} {
        #puts stderr "*** $type image $name $args"
        #parray imagemap
        set _name [namespace tail $name]
        #puts stderr "*** $type image: $_name"
        if {[::info exists imagemap($_name)]} {
            return $imagemap($_name)
        } else {
            eval [list $type insert $name] $args
            #puts stderr "*** $type image: imagemap($_name) is $imagemap($_name)"
            return $imagemap($_name)
        }
    }
}

snit::type IconBitmap {
    pragma -hastypeinfo no
    pragma -hastypedestroy no
    pragma -hasinstances   no
    
    typevariable icondir
    typevariable unknownbm
    typevariable bitmapmap -array {}
    
    typeconstructor {
        set icondir [file dirname [info script]]
        set unknownbm error
        foreach stockbm {error gray75 gray50 gray25 gray12 hourglass info questhead question warning} {
            set bitmapmap($stockbm) $stockbm
        }
        if {$::tcl_platform(platform) eq "macintosh"} {
            foreach macbm {document stationery edition application accessory folder pfolder trash floppy ramdisk cdrom preferences querydoc stop note caution} {
                set bitmapmap($macbm) $macbm
            }
        }
    }
    typemethod insert {_self args} {
        set name [namespace tail $_self]
        set _icondir [from args -icondir $icondir]
        set xbmfile [file join $_icondir $name.xbm]
        if {[file exists $xbmfile]} {
            set bitmapmap($name) @$xbmfile
        } else {
            set bitmapmap($name) $unknownbm
        }
    }
    typemethod bitmap {name} {
        if {[info exists bitmapmap($name)]} {
            return $bitmapmap($name)
        } else {
            $type insert $name
            return $bitmapmap($name)
        }
    }
}

package provide IconImage 1.0

