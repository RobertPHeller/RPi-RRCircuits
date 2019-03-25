##############################################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Wed Feb 26 11:39:01 2014
#  Last Modified : <140226.1206>
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

snit::widget Tree {
    widgetclass Tree
    hulltype frame
    component treeview
    delegate option -cursor to treeview
    delegate option -takefocus to treeview
    delegate option -xscrollcommand to treeview
    delegate option -yscrollcommand to treeview
    delegate option -height to treeview
    delegate option -width to hull
    option -selectmode -readonly yes -default none \
          -type {snit::enum -values {none single multiple}} \
          -configuremethod _configure_selectmode
    method _configure_selectmode {o v} {
        set options($o) $v
        switch $v {
            none {$treeview configure -selectmode none}
            single {$treeview configure -selectmode browse}
            multiple {$treeview configure -selectmode extended}
        }
    }
    
    delegate method delete to treeview
    delegate method exists to treeview
    delegate method index to treeview
    delegate method see to treeview
    delegate method xview to treeview
    delegate method yview to treeview
    constructor {args} {
        install treeview using ttk::treeview $win.treeview \
              -columns {values} -displaycolumns {} -show tree
        pack $treeview -expand yes -fill both
        $self configurelist $args
    }
    method _get_at {x y} {
        return [$treeview identify row $x $y]
    }
    method bindImage {event script} {
        if {$script ne ""} {
            append script " \[[mymethod _get_at %x %y]\]"
        }
        #puts stderr "*** $self bindImage: script is $script"
        $treeview tag bind Image $event $script
    }
    method bindText {event script} {
        if {$script ne ""} {
            append script " \[[mymethod _get_at %x %y]\]"
        }
        #puts stderr "*** $self bindText: script is $script"
        $treeview tag bind Text $event $script
    }
    method insert {index parent item args} {
        if {$parent eq "root"} {set parent {}}
        $treeview insert $parent $index -id $item
        set tags {}
        foreach {opt value} $args {
            switch $opt {
                -data {
                    $treeview item $item -values [list $value]}
                -text {
                    $treeview item $item -text $value
                    if {$value ne ""} {
                        lappend tags Text
                    } else {
                        set i [lsearch $tags Text]
                        if {$i >= 0} {
                            set tags [lreplace $tags $i $i]
                        }
                    }
                }
                -image {
                    $treeview item $item -image $value
                    if {$value ne ""} {
                        lappend tags Image
                    } else {
                        set i [lsearch $tags Image]
                        if {$i >= 0} {
                            set tags [lreplace $tags $i $i]
                        }
                    }
                }
                -fill -
                -font -
                -indent -
                -window {}
            }
        }
        if {[llength $tags] > 0} {
            $treeview item $item -tags $tags
        }
    }
    method itemcget {item option} {
        switch $option {
            -data {
                return [lindex [$treeview item $item -values] 0]
            }
            -text {
                return [$treeview item $item -text]
            }
            -image {
                return [$treeview item $item -image]
            }
            default {return {}}
        }
    }
    method itemconfigure {item args} {
        if {[llength $args] == 0} {
            set result [list]
            foreach o {-data -text -image -fill -font -indent -window } {
                lappend result $o [$self itemcget $o]
            }
            return $result
        } else {
            foreach {opt value} {
                set tags [$treeview item $item -tags]
                switch $opt {
                    -data {
                        $treeview item $item -values [list $value]}
                    -text {
                        $treeview item $item -text $value
                        if {$value ne ""} {
                            lappend tags Text
                        } else {
                            set i [lsearch $tags Text]
                            if {$i >= 0} {
                                set tags [lreplace $tags $i $i]
                            }
                        }
                    }
                    -image {
                        $treeview item $item -image $value
                        if {$value ne ""} {
                            lappend tags Image
                        } else {
                            set i [lsearch $tags Text]
                            if {$i >= 0} {
                                set tags [lreplace $tags $i $i]
                            }
                        }
                    }
                    -fill -
                    -font -
                    -indent -
                    -window {}
                }
            }
            $treeview item $item -tags $tags
        }
    }
    method nodes {node args} {
        if {$node eq "root"} {set node {}}
        if {[llength $args] == 0} {
            return [$treeview children $node]
        } else {
            set allitems [$treeview children $node]
            if {[llength $args] == 1} {
                set first [lindex $args 0]
                if {[string is integer -strict $first]} {
                    return [lindex $allitems $first]
                } else {
                    return [lsearch -inline $allitems $first]
                }
            } elseif {[llength $args] == 2} {
                set first [lindex $args 0]
                set last [lindex $args 1]
                if {![string is integer -strict $first]} {
                    set first [lsearch $allitems $first]
                }
                if {![string is integer -strict $last]} {
                    set last [lsearch $allitems $last]
                }
                return [lrange $allitems $first $last]
            }
        }
    }
    method selection {cmd args} {
        switch $cmd {
            clear {$treeview selection set {}}
            set   {$treeview selection set $args}
            add   {$treeview selection add $args}
            remove {$treeview selection remove $args}
            get   {[return $treeview selection]}
        }
    }
    delegate method parent to treeview
    
}

package provide Tree 1.0
