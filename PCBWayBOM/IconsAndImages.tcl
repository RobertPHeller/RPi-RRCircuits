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
#  Last Modified : <130721.1110>
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
    typevariable icondir
    typevariable unknownimg
    typevariable imagemap -array {}
    typeconstructor {
        set icondir [file dirname [info script]]
        set unknownimg [image create photo -file [file join $icondir unknown.xpm]]
    }
    option -filetype -default "any" -readonly yes \
          -type {snit::enum -values {any png xpm xbm}}
    option -background -default {} -readonly yes
    option -foreground -default black -readonly yes
    constructor {args} {
        #puts stderr "*** $type create $self $args"
        set name [namespace tail $self]
        #puts stderr "*** $type create $self: name is $name"
        $self configurelist $args
        switch $options(-filetype) {
            xbm {
                set xbmfile [file join $icondir $name.xbm]
                set xpmfile {}
                set pngfile {}
            }
            xpm {
                set xbmfile {}
                set xpmfile [file join $icondir $name.xpm]
                set pngfile {}
            }
            png {
                set xbmfile {}
                set xpmfile {}
                set pngfile [file join $icondir $name.png]
            }
            any {
                set xbmfile [file join $icondir $name.xbm]
                set xpmfile [file join $icondir $name.xpm]
                set pngfile [file join $icondir $name.png]
            }
        }
        #puts stderr "*** $type create $self: pngfile file is $pngfile"
        #puts stderr "*** $type create $self: xpmfile file is $xpmfile"
        #puts stderr "*** $type create $self: xbmfile file is $xbmfile"
        if {$pngfile ne {} && [file exists $pngfile]} {
            set imagemap($name) [image create photo -file $pngfile]
        } elseif {$xpmfile ne {} && [file exists $xpmfile]} {
            set imagemap($name) [image create photo -file $xpmfile]
        } elseif {$xbmfile ne {} && [file exists $xbmfile]} {
            set imagemap($name) [image create bitmap -file $xbmfile \
                                 -background [from args -background] \
                                 -foreground [from args -foreground]]
        } else {
            set imagemap($name) $unknownimg
        }
        #puts stderr "*** $type create $self: imagemap($name) is $imagemap($name)"
        
    }
    typemethod image {name args} {
        #puts stderr "*** $type image $name $args"
        #parray imagemap
        set name [namespace tail $name]
        #puts stderr "*** $type image: $name"
        if {[::info exists imagemap($name)]} {
            return $imagemap($name)
        } else {
            eval [list $type create $name] $args
            #puts stderr "*** $type image: imagemap($name) is $imagemap($name)"
            return $imagemap($name)
        }
    }
}

snit::type IconBitmap {
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
    constructor {args} {
        set name [namespace tail $self]
        set xbmfile [file join $icondir $name.xbm]
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
            $type $name
            return $bitmapmap($name)
        }
    }
}

package provide IconImage 1.0

