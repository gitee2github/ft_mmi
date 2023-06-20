/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "timer_manager.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr int32_t MIN_DELAY = -1;
constexpr int32_t MIN_INTERVAL = 50;
constexpr int32_t MAX_INTERVAL = 4096;
constexpr int32_t MAX_TIMER_COUNT = 64;
constexpr int32_t NONEXISTENT_ID = -1;
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "TimerManager" };
} // namespace

TimerManager::TimerManager() {}
TimerManager::~TimerManager() {}

int32_t TimerManager::AddTimer(int32_t intervalMs, int32_t repeatCount, std::function<void()> callback)
{
    return AddTimerInternal(intervalMs, repeatCount, callback);
}

int32_t TimerManager::RemoveTimer(int32_t timerId)
{
    return RemoveTimerInternal(timerId);
}

int32_t TimerManager::ResetTimer(int32_t timerId)
{
    return ResetTimerInternal(timerId);
}

bool TimerManager::IsExist(int32_t timerId)
{
    return IsExistInternal(timerId);
}

int32_t TimerManager::CalcNextDelay()
{
    return CalcNextDelayInternal();
}

void TimerManager::ProcessTimers()
{
    ProcessTimersInternal();
}

int32_t TimerManager::TakeNextTimerId()
{
    uint64_t timerSlot = 0;
    uint64_t one = 1;

    for (const auto &timer : timers_) {
        timerSlot |= (one << timer->id);
    }

    for (int32_t i = 0; i < MAX_TIMER_COUNT; i++) {
        if ((timerSlot & (one << i)) == 0) {
            return i;
        }
    }
    return NONEXISTENT_ID;
}

int32_t TimerManager::AddTimerInternal(int32_t intervalMs, int32_t repeatCount, std::function<void()> callback)
{
    if (intervalMs < MIN_INTERVAL) {
        intervalMs = MIN_INTERVAL;
    } else if (intervalMs > MAX_INTERVAL) {
        intervalMs = MAX_INTERVAL;
    }
    if (!callback) {
        return NONEXISTENT_ID;
    }
    int32_t timerId = TakeNextTimerId();
    if (timerId < 0) {
        return NONEXISTENT_ID;
    }
    auto timer = std::make_unique<TimerItem>();
    timer->id = timerId;
    timer->intervalMs = intervalMs;
    timer->repeatCount = repeatCount;
    timer->callbackCount = 0;
    auto nowTime = GetMillisTime();
    if (!AddInt64(nowTime, timer->intervalMs, timer->nextCallTime)) {
        MMI_HILOGE("The addition of nextCallTime in TimerItem overflows");
        return NONEXISTENT_ID;
    }
    timer->callback = callback;
    InsertTimerInternal(timer);
    return timerId;
}

int32_t TimerManager::RemoveTimerInternal(int32_t timerId)
{
    for (auto it = timers_.begin(); it != timers_.end(); ++it) {
        if ((*it)->id == timerId) {
            timers_.erase(it);
            return RET_OK;
        }
    }
    return RET_ERR;
}

int32_t TimerManager::ResetTimerInternal(int32_t timerId)
{
    for (auto it = timers_.begin(); it != timers_.end(); ++it) {
        if ((*it)->id == timerId) {
            auto timer = std::move(*it);
            timers_.erase(it);
            auto nowTime = GetMillisTime();
            if (!AddInt64(nowTime, timer->intervalMs, timer->nextCallTime)) {
                MMI_HILOGE("The addition of nextCallTime in TimerItem overflows");
                return RET_ERR;
            }
            timer->callbackCount = 0;
            InsertTimerInternal(timer);
            return RET_OK;
        }
    }
    return RET_ERR;
}

bool TimerManager::IsExistInternal(int32_t timerId)
{
    for (auto it = timers_.begin(); it != timers_.end(); ++it) {
        if ((*it)->id == timerId) {
            return true;
        }
    }
    return false;
}

void TimerManager::InsertTimerInternal(std::unique_ptr<TimerItem>& timer)
{
    for (auto it = timers_.begin(); it != timers_.end(); ++it) {
        if ((*it)->nextCallTime > timer->nextCallTime) {
            timers_.insert(it, std::move(timer));
            return;
        }
    }
    timers_.push_back(std::move(timer));
}

int32_t TimerManager::CalcNextDelayInternal()
{
    auto delay = MIN_DELAY;
    if (!timers_.empty()) {
        auto nowTime = GetMillisTime();
        const auto& item = *timers_.begin();
        if (nowTime >= item->nextCallTime) {
            delay = 0;
        } else {
            delay = item->nextCallTime - nowTime;
        }
    }
    return delay;
}

void TimerManager::ProcessTimersInternal()
{
    if (timers_.empty()) {
        return;
    }
    auto nowTime = GetMillisTime();
    for (;;) {
        auto it = timers_.begin();
        if (it == timers_.end()) {
            break;
        }
        if ((*it)->nextCallTime > nowTime) {
            break;
        }
        auto curTimer = std::move(*it);
        timers_.erase(it);
        ++curTimer->callbackCount;
        if ((curTimer->repeatCount >= 1) && (curTimer->callbackCount >= curTimer->repeatCount)) {
            curTimer->callback();
            continue;
        }
        if (!AddInt64(nowTime, curTimer->intervalMs, curTimer->nextCallTime)) {
            MMI_HILOGE("The addition of nextCallTime in TimerItem overflows");
            return;
        }
        auto callback = curTimer->callback;
        InsertTimerInternal(curTimer);
        callback();
    }
}
} // namespace MMI
} // namespace OHOS
