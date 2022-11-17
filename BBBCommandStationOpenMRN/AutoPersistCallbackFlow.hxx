/** @copyright
 * COPYRIGHT (c) 2019-2020 Mike Dunston
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
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
 * @file AutoPersistCallbackFlow.hxx
 *
 **********************************************************************/

#ifndef AUTO_PERSIST_CB_FLOW_H_
#define AUTO_PERSIST_CB_FLOW_H_

#include <executor/Service.hxx>
#include <executor/StateFlow.hxx>

class AutoPersistFlow : private StateFlowBase
{
public:
  AutoPersistFlow(Service *service
                , uint64_t interval
                , std::function<void(void)> callback)
                : StateFlowBase(service)
                , interval_(interval)
                , callback_(std::move(callback))
  {
    HASSERT(callback_);
    start_flow(STATE(sleep_and_persist));
  }

  void stop()
  {
    set_terminated();
    timer_.ensure_triggered();
  }

private:
  StateFlowTimer timer_{this};
  uint64_t interval_;
  std::function<void(void)> callback_;
  StateFlowBase::Action sleep_and_persist()
  {
    return sleep_and_call(&timer_, interval_, STATE(persist));
  }
  StateFlowBase::Action persist()
  {
    callback_();
    return yield_and_call(STATE(sleep_and_persist));
  }
};
#endif // AUTO_PERSIST_CB_FLOW_H_