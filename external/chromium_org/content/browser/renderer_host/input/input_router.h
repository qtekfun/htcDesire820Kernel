// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_INPUT_INPUT_ROUTER_H_
#define CONTENT_BROWSER_RENDERER_HOST_INPUT_INPUT_ROUTER_H_

#include "base/basictypes.h"
#include "content/port/browser/event_with_latency_info.h"
#include "content/port/common/input_event_ack_state.h"
#include "content/public/browser/native_web_keyboard_event.h"
#include "ipc/ipc_listener.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"

namespace content {

class InputRouterClient;

class InputRouter : public IPC::Listener {
 public:
  virtual ~InputRouter() {}

  
  
  virtual void Flush() = 0;

  
  
  
  virtual bool SendInput(scoped_ptr<IPC::Message> message) = 0;

  
  virtual void SendMouseEvent(
      const MouseEventWithLatencyInfo& mouse_event) = 0;
  virtual void SendWheelEvent(
      const MouseWheelEventWithLatencyInfo& wheel_event) = 0;
  virtual void SendKeyboardEvent(
      const NativeWebKeyboardEvent& key_event,
      const ui::LatencyInfo& latency_info,
      bool is_shortcut) = 0;
  virtual void SendGestureEvent(
      const GestureEventWithLatencyInfo& gesture_event) = 0;
  virtual void SendTouchEvent(
      const TouchEventWithLatencyInfo& touch_event) = 0;

  
  virtual const NativeWebKeyboardEvent* GetLastKeyboardEvent() const = 0;

  
  
  
  virtual bool ShouldForwardTouchEvent() const = 0;

  
  
  enum ViewFlags {
    VIEW_FLAGS_NONE   = 0,
    FIXED_PAGE_SCALE  = 1 << 0,
    MOBILE_VIEWPORT   = 1 << 1
  };
  virtual void OnViewUpdated(int view_flags) = 0;
};

}  

#endif 
