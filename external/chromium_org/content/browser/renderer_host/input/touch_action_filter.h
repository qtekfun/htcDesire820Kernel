// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_INPUT_TOUCH_ACTION_FILTER_H_
#define CONTENT_BROWSER_RENDERER_HOST_INPUT_TOUCH_ACTION_FILTER_H_

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/common/input/touch_action.h"

namespace blink {
class WebGestureEvent;
}

namespace content {

class CONTENT_EXPORT TouchActionFilter {
public:
  TouchActionFilter();

  
  
  bool FilterGestureEvent(const blink::WebGestureEvent& gesture_event);

  
  
  void OnSetTouchAction(content::TouchAction touch_action);

private:
  
  bool drop_scroll_gesture_events_;

  
  content::TouchAction allowed_touch_action_;

  DISALLOW_COPY_AND_ASSIGN(TouchActionFilter);
};

}
#endif 
