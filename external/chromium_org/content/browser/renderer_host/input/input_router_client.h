// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_INPUT_INPUT_ROUTER_CLIENT_H_
#define CONTENT_BROWSER_RENDERER_HOST_INPUT_INPUT_ROUTER_CLIENT_H_

#include "content/common/content_export.h"
#include "content/port/browser/event_with_latency_info.h"
#include "content/port/common/input_event_ack_state.h"
#include "content/public/browser/native_web_keyboard_event.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"

namespace ui {
struct LatencyInfo;
}

namespace content {

class OverscrollController;

class CONTENT_EXPORT InputRouterClient {
 public:
  virtual ~InputRouterClient() {}

  
  
  
  
  
  
  virtual InputEventAckState FilterInputEvent(
      const blink::WebInputEvent& input_event,
      const ui::LatencyInfo& latency_info) = 0;

  
  virtual void IncrementInFlightEventCount() = 0;

  
  virtual void DecrementInFlightEventCount() = 0;

  
  virtual void OnHasTouchEventHandlers(bool has_handlers) = 0;

  
  
  
  virtual OverscrollController* GetOverscrollController() const = 0;

  
  
  
  virtual void SetNeedsFlush() = 0;

  
  
  
  virtual void DidFlush() = 0;
};

} 

#endif 
