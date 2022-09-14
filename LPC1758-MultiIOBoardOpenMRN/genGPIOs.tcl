#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon Sep 12 19:41:31 2022
#  Last Modified : <220912.1944>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2022  Robert Heller D/B/A Deepwoods Software
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


for {set P 0} {$P < 3} {incr P} {
    for {set B 0} {$B < 8} {incr B} {
        puts [format {GPIO_PIN(C0P%dB%d, INPUT_PIN, port, num);} $P $B]
    }
}

for {set P 0} {$P < 3} {incr P} {
    for {set B 0} {$B < 8} {incr B} {
        puts [format {GPIO_PIN(C1P%dB%d, OUTPUT_PIN, port, num);} $P $B]
    }
}

