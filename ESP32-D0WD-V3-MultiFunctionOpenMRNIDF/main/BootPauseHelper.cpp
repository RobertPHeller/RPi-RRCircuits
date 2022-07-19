// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Jul 18 13:22:30 2022
//  Last Modified : <220719.1235>
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

static const char rcsid[] = "@(#) : $Id$";

#include <algorithm>
#include <ctype.h>
#include <driver/i2c.h>
#include <driver/uart.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_ota_ops.h>
#include <esp_system.h>
#include <esp_task_wdt.h>
#include <esp32/rom/rtc.h>
#include "nvs_config.hxx"
#include "hardware.hxx"
#include <freertos_includes.h>
#include "BootPauseHelper.hxx"

void BootPauseHelper::CheckPause()
{
    bool need_driver_unload = !uart_is_driver_installed(UART_NUM_0);
    if (need_driver_unload)
    {
        esp_err_t res = uart_driver_install(UART_NUM_0,RXBufferLength,
                                            0,0,NULL,0);
        if (res != ESP_OK)
        {
            LOG_ERROR("[CheckPause] Failed to start UART");
            return;
        }
    }
    size_t bytesavailable;
    for (int i=0;i<PauseLoopCount;i++)
    {
        esp_err_t res = uart_get_buffered_data_len(UART_NUM_0,&bytesavailable);
        if (res != ESP_OK)
        {
            LOG_ERROR("[CheckPause] Failed to get buffered data len");
            return;
        }
        if (bytesavailable > 0)
        {
            break;
        }
        esp_rom_delay_us(PauseLoopDelay_us);
    }
    if (bytesavailable > 0)
    {
        PauseConsole();
    }
    if (need_driver_unload)
    {
        esp_err_t res = uart_driver_delete(UART_NUM_0);
        if (res != ESP_OK)
        {
            LOG_ERROR("[CheckPause] Failed to delete uart");
            return;
        }
    }
}

void BootPauseHelper::PauseConsole()
{
    char receivebuffer[RXBufferLength];
    char transmitBuffer[TXBufferLength];
    
    uart_write_bytes(UART_NUM_0,"Ready.\r\n",8);
    
    while (true)
    {
        uart_write_bytes(UART_NUM_0,">>>",3);
        size_t len = ReadLine(UART_NUM_0,receivebuffer,sizeof(receivebuffer));
        if (len == 0)
        {
            continue;
        }
        switch (toupper(receivebuffer[0]))
        {
        case SETNODE:
            {
                uint64_t nid = ParseNode(&receivebuffer[1],len-1);
                set_node_id(nid);
                int l = snprintf(transmitBuffer,sizeof(transmitBuffer),"Set Node ID: %0llx\r\n",nid);
                uart_write_bytes(UART_NUM_0,transmitBuffer,l);
                break;
            }
        case BOOTLOADER:
            config_->bootloader_req = true;
            save_config(config_);
            break;
        case EVENTRESET:
            config_->reset_events_req = true;
            save_config(config_);
            break;
        case FACTORYRESET:
            force_factory_reset();
            break;
        case RESUME:
            return;
        default:
            {
                int l = snprintf(transmitBuffer,sizeof(transmitBuffer),"Err: %c\r\n",receivebuffer[0]);
                uart_write_bytes(UART_NUM_0,transmitBuffer,l);
                break;
            }
        }
    }
}

uint64_t BootPauseHelper::ParseNode(char * buffer, size_t bufferlen)
{
    uint64_t result = 0;
    char *p = NULL;
    size_t l = 0;
    for (p = buffer,l = bufferlen; l > 0 && *p != '\0';p++,l--)
    {
        if (isxdigit(*p))
        {
            if (isdigit(*p))
            {
                result = (result << 4) + (*p - '0');
            }
            else if (islower(*p))
            {
                result = (result << 4) + ((*p - 'a')+10);
            }
            else
            {
                result = (result << 4) + ((*p - 'A')+10);
            }
        }
    }
    return result;
}

size_t BootPauseHelper::ReadLine(uart_port_t uart_num,char *buffer,
                                 size_t bufferlen)
{
    char *p = buffer;
    size_t remainder = bufferlen;
    size_t numread   = 0;
    
    while (remainder > 0)
    {
        int r = uart_read_bytes(uart_num,p,1,100);
        if (r > 0)
        {
            if (*p == EOL)
            {
                numread++;
                *p = '\0';
                break;
            }
            else
            {
                numread++;
                p++;
                remainder--;
            }
        }
    }
    return numread;
}

