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

#ifndef JS_POINTER_MANAGER_H
#define JS_POINTER_MANAGER_H

#include <memory>

#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "napi_constants.h"
#include "utils/log.h"
#include "refbase.h"

#include "stream_buffer.h"
#include "define_multimodal.h"
#include "error_multimodal.h"
#include "input_manager.h"
#include "util_napi.h"
#include "util_napi_error.h"

namespace OHOS {
namespace MMI {
class JsCommon {
public:
    static bool TypeOf(napi_env env, napi_value value, napi_valuetype type);
};

struct AsyncContext : RefBase {
    napi_env env { nullptr };
    napi_async_work work { nullptr };
    napi_deferred deferred { nullptr };
    napi_ref callback { nullptr };
    int32_t errorCode { -1 };
    StreamBuffer reserve;
    AsyncContext(napi_env env) : env(env) {}
    ~AsyncContext();
};

class JsPointerManager {
public:
    JsPointerManager() = default;
    ~JsPointerManager() = default;
    DISALLOW_COPY_AND_MOVE(JsPointerManager);

    void ResetEnv();
    napi_value SetPointerVisible(napi_env env, bool visible, napi_value handle = nullptr);
    napi_value IsPointerVisible(napi_env env, napi_value handle = nullptr);
    napi_value SetPointerSpeed(napi_env env, int32_t pointerSpeed, napi_value handle = nullptr);
    napi_value GetPointerSpeed(napi_env env, napi_value handle = nullptr);
    napi_value SetPointerStyle(napi_env env, int32_t windowid, int32_t pointerStyle, napi_value handle = nullptr);
    napi_value GetPointerStyle(napi_env env, int32_t windowid, napi_value handle = nullptr);
};
} // namespace MMI
} // namespace OHOS

#endif // JS_POINTER_MANAGER_H