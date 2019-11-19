// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Nov 18 23:46:08 2019
//  Last Modified : <191119.0032>
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

class DCCPru {
public:
    DCCPru(const char *firmwareName, int channel, int procnum)
                : firmwareName_(firmwareName)
          , channel_(channel)
          , procnum_(procnum)
    {
    }
    void StartPRU()
    {
        char buffer[128];
        int wrote;
        snprintf(buffer,sizeof(buffer),"/dev/rpmsg_pru%d",channel_);
        if (access(buffer,F_OK)) {
            FILE *sysfs_node;
            printf("%s does not exist, loading and starting PRU firmware\n",buffer);
            snprintf(buffer,sizeof(buffer),"/sys/class/remoteproc/remoteproc%d/firmware",procnum_);
            sysfs_node = fopen(buffer, "r+");
            if (sysfs_node == NULL) {
                printf("Cannot open firmware sysfs_node %s (%d)\n", buffer, errno);
                exit(99);
            }
            wrote = fwrite(firmwareName_,sizeof(uint8_t),strlen(firmwareName_),sysfs_node);
            if (wrote < strlen(firmwareName_)) {
                printf("Error writing %s: %d\n",firmwareName_,errno);
            }
            fclose(sysfs_node);
            snprintf(buffer,sizeof(buffer),"/sys/class/remoteproc/remoteproc%d/state",procnum_);
            sysfs_node = fopen(buffer, "r+");
            if (sysfs_node == NULL) {
                printf("Cannot open state sysfs_node %s (%d)\n", buffer, errno);
                exit(99);
            }
            wrote = fwrite("start",sizeof(uint8_t),strlen("start"),sysfs_node);
            if (wrote < strlen("start")) {
                printf("Error writing start: %d\n",errno);
            }
            fclose(sysfs_node);
            sleep(3);
            snprintf(buffer,sizeof(buffer),"/dev/rpmsg_pru%d",channel_);
            if (access(buffer,F_OK)) {
                printf("Cannot access %s (%d)\n",buffer,errno);
                exit(99);
            }
        } else {
            printf("%s exists already\n",buffer);
        }
    }
    void StopPRU() 
    {
        int wrote;
        char buffer[128];
        snprintf(buffer,sizeof(buffer),"/dev/rpmsg_pru%d",channel_);
        if (!access(buffer,F_OK)) {
            FILE *sysfs_node;
            printf("%s exists, stopping PRU\n",buffer);
            snprintf(buffer,sizeof(buffer),"/sys/class/remoteproc/remoteproc%d/state",procnum_);
            sysfs_node = fopen(buffer, "r+");
            if (sysfs_node == NULL) {
                printf("Cannot open state sysfs_node %s (%d)\n", buffer, errno);
                exit(99);
            }
            wrote = fwrite("stop",sizeof(uint8_t),strlen("stop"),sysfs_node);
            if (wrote < strlen("stop")) {
                printf("Error writing stop: %d\n",errno);
                exit(99);
            }
            fclose(sysfs_node);
            sleep(3);
            snprintf(buffer,sizeof(buffer),"/dev/rpmsg_pru%d",channel_);
            if (!access(buffer,F_OK)) {
                printf("%s still exists...\n",buffer);
                exit(99);
            }
        } else {
            printf("%s does not exist\n",buffer);
        }
    }
    void PRUState()
    {
        FILE *sysfs_node;
        int read_;
        char buffer[128];
        snprintf(buffer,sizeof(buffer),"/sys/class/remoteproc/remoteproc%d/state",procnum_);
        sysfs_node = fopen(buffer, "r+");
        if (sysfs_node == NULL) {
            printf("Cannot open state sysfs_node %s (%d)\n", buffer, errno);
            exit(99);
        }
        read_ = fread(buffer,sizeof(char),sizeof(buffer),sysfs_node);
        buffer[read_] = '\0';
        fclose(sysfs_node);
        printf("Pru state is %s\n",buffer);
    }
private:
    const char *firmwareName_;
    const int channel_;
    const int procnum_;
};

int main(int argc, char *argv[])
{
    char firmware[64];
    int opt, chan, proc;
    bool stop = false;
    bool query = false;
    
    while ((opt = getopt(argc, argv, "f:c:p:sq")) >= 0)
    {
        switch (opt)
        {
        case 'f':
            strncpy(firmware,optarg,sizeof(firmware));
            break;
        case 'c':
            chan = atoi(optarg);
            break;
        case 'p':
            proc = atoi(optarg);
            break;
        case 's':
            stop = true;
            break;
        case 'q':
            query = true;
            break;
        default:
            fprintf(stderr, "Unknown option %c\n", opt);
            exit(99);
        }
    }
    DCCPru testpru(firmware,chan,proc);
    if (query) {
        testpru.PRUState();
    } else if (stop) {
        testpru.StopPRU();
    } else {
        testpru.StartPRU();
    }
}

    
