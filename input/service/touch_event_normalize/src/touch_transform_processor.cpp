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

#include "touch_transform_processor.h"

#include <linux/input.h>

#include "event_log_helper.h"
#include "input_windows_manager.h"
#include "mmi_log.h"

namespace OHOS {
namespace MMI {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL { LOG_CORE, MMI_LOG_DOMAIN, "TouchTransformProcessor" };
constexpr int32_t MT_TOOL_NONE { -1 };
constexpr int32_t BTN_DOWN { 1 };
} // namespace

TouchTransformProcessor::TouchTransformProcessor(int32_t deviceId)
    : deviceId_(deviceId)
{
    InitToolTypes();
}

bool TouchTransformProcessor::OnEventTouchDown(struct libinput_event *event)
{
    CALL_DEBUG_ENTER;
    CHKPF(event);
    auto touch = libinput_event_get_touch_event(event);
    CHKPF(touch);
    auto device = libinput_event_get_device(event);
    CHKPF(device);
    EventTouch touchInfo;
    int32_t logicalDisplayId = -1;
    if (!WinMgr->TouchPointToDisplayPoint(deviceId_, touch, touchInfo, logicalDisplayId)) {
        MMI_HILOGE("TouchDownPointToDisplayPoint failed");
        return false;
    }
    auto pointIds = pointerEvent_->GetPointerIds();
    int64_t time = GetSysClockTime();
    if (pointIds.empty()) {
        pointerEvent_->SetActionStartTime(time);
        pointerEvent_->SetTargetDisplayId(logicalDisplayId);
    }
    pointerEvent_->SetActionTime(time);
    pointerEvent_->SetPointerAction(PointerEvent::POINTER_ACTION_DOWN);

    PointerEvent::PointerItem item;
    double pressure = libinput_event_touch_get_pressure(touch);
    int32_t seatSlot = libinput_event_touch_get_seat_slot(touch);
    int32_t longAxis = libinput_event_get_touch_contact_long_axis(touch);
    int32_t shortAxis = libinput_event_get_touch_contact_short_axis(touch);
    item.SetPressure(pressure);
    item.SetLongAxis(longAxis);
    item.SetShortAxis(shortAxis);
    int32_t toolType = GetTouchToolType(touch, device);
    item.SetToolType(toolType);
    item.SetPointerId(seatSlot);
    item.SetDownTime(time);
    item.SetPressed(true);
    item.SetDisplayX(touchInfo.point.x);
    item.SetDisplayY(touchInfo.point.y);
    item.SetToolDisplayX(touchInfo.toolRect.point.x);
    item.SetToolDisplayY(touchInfo.toolRect.point.y);
    item.SetToolWidth(touchInfo.toolRect.width);
    item.SetToolHeight(touchInfo.toolRect.height);
    item.SetDeviceId(deviceId_);
    pointerEvent_->SetDeviceId(deviceId_);
    pointerEvent_->AddPointerItem(item);
    pointerEvent_->SetPointerId(seatSlot);
    EventLogHelper::PrintEventData(pointerEvent_, pointerEvent_->GetPointerAction(),
        pointerEvent_->GetPointerIds().size());
    return true;
}

bool TouchTransformProcessor::OnEventTouchMotion(struct libinput_event *event)
{
    CALL_DEBUG_ENTER;
    CHKPF(event);
    auto touch = libinput_event_get_touch_event(event);
    CHKPF(touch);
    int64_t time = GetSysClockTime();
    pointerEvent_->SetActionTime(time);
    pointerEvent_->SetPointerAction(PointerEvent::POINTER_ACTION_MOVE);
    EventTouch touchInfo;
    int32_t logicalDisplayId = pointerEvent_->GetTargetDisplayId();
    if (!WinMgr->TouchPointToDisplayPoint(deviceId_, touch, touchInfo, logicalDisplayId)) {
        MMI_HILOGE("Get TouchMotionPointToDisplayPoint failed");
        return false;
    }
    PointerEvent::PointerItem item;
    int32_t seatSlot = libinput_event_touch_get_seat_slot(touch);
    if (!(pointerEvent_->GetPointerItem(seatSlot, item))) {
        MMI_HILOGE("Get pointer parameter failed");
        return false;
    }
    double pressure = libinput_event_touch_get_pressure(touch);
    int32_t longAxis = libinput_event_get_touch_contact_long_axis(touch);
    int32_t shortAxis = libinput_event_get_touch_contact_short_axis(touch);
    item.SetPressure(pressure);
    item.SetLongAxis(longAxis);
    item.SetShortAxis(shortAxis);
    item.SetDisplayX(touchInfo.point.x);
    item.SetDisplayY(touchInfo.point.y);
    item.SetToolDisplayX(touchInfo.toolRect.point.x);
    item.SetToolDisplayY(touchInfo.toolRect.point.y);
    item.SetToolWidth(touchInfo.toolRect.width);
    item.SetToolHeight(touchInfo.toolRect.height);
    pointerEvent_->UpdatePointerItem(seatSlot, item);
    pointerEvent_->SetPointerId(seatSlot);
    EventLogHelper::PrintEventData(pointerEvent_, pointerEvent_->GetPointerAction(),
        pointerEvent_->GetPointerIds().size());
    return true;
}

bool TouchTransformProcessor::OnEventTouchUp(struct libinput_event *event)
{
    CALL_DEBUG_ENTER;
    CHKPF(event);
    auto touch = libinput_event_get_touch_event(event);
    CHKPF(touch);
    int64_t time = GetSysClockTime();
    pointerEvent_->SetActionTime(time);
    pointerEvent_->SetPointerAction(PointerEvent::POINTER_ACTION_UP);

    PointerEvent::PointerItem item;
    int32_t seatSlot = libinput_event_touch_get_seat_slot(touch);
    if (!(pointerEvent_->GetPointerItem(seatSlot, item))) {
        MMI_HILOGE("Get pointer parameter failed");
        return false;
    }
    item.SetPressed(false);
    pointerEvent_->UpdatePointerItem(seatSlot, item);
    pointerEvent_->SetPointerId(seatSlot);
    return true;
}

std::shared_ptr<PointerEvent> TouchTransformProcessor::OnEvent(struct libinput_event *event)
{
    CALL_DEBUG_ENTER;
    CHKPP(event);
    if (pointerEvent_ == nullptr) {
        pointerEvent_ = PointerEvent::Create();
        CHKPP(pointerEvent_);
    }
    auto type = libinput_event_get_type(event);
    switch (type) {
        case LIBINPUT_EVENT_TOUCH_DOWN: {
            if (!OnEventTouchDown(event)) {
                MMI_HILOGE("Get OnEventTouchDown failed");
                return nullptr;
            }
            break;
        }
        case LIBINPUT_EVENT_TOUCH_UP: {
            if (!OnEventTouchUp(event)) {
                MMI_HILOGE("Get OnEventTouchUp failed");
                return nullptr;
            }
            break;
        }
        case LIBINPUT_EVENT_TOUCH_MOTION: {
            if (!OnEventTouchMotion(event)) {
                MMI_HILOGE("Get OnEventTouchMotion failed");
                return nullptr;
            }
            break;
        }
        default: {
            MMI_HILOGE("Unknown event type, touchType:%{public}d", type);
            return nullptr;
        }
    }
    pointerEvent_->SetSourceType(PointerEvent::SOURCE_TYPE_TOUCHSCREEN);
    pointerEvent_->UpdateId();
    WinMgr->UpdateTargetPointer(pointerEvent_);
    return pointerEvent_;
}

int32_t TouchTransformProcessor::GetTouchToolType(struct libinput_event_touch *data,
    struct libinput_device *device)
{
    int32_t toolType = libinput_event_touch_get_tool_type(data);
    switch (toolType) {
        case MT_TOOL_NONE: {
            return GetTouchToolType(device);
        }
        case MT_TOOL_FINGER: {
            return PointerEvent::TOOL_TYPE_FINGER;
        }
        case MT_TOOL_PEN: {
            return PointerEvent::TOOL_TYPE_PEN;
        }
        default : {
            MMI_HILOGW("Unknown tool type, identified as finger, toolType:%{public}d", toolType);
            return PointerEvent::TOOL_TYPE_FINGER;
        }
    }
}

int32_t TouchTransformProcessor::GetTouchToolType(struct libinput_device *device)
{
    for (const auto &item : vecToolType_) {
        if (libinput_device_touch_btn_tool_type_down(device, item.first) == BTN_DOWN) {
            return item.second;
        }
    }
    MMI_HILOGW("Unknown Btn tool type, identified as finger");
    return PointerEvent::TOOL_TYPE_FINGER;
}

void TouchTransformProcessor::InitToolTypes()
{
    vecToolType_.emplace_back(std::make_pair(BTN_TOOL_PEN, PointerEvent::TOOL_TYPE_PEN));
    vecToolType_.emplace_back(std::make_pair(BTN_TOOL_RUBBER, PointerEvent::TOOL_TYPE_RUBBER));
    vecToolType_.emplace_back(std::make_pair(BTN_TOOL_BRUSH, PointerEvent::TOOL_TYPE_BRUSH));
    vecToolType_.emplace_back(std::make_pair(BTN_TOOL_PENCIL, PointerEvent::TOOL_TYPE_PENCIL));
    vecToolType_.emplace_back(std::make_pair(BTN_TOOL_AIRBRUSH, PointerEvent::TOOL_TYPE_AIRBRUSH));
    vecToolType_.emplace_back(std::make_pair(BTN_TOOL_FINGER, PointerEvent::TOOL_TYPE_FINGER));
    vecToolType_.emplace_back(std::make_pair(BTN_TOOL_MOUSE, PointerEvent::TOOL_TYPE_MOUSE));
    vecToolType_.emplace_back(std::make_pair(BTN_TOOL_LENS, PointerEvent::TOOL_TYPE_LENS));
}
} // namespace MMI
} // namespace OHOS
