// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat Dec 17 13:13:14 2022
//  Last Modified : <221217.1640>
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

#ifndef __NVSMANAGER_HXX
#define __NVSMANAGER_HXX

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

namespace esp32multifunction
{

class NvsManager : public Singleton<NvsManager>
{
public:
    void init(uint8_t reset_reason);
    inline bool should_reset_config() {return config_.force_reset;}
    inline bool should_reset_events() {return config_.reset_events_req;}
    inline bool should_start_bootloader() {return config_.bootloader_req;}
    inline void force_factory_reset() 
    {                                        
        config_.force_reset = true;
        need_persist_ = true;
    }                                     
    inline void clear_factory_reset()
    {
        config_.force_reset = false;
        need_persist_ = true;
    }
    inline void force_reset_events()
    {
        config_.reset_events_req = true;
        need_persist_ = true;
    }
    inline void clear_reset_events()
    {
        config_.reset_events_req = false;
        need_persist_ = true;
    }
    inline void force_bootloader()
    {
        config_.bootloader_req = false;
        need_persist_ = true;          
    }
    inline void clear_bootloader()
    {
        config_.bootloader_req = false;
        need_persist_ = true;
    }
    inline uint64_t node_id() {return config_.node_id;}
    inline void node_id(uint64_t node_id)
    {
        config_.node_id = node_id;
        need_persist_ = true;
    }
    NvsManager() : need_persist_(false) 
    {
    }
    inline void CheckPersist() 
    {
        if (need_persist_) {persist_configuration();}
    }
    void DisplayNvsConfiguration();
private:
    /// NVS Persistence namespace.
    static constexpr char NVS_NAMESPACE[] = "node";

    /// NVS Persistence key.
    static constexpr char NVS_CFG_KEY[] = "cfg";

    struct {
        uint64_t node_id;
        bool force_reset;
        bool bootloader_req;
        bool reset_events_req;
        uint8_t reserved[20];
    } config_;
    bool need_persist_;
    inline void persist_configuration()
    {
        nvs_handle_t nvs;
        ESP_ERROR_CHECK(nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs));
        ESP_ERROR_CHECK(nvs_set_blob(nvs, NVS_CFG_KEY, &config_, sizeof(config_)));
        ESP_ERROR_CHECK(nvs_commit(nvs));
        nvs_close(nvs);
        need_persist_ = false;
    }
};

}

#endif // __NVSMANAGER_HXX

