// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Oct 21 16:43:24 2019
//  Last Modified : <210318.1402>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
//			51 Locke Hill Road
//			Wendell, MA 01379-9728
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// 
//
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <poll.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <utils/logging.h>
#include "CommandStationDCCMainTrack.hxx"


#define MAX_BUFFER_SIZE			512
#define RPMSG_BUF_SIZE MAX_BUFFER_SIZE
#define RPMSG_BUF_HEADER_SIZE           16
///char payload[RPMSG_BUF_SIZE - RPMSG_BUF_HEADER_SIZE];



CommandStationDCCMainTrack::CommandStationDCCMainTrack(Service *service, int pool_size)
      : StateFlow<Buffer<dcc::Packet>, QList<1>>(service)
, pool_(sizeof(Buffer<dcc::Packet>), pool_size)
{
}

void CommandStationDCCMainTrack::StartPRU()
{
    /* test that /dev/rpmsg_pru30 exists */
    static char firmwarePath[256];
    static uint8_t buffer[2048];
    int len;
    //fprintf(stderr,"*** CommandStationDCCMainTrack::StartPRU() (top if) : access(\"%s\",F_OK) returns %d\n",pruMessageDevice,access(pruMessageDevice,F_OK));
    if (access(pruMessageDevice,F_OK)) {
        FILE *sysfs_node, *firmfd;
        sysfs_node = fopen(pruState,"r+");
        fprintf(sysfs_node,"stop\n");
        fclose(sysfs_node);
        sysfs_node = fopen(pruFirmware, "w");
        if (sysfs_node == NULL) {
            LOG(FATAL, "CommandStationDCCMainTrack::StartPRU(): Cannot open firmware sysfs_node %s (%d)", pruFirmware, errno);
        }
        strcpy(&firmwarePath[0],firmwareName);
        firmfd = fopen(firmwarePath,"r");
        if (firmfd == NULL) {
            LOG(FATAL, "CommandStationDCCMainTrack::StartPRU(): Cannot open PRU program (%s)\n",firmwarePath);
        }
        while ((len = fread(buffer,sizeof(uint8_t),2048,firmfd))>0) {
            len = fwrite(buffer,sizeof(uint8_t),len,sysfs_node);
        }
        fclose(firmfd);
        fclose(sysfs_node);
        sysfs_node = fopen(pruState, "r+");
        if (sysfs_node == NULL) {
            LOG(FATAL, "CommandStationDCCMainTrack::StartPRU(): Cannot open firmware sysfs_node %s (%d)", pruState, errno);
        }
        fprintf(sysfs_node,"start\n");
        fclose(sysfs_node);
        /* give RPMSG time to initialize */
        sleep(3);
        //fprintf(stderr,"*** CommandStationDCCMainTrack::StartPRU() (bottom if) : access(\"%s\",F_OK) returns %d\n",pruMessageDevice,access(pruMessageDevice,F_OK));
        if (access(pruMessageDevice,F_OK)) {
            LOG(FATAL, "CommandStationDCCMainTrack::StartPRU(): Could not open %s (%d)", pruMessageDevice, errno);
        }
    }
    
}

StateFlowBase::Action CommandStationDCCMainTrack::entry()
{
    //auto *p = message()->data();
    // -- send p to the PRU to send to the track.
    return finish();
}
