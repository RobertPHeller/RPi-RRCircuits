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
 * \file fs.cpp
 *
 * Filesystem management for the ESP32 IO Board.
 *
 * @author Mike Dunston
 * @date 4 July 2020
 */

#include "fs.hxx"
extern "C"
{
    #include <dirent.h>
}
#include <esp_spiffs.h>
#include <esp_vfs.h>
#include <utils/logging.h>

/// Partition name for the persistent filesystem.
static constexpr char FS_PARTITION[] = "fs";

/// Mount point for the persistent filesystem.
static constexpr char FS_MOUNTPOINT[] = "/fs";

void recursive_dump_tree(const std::string &path, bool remove = false, bool first = true)
{
    if (first && !remove)
    {
        LOG(INFO, "[FS] Dumping content of filesystem: %s", path.c_str());
    }
    DIR *dir = opendir(path.c_str());
    if (dir)
    {
        dirent *ent = NULL;
        while ((ent = readdir(dir)) != NULL)
        {
            string fullPath = path + "/" + ent->d_name;
            if (ent->d_type == DT_REG)
            {
                struct stat statbuf;
                stat(fullPath.c_str(), &statbuf);
                if (remove)
                {
                    LOG(VERBOSE, "[FS] Deleting %s (%lu bytes)"
                      , fullPath.c_str(), statbuf.st_size);
                    ERRNOCHECK(fullPath.c_str(), unlink(fullPath.c_str()));
                }
                else
                {
                    // NOTE: not using LOG(INFO, ...) here due to ctime injecting a
                    // newline at the end of the string.
                    printf("[FS] %s (%lu bytes) mtime: %s", fullPath.c_str()
                         , statbuf.st_size, ctime(&statbuf.st_mtime));
                }
            }
            else if (ent->d_type == DT_DIR)
            {
                recursive_dump_tree(fullPath, remove, false);
            }
        }
        closedir(dir);
        if (remove)
        {
            rmdir(path.c_str());
        }
    }
    else
    {
        LOG_ERROR("[FS] Failed to open directory: %s", path.c_str());
    }
}

void mount_fs(bool cleanup)
{
    size_t total_len = 0, free_len = 0;
    esp_vfs_spiffs_conf_t conf =
    {
      .base_path = FS_MOUNTPOINT,
      .partition_label = FS_PARTITION,
      .max_files = 10,
      .format_if_mount_failed = true
    };
    LOG(INFO, "[FS] Mounting SPIFFS: %s...", FS_PARTITION);
    // Attempt to mount the partition
    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));
    // check that the partition mounted
    size_t used = 0;
    ESP_ERROR_CHECK(esp_spiffs_info(FS_PARTITION, &total_len, &used));
    free_len = total_len - used;
    LOG(INFO, "[FS] %.2f/%.2f kb space used", (float)(free_len / 1024.0f)
      , (float)(total_len / 1024.0f));
    recursive_dump_tree(FS_MOUNTPOINT, cleanup);
    LOG(INFO, "[FS] mount done.");
}

void unmount_fs()
{
    LOG(INFO, "[FS] Unmounting SPIFFS: %s...", FS_PARTITION);
    ESP_ERROR_CHECK(esp_vfs_spiffs_unregister(FS_PARTITION));
}