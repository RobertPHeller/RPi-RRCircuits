/** \copyright
 * Copyright (c) 2020, Mike Dunston
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are  permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \file nvs_config.cpp
 *
 * NVS based configuration management for the ESP32 IO Board.
 *
 * @author Mike Dunston
 * @date 4 July 2020
 */

#include "nvs_config.hxx"
#include "sdkconfig.h"

#include <nvs.h>
#include <nvs_flash.h>
#include <string>

// TODO: adjust format_utils.hxx not to require this line here.
using std::string;

#include <utils/format_utils.hxx>
#include <utils/logging.h>

/// NVS Persistence namespace.
static constexpr char NVS_NAMESPACE[] = "iocfg";

/// NVS Persistence key.
static constexpr char NVS_CFG_KEY[] = "node";

esp_err_t load_config(node_config_t *config)
{
    LOG(INFO, "[NVS] Loading configuration");
    // load non-CDI based config from NVS
    nvs_handle_t nvs;
    size_t size = sizeof(node_config_t);
    esp_err_t res =
        ESP_ERROR_CHECK_WITHOUT_ABORT(
            nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs));
    if (res != ESP_OK)
    {
        LOG_ERROR("[NVS] Configuration load failed: %s (%d)"
                , esp_err_to_name(res), res);
        return res;
    }
    res = nvs_get_blob(nvs, NVS_CFG_KEY, config, &size);
    nvs_close(nvs);

    // if the size read in is not as expected reset the result code to failure.
    if (size != sizeof(node_config_t))
    {
        LOG_ERROR("[NVS] Configuration load failed (loaded size incorrect: "
                  "%zu vs %zu)", size, sizeof(node_config_t));
        res = ESP_FAIL;
    }
    return res;
}

esp_err_t save_config(node_config_t *config)
{
    nvs_handle_t nvs;
    esp_err_t res =
        ESP_ERROR_CHECK_WITHOUT_ABORT(
            nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs));
    if (res != ESP_OK)
    {
        LOG_ERROR("[NVS] Configuration save failed: %s (%d)"
                , esp_err_to_name(res), res);
        return res;
    }
    res =
        ESP_ERROR_CHECK_WITHOUT_ABORT(
            nvs_set_blob(nvs, NVS_CFG_KEY, config, sizeof(node_config_t)));
    if (res != ESP_OK)
    {
        LOG_ERROR("[NVS] Configuration save failed: %s (%d)"
                , esp_err_to_name(res), res);
        return res;
    }
    res = ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_commit(nvs));
    nvs_close(nvs);
    if (res != ESP_OK)
    {
        LOG_ERROR("[NVS] Commit failed: %s (%d)", esp_err_to_name(res), res);
    }
    return res;
}


esp_err_t default_config(node_config_t *config)
{
    LOG(INFO, "[NVS] Initializing default configuration");
    bzero(config, sizeof(node_config_t));
    config->node_id = CONFIG_OLCB_NODE_ID;
    return save_config(config);
}

void die_with(bool activity1, bool activity2, unsigned period = 1000
            , bool toggle_both = false);

void nvs_init()
{
    // Initialize NVS before we do any other initialization as it may be
    // internally used by various components even if we disable it's usage in
    // the WiFi connection stack.
    LOG(INFO, "[NVS] Initializing NVS");
    if (ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_flash_init()) == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        const esp_partition_t *part =
            esp_partition_find_first(ESP_PARTITION_TYPE_DATA
                                   , ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);
        if (part != NULL)
        {
            LOG(INFO, "[NVS] Erasing partition %s...", part->label);
            ESP_ERROR_CHECK(esp_partition_erase_range(part, 0, part->size));
            ESP_ERROR_CHECK(nvs_flash_init());
        }
        else
        {
            LOG_ERROR("[NVS] Unable to locate NVS partition!");
            die_with(true, false);
        }
    }
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

    return save_config(&config) == ESP_OK;
}

bool set_node_id(uint64_t node_id)
{
    node_config_t config;
    load_config(&config);
    config.node_id = node_id;
    config.force_reset = true;

    return save_config(&config) == ESP_OK;
}