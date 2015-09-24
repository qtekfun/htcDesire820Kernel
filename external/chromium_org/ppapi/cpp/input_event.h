// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_INPUT_EVENT_H_
#define PPAPI_CPP_INPUT_EVENT_H_

#include <string>
#include <vector>

#include "ppapi/c/ppb_input_event.h"
#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/touch_point.h"


namespace pp {

class FloatPoint;
class InstanceHandle;
class Point;
class Var;

class InputEvent : public Resource {
 public:
  
  InputEvent();

  
  
  
  
  
  explicit InputEvent(PP_Resource input_event_resource);

  ~InputEvent();

  
  
  
  
  
  PP_InputEvent_Type GetType() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  PP_TimeTicks GetTimeStamp() const;

  
  
  
  
  
  
  uint32_t GetModifiers() const;
};

class MouseInputEvent : public InputEvent {
 public:
  
  MouseInputEvent();

  
  
  
  
  
  explicit MouseInputEvent(const InputEvent& event);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  MouseInputEvent(const InstanceHandle& instance,
                  PP_InputEvent_Type type,
                  PP_TimeTicks time_stamp,
                  uint32_t modifiers,
                  PP_InputEvent_MouseButton mouse_button,
                  const Point& mouse_position,
                  int32_t click_count,
                  const Point& mouse_movement);

  
  
  
  
  
  PP_InputEvent_MouseButton GetButton() const;

  
  
  
  
  
  
  
  Point GetPosition() const;

  
  int32_t GetClickCount() const;

  
  
  
  
  
  
  
  Point GetMovement() const;
};

class WheelInputEvent : public InputEvent {
 public:
  
  WheelInputEvent();

  
  
  
  
  
  
  explicit WheelInputEvent(const InputEvent& event);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  WheelInputEvent(const InstanceHandle& instance,
                  PP_TimeTicks time_stamp,
                  uint32_t modifiers,
                  const FloatPoint& wheel_delta,
                  const FloatPoint& wheel_ticks,
                  bool scroll_by_page);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  FloatPoint GetDelta() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  FloatPoint GetTicks() const;

  
  
  
  
  
  bool GetScrollByPage() const;
};

class KeyboardInputEvent : public InputEvent {
 public:
  
  KeyboardInputEvent();

  
  
  
  
  
  explicit KeyboardInputEvent(const InputEvent& event);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  KeyboardInputEvent(const InstanceHandle& instance,
                     PP_InputEvent_Type type,
                     PP_TimeTicks time_stamp,
                     uint32_t modifiers,
                     uint32_t key_code,
                     const Var& character_text);

  
  
  uint32_t GetKeyCode() const;

  
  
  
  
  
  Var GetCharacterText() const;
};

class TouchInputEvent : public InputEvent {
 public:
  
  TouchInputEvent();

  
  
  
  explicit TouchInputEvent(const InputEvent& event);

  
  
  
  
  
  
  
  
  
  
  
  
  TouchInputEvent(const InstanceHandle& instance,
                  PP_InputEvent_Type type,
                  PP_TimeTicks time_stamp,
                  uint32_t modifiers);

  
  void AddTouchPoint(PP_TouchListType list, PP_TouchPoint point);

  
  uint32_t GetTouchCount(PP_TouchListType list) const;

  
  
  TouchPoint GetTouchByIndex(PP_TouchListType list, uint32_t index) const;

  
  
  
  TouchPoint GetTouchById(PP_TouchListType list, uint32_t id) const;
};

class IMEInputEvent : public InputEvent {
 public:
  
  IMEInputEvent();

  
  
  
  
  
  explicit IMEInputEvent(const InputEvent& event);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  IMEInputEvent(const InstanceHandle& instance,
                PP_InputEvent_Type type,
                PP_TimeTicks time_stamp,
                const Var& text,
                const std::vector<uint32_t>& segment_offsets,
                int32_t target_segment,
                const std::pair<uint32_t, uint32_t>& selection);

  
  
  
  
  Var GetText() const;

  
  
  
  
  uint32_t GetSegmentNumber() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  uint32_t GetSegmentOffset(uint32_t index) const;

  
  
  
  
  
  int32_t GetTargetSegment() const;

  
  
  
  
  
  void GetSelection(uint32_t* start, uint32_t* end) const;
};
}  

#endif  
