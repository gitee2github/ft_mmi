/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#ifndef BYTRACE_ADAPTER_H
#define BYTRACE_ADAPTER_H

#include <memory>

#include "key_event.h"
#include "pointer_event.h"

namespace OHOS {
namespace MMI {
class BytraceAdapter {
public:
    enum TraceBtn {
        TRACE_STOP = 0,
        TRACE_START = 1
    };
    enum HandlerType {
        KEY_INTERCEPT_EVENT = 1,
        KEY_LAUNCH_EVENT = 2,
        KEY_SUBSCRIBE_EVENT = 3,
        KEY_DISPATCH_EVENT = 4,
        POINT_INTERCEPT_EVENT = 5,
        POINT_DISPATCH_EVENT = 6
    };
    enum EventType {
        START_EVENT = 1,
        LAUNCH_EVENT = 2,
        STOP_EVENT = 3
    };

    static void StartBytrace(std::shared_ptr<KeyEvent> keyEvent);
    static void StartBytrace(std::shared_ptr<KeyEvent> key, HandlerType handlerType);
    static void StartBytrace(std::shared_ptr<PointerEvent> pointerEvent, TraceBtn traceBtn);
    static void StartBytrace(std::shared_ptr<KeyEvent> keyEvent, TraceBtn traceBtn, HandlerType handlerType);
    static void StartBytrace(std::shared_ptr<PointerEvent> pointerEvent, TraceBtn traceBtn, HandlerType handlerType);
    static void StartBytrace(TraceBtn traceBtn, EventType eventType);
};
} // namespace MMI
} // namespace OHOS
#endif // BYTRACE_ADAPTER_H
