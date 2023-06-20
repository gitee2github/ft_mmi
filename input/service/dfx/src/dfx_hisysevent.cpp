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
#include "dfx_hisysevent.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MMI_LOG_DOMAIN, "DfxHisysevent" };
constexpr int32_t INVALID_DEVICE_ID = -1;
constexpr uint32_t REPORT_DISPATCH_TIMES = 100;
constexpr uint32_t REPORT_COMBO_START_TIMES = 100;
constexpr uint32_t POINTER_CLEAR_TIMES = 10;
constexpr int32_t CONVERSION_US_TO_MS = 1000;
constexpr int32_t TIMES_LEVEL1 = 10;
constexpr int32_t TIMES_LEVEL2 = 25;
constexpr int32_t TIMES_LEVEL3 = 30;
constexpr int32_t TIMES_LEVEL4 = 50;
} // namespace

void DfxHisysevent::OnDeviceConnect(int32_t id, OHOS::HiviewDFX::HiSysEvent::EventType type)
{
    std::shared_ptr dev = InputDevMgr->GetInputDevice(id);
    CHKPV(dev);
    std::string message;
    std::string name = "";
    if (type == OHOS::HiviewDFX::HiSysEvent::EventType::FAULT) {
        message = "The input_device connection failed for already existing";
        name = "INPUT_DEV_CONNECTION_FAILURE";
    } else {
        message = "The input_device connection succeed";
        name = "INPUT_DEV_CONNECTION_SUCCESS";
    }
    if (id == INT32_MAX) {
        int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
            name,
            type,
            "MSG", "The input_device connection failed because the nextId_ exceeded the upper limit");
        if (ret != 0) {
            MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
        }
    } else {
        int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
            name,
            type,
            "DEVICE_ID", id,
            "DEVICE_PHYS", dev->GetPhys(),
            "DEVICE_NAME", dev->GetName(),
            "DEVICE_TYPE", dev->GetType(),
            "MSG", message);
        if (ret != 0) {
            MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
        }
    }
}

void DfxHisysevent::OnDeviceDisconnect(int32_t id, OHOS::HiviewDFX::HiSysEvent::EventType type)
{
    if (id == INVALID_DEVICE_ID) {
        int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
            "INPUT_DEV_DISCONNECTION_FAILURE",
            type,
            "MSG", "The input device failed to disconnect to server");
        if (ret != 0) {
            MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
        }
    } else {
        std::shared_ptr dev = InputDevMgr->GetInputDevice(id);
        CHKPV(dev);
        int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
            "INPUT_DEV_DISCONNECTION_SUCCESS",
            type,
            "DEVICE_Id", id,
            "DEVICE_PHYS", dev->GetPhys(),
            "DEVICE_NAME", dev->GetName(),
            "DEVICE_TYPE", dev->GetType(),
            "MSG", "The input device successfully disconnect to server");
        if (ret != 0) {
            MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
        }
    }
}

void DfxHisysevent::OnClientConnect(const ClientConnectData &data, OHOS::HiviewDFX::HiSysEvent::EventType type)
{
    if (type == OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR) {
        int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
            "CLIENT_CONNECTION_SUCCESS",
            type,
            "PID", data.pid,
            "UID", data.uid,
            "MODULE_TYPE", data.moduleType,
            "SERVER_FD", data.serverFd,
            "PROGRAMNAME", data.programName,
            "MSG", "The client successfully connected to the server");
        if (ret != 0) {
            MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
        }
    } else {
        int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
            "CLIENT_CONNECTION_FAILURE",
            type,
            "PID", data.pid,
            "UID", data.uid,
            "MODULE_TYPE", data.moduleType,
            "PROGRAMNAME", data.programName,
            "MSG", "The client failed to connect to the server");
        if (ret != 0) {
            MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
        }
    }
}

void DfxHisysevent::OnClientDisconnect(const SessionPtr& secPtr, int32_t fd,
    OHOS::HiviewDFX::HiSysEvent::EventType type)
{
    CHKPV(secPtr);
    if (type == OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR) {
        int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
            "CLIENT_DISCONNECTION_SUCCESS",
            type,
            "PID", secPtr->GetPid(),
            "UID", secPtr->GetUid(),
            "MODULE_TYPE", secPtr->GetModuleType(),
            "FD", fd,
            "PROGRAMNAME", secPtr->GetProgramName(),
            "MSG", "The client successfully disconnected to the server");
        if (ret != 0) {
            MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
        }
    } else {
        if (secPtr == nullptr) {
            int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
                OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
                "CLIENT_DISCONNECTION_FAILURE",
                type,
                "MSG", "The client failed to disconnect to the server because secPtr is nullptr");
            if (ret != 0) {
                MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
            }
        } else {
            int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
                OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
                "CLIENT_DISCONNECTION_FAILURE",
                type,
                "MSG", "The client failed to disconnect to the server because close(fd) return error");
            if (ret != 0) {
                MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
            }
        }
    }
}

void DfxHisysevent::OnUpdateTargetPointer(std::shared_ptr<PointerEvent> pointer, int32_t fd,
    OHOS::HiviewDFX::HiSysEvent::EventType type)
{
    CHKPV(pointer);
    if (type == OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR) {
        int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
            "TARGET_POINTER_EVENT_SUCCESS",
            type,
            "EVENTTYPE", pointer->GetEventType(),
            "AGENT_WINDOWID", pointer->GetAgentWindowId(),
            "TARGET_WINDOWID", pointer->GetTargetWindowId(),
            "PID", WinMgr->GetWindowPid(pointer->GetTargetWindowId()),
            "FD", fd,
            "MSG", "The window manager successfully update target pointer");
        if (ret != 0) {
            MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
        }
    } else {
        int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
            "TARGET_POINTER_EVENT_FAILURE",
            OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
            "EVENTTYPE", pointer->GetEventType(),
            "MSG", "The window manager failed to update target pointer");
        if (ret != 0) {
            MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
        }
    }
}

void DfxHisysevent::OnUpdateTargetKey(std::shared_ptr<KeyEvent> key, int32_t fd,
    OHOS::HiviewDFX::HiSysEvent::EventType type)
{
    CHKPV(key);
    if (type == OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR) {
        int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
            "TARGET_KEY_EVENT_SUCCESS",
            type,
            "EVENTTYPE", key->GetEventType(),
            "KEYCODE", key->GetKeyCode(),
            "ACTION", key->GetAction(),
            "ACTION_TIME", key->GetActionTime(),
            "ACTION_STARTTIME", key->GetActionStartTime(),
            "FLAG", key->GetFlag(),
            "KEYACTION", key->GetKeyAction(),
            "FD", fd,
            "AGENT_WINDOWID", key->GetAgentWindowId(),
            "TARGET_WINDOWID", key->GetTargetWindowId(),
            "PID", WinMgr->GetWindowPid(key->GetTargetWindowId()),
            "MSG", "The window manager successfully update target key");
        if (ret != 0) {
            MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
        }
    } else {
        int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
            "TARGET_KEY_EVENT_FAILURE",
            type,
            "EVENTTYPE", key->GetEventType(),
            "KEYCODE", key->GetKeyCode(),
            "ACTION", key->GetAction(),
            "ACTION_TIME", key->GetActionTime(),
            "ACTION_STARTTIME", key->GetActionStartTime(),
            "FLAG", key->GetFlag(),
            "KEYACTION", key->GetKeyAction(),
            "MSG", "The window manager failed to update target key");
        if (ret != 0) {
            MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
        }
    }
}

void DfxHisysevent::OnFocusWindowChanged(int32_t oldFocusWindowId, int32_t newFocusWindowId,
    int32_t oldFocusWindowPid, int32_t newFocusWindowPid)
{
    int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
        OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
        "FOCUS_WINDOW_CHANGE",
        OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR,
        "OLD_FOCUS_WINDOWID", oldFocusWindowId,
        "NEW_FOCUS_WINDOWID", newFocusWindowId,
        "OLD_FOCUS_WINDOWPID", oldFocusWindowPid,
        "NEW_FOCUS_WINDOWPID", newFocusWindowPid,
        "MSG", "The focusWindowId changing succeeded");
    if (ret != 0) {
        MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
    }
}

void DfxHisysevent::OnZorderWindowChanged(int32_t oldZorderFirstWindowId, int32_t newZorderFirstWindowId,
    int32_t oldZorderFirstWindowPid, int32_t newZorderFirstWindowPid)
{
    int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
        OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
        "Z_ORDER_WINDOW_CHANGE",
        OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR,
        "OLD_ZORDER_FIRST_WINDOWID", oldZorderFirstWindowId,
        "NEW_ZORDER_FIRST_WINDOWID", newZorderFirstWindowId,
        "OLD_ZORDER_FIRST_WINDOWPID", oldZorderFirstWindowPid,
        "NEW_ZORDER_FIRST_WINDOWPID", newZorderFirstWindowPid,
        "MSG", "The ZorderFirstWindow changing succeeded");
    if (ret != 0) {
        MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
    }
}

void DfxHisysevent::ApplicationBlockInput(const SessionPtr& sess)
{
    int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
        OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
        "APPLICATION_BLOCK_INPUT",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        "PID", sess->GetPid(),
        "UID", sess->GetUid(),
        "PACKAGE_NAME", sess->GetProgramName(),
        "PROCESS_NAME", sess->GetProgramName(),
        "MSG", "User input does not respond");
    if (ret != 0) {
        MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
    }
}

void DfxHisysevent::CalcKeyDispTimes()
{
    int64_t endTime = GetSysClockTime();
    dispCastTime_.totalTimes++;
    int64_t castTime = (endTime - dispatchStartTime_)  / CONVERSION_US_TO_MS;
    if (castTime <= TIMES_LEVEL1) {
        dispCastTime_.below10msTimes++;
    } else if (castTime <= TIMES_LEVEL2) {
        dispCastTime_.below25msTimes++;
    } else if (castTime <= TIMES_LEVEL4) {
        dispCastTime_.below50msTimes++;
    } else {
        dispCastTime_.above50msTimes++;
    }
}

void DfxHisysevent::CalcPointerDispTimes()
{
    int64_t endTime = GetSysClockTime();
    dispCastTime_.sampleCount++;
    int64_t castTime = (endTime - dispatchStartTime_)  / CONVERSION_US_TO_MS;
    if (dispCastTime_.sampleCount == POINTER_CLEAR_TIMES) {
        dispCastTime_.sampleCount = 0;
        dispCastTime_.totalTimes++;
        if (castTime <= TIMES_LEVEL1) {
            dispCastTime_.below10msTimes++;
        } else if (castTime <= TIMES_LEVEL2) {
            dispCastTime_.below25msTimes++;
        } else if (castTime <= TIMES_LEVEL4) {
            dispCastTime_.below50msTimes++;
        } else {
            dispCastTime_.above50msTimes++;
        }
    }
}

void DfxHisysevent::ReportDispTimes()
{
    if (dispCastTime_.totalTimes >= REPORT_DISPATCH_TIMES) {
        int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
            "INPUT_DISPATCH_TIME",
            OHOS::HiviewDFX::HiSysEvent::EventType::STATISTIC,
            "BELOW10MS", dispCastTime_.below10msTimes,
            "BELOW25MS", dispCastTime_.below25msTimes,
            "BELOW50MS", dispCastTime_.below50msTimes,
            "ABOVE50MS", dispCastTime_.above50msTimes,
            "MSG", "The costing time to dispatch event");
        if (ret != 0) {
            MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
        } else {
            dispCastTime_.sampleCount = 0;
            dispCastTime_.totalTimes = 0;
            dispCastTime_.below10msTimes = 0;
            dispCastTime_.below25msTimes = 0;
            dispCastTime_.below50msTimes = 0;
            dispCastTime_.above50msTimes = 0;
        }
    }
}

void DfxHisysevent::CalcComboStartTimes(const int32_t keyDownDuration)
{
    int64_t endTime = GetSysClockTime();
    comboStartCastTime_.totalTimes++;
    int64_t castTime = (endTime - comboStartTime_) / CONVERSION_US_TO_MS - keyDownDuration;
    if (castTime <= TIMES_LEVEL1) {
        comboStartCastTime_.below10msTimes++;
    } else if (castTime <= TIMES_LEVEL3) {
        comboStartCastTime_.below30msTimes++;
    } else if (castTime <= TIMES_LEVEL4) {
        comboStartCastTime_.below50msTimes++;
    } else {
        comboStartCastTime_.above50msTimes++;
    }
}

void DfxHisysevent::ReportComboStartTimes()
{
    if (comboStartCastTime_.totalTimes >= REPORT_COMBO_START_TIMES) {
        int32_t ret = OHOS::HiviewDFX::HiSysEvent::Write(
            OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MODAL_INPUT,
            "COMBO_START_TIME",
            OHOS::HiviewDFX::HiSysEvent::EventType::STATISTIC,
            "BELOW10MS", comboStartCastTime_.below10msTimes,
            "BELOW30MS", comboStartCastTime_.below30msTimes,
            "BELOW50MS", comboStartCastTime_.below50msTimes,
            "ABOVE50MS", comboStartCastTime_.above50msTimes,
            "MSG", "The costing time to launch application of combination");
        if (ret != 0) {
            MMI_HILOGE("HiviewDFX Write failed, ret:%{public}d", ret);
        } else {
            comboStartCastTime_.totalTimes = 0;
            comboStartCastTime_.below10msTimes = 0;
            comboStartCastTime_.below30msTimes = 0;
            comboStartCastTime_.below50msTimes = 0;
            comboStartCastTime_.above50msTimes = 0;
        }
    }
}
}
}

