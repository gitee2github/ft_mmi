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

#include "registerdevListener_fuzzer.h"

#include "input_manager.h"
#include "mmi_log.h"
#include "i_input_device_listener.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "RegisterDevListenerFuzzTest" };
} // namespace

class InputDeviceListenerTest : public IInputDeviceListener {
public:
    InputDeviceListenerTest() : IInputDeviceListener() {}
    void OnDeviceAdded(int32_t deviceId, const std::string &type) override
    {
        MMI_HILOGD("Add device success");
    };
    void OnDeviceRemoved(int32_t deviceId, const std::string &type) override
    {
        MMI_HILOGD("Remove device success");
    };
};

void RegisterDevListenerFuzzTest(const uint8_t* data, size_t  size)
{
    if ((data == nullptr) || (size <= 0)) {
        return;
    }
    MMI_HILOGD("RegisterDevListenerFuzzTest");
    const std::string type(reinterpret_cast<const char*>(data), size);
    std::shared_ptr<InputDeviceListenerTest> listener = std::make_shared<InputDeviceListenerTest>();
    InputManager::GetInstance()->RegisterDevListener(type, listener);
}
} // MMI
} // OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::MMI::RegisterDevListenerFuzzTest(data, size);
    return 0;
}

