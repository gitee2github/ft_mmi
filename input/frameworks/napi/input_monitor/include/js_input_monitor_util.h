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
#ifndef JS_INPUT_MONITOR_UTIL_H
#define JS_INPUT_MONITOR_UTIL_H

#include "napi/native_api.h"
#include "napi/native_node_api.h"

namespace OHOS {
namespace MMI {
        napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, bool value);
        napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, uint16_t value);
        napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, int32_t value);
        napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, uint32_t value);
        napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, float value);
        napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, double value);
        napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, int64_t value);
        napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, std::string value);
        napi_status SetNameProperty(const napi_env& env, napi_value object, const std::string& name, napi_value value);

        bool GetNamePropertyBool(const napi_env& env, const napi_value& object, const std::string& name);
        std::string GetNamePropertyString(const napi_env& env, const napi_value& object, const std::string& name);
        int32_t GetNamePropertyInt32(const napi_env& env, const napi_value& object, const std::string& name);
        int64_t GetNamePropertyInt64(const napi_env& env, const napi_value& object, const std::string& name);
        uint32_t GetNamePropertyUint32(const napi_env& env, const napi_value& object, const std::string& name);
} // namespace MMI
} // namespace OHOS
#endif // JS_INPUT_MONITOR_UTIL_H
