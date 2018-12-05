/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "cyber/scheduler/policy/classic.h"

#include "cyber/event/perf_event_cache.h"

namespace apollo {
namespace cyber {
namespace scheduler {

using apollo::cyber::croutine::RoutineState;
using apollo::cyber::base::ReadLockGuard;
using apollo::cyber::base::AtomicRWLock;
using apollo::cyber::croutine::CRoutine;
using apollo::cyber::event::PerfEventCache;
using apollo::cyber::event::SchedPerf;

namespace {
static constexpr auto MIN_SLEEP_INTERVAL = std::chrono::milliseconds(1);
}

std::mutex ClassicContext::mtx_wq_;
std::condition_variable ClassicContext::cv_wq_;
std::array<AtomicRWLock, MAX_PRIO> ClassicContext::rq_locks_;
std::array<std::vector<std::shared_ptr<CRoutine>>, MAX_PRIO>
    ClassicContext::rq_;

std::shared_ptr<CRoutine> ClassicContext::NextRoutine() {
  if (unlikely(stop_)) {
    return nullptr;
  }

  for (int i = MAX_PRIO - 1; i >= 0; --i) {
    ReadLockGuard<AtomicRWLock> lk(rq_locks_[i]);
    for (auto& cr : rq_[i]) {
      if (!cr->Acquire()) {
        continue;
      }

      if (cr->UpdateState() == RoutineState::READY) {
        PerfEventCache::Instance()->AddSchedEvent(SchedPerf::NEXT_RT, cr->id(),
                                                  cr->processor_id());
        return cr;
      }

      if (unlikely(cr->state() == RoutineState::SLEEP)) {
        if (!need_sleep_ || wake_time_ > cr->wake_time()) {
          need_sleep_ = true;
          wake_time_ = cr->wake_time();
        }
      }

      cr->Release();
    }
  }

  return nullptr;
}

void ClassicContext::Wait() {
  std::unique_lock<std::mutex> lk(mtx_wq_);
  if (stop_) {
    return;
  }

  if (unlikely(need_sleep_)) {
    auto duration = wake_time_ - std::chrono::steady_clock::now();
    cv_wq_.wait_for(lk, duration);
    need_sleep_ = false;
  } else {
    cv_wq_.wait(lk);
  }
}

void ClassicContext::Shutdown() {
  {
    std::lock_guard<std::mutex> lg(mtx_wq_);
    if (!stop_) {
      stop_ = true;
    }
  }
  cv_wq_.notify_all();
}

void ClassicContext::Notify() { cv_wq_.notify_one(); }

}  // namespace scheduler
}  // namespace cyber
}  // namespace apollo
