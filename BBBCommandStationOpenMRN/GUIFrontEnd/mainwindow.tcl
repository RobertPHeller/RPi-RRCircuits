#* 
#* ------------------------------------------------------------------
#* mainwindow.tcl - General purpose main window
#* Created by Robert Heller on Mon Feb 27 13:14:39 2006
#* ------------------------------------------------------------------
#* Modification History: $Log$
#* Modification History: Revision 1.3  2007/04/19 17:23:23  heller
#* Modification History: April 19 Lock Down
#* Modification History:
#* Modification History: Revision 1.2  2006/05/16 19:27:46  heller
#* Modification History: May162006 Lockdown
#* Modification History:
#* Modification History: Revision 1.1  2006/03/06 18:46:20  heller
#* Modification History: March 6 lockdown
#* Modification History:
#* Modification History: Revision 1.1  2002/07/28 14:03:50  heller
#* Modification History: Add it copyright notice headers
#* Modification History:
#* ------------------------------------------------------------------
#* Contents:
#* ------------------------------------------------------------------
#*  
#*     Model RR System, Version 2
#*     Copyright (C) 1994,1995,2002-2005  Robert Heller D/B/A Deepwoods Software
#* 			51 Locke Hill Road
#* 			Wendell, MA 01379-9728
#* 
#*     This program is free software; you can redistribute it and/or modify
#*     it under the terms of the GNU General Public License as published by
#*     the Free Software Foundation; either version 2 of the License, or
#*     (at your option) any later version.
#* 
#*     This program is distributed in the hope that it will be useful,
#*     but WITHOUT ANY WARRANTY; without even the implied warranty of
#*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#*     GNU General Public License for more details.
#* 
#*     You should have received a copy of the GNU General Public License
#*     along with this program; if not, write to the Free Software
#*     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#* 
#*  
#* 

package require snit
package require Tk
package require tile
package require gettext
package require MainFrame
package require snitStdMenuBar
package require ScrollWindow
package require ButtonBox
package require ROText

## @addtogroup TclCommon
# @{

snit::widgetadaptor cmdmainwindow {
## A widget that is heavily extended from the BWidget MainFrame windget.
# This widget starts with a MainFrame, and adds a paned window with a
# scrolled window and a button menu, and zero or more slide out frames.
# also management methods for toolbars and for menus.
#
# @param path The widget path.
# @param ... Options:
# @arg -menu The basic MainFrame -menu option.  Defaults to the Motif
#		 standard set of menus (File, Edit, View, Options, and Help).
# @arg -extramenus Like the basic MainFrame -menu option, but can be used
# 	   when the just additional menus need to be added to the standard
#	   set.
# @arg -height Widget height.  Delegated to the hull (MainFrame) widget.
# @arg -width Widget width.  Delegated to the hull (MainFrame) widget.
# @arg -separator Include a separator between windows on the MainFrame
#		widget.
# @arg -dontwithdraw Boolean to suppress withdrawing the toplevel while
#		it is being built.
# @arg -scrolling Boolean to enable the scrollwindow.
#
# @author Robert Heller \<heller\@deepsoft.com\>
#
# @section mainwindow_package Package provided
#
# MainWindow 1.0
#

  option -menu \
	-readonly yes \
	-default {
	   "&File" {file:menu} {file} 0 {
	        {command "&New"     {file:new} ""     {Ctrl n}}
	        {command "&Open..." {file:open} "" {Ctrl o}}
	        {command "&Save"    {file:save} "" {Ctrl s}}
		{command "Save &As..." {file:saveas} "" {Ctrl a}}
		{command "&Print..." {file:print} "" {Ctrl p}}
	        {command "&Close" {file:close} "Close the application" {Ctrl c}}
	        {command "E&xit" {file:exit} "Exit the application" {Ctrl q}}
	    }
	    "&Edit" {edit:menu} {edit} 0 {
		{command "&Undo" {edit:undo} "Undo last change" {Ctrl z}}
		{command "Cu&t" {edit:cut edit:havesel} "Cut selection to the paste buffer" {Ctrl x} -command {StdMenuBar EditCut}}
		{command "&Copy" {edit:copy edit:havesel} "Copy selection to the paste buffer" {Ctrl c} -command {StdMenuBar EditCopy}}
		{command "&Paste" {edit:paste edit:havesel} "Paste in the paste buffer" {Ctrl v} -command {StdMenuBar EditPaste}}
		{command "C&lear" {edit:clear edit:havesel} "Clear selection" {} -command {StdMenuBar EditClear}}
		{command "&Delete" {edit:delete edit:havesel} "Delete selection" {Ctrl d}}
		{separator}
		{command "Select All" {edit:selectall} "Select everything" {}}
		{command "De-select All" {edit:deselectall edit:havesel} "Select nothing" {}}
	    }
	    "&View" {view:menu} {view} 0 {
	    }
	    "&Options" {options:menu} {options} 0 {
	    }
	    "&Help" {help:menu} {help} 0 {
		{command "On &Help..." {help:help} "Help on help" {}}
		{command "On &Keys..." {help:keys} "Help on keyboard accelerators" {}}
		{command "&Index..." {help:index} "Help index" {}}
		{command "&Tutorial..." {help:tutorial} "Tutorial" {}}
		{command "On &Version" {help:version} "Version" {}}
		{command "Warranty" {help:warranty} "Warranty" {}}
		{command "Copying" {help:copying} "Copying" {}}
	    }
	}
  option {-extramenus extraMenus ExtraMenus} \
	-readonly yes \
	-default {}
  delegate option -height to hull
  delegate option -width  to hull
  option -separator -default both
  option {-dontwithdraw dontWithdraw DontWithdraw} -readonly yes -default 0
  option -scrolling -readonly yes -default yes -type snit::boolean
  delegate method {mainframe *} to hull except {getframe addtoobar gettoolbar 
						showtoolbar}
  component scrollwindow
  ## @privatesection ScrollWindow component.
  delegate method {scrollwindow *} to scrollwindow
  component wipmessage
  ## Work-In-Progress message component.
  delegate method {wipmessage *} to wipmessage
  component upper
  component lower
  
  component panewindow
  ## PaneWindow holding the log area at the bottom and the other widgets above.
  # and the slideouts).
  component log 
  delegate method {log *} to log
  ## Log window (below)
  component commandframe
  ## PaneWindow holding additional widgets (above)
  variable progress
  ## The value of the progreee bar.
  variable status
  ## Status value.
  component toolbar
  ## toolbar

  method {toolbar show} {} {
      $hull showtoolbar 0 yes
  }
  method {toolbar hide} {} {
      $hull showtoolbar 0 no
  }
  method {toolbar setbuttonstate} {state} {
      foreach b [winfo children $toolbar] {
          catch [list $b configure -state $state]
      }
  }
  method {toolbar addbutton} {bname args} {
      set helptext [from args -helptext]
      set helptype [from args -helptype]
      set helpvar  [from args -helpvar]
      if {[string length "$helptext"]} {
          #lappend args -helptext "$helptext" -helptype variable \
          #-helpvar [myvar status]
      }
      pack [eval [list ttk::button $toolbar.$bname -style Toolbutton] $args] -side left
  }
  method {toolbar buttonconfigure} {bname args} {
  ## Method to configure a button on a toolbar.
  # @param bname The name of the button.
  # @param ... Button configuration options (passed to configure).

  set helptext [from args -helptext]
  set helptype [from args -helptype]
  set helpvar  [from args -helpvar]
  if {[string length "$helptext"]} {
      lappend args -helptext "$helptext" -helptype variable \
            -helpvar [myvar status]
      }
      return [eval [list $toolbar.$bname configure] $args]
  }

  method {toolbar buttoncget} {bname option} {
      ## Method to get a configuration option of a button on a toolbar.
      # @param bname The name of the button.
      # @param option Button configuration option (passed to cget).
      
      return [$toolbar.$bname cget $option]
  }
  
  method {menu activate} {menuid index} {
  ## Method to activate a menu on the main frame.
  # @param menuid Menu id.
  # @param index Menu item index.

    set menu [$hull getmenu $menuid]
    return [eval [list $menu activate $index]]
  }

  method {menu add} {menuid entrytype args} {
  ## Method to add a menu entry to a menu on the main frame.
  # @param menuid Menu id.
  # @param entrytype The type of entry.
  # @param ... The arguments to pass to the  entry creation command.

    set menu [$hull getmenu $menuid]
#    puts stderr "*** ${type}::menu add (before from): args = $args"
    set dynhelp [from args -dynamichelp]
#    puts stderr "*** ${type}::menu add (after from): args = $args, dynhelp = $dynhelp"
    set res [eval [list $menu add $entrytype] $args]
    if {[string length "$dynhelp"]} {
      DynamicHelp add $menu -index [$menu index end] \
			     -variable [myvar status] \
			     -text "$dynhelp"
    }
    return $res
  }

  method {menu delete} {menuid index args} {
  ## Method to delete an entry from a menu.
  # @param menuid Menu id.
  # @param index Menu item index.
  # @param ... The arguments to pass to the menu delete command.
  
    set menu [$hull getmenu $menuid]
    return [eval [list $menu delete $index] $args]
  }

  method {menu entrycget} {menuid index option} {
  ## Method to get an option value of a menu entry.
  # @param menuid Menu id.
  # @param index Menu item index.
  # @param option The option to fetch.

    set menu [$hull getmenu $menuid]
    return [eval [list $menu entrycget $index $option]]
  }

  method {menu entryconfigure} {menuid index args} {
  ## Method to configure options of a menu entry.
  # @param menuid Menu id.
  # @param index Menu item index.
  # @param ... The arguments to pass on to entryconfigure.

    set menu [$hull getmenu $menuid]
    set dynhelp [from args -dynamichelp]
    if {[string length "$dynhelp"]} {
      DynamicHelp add $menu -index [$menu index $index] \
			     -variable [myvar status] \
			     -text "$dynhelp"
    }
    return [eval [list $menu entryconfigure $index] $args]
  }

  method {menu sethelpvar} {menuid} {
  ## Method to set the bind the status line to the help variable of the menu.
  # @param menuid Menu id.

    set menu [$hull getmenu $menuid]
    DynamicHelp add $menu -variable [myvar status]
  }

  method {menu index} {menuid index} {
  ## Method to get the index of a menu entry.
  # @param menuid Menu id.
  # @param index  The index of the menu entry.

    set menu [$hull getmenu $menuid]
    return [eval [list $menu index $index]]
  }

  method {menu insert} {menuid index entrytype args} {
  ## Method to insert a menu entry to a menu on the main frame.
  # @param menuid Menu id.
  # @param index  The index to insert before.
  # @param entrytype The type of entry.
  # @param ... The arguments to pass to the  entry creation command.

    set menu [$hull getmenu $menuid]
    set dynhelp [from args -dynamichelp]
    set index [$menu index $index]
    set res [eval [list $menu insert $index $entrytype] $args]
    if {[string length "$dynhelp"]} {
      DynamicHelp add $menu -index $index \
			     -variable [myvar status] \
			     -text "$dynhelp"
    }
    return $res
  }

  method {menu invoke} {menuid index} {
  ## Method to invoke a menu entry.
  # @param menuid Menu id.
  # @param index  The index to invoke.

    set menu [$hull getmenu $menuid]
    return [eval [list $menu invoke $index]]
  }

  method {menu type} {menuid index} {
  ## Method to return the type of a menu entry.
  # @param menuid Menu id.   
  # @param index  The index to get the type of.

    set menu [$hull getmenu $menuid]
    return [eval [list $menu type $index]]
  }

  method showit {{extraX 0}} {
  ## Method to show the main window.
  # @param extraX Extra width to add when computing the position to map the 
  #		window at.  Defaults to 0.

    set toplevel [winfo toplevel $win]
    if {![string equal [wm state $toplevel] {withdrawn}]} {return}
    update idle
    set x [expr {[winfo screenwidth $toplevel]/2 - ([winfo reqwidth $toplevel]+$extraX)/2 \
	    - [winfo vrootx $toplevel]}]
    set y [expr {[winfo screenheight $toplevel]/2 - [winfo reqheight $toplevel]/2 \
	    - [winfo vrooty $toplevel]}]
    if {$x < 0} {set x 0}
    if {$y < 0} {set y 0}
    wm geom $toplevel +$x+$y
    wm deiconify $toplevel
  }

  method setstatus {statusmessage} {
  ## Method to set the status message.
  # @param statusmessage The status message to display.

    set status "$statusmessage"
  }

  method setprogress {progressvalue} {
  ## Method to set the progress bar value.
  # @param progressvalue The amount of the progress.

    if {![string is integer -strict "$progressvalue"]} {
      error "Expected an integer, got $progressvalue"
    }
    if {$progressvalue < 0 || $progressvalue > 100} {
      error "Expected an integer between 0 and 100, got $progressvalue"
    }
    set progress $progressvalue
  }
  delegate method {upper *} to upper
  method getframe {} {return $upper}
  constructor {args} {
  ## Constructor build a full featured main window.
  # @param ...  Option value list.

#    puts stderr "*** ${type}::constructor $args"
    set options(-dontwithdraw) [from args -dontwithdraw]
#    puts stderr "*** ${type}::constructor: options(-dontwithdraw) = $options(-dontwithdraw)"
    set options(-menu) [from args -menu]
    set options(-extramenus) [from args -extramenus]
    if {[llength $options(-extramenus)] > 0} {
      set helpIndex [lsearch -exact $options(-menu) "&Help"]
      set menudesc  [eval [list linsert $options(-menu) $helpIndex] \
                     $options(-extramenus)]
    } else {
      set menudesc $options(-menu)
    }
    #puts stderr "*** $type create $self: menudesc = $menudesc (length is [llength $menudesc])"
    set options(-separator) [from args -separator]
    set status {}
    set progress 0
    installhull using MainFrame -menu $menudesc \
			-separator $options(-separator) \
			-textvariable [myvar status] \
			-progressvar [myvar progress] \
			-progressmax 100 \
			-progresstype normal
    $hull showstatusbar progression
    set toolbar [$hull addtoolbar]
    set toplevel [winfo toplevel $win]
#    puts stderr "*** ${type}::constructor: wm state $toplevel = [wm state $toplevel]"
    if {!$options(-dontwithdraw)} {wm withdraw $toplevel}
    set frame [$hull getframe]
    install panewindow using ttk::panedwindow $frame.panewindow -orient vertical
    pack $panewindow -expand yes -fill both
    install upper using ttk::panedwindow $panewindow.upper -orient horizontal
    $panewindow add $upper -weight 4
    install lower using ttk::frame $panewindow.lower
    $panewindow add $lower -weight 1
    set options(-scrolling) [from args -scrolling]
    install scrollwindow using ScrolledWindow $lower.scrollwindow \
              -scrollbar both -auto both -managed $options(-scrolling)
    pack $scrollwindow -fill both -expand yes
    install log using ROText [$scrollwindow getframe].log -height 3
    $scrollwindow setwidget $log
    install wipmessage using message $frame.wipmessage \
		-aspect 1500 -anchor w -justify left
    pack $wipmessage -fill x
    $self configurelist $args
#    update
  }

}


## @}

package provide CMDMainWindow 1.0

