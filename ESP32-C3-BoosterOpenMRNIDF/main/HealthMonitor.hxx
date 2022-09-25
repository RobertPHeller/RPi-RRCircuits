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
 * \file HealthMonitor.hxx
 *
 * Provides a periodic health report.
 *
 * @author Mike Dunston
 * @date 14 July 2020
 */

#ifndef HEARTBEAT_LED_H_
#define HEARTBEAT_LED_H_

#include <esp_log.h>
#include <executor/Service.hxx>
#include <executor/StateFlow.hxx>
#include <utils/logging.h>

#include "sdkconfig.h"

namespace esp32multifunction
{

/// Utility class providing periodic reporting of general health of the
/// ESP32 IO Board.
class HealthMonitor : public StateFlowBase
{
public:
    /// Constructor.
    ///
    /// @param service is the @ref Service to attach this flow to.
    HealthMonitor(Service *service) : StateFlowBase(service)
    {
        start_flow(STATE(update));
    }

    /// Stops the flow and cancels the timer (if needed).
    void stop()
    {
        shutdown_ = true;
        set_terminated();
        timer_.ensure_triggered();
    }
private:
    /// @ref StateFlowTimer used for periodic wakeup.
    StateFlowTimer timer_{this};

    /// Interval at which to wake up.
    const uint64_t reportInterval_{SEC_TO_NSEC(CONFIG_HEALTH_INTERVAL)};

#if CONFIG_ENABLE_TASK_LIST_REPORTING
    /// Interval at which to display the currently running task list.
    const uint64_t taskListInterval_{
        (uint64_t)SEC_TO_USEC(CONFIG_TASK_LIST_INTERVAL)};

    /// Last time stamp when the task list was displayed.
    uint64_t lastTaskList_{0};
#endif // CONFIG_ENABLE_TASK_LIST_REPORTING

    /// Internal flag to track if a shutdown request has been requested.
    bool shutdown_{false};

    /// Wakes up and blinks the heartbeat LED and prints general health when
    /// the required count of wakeups has expired.
    Action update()
    {
        if (shutdown_)
        {
            return exit();
        }

        UBaseType_t taskCount = uxTaskGetNumberOfTasks();
        LOG(INFO, "%s: free heap: %.2fkB (max block size: %.2fkB), "
#if CONFIG_SPIRAM_SUPPORT
                  "Free PSRAM: %.2fkB (max block size: %.2fkB), "
#endif // CONFIG_SPIRAM_SUPPORT
                  "mainBufferPool: %.2fkB, task count: %d"
          , esp_log_system_timestamp()
          , heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / 1024.0f
          , heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL) / 1024.0f
#if CONFIG_SPIRAM_SUPPORT
          , heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024.0f
          , heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM) / 1024.0f
#endif // CONFIG_SPIRAM_SUPPORT
          , mainBufferPool->total_size() / 1024.0f, taskCount);
#if CONFIG_ENABLE_TASK_LIST_REPORTING
        std::vector<TaskStatus_t> taskList;
        uint64_t now = esp_timer_get_time();
        uint32_t ulTotalRunTime{0};
        if ((now - lastTaskList_) > taskListInterval_ || !lastTaskList_)
        {
            taskList.resize(taskCount);
            UBaseType_t retrievedTaskCount =
                uxTaskGetSystemState(&taskList[0], taskCount, &ulTotalRunTime);
            // adjust this time so we can use it for percentages.
            ulTotalRunTime /= 100;
            // sort by runtime
            std::sort(taskList.begin(), taskList.end(),
            [](const TaskStatus_t &left, const TaskStatus_t &right)
            {
                return left.ulRunTimeCounter > right.ulRunTimeCounter;
            });
            for (int task = 0; task < retrievedTaskCount; task++)
            {
                LOG(INFO
                  , "%s: %-16s id:%3d, priority:%2d/%2d, free stack:%5d, "
                    "core:%4s, cpu%%:%-3d, state:%s"
                  , esp_log_system_timestamp(), taskList[task].pcTaskName
                  , taskList[task].xTaskNumber
                  , taskList[task].uxCurrentPriority
                  , taskList[task].uxBasePriority
                  , taskList[task].usStackHighWaterMark
                  , taskList[task].xCoreID == tskNO_AFFINITY ? "BOTH" :
                    taskList[task].xCoreID == PRO_CPU_NUM ? "PRO" :
                    taskList[task].xCoreID == APP_CPU_NUM ? "APP" : "UNK"
                  , (taskList[task].ulRunTimeCounter / portNUM_PROCESSORS) / ulTotalRunTime
                  , taskList[task].eCurrentState == eRunning ? "Running" :
                    taskList[task].eCurrentState == eReady ? "Ready" :
                    taskList[task].eCurrentState == eBlocked ? "Blocked" :
                    taskList[task].eCurrentState == eSuspended ? "Suspended" :
                    taskList[task].eCurrentState == eDeleted ? "Deleted" : "Unknown"
                );
            }
            lastTaskList_ = now;
        }
#endif // CONFIG_ENABLE_TASK_LIST_REPORTING
        return sleep_and_call(&timer_, reportInterval_, STATE(update));
    }
};

} // namespace esp32multifunction

#endif // HEARTBEAT_LED_H_