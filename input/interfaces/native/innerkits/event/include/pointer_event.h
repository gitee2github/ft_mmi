/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef POINTER_EVENT_H
#define POINTER_EVENT_H

#include <array>
#include <list>
#include <memory>
#include <ostream>
#include <set>
#include <vector>

#include "nocopyable.h"
#include "parcel.h"

#include "input_event.h"

namespace OHOS {
namespace MMI {
class PointerEvent : public InputEvent {
public:
    /**
     * Indicates an unknown pointer action. It is usually used as initial value.
     *
     * @since 9
     */
    static constexpr int32_t POINTER_ACTION_UNKNOWN = 0;

    /**
     * Indicates a pointer action that has been canceled.
     *
     * @since 9
     */
    static constexpr int32_t POINTER_ACTION_CANCEL = 1;

    /**
     * Indicates a pointer action representing that a finger is pressed on a touchscreen or touchpad.
     *
     * @since 9
     */
    static constexpr int32_t POINTER_ACTION_DOWN = 2;

    /**
     * Indicates a pointer action representing that a finger moves on a touchscreen or touchpad or a mouse
     * pointer moves.
     *
     * @since 9
     */
    static constexpr int32_t POINTER_ACTION_MOVE = 3;

    /**
     * Indicates a pointer action representing that a finger leaves  the touchscreen or touchpad.
     *
     * @since 9
     */
    static constexpr int32_t POINTER_ACTION_UP = 4;

    /**
     * Indicates the start action of the axis event related to the pointer.
     *
     * @since 9
     */
    static constexpr int32_t POINTER_ACTION_AXIS_BEGIN = 5;

    /**
     * Indicates the update action of the axis event related to the pointer.
     *
     * @since 9
     */
    static constexpr int32_t POINTER_ACTION_AXIS_UPDATE = 6;

    /**
     * Indicates the end action of the axis event related to the pointer.
     *
     * @since 9
     */
    static constexpr int32_t POINTER_ACTION_AXIS_END = 7;

    /**
     * Indicates a pointer action representing that a button is pressed.
     *
     * @since 9
     */
    static constexpr int32_t POINTER_ACTION_BUTTON_DOWN = 8;

    /**
     * Indicates a pointer action representing that a button is released.
     *
     * @since 9
     */
    static constexpr int32_t POINTER_ACTION_BUTTON_UP = 9;

    /**
     * Indicates that the pointer enters the window.
     *
     * @since 9
     */
    static constexpr int32_t POINTER_ACTION_ENTER_WINDOW = 10;

    /**
     * Indicates that the pointer leaves the window.
     *
     * @since 9
     */
    static constexpr int32_t POINTER_ACTION_LEAVE_WINDOW = 11;

    enum AxisType {
        /**
         * Indicates an unknown axis type. It is generally used as the initial value.
         *
         * @since 9
         */
        AXIS_TYPE_UNKNOWN,

        /**
         * Indicates the vertical scroll axis. When you scroll the mouse wheel or make certain gestures on the touchpad,
         * the status of the vertical scroll axis changes.
         *
         * @since 9
         */
        AXIS_TYPE_SCROLL_VERTICAL,

        /**
         * Indicates the horizontal scroll axis. When you scroll the mouse wheel or make certain gestures on the touchpad,
         * the status of the horizontal scroll axis changes.
         *
         * @since 9
         */
        AXIS_TYPE_SCROLL_HORIZONTAL,

        /**
         * Indicates the pinch axis, which is used to describe a pinch gesture on the touchscreen or touchpad.
         *
         * @since 9
         */
        AXIS_TYPE_PINCH,

        /**
         * Indicates the maximum number of defined axis types.
         *
         * @since 9
         */
        AXIS_TYPE_MAX
    };

    /**
     * Indicates an unknown input source type. It is usually used as the initial value.
     *
     * @since 9
     */
    static constexpr int32_t SOURCE_TYPE_UNKNOWN = 0;

    /**
     * Indicates that the input source generates events similar to mouse cursor movement,
     * button press and release, and wheel scrolling.
     *
     * @since 9
     */
    static constexpr int32_t SOURCE_TYPE_MOUSE = 1;

    /**
     * Indicates that the input source generates a touchscreen multi-touch event.
     *
     * @since 9
     */
    static constexpr int32_t SOURCE_TYPE_TOUCHSCREEN = 2;

    /**
     * Indicates that the input source generates a touchpad multi-touch event.
     *
     * @since 9
     */
    static constexpr int32_t SOURCE_TYPE_TOUCHPAD = 3;

    /**
     * 表明输入源产生类似于操纵杆的事件，包含按钮按下、按钮抬起、滚轮滚动。
     *
     * @since 9
     */
    static constexpr int32_t SOURCE_TYPE_JOYSTICK = 4;

    /**
     * Indicates an invalid button ID.
     *
     * @since 9
     */
    static constexpr int32_t BUTTON_NONE = -1;

    /**
     * Indicates the left button on a mouse.
     *
     * @since 9
     */
    static constexpr int32_t MOUSE_BUTTON_LEFT = 0;

    /**
     * Indicates the right button on a mouse.
     *
     * @since 9
     */
    static constexpr int32_t MOUSE_BUTTON_RIGHT = 1;

    /**
     * Indicates the middle button on a mouse.
     *
     * @since 9
     */
    static constexpr int32_t MOUSE_BUTTON_MIDDLE = 2;

    /**
     * Indicates the side button on a mouse.
     *
     * @since 9
     */
    static constexpr int32_t MOUSE_BUTTON_SIDE = 3;

    /**
     * Indicates the extra button on a mouse.
     *
     * @since 9
     */
    static constexpr int32_t MOUSE_BUTTON_EXTRA = 4;

    /**
     * Indicates the forward button on a mouse.
     *
     * @since 9
     */
    static constexpr int32_t MOUSE_BUTTON_FORWARD = 5;

    /**
     * Indicates the back button on a mouse.
     *
     * @since 9
     */
    static constexpr int32_t MOUSE_BUTTON_BACK = 6;

    /**
     * Indicates the task button on a mouse.
     *
     * @since 9
     */
    static constexpr int32_t MOUSE_BUTTON_TASK = 7;

    /**
     * Indicates a finger.
     *
     * @since 9
     */
    static constexpr int32_t TOOL_TYPE_FINGER = 0;

    /**
     * Indicates a stylus.
     *
     * @since 9
     */
    static constexpr int32_t TOOL_TYPE_PEN = 1;

    /**
     * Indicates an eraser.
     *
     * @since 9
     */
    static constexpr int32_t TOOL_TYPE_RUBBER = 2;

    /**
     * Indicates a brush.
     *
     * @since 9
     */
    static constexpr int32_t TOOL_TYPE_BRUSH = 3;

    /**
     * Indicates a pencil.
     *
     * @since 9
     */
    static constexpr int32_t TOOL_TYPE_PENCIL = 4;

    /**
     * Indicates an air brush.
     *
     * @since 9
     */
    static constexpr int32_t TOOL_TYPE_AIRBRUSH = 5;

    /**
     * Indicates a mouse.
     *
     * @since 9
     */
    static constexpr int32_t TOOL_TYPE_MOUSE = 6;

    /**
     * Indicates a lens.
     *
     * @since 9
     */
    static constexpr int32_t TOOL_TYPE_LENS = 7;

public:
    static std::shared_ptr<PointerEvent> from(std::shared_ptr<InputEvent> inputEvent);

public:
    class PointerItem {
    public:
        PointerItem();
        ~PointerItem();

    public:
        /**
         * @brief Obtains the ID of the pointer in this event.
         * @return Returns the pointer ID.
         * @since 9
         */
        int32_t GetPointerId() const;

        /**
         * @brief Sets the ID of the pointer in this event.
         * @param pointerId Indicates the pointer ID to set.
         * @return void
         * @since 9
         */
        void SetPointerId(int32_t pointerId);

        /**
         * @brief Obtains the time when the pointer is pressed.
         * @return Returns the time.
         * @since 9
         */
        int64_t GetDownTime() const;

        /**
         * @brief Sets the time when the pointer is pressed.
         * @param downTime Indicates the time to set.
         * @return void
         * @since 9
         */
        void SetDownTime(int64_t downTime);

        /**
         * @brief Checks whether the pointer is pressed.
         * @return Returns <b>true</b> if the pointer is pressed; returns <b>false</b> otherwise.
         * @since 9
         */
        bool IsPressed() const;

        /**
         * @brief Sets whether to enable the pressed state for the pointer.
         * @param pressed Specifies whether to set the pressed state for the pointer.
         * The value <b>true</b> means to set the pressed state for the pointer, and the
         * value <b>false</b> means the opposite.
         * @return void
         * @since 9
         */
        void SetPressed(bool pressed);

        /**
         * @brief Obtains the x coordinate relative to the upper left corner of the screen.
         * For a touchpad input event, the value is the absolute x coordinate on the touchpad.
         * For other pointer input events, the value is the x coordinate on the target screen.
         * @return Returns the x coordinate.
         * @since 9
         */
        int32_t GetDisplayX() const;

        /**
         * @brief Sets the x coordinate relative to the upper left corner of the screen.
         * @param displayX Indicates the x coordinate to set.
         * @return void
         * @since 9
         */
        void SetDisplayX(int32_t displayX);

        /**
         * @brief Obtains the y coordinate relative to the upper left corner of the screen.
         * For a touchpad input event, the value is the absolute y coordinate on the touchpad.
         * For other pointer input events, the value is the y coordinate on the target screen.
         * @return Returns the y coordinate.
         * @since 9
         */
        int32_t GetDisplayY() const;

        /**
         * @brief Sets the y coordinate relative to the upper left corner of the screen.
         * @param displayY Indicates the y coordinate to set.
         * @return void
         * @since 9
         */
        void SetDisplayY(int32_t displayY);

        /**
         * @brief Obtains the x coordinate of the active window.
         * @return Returns the x coordinate.
         * @since 9
         */
        int32_t GetWindowX() const;

        /**
         * @brief Sets the x coordinate of the active window.
         * @param x Indicates the x coordinate to set.
         * @return void
         * @since 9
         */
        void SetWindowX(int32_t x);

        /**
         * @brief Obtains the y coordinate of the active window.
         * @return Returns the y coordinate.
         * @since 9
         */
        int32_t GetWindowY() const;

        /**
         * @brief Sets the y coordinate of the active window.
         * @param y Indicates the y coordinate to set.
         * @return void
         * @since 9
         */
        void SetWindowY(int32_t y);

        /**
         * @brief Obtains the width of the pressed area.
         * @return Returns the width.
         * @since 9
         */
        int32_t GetWidth() const;

        /**
         * @brief Sets the width of the pressed area.
         * @param width Indicates the width to set.
         * @return void
         * @since 9
         */
        void SetWidth(int32_t width);

        /**
         * @brief Obtains the height of the pressed area.
         * @return Returns the height.
         * @since 9
         */
        int32_t GetHeight() const;

        /**
         * @brief Sets the height of the pressed area.
         * @param height Indicates the height to set.
         * @return void
         * @since 9
         */
        void SetHeight(int32_t height);

        /**
         * @brief Obtains the X coordinate of the tool area's center point relative to the
         * upper left corner of the screen.
         * @return Returns the X coordinate.
         * @since 9
         */
        int32_t GetToolDisplayX() const;

        /**
         * @brief Sets the X coordinate of the tool area's center point relative to the
         * upper left corner of the screen.
         * @param x Indicates the X coordinate.
         * @return void
         * @since 9
         */
        void SetToolDisplayX(int32_t displayX);

        /**
         * @brief Obtains the Y coordinate of the tool area's center point relative to the
         * upper left corner of the screen.
         * @return Returns the Y coordinate.
         * @since 9
         */
        int32_t GetToolDisplayY() const;

        /**
         * @brief Sets the Y coordinate of the tool area's center point relative to the
         * upper left corner of the screen.
         * @param y Indicates the Y coordinate.
         * @return void
         * @since 9
         */
        void SetToolDisplayY(int32_t displayY);

        /**
         * @brief Obtains the X coordinate of the tool area's center point relative to the
         * upper left corner of the window.
         * @return Returns the X coordinate.
         * @since 9
         */
        int32_t GetToolWindowX() const;

        /**
         * @brief Sets the X coordinate of the tool area's center point relative to the
         * upper left corner of the window.
         * @param x Indicates the X coordinate.
         * @return void
         * @since 9
         */
        void SetToolWindowX(int32_t x);

        /**
         * @brief Obtains the Y coordinate of the tool area's center point relative to the
         * upper left corner of the window.
         * @return Returns the Y coordinate.
         * @since 9
         */
        int32_t GetToolWindowY() const;

        /**
         * @brief Sets the Y coordinate of the tool area's center point relative to the
         * upper left corner of the window.
         * @param y Indicates the Y coordinate.
         * @return void
         * @since 9
         */
        void SetToolWindowY(int32_t y);

        /**
         * @brief Obtains the width of the tool area.
         * @return Returns the width of the tool area.
         * @since 9
         */
        int32_t GetToolWidth() const;

        /**
         * @brief Sets the width of the tool area.
         * @param width Indicates the width of the tool area.
         * @return void
         * @since 9
         */
        void SetToolWidth(int32_t width);

        /**
         * @brief Obtains the height of the tool area.
         * @return Returns the height of the tool area.
         * @since 9
         */
        int32_t GetToolHeight() const;

        /**
         * @brief Sets the height of the tool area.
         * @param height Indicates the height of the tool area.
         * @return void
         * @since 9
         */
        void SetToolHeight(int32_t height);

        /**
         * @brief Obtains the tilt angle of the x axis.
         * @return Returns the tilt angle of the x axis.
         * @since 9
         */
        double GetTiltX() const;

        /**
         * @brief Sets the tilt angle of the x axis.
         * @param tiltX Indicates the tilt angle to set.
         * @return void
         * @since 9
         */
        void SetTiltX(double tiltX);

        /**
         * @brief Obtains the tilt angle of the y axis.
         * @return Returns the tilt angle of the y axis.
         * @since 9
         */
        double GetTiltY() const;

        /**
         * @brief Sets the tilt angle of the y axis.
         * @param tiltY Indicates the tilt angle to set.
         * @return void
         * @since 9
         */
        void SetTiltY(double tiltY);

        /**
         * @brief Obtains the pressure in this event.
         * @return Returns the pressure.
         * @since 9
         */
        double GetPressure() const;

        /**
         * @brief Sets the pressure for this event.
         * @param pressure Indicates the pressure to set.
         * @return void
         * @since 9
         */
        void SetPressure(double pressure);

        /**
         * @brief Obtains the long axis of the touch point area.
         * @return Returns the long axis of the touch point area.
         * @since 9
         */
        int32_t GetLongAxis() const;

        /**
         * @brief Sets the long axis of the touch point area.
         * @param longAxis Indicates the long axis of the touch point area.
         * @return void
         * @since 9
         */
        void SetLongAxis(int32_t longAxis);

        /**
         * @brief Obtains the short axis of the touch point area.
         * @return Returns the short axis of the touch point area.
         * @since 9
         */
        int32_t GetShortAxis() const;

        /**
         * @brief Sets the short axis of the touch point area.
         * @param shortAxis Indicates the short axis of the touch point area.
         * @return void
         * @since 9
         */
        void SetShortAxis(int32_t shortAxis);

        /**
         * @brief Obtains the ID of the current device.
         * @return Returns the device ID.
         * @since 9
         */
        int32_t GetDeviceId() const;

        /**
         * @brief Sets the ID for the current device.
         * @param deviceId Indicates the device ID to set.
         * @return void
         * @since 9
         */
        void SetDeviceId(int32_t deviceId);

        /**
         * @brief Obtains the tool type.
         * @return Returns the tool type.
         * @since 9
         */
        int32_t GetToolType() const;

        /**
         * @brief Sets the tool type.
         * @param toolType Indicates the tool type to set.
         * @return void
         * @since 9
         */
        void SetToolType(int32_t toolType);

        /**
         * @brief Obtains the ID of the window corresponding to the finger touch position.
         * @return Returns the ID of the target window.
         * @since 9
         */
        int32_t GetTargetWindowId() const;

        /**
         * @brief Sets the ID of the window corresponding to the finger touch position.
         * @param windowId Indicates the ID of the target window.
         * @return void
         * @since 9
         */
        void SetTargetWindowId(int32_t windowId);

        /**
         * @brief Writes data to a <b>Parcel</b> object.
         * @param out Indicates the object into which data will be written.
         * @return Returns <b>true</b> if the data is successfully written; returns <b>false</b> otherwise.
         * @since 9
         */
        bool WriteToParcel(Parcel &out) const;

        /**
         * @brief Reads data from a <b>Parcel</b> object.
         * @param in Indicates the object from which data will be read.
         * @return Returns <b>true</b> if the data is successfully read; returns <b>false</b> otherwise.
         * @since 9
         */
        bool ReadFromParcel(Parcel &in);
		
        /**
         * @brief 获取原始X坐标数值.
         * @return  返回原始X坐标值.
         * @since 9
         */
        int32_t GetRawDx() const;
		
        /**
         * @brief 设置原始X坐标值.
         * @param rawDx 待设置的原始X坐标值.
         * @return void
         * @since 9
         */
        void SetRawDx(int32_t rawDx);
        /**
         * @brief 获取原始Y坐标数值.
         * @return  返回原始Y坐标值.
         * @since 9
         */
        int32_t GetRawDy() const;
		
        /**
         * @brief 设置原始Y坐标值.
         * @param rawDy 待设置的原始Y坐标值.
         * @return void
         * @since 9
         */
        void SetRawDy(int32_t rawDy);
    private:
        int32_t pointerId_ {};
        bool pressed_ { false };
        int32_t displayX_ {};
        int32_t displayY_ {};
        int32_t windowX_ {};
        int32_t windowY_ {};
        int32_t width_ {};
        int32_t height_ {};
        double  tiltX_ {};
        double  tiltY_ {};
        int32_t toolDisplayX_ {};
        int32_t toolDisplayY_ {};
        int32_t toolWindowX_ {};
        int32_t toolWindowY_ {};
        int32_t toolWidth_ {};
        int32_t toolHeight_ {};
        double  pressure_ {};
        int32_t longAxis_ {};
        int32_t shortAxis_ {};
        int32_t deviceId_ {};
        int64_t downTime_ {};
        int32_t toolType_ {};
        int32_t targetWindowId_ { -1 };
        int32_t rawDx_ {};
        int32_t rawDy_ {};
    };

public:
    /**
     * @brief Copy constructor function for PointerEvent
     * @since 9
     */
    PointerEvent(const PointerEvent& other);

    /**
     * Virtual destructor of PointerEvent
     *
     * @since 9
     */
    virtual ~PointerEvent();

    PointerEvent& operator=(const PointerEvent& other) = delete;
    DISALLOW_MOVE(PointerEvent);

    /**
     * @brief Create PointerEvent object
     * @since 9
     */
    static std::shared_ptr<PointerEvent> Create();

    virtual void Reset() override;

    /**
     * @brief Obtains the pointer action in this event.
     * @return Returns the pointer action.
     * @since 9
     */
    int32_t GetPointerAction() const;

    /**
     * @brief Sets a pointer action for this event.
     * @param pointerAction Indicates the pointer action to set.
     * @return void
     * @since 9
     */
    void SetPointerAction(int32_t pointerAction);

    /**
     * @brief Dumps the action of this pointer input event as a string.
     * @return Returns the pointer to the string.
     * @since 9
     */
    const char* DumpPointerAction() const;

    /**
     * @brief Obtains the pointer ID in this event.
     * @return Returns the pointer ID.
     * @since 9
     */
    int32_t GetPointerId() const;

    /**
     * @brief Sets an ID for the pointer in this event.
     * @param pointerId Indicates the pointer ID to set.
     * @return void
     * @since 9
     */
    void SetPointerId(int32_t pointerId);

    /**
     * @brief Obtains the pointer item of a specified pointer ID.
     * @param pointerId Indicates the pointer ID.
     * @param pointerItem Indicates the item used to receive the data of the pointer.
     * @return Returns <b>true</b> if the data of the pointer with the specified ID exists;
     * returns <b>false</b> otherwise.
     * @since 9
     */
    bool GetPointerItem(int32_t pointerId, PointerItem &pointerItem);

    /**
     * @brief Adds a pointer item.
     * @param pointerItem Indicates the pointer item to add.
     * @return void
     * @since 9
     */
    void AddPointerItem(PointerItem &pointerItem);

    /**
     * @brief Removes a pointer item based on the pointer ID.
     * @param pointerId Indicates the ID of the pointer from which the pointer item is to be removed.
     * @return void
     * @since 9
     */
    void RemovePointerItem(int32_t pointerId);

    /**
     * @brief Updates a pointer item based on the pointer ID.
     * @param pointerId Indicates the ID of the pointer from which the pointer item is to be updated.
     * @param pointerItem Indicates the pointer item to update.
     * @return void
     * @since 9
     */
    void UpdatePointerItem(int32_t pointerId, PointerItem &pointerItem);

    /**
     * @brief Obtains the set of pressed buttons.
     * @return Returns the pressed buttons.
     * @since 9
     */
    std::set<int32_t> GetPressedButtons() const;

    /**
     * @brief Checks whether a specified button is being pressed.
     * @param buttonId Indicates the button ID.
     * @return Returns <b>true</b> if the button is being pressed; returns <b>false</b> otherwise.
     * @since 9
     */
    bool IsButtonPressed(int32_t buttonId) const;

    /**
     * @brief Sets the pressed state for a button.
     * @param buttonId Indicates the button ID of the button to be set in the pressed state.
     * @return void
     * @since 9
     */
    void SetButtonPressed(int32_t buttonId);

    /**
     * @brief Deletes a released button.
     * @param buttonId Indicates the button ID of the button.
     * @return void
     * @since 9
     */
    void DeleteReleaseButton(int32_t buttonId);

    /**
     * @brief Clears the button in the pressed state.
     * @return void
     * @since 9
     */
    void ClearButtonPressed();

    /**
     * @brief Obtains all pointers in this event.
     * @return Returns all the pointer IDs.
     * @since 9
     */
    std::vector<int32_t> GetPointerIds() const;

    /**
     * @brief Obtains the source type of this event.
     * @return Returns the source type.
     * @since 9
     */
    int32_t GetSourceType() const;

    /**
     * @brief Sets the source type for this event.
     * @param sourceType Indicates the source type to set.
     * @return void
     * @since 9
     */
    void SetSourceType(int32_t sourceType);

    /**
     * @brief Dumps the source type of this pointer input event as a string.
     * @return Returns the pointer to the string.
     * @since 9
     */
    const char* DumpSourceType() const;

    /**
     * @brief Obtains the button ID in this event.
     * @return Returns the button ID.
     * @since 9
     */
    int32_t GetButtonId() const;

    /**
     * @brief Sets the button ID for this event.
     * @param buttonId Indicates the button ID to set.
     * @return void
     * @since 9
     */
    void SetButtonId(int32_t buttonId);

    /**
     * @brief Obtains the axis value.
     * @param axis Indicates the axis type.
     * @return Returns the axis value.
     * @since 9
     */
    double GetAxisValue(AxisType axis) const;

    /**
     * @brief Sets the axis value.
     * @param axis Indicates the axis type.
     * @param axisValue Indicates the axis value to set.
     * @return void
     * @since 9
     */
    void SetAxisValue(AxisType axis, double axisValue);

    /**
     * @brief Clear the axis value of PointerEvent when a mouse event is received.
     * @return void
     * @since 9
     */
    void ClearAxisValue();

    /**
     * @brief Checks whether this event contains a specified axis type.
     * @param axis Indicates the axis type.
     * @return Returns <b>true</b> if the event contains the specified axis type; returns <b>false</b> otherwise.
     * @since 9
     */
    bool HasAxis(AxisType axis) const;

    /**
     * @brief Obtains all axis of this event.
     * @return Returns all the axis, Each bit indicates an axis.
     * @since 9
     */
    uint32_t GetAxes() const;

    /**
     * @brief Set the front keys in the key combination.
     * @param pressedKeys Indicates the front keys to set.
     * @return void.
     * @since 9
     */
    void SetPressedKeys(const std::vector<int32_t> pressedKeys);

    /**
     * @brief Obtains the set of pressed keys.
     * @return Returns the pressed keys.
     * @since 9
     */
    std::vector<int32_t> GetPressedKeys() const;

    /**
     * @brief Checks whether this input event is valid.
     * @return Returns <b>true</b> if the input event is valid; returns <b>false</b> otherwise.
     * @since 9
     */
    bool IsValid() const;
public:
    /**
     * @brief Checks whether the axes set represented by <b>axes</b> contains a specified type of axis.
     * @param axes Indicates the set of axes. Each bit indicates an axis.
     * @param axis Indicates the type of the axis to check.
     * @return Returns <b>true</b> if the axes set contains the specified axis type; returns <b>false</b> otherwise.
     * @since 9
     */
    static bool HasAxis(uint32_t axes, AxisType axis);

public:
    /**
     * @brief Writes data to a <b>Parcel</b> object.
     * @param out Indicates the object into which data will be written.
     * @return Returns <b>true</b> if the data is successfully written; returns <b>false</b> otherwise.
     * @since 9
     */
    bool WriteToParcel(Parcel &out) const;

    /**
     * @brief Reads data from a <b>Parcel</b> object.
     * @param in Indicates the object from which data will be read.
     * @return Returns <b>true</b> if the data is successfully read; returns <b>false</b> otherwise.
     * @since 9
     */
    bool ReadFromParcel(Parcel &in);

protected:
    /**
     * @brief Constructs an input event object by using the specified input event type. Generally, this method
     * is used to construct a base class object when constructing a derived class object.
     * @since 9
     */
    explicit PointerEvent(int32_t eventType);

private:
    bool IsValidCheckMouseFunc() const;
    bool IsValidCheckMouse() const;
    bool IsValidCheckTouchFunc() const;
    bool IsValidCheckTouch() const;

private:
    int32_t pointerId_ { -1 };
    std::list<PointerItem> pointers_;
    std::set<int32_t> pressedButtons_;
    int32_t sourceType_ { SOURCE_TYPE_UNKNOWN };
    int32_t pointerAction_ { POINTER_ACTION_UNKNOWN };
    int32_t buttonId_ { -1 };
    uint32_t axes_ { 0U };
    std::array<double, AXIS_TYPE_MAX>   axisValues_ {};
    std::vector<int32_t> pressedKeys_;
};

inline bool PointerEvent::HasAxis(AxisType axis) const
{
    return HasAxis(axes_, axis);
}

inline uint32_t PointerEvent::GetAxes() const
{
    return axes_;
}
} // namespace MMI
} // namespace OHOS
#endif // POINTER_EVENT_H
