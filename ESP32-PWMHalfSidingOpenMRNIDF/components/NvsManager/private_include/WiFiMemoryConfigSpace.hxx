// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Dec 18 12:33:51 2022
//  Last Modified : <221224.1147>
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

#ifndef __WIFIMEMORYCONFIGSPACE_HXX
#define __WIFIMEMORYCONFIGSPACE_HXX

#include <WiFiConfigurationGroup.hxx>
#include <NvsManager.hxx>
#include <openlcb/SimpleStack.hxx>
#include <openlcb/VirtualMemorySpace.hxx>
#include <utils/format_utils.hxx>
#include <utils/logging.h>
#include <utils/Singleton.hxx>
#include <utils/Base64.hxx>
#include "StringUtils.hxx"
#include "sdkconfig.h"

namespace esp32pwmhalfsiding {

/// Node configuration holder
static constexpr WiFiConfiguration WiFiConfigHolder(CONFIG_OLCB_WIFI_MEMORY_SPACE_OFFSET);

/// Virtual memory space that allows reconfiguration of the persistent node
/// identifier.
class WiFiMemoryConfigSpace : public openlcb::VirtualMemorySpace,
public Singleton<WiFiMemoryConfigSpace>
{
public:
    // Constructor.
    ///
    /// @param stack is the @ref SimpleStackBase that this memory space
    /// should be registered with.
    /// @param node_id is the current node identifier.
    WiFiMemoryConfigSpace(openlcb::SimpleStackBase *stack,
                          NvsManager *nvs) : nvs_(nvs)
    {
        #define REGISTER_NUMBER(field, type, name)                         \
            LOG(VERBOSE, "[WiFiMemCfg:%02x] register_numeric(%s): %d",     \
                SPACE, name, field.offset());                              \
            register_numeric(field, numeric_reader<type>(field.offset()),  \
                                    numeric_writer<type>(field.offset()))
        #define REGISTER_STRING(field, name)                               \
            LOG(VERBOSE, "[WiFiMemCfg:%02x] register_string(%s): %d",      \
                SPACE, name, field.offset());                              \
            register_string(field, string_reader(field.offset()),          \
                                   string_writer(field.offset()))
        
        REGISTER_NUMBER(WiFiConfigHolder.wifi_mode(), uint8_t,
                        "wifi_mode");
        REGISTER_STRING(WiFiConfigHolder.hostname_prefix(),
                        "hostname_prefix");
        // Station Config
        REGISTER_STRING(WiFiConfigHolder.station().ssid(), "station_ssid");
        REGISTER_STRING(WiFiConfigHolder.station().password(),
                        "station_pass");
        stack->memory_config_handler()->registry()->insert(
                                       stack->node(), SPACE, this);
        #undef REGISTER_NUMBER
        #undef REGISTER_STRING
        LOG(INFO, "[WiFiMemCfg:%02x] registered", SPACE);
    }
    
private:
    NvsManager *nvs_;
    static constexpr uint8_t SPACE = CONFIG_OLCB_WIFI_MEMORY_SPACE_ID;
    template <typename T>
          typename std::function<T(unsigned repeat, BarrierNotifiable *done)>
          numeric_reader(int offset)
    {
        return [this, offset](unsigned repeat, BarrierNotifiable *done)
        {
            AutoNotify n(done);
            LOG(VERBOSE, "[WiFiMemCfg:%02x-RD] offs: %d", SPACE, offset);
            T value = (T)0;
            switch(offset)
            {
            case WiFiConfigHolder.wifi_mode().offset():
                LOG(VERBOSE, "[WiFiMemCfg:%02x-RD] wifi_mode: %d",
                    SPACE, nvs_->wifi_mode());
                value = nvs_->wifi_mode();
                break;
            default:
                LOG_ERROR("[WiFiMemCfg:%02x-RD] request for "
                          "unrecognized offset:%d", SPACE, offset);
            }
            return value;
        };
    }
    template <typename T>
          std::function<void(unsigned repeat, T value, BarrierNotifiable *done)>
          numeric_writer(int offset)
    {
        return [this, offset](unsigned repeat, T value, BarrierNotifiable *done)
        {
            AutoNotify n(done);
            LOG(VERBOSE, "[WiFiMemCfg:%02x-WR] offs: %d, value: %d",
                SPACE, offset, (uint32_t)value);
            switch(offset)
            {
            case WiFiConfigHolder.wifi_mode().offset():
                LOG(VERBOSE, "[WiFiMemCfg:%02x-RD] wifi_mode: %d",
                    SPACE, value);
                nvs_->wifi_mode((wifi_mode_t)value);
                break;
            default:
                LOG_ERROR("[WiFiMemCfg:%02x-WR] request for "
                          "unrecognized offset:%d", SPACE, offset);
            }
        };
    }
    std::function<void(unsigned repeat, string *value, BarrierNotifiable *done)>
          string_reader(int offset)
    {
        return [this, offset](unsigned repeat, string *value, BarrierNotifiable *done)
        {
            AutoNotify n(done);
            LOG(VERBOSE, "[WiFiMemCfg:%02x-RDSTR] offs: %d", SPACE, offset);
            switch (offset)
            {
            case WiFiConfigHolder.hostname_prefix().offset():
                *value = nvs_->hostname_prefix();
                LOG(VERBOSE,
                    "[WiFiMemCfg:%02x-RDSTR] hostname_prefix: %s",
                    SPACE, value->c_str());
                break;
            case WiFiConfigHolder.station().ssid().offset():
                *value = nvs_->station_ssid();
                LOG(VERBOSE,
                    "[WiFiMemCfg:%02x-RDSTR] station_ssid: %s", SPACE,
                    value->c_str());
                break;
            case WiFiConfigHolder.station().password().offset():
                {
                    string encoded =
                          base64_encode(nvs_->station_pass());
                    encoded.insert(0, "***");
                    *value = encoded;
                }
                LOG(VERBOSE,
                    "[WiFiMemCfg:%02x-RDSTR] station_pass: %s", SPACE,
                    value->c_str());
                break;
            default:
                LOG_ERROR("[WiFiMemCfg:%02x--RSTR] request for "
                          "unrecognized offset:%d", SPACE, offset);
                *value = "";
            }
        };
    }
    std::function<void(unsigned repeat, string value, BarrierNotifiable *done)>
          string_writer(int offset)
    {
        return [this, offset](unsigned repeat, string value, BarrierNotifiable *done)
        {
            AutoNotify n(done);
            // strip off nulls (if found)
            value.erase(
                        std::remove(value.begin(), value.end(), '\0'), value.end());
            LOG(VERBOSE, "[WiFiMemCfg:%02x-WRSTR] offs: %d, value:%s",
                SPACE, offset, value.c_str());
            switch(offset)
            {
            case WiFiConfigHolder.hostname_prefix().offset():
                LOG(VERBOSE,
                    "[WiFiMemCfg:%02x-RD] hostname_prefix: %s", SPACE,
                    value.c_str());
                nvs_->hostname_prefix(value.c_str());
                break;
            case WiFiConfigHolder.station().ssid().offset():
                LOG(VERBOSE, "[WiFiMemCfg:%02x-RD] station_ssid: %s",
                    SPACE, value.c_str());
                nvs_->station_ssid(value.c_str());
                break;
            case WiFiConfigHolder.station().password().offset():
                if (value.rfind("***", 0) == 0)
                {
                    string encodedpw = value.substr(3);
                    string value = "";
                    base64_decode(encodedpw, &value);
                }
                LOG(VERBOSE, "[WiFiMemCfg:%02x-RD] station_pass: %s",
                    SPACE, value.c_str());
                nvs_->station_pass(value.c_str());
                break;
            default:
                LOG_ERROR("[WiFiMemCfg:%02x-WSTR] request for "
                          "unrecognized offset:%d", SPACE, offset);
            }
        };
    }
};
}
#endif // __WIFIMEMORYCONFIGSPACE_HXX

