// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_UI_EVENTS_HELPER_H_
#define CONTENT_BROWSER_RENDERER_HOST_UI_EVENTS_HELPER_H_

#include "base/memory/scoped_vector.h"
#include "content/common/content_export.h"
#include "content/port/browser/event_with_latency_info.h"

namespace blink {
class WebGestureEvent;
class WebTouchEvent;
class WebTouchPoint;
}

namespace ui {
class GestureEvent;
class TouchEvent;
}

namespace content {

enum TouchEventCoordinateSystem {
  SCREEN_COORDINATES,
  LOCAL_COORDINATES
};

CONTENT_EXPORT bool MakeUITouchEventsFromWebTouchEvents(
    const TouchEventWithLatencyInfo& touch,
    ScopedVector<ui::TouchEvent>* list,
    TouchEventCoordinateSystem coordinate_system);

blink::WebGestureEvent MakeWebGestureEventFromUIEvent(
    const ui::GestureEvent& event);

int EventFlagsToWebEventModifiers(int flags);

blink::WebTouchPoint* UpdateWebTouchEventFromUIEvent(
    const ui::TouchEvent& event,
    blink::WebTouchEvent* web_event);
}

#endif  
