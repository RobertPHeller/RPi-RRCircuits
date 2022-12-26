// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Dec 17 13:49:15 2022
//  Last Modified : <221226.0911>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2022  Robert Heller D/B/A Deepwoods Software
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

#ifndef __BOOTPAUSEHELPER_HXX
#define __BOOTPAUSEHELPER_HXX

#include <esp_err.h>
#include <esp_log.h>
#include <esp_ota_ops.h>
#include <esp_system.h>
#include <esp_task_wdt.h>
#include <esp32s3/rom/rtc.h>
#include <utils/Singleton.hxx>

namespace esp32multifunction
{
class BootPauseHelper : public Singleton<BootPauseHelper> {
public:
    enum {
        PauseLoopCount = 100,
        PauseLoopDelay_us = 100*1000,
        RXBufferLength = 256,
        TXBufferLength = 256,
        EOL = '\r',
        SETNODE = 'N',
        BOOTLOADER = 'B',
        EVENTRESET = 'E',
        FACTORYRESET = 'F',
        TESTSIGLAMPS = 'T',
        RESUME = 'R'
    };
    void CheckPause();
private:
    void PauseConsole();
    uint64_t ParseNode(char *buffer,size_t bufferlen);
    size_t ReadLine(uart_port_t uart_num,char *buffer, size_t bufferlen);
};

}

#endif // __BOOTPAUSEHELPER_HXX

