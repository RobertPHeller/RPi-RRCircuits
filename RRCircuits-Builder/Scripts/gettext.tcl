#* 
#* ------------------------------------------------------------------
#* gettext.tcl - Gettext (msgcat) interface code.
#* Created by Robert Heller on Wed Mar 23 10:13:56 2011
#* ------------------------------------------------------------------
#* Modification History: $Log$
#* Modification History: Revision 1.1  2002/07/28 14:03:50  heller
#* Modification History: Add it copyright notice headers
#* Modification History:
#* ------------------------------------------------------------------
#* Contents:
#* ------------------------------------------------------------------
#*  
#*     Model RR System, Version 2
#*     Copyright (C) 2011  Robert Heller D/B/A Deepwoods Software
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

## @addtogroup TclCommon
# @{

package require msgcat

namespace eval gettext {
## Localization functions.  Functions to fetch localized messages from the 
# message catalog.
  #
  #  @author Robert Heller @<heller\@deepsoft.com@>
  # 
  #  @section gettext_package Package provided
  #
  #  gettext 1.0
  #

proc _m {msgid args} {
  ## Handle messages with a context hint prefix (eg Label|lab).
  set msgval [eval [list ::msgcat::mc "$msgid"] $args]
  set msgval [lindex [split $msgval {|}] end]
  return $msgval
}

proc _mx {args} {
  ## Get maxlength of a set of messages with a context hint prefix.  Used to compute
  # label widths.
  set result 0
  foreach tr $args {
    set len [string length [_m $tr]]
    if {$len > $result} {set result $len}
  }
  return $result
}
proc _ args {
  ## Get a localized from the message catalog and deal with formating
  # possible arguments, by calling ::msgcat::mc.
  return [eval ::msgcat::mc $args]
}

namespace export _*

}

namespace import ::gettext::_*

## @}

package provide gettext 1.0
