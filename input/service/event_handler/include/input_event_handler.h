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

#ifndef INPUT_EVENT_HANDLER_H
#define INPUT_EVENT_HANDLER_H

#include <memory>

#include "singleton.h"

#include "event_dispatch_handler.h"
#include "event_filter_handler.h"
#include "event_interceptor_handler.h"
#include "event_monitor_handler.h"
#include "event_normalize_handler.h"
#include "i_event_filter.h"
#include "i_input_event_handler.h"
#include "key_subscriber_handler.h"
#include "key_command_handler.h"
#include "mouse_event_normalize.h"

namespace OHOS {
namespace MMI {
using EventFun = std::function<int32_t(libinput_event *event)>;
using NotifyDeviceChange = std::function<void(int32_t, int32_t, char *)>;
class InputEventHandler final {
    DECLARE_DELAYED_SINGLETON(InputEventHandler);
public:
    DISALLOW_COPY_AND_MOVE(InputEventHandler);
    void Init(UDSServer& udsServer);
    void OnEvent(void *event);
    UDSServer *GetUDSServer() const;

    std::shared_ptr<EventNormalizeHandler> GetEventNormalizeHandler() const;
    std::shared_ptr<EventInterceptorHandler> GetInterceptorHandler() const;
    std::shared_ptr<KeySubscriberHandler> GetSubscriberHandler() const;
    std::shared_ptr<KeyCommandHandler> GetKeyCommandHandler() const;
    std::shared_ptr<EventMonitorHandler> GetMonitorHandler() const;
    std::shared_ptr<EventFilterHandler> GetFilterHandler() const;
#ifdef OHOS_BUILD_ENABLE_COOPERATE
    void SetJumpInterceptState(bool isJump);
    bool GetJumpInterceptState() const;
#endif // OHOS_BUILD_ENABLE_COOPERATE

private:
    int32_t BuildInputHandlerChain();

    UDSServer *udsServer_ { nullptr };
    std::shared_ptr<EventNormalizeHandler> eventNormalizeHandler_ { nullptr };
    std::shared_ptr<EventFilterHandler> eventFilterHandler_ { nullptr };
    std::shared_ptr<EventInterceptorHandler> eventInterceptorHandler_ { nullptr };
    std::shared_ptr<KeySubscriberHandler> eventSubscriberHandler_ { nullptr };
    std::shared_ptr<KeyCommandHandler> eventKeyCommandHandler_ { nullptr };
    std::shared_ptr<EventMonitorHandler> eventMonitorHandler_ { nullptr };

    uint64_t idSeed_ { 0 };
#ifdef OHOS_BUILD_ENABLE_COOPERATE
    bool isJumpIntercept_ { false };
#endif // OHOS_BUILD_ENABLE_COOPERATE
};
#define InputHandler ::OHOS::DelayedSingleton<InputEventHandler>::GetInstance()
} // namespace MMI
} // namespace OHOS
#endif // INPUT_EVENT_HANDLER_H
