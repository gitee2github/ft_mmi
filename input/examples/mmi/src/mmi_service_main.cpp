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

#include "mmi_service.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "MmiServiceDemo" };
} // namespace
} // namespace MMI
} // namespace OHOS

int32_t main(int32_t argc, const char *argv[])
{
    using namespace OHOS::MMI;
    auto service = OHOS::DelayedSingleton<MMIService>::GetInstance();
    service->OnStart();
    constexpr int32_t sleepTime = 10 * 60;
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(sleepTime));
    }
    service->OnStop();
    service->OnDump();

    MMI_HILOGD("mmi-service stopping. argc:%{public}d, argv:%{public}s", argc, argv[0]);
    return RET_OK;
}
