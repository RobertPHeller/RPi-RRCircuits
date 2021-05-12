// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon May 10 14:12:56 2021
//  Last Modified : <210512.1037>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2021  Robert Heller D/B/A Deepwoods Software
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

#ifndef __COMMANDSTATIONDCCPRUTRACK_HXX
#define __COMMANDSTATIONDCCPRUTRACK_HXX

#include "executor/Executor.hxx"
#include "executor/StateFlow.hxx"
#include "dcc/Packet.hxx"
#include "utils/Singleton.hxx"
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


// Interface to a PRU to generate DCC pulse waveforms.
// PRU_NUM is 0 or 1 and identifies the PRU number to initialize and
// interface with.

template <uint8_t PRU_NUM>
class CommandStationDCCPRUTrack : 
public StateFlow<Buffer<dcc::Packet>
, QList<1>>
//, Singleton<CommandStationDCCPRUTrack<uint8_t PRU_NUM>>
{
public:
    // Force compile error if PRU_NUM is not 0 or 1.
    static_assert(PRU_NUM < 2, "Only 2 PRUs, 0 and 1!");
    static constexpr const uint8_t PRU = PRU_NUM;
    CommandStationDCCPRUTrack(Service *service, int pool_size, 
                              const char *firmwareName)
                : StateFlow<Buffer<dcc::Packet>, QList<1>>(service)
          , pool_(sizeof(Buffer<dcc::Packet>), pool_size)
    {
        HASSERT(! hasInstance_);
        hasInstance_ = true;
        snprintf(pruFirmware,sizeof(pruFirmware),pruFirmwareFMT,PRU);
        snprintf(pruState,sizeof(pruState),pruStateFMT,PRU);
        snprintf(pruMessageDevice,sizeof(pruMessageDevice),pruMessageDeviceFMT,PRU);
        StartPRU(firmwareName);
    }
    ~CommandStationDCCPRUTrack()
    {
        hasInstance_ = false;
        FILE *sysfs_node = fopen(pruState,"r+");
        if (sysfs_node == NULL) {
            LOG(FATAL, "CommandStationDCCPRUTrack::StartPRU(): Cannot open firmware sysfs_node %s (%d)", pruState, errno);
        }
        fprintf(sysfs_node,"stop\n");
        fclose(sysfs_node);
    }
    FixedPool *pool() OVERRIDE
    {
        return &pool_;
    }
protected:
    Action entry() OVERRIDE
    {
        dcc::Packet *p = message()->data();
        // -- send p to the PRU to send to the track.
        int f = open(pruMessageDevice,O_WRONLY);
        if (f < 0) {
            LOG(FATAL, "CommandStationDCCPRUTrack::entry(): Could not open %s (%d)", pruMessageDevice, errno);
        }
        int status = write(f,(const void *)p,sizeof(dcc::Packet));
        if (status < (int)sizeof(dcc::Packet)) {
            LOG(FATAL, "CommandStationDCCPRUTrack::entry(): Could not write a packet to %s (%d)", pruMessageDevice, errno);
        }
        status = close(f);
        if (status < 0) {
            LOG(FATAL, "CommandStationDCCPRUTrack::entry(): Could not close %s (%d)", pruMessageDevice, errno);
        }
        return finish();
    }
    
    /// @return next action.
    Action finish()
    {
        return release_and_exit();
    }
    /// Packet pool from which to allocate packets.
    FixedPool pool_;
private:
    void StartPRU(const char *firmwareName)
    {
        static char firmwarePath[256];
        static uint8_t buffer[2048];
        int len;
        FILE *sysfs_node, *firmfd;
        sysfs_node = fopen(pruState,"r+");
        if (sysfs_node == NULL) {
            LOG(FATAL, "CommandStationDCCPRUTrack::StartPRU(): Cannot open firmware sysfs_node %s (%d)", pruState, errno);
        }
        fprintf(sysfs_node,"stop\n");
        fclose(sysfs_node);
        sysfs_node = fopen(pruFirmware, "w");
        if (sysfs_node == NULL) {
            LOG(FATAL, "CommandStationDCCPRUTrack::StartPRU(): Cannot open firmware sysfs_node %s (%d)", pruFirmware, errno);
        }
        strcpy(&firmwarePath[0],firmwareName);
        LOG(INFO,"[CommandStationDCCPRUTrack] StartPRU with %s",firmwarePath);
        firmfd = fopen(firmwarePath,"r");
        if (firmfd == NULL) {
            LOG(FATAL, "CommandStationDCCPRUTrack::StartPRU(): Cannot open PRU program (%s)\n",firmwarePath);
        }
        while ((len = fread(buffer,sizeof(uint8_t),2048,firmfd))>0) {
            len = fwrite(buffer,sizeof(uint8_t),len,sysfs_node);
        }
        fclose(firmfd);
        fclose(sysfs_node);
        sysfs_node = fopen(pruState, "r+");
        if (sysfs_node == NULL) {
            LOG(FATAL, "CommandStationDCCPRUTrack::StartPRU(): Cannot open firmware sysfs_node %s (%d)", pruState, errno);
        }
        fprintf(sysfs_node,"start\n");
        fclose(sysfs_node);
        /* give RPMSG time to initialize */
        sleep(3);
        //fprintf(stderr,"*** CommandStationDCCPRUTrack::StartPRU() (bottom if) : access(\"%s\",F_OK) returns %d\n",pruMessageDevice,access(pruMessageDevice,F_OK));
        if (access(pruMessageDevice,F_OK)) {
            LOG(FATAL, "CommandStationDCCPRUTrack::StartPRU(): Could not open %s (%d)", pruMessageDevice, errno);
        }
    }
    static constexpr char const *pruFirmwareFMT =  "/lib/firmware/am335x-pru%d-fw";
    static constexpr char const *pruStateFMT = "/dev/remoteproc/pruss-core%d/state";
    static constexpr char const *pruMessageDeviceFMT = "/dev/rpmsg_pru3%d";
    char pruFirmware[30];
    char pruState[36];
    char pruMessageDevice[20];
    static bool hasInstance_;
};

#endif // __COMMANDSTATIONDCCPRUTRACK_HXX

