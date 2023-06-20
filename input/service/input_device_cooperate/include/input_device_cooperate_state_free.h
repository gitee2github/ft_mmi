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

#ifndef INPUT_DEVICE_COOPERATE_STATE_FREE_H
#define INPUT_DEVICE_COOPERATE_STATE_FREE_H

#include "i_input_device_cooperate_state.h"

namespace OHOS {
namespace MMI {
class InputDeviceCooperateStateFree : public IInputDeviceCooperateState {
public:
    virtual int32_t StartInputDeviceCooperate(const std::string &remoteNetworkId, int32_t startInputDeviceId) override;

private:
    int32_t ProcessStart(const std::string &remoteNetworkId, int32_t startInputDeviceId);
};
} // namespace MMI
} // namespace OHOS
#endif // INPUT_DEVICE_COOPERATE_STATE_FREE_H
