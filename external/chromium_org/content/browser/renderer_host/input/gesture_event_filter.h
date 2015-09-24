// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_INPUT_GESTURE_EVENT_FILTER_H_
#define CONTENT_BROWSER_RENDERER_HOST_INPUT_GESTURE_EVENT_FILTER_H_

#include <deque>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "content/common/content_export.h"
#include "content/port/browser/event_with_latency_info.h"
#include "content/port/common/input_event_ack_state.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"
#include "ui/gfx/transform.h"

namespace content {
class GestureEventFilterTest;
class InputRouter;
class MockRenderWidgetHost;
class TouchpadTapSuppressionController;
class TouchpadTapSuppressionControllerClient;
class TouchscreenTapSuppressionController;

class CONTENT_EXPORT GestureEventFilterClient {
 public:
  virtual ~GestureEventFilterClient() {}

  virtual void SendGestureEventImmediately(
      const GestureEventWithLatencyInfo& event) = 0;

  virtual void OnGestureEventAck(
      const GestureEventWithLatencyInfo& event,
      InputEventAckState ack_result) = 0;
};

class CONTENT_EXPORT GestureEventFilter {
 public:
  
  GestureEventFilter(GestureEventFilterClient* client,
                     TouchpadTapSuppressionControllerClient* touchpad_client);
  ~GestureEventFilter();

  
  
  bool ShouldForward(const GestureEventWithLatencyInfo&);

  
  
  
  void ProcessGestureAck(InputEventAckState ack_result,
                         blink::WebInputEvent::Type type,
                         const ui::LatencyInfo& latency);

  
  
  void FlingHasBeenHalted();

  
  TouchpadTapSuppressionController* GetTouchpadTapSuppressionController();

  
  bool HasQueuedGestureEvents() const;

  void ForwardGestureEvent(const GestureEventWithLatencyInfo& gesture_event);

  void set_debounce_enabled_for_testing(bool enabled) {
    debounce_enabled_ = enabled;
  }

  void set_debounce_interval_time_ms_for_testing(int interval_time_ms) {
    debounce_interval_time_ms_ = interval_time_ms;
  }

 private:
  friend class GestureEventFilterTest;
  friend class MockRenderWidgetHost;

  
  
  

  
  
  void SendScrollEndingEventsNow();

  
  
  bool ShouldDiscardFlingCancelEvent(
      const GestureEventWithLatencyInfo& gesture_event) const;

  
  
  bool ShouldHandleEventNow() const;

  
  
  void MergeOrInsertScrollAndPinchEvent(
      const GestureEventWithLatencyInfo& gesture_event);

  
  bool ShouldForwardForZeroVelocityFlingStart(
      const GestureEventWithLatencyInfo& gesture_event) const;

  
  bool ShouldForwardForBounceReduction(
      const GestureEventWithLatencyInfo& gesture_event);

  
  bool ShouldForwardForGFCFiltering(
      const GestureEventWithLatencyInfo& gesture_event) const;

  
  bool ShouldForwardForTapSuppression(
      const GestureEventWithLatencyInfo& gesture_event);

  
  
  
  bool ShouldForwardForCoalescing(
      const GestureEventWithLatencyInfo& gesture_event);

  
  
  
  bool ShouldTryMerging(
      const GestureEventWithLatencyInfo& new_event,
      const GestureEventWithLatencyInfo& event_in_queue)const;

  
  
  
  gfx::Transform GetTransformForEvent(
      const GestureEventWithLatencyInfo& gesture_event) const;

  
  
  void EnqueueEvent(const GestureEventWithLatencyInfo& gesture_event);

  
  GestureEventFilterClient* client_;

  
  
  bool fling_in_progress_;

  
  bool scrolling_in_progress_;

  
  
  bool ignore_next_ack_;

  
  
  gfx::Transform combined_scroll_pinch_;

  
  
  
  
  
  scoped_ptr<TouchpadTapSuppressionController>
      touchpad_tap_suppression_controller_;

  
  
  
  scoped_ptr<TouchscreenTapSuppressionController>
      touchscreen_tap_suppression_controller_;

  typedef std::deque<GestureEventWithLatencyInfo> GestureEventQueue;

  
  
  
  
  
  
  GestureEventQueue coalesced_gesture_events_;

  
  base::OneShotTimer<GestureEventFilter> debounce_deferring_timer_;

  
  GestureEventQueue debouncing_deferral_queue_;

  
  
  int debounce_interval_time_ms_;

  
  
  bool debounce_enabled_;

  DISALLOW_COPY_AND_ASSIGN(GestureEventFilter);
};

}  

#endif  
