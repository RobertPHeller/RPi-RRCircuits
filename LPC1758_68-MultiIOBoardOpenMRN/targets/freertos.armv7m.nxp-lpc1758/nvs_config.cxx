// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Tue Sep 13 20:57:15 2022
//  Last Modified : <220914.0847>
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

#include "nvs_config.hxx"

#include "iap.h"
#include "chip.h"

#include <string.h>
#include <utils/format_utils.hxx>
#include <utils/logging.h>

extern const char __nvs_start;
extern const char __nvs_end;

static uint8_t get_nvs_sector() {
    unsigned nvsstart = (unsigned)(&__nvs_start);
    if (nvsstart < 0x10000) {
        return nvsstart / 0x1000;
    } else {
        return 16 - 2 + nvsstart / 0x8000;
    }
}

static uint32_t compute_checksum (const uint32_t *start, 
                                  const uint32_t *end) {
    uint32_t result = 0L;
    for (const uint32_t *lp = start; lp < end; lp++) {
        result += *lp;
    }
    return result;
}

void nvs_init()
{
}

uint8_t load_config(node_config_t *config)
{
    node_config_t *stored_config = (node_config_t *)(&__nvs_start);
    uint32_t checksum = compute_checksum((uint32_t *)stored_config,
                                         &stored_config->checksum);
    if (checksum != stored_config->checksum)
    {
        return -1;
    }
    memcpy((void *)config,(void *)stored_config,sizeof(node_config_t));
    return 0;
}

uint8_t save_config(node_config_t *config)
{
    uint32_t scratch[256];
    config->checksum = compute_checksum((uint32_t *)config,
                                        &config->checksum);
    memset((void *)scratch,0xFF,sizeof(scratch));
    memcpy((void *)scratch,(void *)config,sizeof(node_config_t));
    unsigned s = get_nvs_sector();
    portENTER_CRITICAL(); 
    Chip_IAP_PreSectorForReadWrite(s, s);
    Chip_IAP_EraseSector(s, s);
    portEXIT_CRITICAL();
    
    SystemCoreClockUpdate();
    uint32_t start_address = (uintptr_t)(&__nvs_start);
    
    portENTER_CRITICAL();
    Chip_IAP_PreSectorForReadWrite(s, s);
    Chip_IAP_CopyRamToFlash(start_address, (uint32_t*)scratch, 256);
    portEXIT_CRITICAL();
    
    return 0;
}

uint8_t default_config(node_config_t *config)
{
    memset(config, 0x00, sizeof(node_config_t));
    config->node_id = 0x050101012200ULL;
    return save_config(config);
}

void dump_config(node_config_t *config)
{
    LOG(INFO, "[NVS] Node ID: %s"
        , uint64_to_string_hex(config->node_id).c_str());
}

bool force_factory_reset()
{
    node_config_t config;
    load_config(&config);
    config.force_reset = true;
    return save_config(&config) == 0;
}


bool set_node_id(uint64_t node_id)
{
    node_config_t config;
    load_config(&config);
    config.node_id = node_id;
    config.force_reset = true;
    return save_config(&config) == 0;
}

