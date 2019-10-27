/* -*- C -*- ****************************************************************
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : Robert Heller
 *  Created       : Sun Oct 27 19:56:12 2019
 *  Last Modified : <191027.2051>
 *
 *  Description	
 *
 *  Notes
 *
 *  History
 *	
 ****************************************************************************
 *
 *    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
 *			51 Locke Hill Road
 *			Wendell, MA 01379-9728
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * 
 *
 ****************************************************************************/

/** Code lifted from the Userspace-Arduino code. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "AnalogReadSysFS.h"




int sysfs_read(const char *path, const char *filename, char *value)
{
    FILE *fd;
    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), "%s%s", path, filename);
    fd = fopen(buf, "r");
    if (fd == NULL) {
        perror(buf);
        return -1;
    }
    fscanf(fd, "%s", value);
    if (fclose(fd) != 0)
        perror(buf);
    return 0;
}



/* sysfs_adc_getvalue
 *  * Gets a value from the sysfs adc entries
 *  * Returns a value on the scale of (0 to 4095)
 *  */
uint32_t sysfs_adc_getvalue(uint32_t channel)
{
    char buf[MAX_BUF], channelname[20], value[8];
    snprintf(buf, sizeof(buf), SYSFS_ADC_DIR"/");
    snprintf(channelname, sizeof(channelname), "in_voltage%d_raw", channel);
    sysfs_read(buf, channelname, value);
    return atol(value);
}

