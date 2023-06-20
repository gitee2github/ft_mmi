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

#include "addmonitor_fuzzer.h"

#include "input_manager.h"
#include "mmi_log.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "AddMonitorFuzzTeset" };
} // namespace

class InputEventConsumerTest : public IInputEventConsumer {
public:
    virtual void OnInputEvent(std::shared_ptr<KeyEvent> keyEvent) const override {};
    virtual void OnInputEvent(std::shared_ptr<PointerEvent> pointerEvent) const override
    {
        MMI_HILOGD("Report pointer event success");
    };
    virtual void OnInputEvent(std::shared_ptr<AxisEvent> axisEvent) const override {};
};

bool AddMonitorFuzzTeset(const uint8_t* data, size_t /* size */)
{
    auto keyEventFun = [](std::shared_ptr<KeyEvent> event) {
        MMI_HILOGD("Add monitor success");
    };
    int32_t monitorId = InputManager::GetInstance()->AddMonitor(keyEventFun);
    InputManager::GetInstance()->RemoveMonitor(monitorId);

    auto PointerEventFun = [](std::shared_ptr<PointerEvent> event) {
        MMI_HILOGD("Add monitor success");
    };
    monitorId = InputManager::GetInstance()->AddMonitor(PointerEventFun);
    InputManager::GetInstance()->RemoveMonitor(monitorId);

    auto consumer = std::make_shared<InputEventConsumerTest>();
    monitorId = InputManager::GetInstance()->AddMonitor(consumer);
    InputManager::GetInstance()->RemoveMonitor(monitorId);
    return true;
}
} // MMI
} // OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::MMI::AddMonitorFuzzTeset(data, size);
    return 0;
}

