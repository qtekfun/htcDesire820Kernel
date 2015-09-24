// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_INPUT_TOUCH_EVENT_QUEUE_H_
#define CONTENT_BROWSER_RENDERER_HOST_INPUT_TOUCH_EVENT_QUEUE_H_

#include <deque>
#include <map>

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/port/browser/event_with_latency_info.h"
#include "content/port/common/input_event_ack_state.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"

namespace content {

class CoalescedWebTouchEvent;

class CONTENT_EXPORT TouchEventQueueClient {
 public:
  virtual ~TouchEventQueueClient() {}

  virtual void SendTouchEventImmediately(
      const TouchEventWithLatencyInfo& event) = 0;

  virtual void OnTouchEventAck(
      const TouchEventWithLatencyInfo& event,
      InputEventAckState ack_result) = 0;
};

class CONTENT_EXPORT TouchEventQueue {
 public:

  
  explicit TouchEventQueue(TouchEventQueueClient* client);
  ~TouchEventQueue();

  
  
  
  
  void QueueEvent(const TouchEventWithLatencyInfo& event);

  
  
  
  void ProcessTouchAck(InputEventAckState ack_result,
                       const ui::LatencyInfo& latency_info);

  
  
  
  
  void OnGestureScrollEvent(const GestureEventWithLatencyInfo& gesture_event);

  
  
  void FlushQueue();

  
  
  bool IsPendingAckTouchStart() const;

  
  
  void SetAckTimeoutEnabled(bool enabled, size_t ack_timeout_delay_ms);

  bool empty() const WARN_UNUSED_RESULT {
    return touch_queue_.empty();
  }

  size_t size() const {
    return touch_queue_.size();
  }

  bool ack_timeout_enabled() const {
    return ack_timeout_enabled_;
  }

 private:
  class TouchTimeoutHandler;
  friend class TouchTimeoutHandler;
  friend class TouchEventQueueTest;

  bool HasTimeoutEvent() const;
  bool IsTimeoutRunningForTesting() const;
  const TouchEventWithLatencyInfo& GetLatestEventForTesting() const;

  
  
  
  void TryForwardNextEventToRenderer();

  
  
  void PopTouchEventToClient(InputEventAckState ack_result,
                             const ui::LatencyInfo& renderer_latency_info);

  bool ShouldForwardToRenderer(const blink::WebTouchEvent& event) const;
  void ForwardToRenderer(const TouchEventWithLatencyInfo& event);
  void UpdateTouchAckStates(const blink::WebTouchEvent& event,
                            InputEventAckState ack_result);


  
  TouchEventQueueClient* client_;

  typedef std::deque<CoalescedWebTouchEvent*> TouchQueue;
  TouchQueue touch_queue_;

  
  typedef std::map<int, InputEventAckState> TouchPointAckStates;
  TouchPointAckStates touch_ack_states_;

  
  
  
  CoalescedWebTouchEvent* dispatching_touch_ack_;

  
  
  bool dispatching_touch_;

  
  bool no_touch_to_renderer_;

  
  
  bool renderer_is_consuming_touch_gesture_;

  
  bool ack_timeout_enabled_;
  scoped_ptr<TouchTimeoutHandler> timeout_handler_;

  DISALLOW_COPY_AND_ASSIGN(TouchEventQueue);
};

}  

#endif  
