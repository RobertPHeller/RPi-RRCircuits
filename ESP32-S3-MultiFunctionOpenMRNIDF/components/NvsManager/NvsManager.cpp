// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Dec 17 14:03:56 2022
//  Last Modified : <221217.1642>
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

#include "sdkconfig.h"
#if CONFIG_IDF_TARGET_ESP32
#include <esp32/rom/rtc.h>
#elif CONFIG_IDF_TARGET_ESP32S3
#include <esp32s3/rom/rtc.h>
#endif
#include <esp_err.h>
#include <esp_partition.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <utils/Singleton.hxx>
#include "NvsManagerStruct.hxx"
#include "NvsManager.hxx"
#include <utils/logging.h>

namespace esp32multifunction
{
void NvsManager::init(uint8_t reset_reason)
{
    LOG(VERBOSE, "NVS.init(%d)", reset_reason);
    
    nvs_handle_t nvs;
    size_t config_size = sizeof(config_);
    
    // Initialize NVS before we do any other initialization as it may be
    // internally used by various components even if we disable it's usage in
    // the WiFi connection stack.
    LOG(INFO, "[NVS] Initializing NVS");
    if (ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_flash_init()) == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        const esp_partition_t *partition =
              esp_partition_find_first(ESP_PARTITION_TYPE_DATA,
                                       ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);
        if (partition != NULL)
        {
            LOG(INFO, "[NVS] Erasing partition %s...", partition->label);
            ESP_ERROR_CHECK(esp_partition_erase_range(partition, 0, partition->size));
            ESP_ERROR_CHECK(nvs_flash_init());
        }
    }
    LOG(INFO, "[NVS] Loading configuration");
    // load non-CDI based config from NVS
    ESP_ERROR_CHECK(nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs));
    esp_err_t res = ESP_ERROR_CHECK_WITHOUT_ABORT(
                 nvs_get_blob(nvs, NVS_CFG_KEY, &config_, &config_size));
    need_persist_ = false;
    if (config_size != sizeof(config_) || res != ESP_OK)
    {
        LOG_ERROR("[NVS] Configuration missing or corrupt, using defaults");
        memset(&config_, 0, sizeof(config_));
        config_.node_id = CONFIG_OLCB_NODE_ID;
        config_.force_reset = false;
        config_.reset_events_req = false;
        config_.bootloader_req = false;
        need_persist_ = true;
    }
    CheckPersist();
    DisplayNvsConfiguration();
}

void NvsManager::DisplayNvsConfiguration()
{
    // display current configuration settings.
    LOG(INFO, "[NVS] Node ID: 0x%llX", config_.node_id);
    LOG(INFO, "[NVS] Force Reset: %s",
        config_.force_reset ? "true" : "false");
    LOG(INFO, "[NVS] Reset events: %s",
        config_.reset_events_req ? "true" : "false");
    LOG(INFO, "[NVS] Bootloader: %s",
        config_.bootloader_req ? "true" : "false");
}


DEFINE_SINGLETON_INSTANCE(NvsManager);

/// NVS Persistence namespace.
const char NvsManager::NVS_NAMESPACE[];

/// NVS Persistence key.
const char NvsManager::NVS_CFG_KEY[];


}
