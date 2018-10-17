#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Thu Oct 11 10:18:57 2018
#  Last Modified : <181017.1328>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
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


package require snit
package require ParseXML
package require OpenLCB_Common

namespace eval configuration {
    snit::type Configuration {
        pragma -hastypeinfo no -hastypedestroy no -hasinstances no
        
        typecomponent xmlconfig;#         Parsed Psuedo-CDI
        typemethod GetXMLConfig {} {return $xmlconfig}
        typecomponent editContextMenu
        typecomponent configuration;#     Parsed  XML configuration
        OpenLCB_Common::transportProcs
        OpenLCB_Common::identificationProcs
        typemethod GetTransport {transportConstructorVar transportOptsVar} {
            upvar $transportConstructorVar transportConstructor
            upvar $transportOptsVar transportOpts
            getTransport \
                  [$configuration getElementsByTagName "transport"] \
                  transportConstructor transportOpts
        }
        typemethod GetIdentification {nameVar descrVar} {
            upvar $nameVar name
            upvar $descrVar descriptor
            getIdentification \
                  [$configuration getElementsByTagName "identification"] \
                  name descriptor
        }
        typeconstructor {
            set xmlconfig [XmlConfiguration create %AUTO% {
<configure>
  <group tagname="turnout" repname="Turnout" mincount="2" maxcount="2">
    <string tagname="description">Description</string>
    <eventid tagname="normal">Motor Normal</eventid>
    <eventid tagname="reversed">Motor Reversed</eventid>
  </group>
  <group tagname="points" repname="Points" mincount="2" maxcount="2">
    <string tagname="description">Description</string>
    <eventid tagname="eventon">Points Normal</eventid>
    <eventid tagname="eventoff">Points Reversed</eventid>
  </group>
  <group tagname="occupancy" repname="OS Occupancy" mincount="1" maxcount="1">
    <string tagname="description">Description</string>
    <eventid tagname="eventoff">Block Clear</eventid>
    <eventid tagname="eventon">Block Occupied</eventid>
    <int tagname="debounce" default="3" min="1" max="8">Debounce parameter</int>
  </group>
  <group tagname="eastocc" repname="Next eastward (Points) Occupancy" mincount="1" maxcount="1">
    <string tagname="description">Description</string>
    <int tagname="debounce" default="3" min="1" max="8">Debounce parameter</int>
  </group>
    <group tagname="westmainocc" repname="Next westward (frog, main) Occupancy" mincount="1" maxcount="1">
    <string tagname="description">Description</string>
    <int tagname="debounce" default="3" min="1" max="8">Debounce parameter</int>
  </group>
  <group tagname="westdivocc" repname="Next westward (frog, diversion) Occupancy" mincount="1" maxcount="1">
    <string tagname="description">Description</string>
    <int tagname="debounce" default="3" min="1" max="8">Debounce parameter</int>
  </group>
    <group tagname="masts" repname="Masts" mincount="1" maxcount="1">
      <group tagname="points" repname="Points Mast (3 over 2)" mincount="1" maxcount="1">
        <string tagname="description">Description</string>
        <eventid tagname="stop">Stop Aspect</eventid>
        <eventid tagname="approachlimited">Approach Limited Aspect</eventid>
        <eventid tagname="approach">Approach Aspect</eventid>
        <eventid tagname="clear">Clear Aspect</eventid>
      </group>
      <group tagname="frogmain" repname="Frog Main Mast (3 over 1)" mincount="1" maxcount="1">
        <string tagname="description">Description</string>
        <eventid tagname="stop">Stop Aspect</eventid>
        <eventid tagname="approach">Approach Aspect</eventid>
        <eventid tagname="clear">Clear Aspect</eventid>
      </group>
        <group tagname="frogdiv" repname="Frog Divergence Mast (1 over 3)" mincount="1" maxcount="1">
        <string tagname="description">Description</string>
        <eventid tagname="stop">Stop Aspect</eventid>
        <eventid tagname="approach">Approach Aspect</eventid>
        <eventid tagname="clear">Clear Aspect</eventid>
      </group>
    </group>
    <group tagname="absslave" repname="Slave" mincount="0" maxcount="64">
      <string tagname="description">Description</string>
      <boolean tagname="enabled" default="false">Node Enabled</boolean>
      <int tagname="nodeid" default="255" min="0" max="63">Node ID</int>
      <eventid tagname="occupied">Block Occupied Event</eventid>
      <eventid tagname="clear">Block Clear Event</eventid>
      <eventid tagname="stopeast">East Stop Aspect</eventid>
      <eventid tagname="approacheast">East Approach Aspect</eventid>
      <eventid tagname="cleareast">East Clear Aspect</eventid>
      <eventid tagname="stopwest">West Stop Aspect</eventid>
      <eventid tagname="approachwest">West Approach Aspect</eventid>
      <eventid tagname="clearwest">West Clear Aspect</eventid>
    </group>
  </configure>}]
        }
        typemethod ReadConfigfile {conffile} {
            if {[catch {open $conffile r} conffp]} {
                ::log::logError [_ "Could not open %s because: %s" $conffile $conffp]
                exit 99
            }
            set confXML [read $conffp]
            close $conffp
            if {[catch {ParseXML create %AUTO% $confXML} configuration]} {
                ::log::logError [_ "Could not parse configuration file %s: %s" $conffile $configuration]
                exit 98
            }
        }
        typemethod ParseConfiguration {confXML} {
            if {[catch {ParseXML create %AUTO% $confXML} configuration]} {
                ::log::logError [_ "Could not parse configuration file %s: %s" $conffile $configuration]
                exit 98
            }
            return $configuration
        }
            
        typemethod GetConfiguration {} {
            return $configuration
        }
        
        #*** Configuration GUI
        
        typecomponent main;# Main Frame.
        typecomponent scroll;# Scrolled Window.
        typecomponent editframe;# Scrollable Frame
        
        typecomponent   generateEventID
        
        typevariable status {};# Status line
        typevariable conffilename {};# Configuration File Name
        
        #** Menu.
        typevariable _menu {
            "[_m {Menu|&File}]" {file:menu} {file} 0 {
                {command "[_m {Menu|File|&Save}]" {file:save} "[_ {Save}]" {Ctrl s} -command "[mytypemethod _save]"}
                {command "[_m {Menu|File|Save and Exit}]" {file:saveexit} "[_ {Save and exit}]" {} -command "[mytypemethod _saveexit]"}
                {command "[_m {Menu|File|&Exit}]" {file:exit} "[_ {Exit}]" {Ctrl q} -command "[mytypemethod _exit]"}
            } "[_m {Menu|&Edit}]" {edit} {edit} 0 {
                {command "[_m {Menu|Edit|Cu&t}]" {edit:cut edit:havesel} "[_ {Cut selection to the paste buffer}]" {Ctrl x} -command {StdMenuBar EditCut} -state disabled}
                {command "[_m {Menu|Edit|&Copy}]" {edit:copy edit:havesel} "[_ {Copy selection to the paste buffer}]" {Ctrl c} -command {StdMenuBar EditCopy} -state disabled}
                {command "[_m {Menu|Edit|&Paste}]" {edit:paste} "[_ {Paste selection from the paste buffer}]" {Ctrl c} -command {StdMenuBar EditPaste}}
                {command "[_m {Menu|Edit|C&lear}]" {edit:clear edit:havesel} "[_ {Clear selection}]" {} -command {StdMenuBar EditClear} -state disabled}
                {command "[_m {Menu|Edit|&Delete}]" {edit:delete edit:havesel} "[_ {Delete selection}]" {Ctrl d}  -command {StdMenuBar EditClear} -state disabled}
                {separator}
                {command "[_m {Menu|Edit|Select All}]" {edit:selectall} "[_ {Select everything}]" {} -command {StdMenuBar EditSelectAll}}
                {command "[_m {Menu|Edit|De-select All}]" {edit:deselectall edit:havesel} "[_ {Select nothing}]" {} -command {StdMenuBar EditSelectNone} -state disabled}
            } 
        }
        
        typemethod edit_checksel {} {
            if {[catch {selection get}]} {
                $main setmenustate edit:havesel disabled
            } else {
                $main setmenustate edit:havesel normal
            }
        }
        # Default (empty) XML Configuration.
        typevariable default_confXML {<?xml version='1.0'?><OpenLCB_Halfsiding/>}
        typemethod SampleConfiguration {conffile} {
            #** Generate a Sample Configuration
            #
            # @param conffile Name of the configuration file.
            #
            package require GenerateEventID 1.0
            set conffilename $conffile
            set confXML $default_confXML
            if {[file exists $conffilename]} {
                puts -nonewline stdout [_ {Configuration file (%s) already exists. Replace it [yN]? } $conffilename]
                flush stdout
                set answer [string toupper [string index [gets stdin] 0]]
                if {$answer ne "Y"} {exit 1}
            }
            set configuration [ParseXML create %AUTO% $confXML]
            set cdis [$configuration getElementsByTagName OpenLCB_Halfsiding -depth 1]
            set cdi [lindex $cdis 0]
            SampleTransport $cdi
            SampleItentification $cdi
            set generateEventID [GenerateEventID create %AUTO% \
                                 -baseeventid [lcc::EventID create %AUTO% \
                                               -eventidstring "05.01.01.01.22.00.00.00"]]
            $xmlconfig configure -eventidgenerator $generateEventID
            $xmlconfig SampleConfiguration $cdi
            set attrs [$cdi cget -attributes]
            lappend attrs lastevid [$generateEventID currentid]
            $cdi configure -attributes $attrs
            if {![catch {open $conffilename w} conffp]} {
                puts $conffp {<?xml version='1.0'?>}
                $configuration displayTree $conffp
                close $conffp
            }
            ::exit
        }
        

        typemethod ConfiguratorGUI {conffile} {
            #** Configuration GUI
            # 
            # Create the Configuration tool GUI.
            #
            # @param conffile Name of the configuration file.
            
            package require Tk
            package require tile
            package require ParseXML
            package require LabelFrames
            package require ScrollableFrame
            package require ScrollWindow
            package require MainFrame
            package require snitStdMenuBar
            package require ButtonBox
            package require ScrollTabNotebook
            package require HTMLHelp 2.0
            package require GenerateEventID 1.0
            
            set editContextMenu [StdEditContextMenu .editContextMenu]
            $editContextMenu bind Entry
            $editContextMenu bind TEntry
            $editContextMenu bind Text
            $editContextMenu bind ROText
            $editContextMenu bind Spinbox
            
            set conffilename $conffile
            set confXML $default_confXML
            if {![catch {open $conffile r} conffp]} {
                set confXML [read $conffp]
                close $conffp
            }
            if {[catch {ParseXML create %AUTO% $confXML} configuration]} {
                set confXML $default_confXML
                set configuration [ParseXML create %AUTO% $confXML]
            }
            set cdis [$configuration getElementsByTagName OpenLCB_Halfsiding -depth 1]
            if {[llength $cdis] != 1} {
                error [_ "There is no OpenLCB_Halfsiding container in %s" $confXML]
                exit 90
            }
            set cdi [lindex $cdis 0]
            wm protocol . WM_DELETE_WINDOW [mytypemethod _saveexit]
            wm title    . [_ "OpenLCB_Halfsiding Configuration Editor (%s)" $conffile]
            set main [MainFrame .main -menu [subst $_menu] \
                      -textvariable [mytypevar status]]
            pack $main -expand yes -fill both
            [$main getmenu edit] configure -postcommand [mytypemethod edit_checksel]
            set f [$main getframe]
            set scroll [ScrolledWindow $f.scroll -scrollbar vertical \
                        -auto vertical]
            pack $scroll -expand yes -fill both
            set editframe [ScrollableFrame \
                           [$scroll getframe].editframe -constrainedwidth yes]
            $scroll setwidget $editframe
            set frame [$editframe getframe]
            TransportGUI $frame $cdi
            set lastevid [$cdi attribute lastevid]
            if {$lastevid eq {}} {
                set nidindex [lsearch -exact $transopts -nid]
                if {$nidindex >= 0} {
                    incr nidindex
                    set nid [lindex $transopts $nidindex]
                } else {
                    set nid "05:01:01:01:22:00"
                }
                set evlist [list]
                foreach oct [lrange [regexp -inline [::lcc::nid cget -regexp] $nid] 1 end] {
                    lappend evlist [scan $oct %02x]
                }
                lappend evlist 0 0
                set generateEventID [GenerateEventID create %AUTO% \
                                     -baseeventid [lcc::EventID create %AUTO% -eventidlist $evlist]]
            } else {
                set generateEventID [GenerateEventID create %AUTO% \
                                     -baseeventid [lcc::EventID create %AUTO% -eventidstring $lastevid]]
            }
            $xmlconfig configure -eventidgenerator $generateEventID
            IdentificationGUI $frame $cdi
            pack [$xmlconfig createGUINoNoteBook $frame $cdi] \
                  -expand yes -fill both
        }
        typevariable warnings
        typemethod _saveexit {} {
            #** Save and Exit.  Bound to the Save and Exit file menu item
            # Saves the contents of the GUI as an XML file and then exits.
            
            if {[$type _save]} {
                $type _exit
            }
        }
        typemethod _save {} {
            #** Save.  Bound to the Save file menu item.
            # Saves the contents of the GUI as an XML file.
            
            set warnings 0
            set cdis [$configuration getElementsByTagName OpenLCB_Halfsiding -depth 1]
            set cdi [lindex $cdis 0]
            set lastevid [$cdi attribute lastevid]
            if {$lastevid eq {}} {
                set attrs [$cdi cget -attributes]
                lappend attrs lastevid [$generateEventID currentid]
                $cdi configure -attributes $attrs
            } else {
                set attrs [$cdi cget -attributes]
                set findx [lsearch -exact $attrs lastevid]
                incr findx
                set attrs [lreplace $attrs $findx $findx [$generateEventID currentid]]
                $cdi configure -attributes $attrs
            }
            CopyTransFromGUI $cdi
            CopyIdentFromGUI $cdi
            $xmlconfig copyFromGUI [$editframe getframe] $cdi warnings
        
            if {$warnings > 0} {
                tk_messageBox -type ok -icon info \
                      -message [_ "There were %d warnings.  Please correct and try again." $warnings]
                return no
            }
            if {![catch {open $conffilename w} conffp]} {
                puts $conffp {<?xml version='1.0'?>}
                $configuration displayTree $conffp
                close $conffp
            }
            return yes
        }
        typemethod _exit {} {
            #** Exit function.  Bound to the Exit file menu item.
            # Does not save the configuration data!
            
            ::exit
        }
        typemethod GetConfigurationElement {tag {index 0}} {
            ::log::log debug "*** $type GetConfigurationElement $tag $index"
            if {$index eq "*"} {
                return [$configuration getElementsByTagName $tag]
            } else {
                return [lindex [$configuration getElementsByTagName $tag] \
                        $index]
            }
        }
    }
}

snit::macro configuration::ConfigOptions {configureMethod} {
    option -configuration -default {} -readonly yes \
          -type SimpleDOMElement \
          -configuremethod $configureMethod
}


package provide Config 1.0

            
