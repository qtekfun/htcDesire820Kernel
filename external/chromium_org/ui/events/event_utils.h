// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_EVENT_UTILS_H_
#define UI_EVENTS_EVENT_UTILS_H_

#include "base/event_types.h"
#include "ui/events/event_constants.h"
#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/events/events_export.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

namespace gfx {
class Point;
class Vector2d;
}

namespace base {
class TimeDelta;
}

namespace ui {

class Event;

EVENTS_EXPORT void UpdateDeviceList();

EVENTS_EXPORT EventType EventTypeFromNative(
    const base::NativeEvent& native_event);

EVENTS_EXPORT int EventFlagsFromNative(const base::NativeEvent& native_event);

EVENTS_EXPORT base::TimeDelta EventTimeFromNative(
    const base::NativeEvent& native_event);

EVENTS_EXPORT base::TimeDelta EventTimeForNow();

EVENTS_EXPORT gfx::Point EventLocationFromNative(
    const base::NativeEvent& native_event);

EVENTS_EXPORT gfx::Point EventSystemLocationFromNative(
    const base::NativeEvent& native_event);

#if defined(USE_X11)
EVENTS_EXPORT int EventButtonFromNative(const base::NativeEvent& native_event);
#endif

EVENTS_EXPORT KeyboardCode KeyboardCodeFromNative(
    const base::NativeEvent& native_event);

EVENTS_EXPORT const char* CodeFromNative(
    const base::NativeEvent& native_event);

EVENTS_EXPORT bool IsMouseEvent(const base::NativeEvent& native_event);

EVENTS_EXPORT int GetChangedMouseButtonFlagsFromNative(
    const base::NativeEvent& native_event);

EVENTS_EXPORT gfx::Vector2d GetMouseWheelOffset(
    const base::NativeEvent& native_event);

EVENTS_EXPORT int GetTouchId(const base::NativeEvent& native_event);

EVENTS_EXPORT void ClearTouchIdIfReleased(
    const base::NativeEvent& native_event);

EVENTS_EXPORT float GetTouchRadiusX(const base::NativeEvent& native_event);
EVENTS_EXPORT float GetTouchRadiusY(const base::NativeEvent& native_event);

EVENTS_EXPORT float GetTouchAngle(const base::NativeEvent& native_event);

EVENTS_EXPORT float GetTouchForce(const base::NativeEvent& native_event);

EVENTS_EXPORT bool GetFlingData(const base::NativeEvent& native_event,
                            float* vx,
                            float* vy,
                            float* vx_ordinal,
                            float* vy_ordinal,
                            bool* is_cancel);

EVENTS_EXPORT bool GetScrollOffsets(const base::NativeEvent& native_event,
                                float* x_offset,
                                float* y_offset,
                                float* x_offset_ordinal,
                                float* y_offset_ordinal,
                                int* finger_count);

EVENTS_EXPORT bool GetGestureTimes(const base::NativeEvent& native_event,
                               double* start_time,
                               double* end_time);

EVENTS_EXPORT void SetNaturalScroll(bool enabled);

EVENTS_EXPORT bool IsNaturalScrollEnabled();

EVENTS_EXPORT bool ShouldDefaultToNaturalScroll();

EVENTS_EXPORT bool IsTouchpadEvent(const base::NativeEvent& event);

EVENTS_EXPORT bool IsNoopEvent(const base::NativeEvent& event);

EVENTS_EXPORT base::NativeEvent CreateNoopEvent();

#if defined(OS_WIN)
EVENTS_EXPORT int GetModifiersFromACCEL(const ACCEL& accel);
EVENTS_EXPORT int GetModifiersFromKeyState();

EVENTS_EXPORT bool IsMouseEventFromTouch(UINT message);

EVENTS_EXPORT uint16 GetScanCodeFromLParam(LPARAM lParam);
EVENTS_EXPORT LPARAM GetLParamFromScanCode(uint16 scan_code);
#endif

EVENTS_EXPORT bool EventCanceledDefaultHandling(const Event& event);

EVENTS_EXPORT int RegisterCustomEventType();

}  

#endif  
