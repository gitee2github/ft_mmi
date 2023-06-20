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

#ifndef I_UDS_SERVER_H
#define I_UDS_SERVER_H

#include "iremote_broker.h"
#include "uds_session.h"

namespace OHOS {
namespace MMI {
class IUdsServer : public RefBase {
public:
    virtual int32_t AddSocketPairInfo(const std::string& programName, const int32_t moduleType, const int32_t uid,
                                      const int32_t pid, int32_t& serverFd, int32_t& toReturnClientFd,
                                      int32_t& tokenType) = 0;
    virtual SessionPtr GetSessionByPid(int32_t pid) const = 0;
};
} // namespace MMI
} // namespace OHOS
#endif // I_UDS_SERVER_H