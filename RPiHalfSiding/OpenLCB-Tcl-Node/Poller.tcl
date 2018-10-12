#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon Oct 8 23:21:13 2018
#  Last Modified : <181012.1003>
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

snit::macro poller::Poller {{pollmethod poll_33hz}} {
    method _poll30 {} "\$self $pollmethod;after 30 \[mymethod _poll30\]"
    method start_polling {} {
        after 30 [mymethod _poll30]
    }
}

snit::macro poller::TypePoller {{pollmethod poll_33hz}} {
    typemethod _poll30 {} "\$type $pollmethod;after 30 \[mytypemethod _poll30\]"
    typemethod start_polling {} {
        after 30 [mytypemethod _poll30]
    }
}

package provide Poller 1.0
