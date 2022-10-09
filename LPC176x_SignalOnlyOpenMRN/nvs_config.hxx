// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Tue Sep 13 20:52:35 2022
//  Last Modified : <220913.2153>
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

#ifndef __NVS_CONFIG_HXX
#define __NVS_CONFIG_HXX

#include <stdint.h>

typedef struct
{
    bool force_reset;
    bool reset_events_req;
    uint64_t node_id;
    uint32_t checksum;
} node_config_t;

uint8_t load_config(node_config_t *config);
uint8_t save_config(node_config_t *config);
uint8_t default_config(node_config_t *config);
void nvs_init();
void dump_config(node_config_t *config);
bool force_factory_reset();
bool set_node_id(uint64_t node_id);

#endif // __NVS_CONFIG_HXX

