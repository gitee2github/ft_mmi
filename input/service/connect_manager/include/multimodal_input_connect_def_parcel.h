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

#ifndef MULTIMODAL_INPUT_CONNECT_DEF_PARCEL_H
#define MULTIMODAL_INPUT_CONNECT_DEF_PARCEL_H

#include "parcel.h"

#include "multimodal_input_connect_define.h"

namespace OHOS {
namespace MMI {
class ConnectReqParcel final : public Parcelable {
public:
    static ConnectReqParcel *Unmarshalling(Parcel &in);
    ConnectReqParcel() = default;
    ~ConnectReqParcel() override = default;
    bool Marshalling(Parcel &out) const override;
    ConnectDefReq data;
};
} // namespace MMI
} // namespace OHOS
#endif // MULTIMODAL_INPUT_CONNECT_DEF_PARCEL_H