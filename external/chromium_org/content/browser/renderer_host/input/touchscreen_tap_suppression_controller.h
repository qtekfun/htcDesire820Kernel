// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_INPUT_TOUCHSCREEN_TAP_SUPPRESSION_CONTROLLER_H_
#define CONTENT_BROWSER_RENDERER_HOST_INPUT_TOUCHSCREEN_TAP_SUPPRESSION_CONTROLLER_H_

#include "base/memory/scoped_ptr.h"
#include "content/browser/renderer_host/input/gesture_event_filter.h"
#include "content/browser/renderer_host/input/tap_suppression_controller_client.h"

namespace content {

class GestureEventFilter;
class TapSuppressionController;

class TouchscreenTapSuppressionController
    : public TapSuppressionControllerClient {
 public:
  explicit TouchscreenTapSuppressionController(GestureEventFilter* gef);
  virtual ~TouchscreenTapSuppressionController();

  
  void GestureFlingCancel();

  
  
  void GestureFlingCancelAck(bool processed);

  
  
  bool ShouldDeferGestureTapDown(const GestureEventWithLatencyInfo& event);

  
  
  bool ShouldDeferGestureShowPress(const GestureEventWithLatencyInfo& event);

  
  
  bool ShouldSuppressGestureTapEnd();

 private:
  
  virtual int MaxCancelToDownTimeInMs() OVERRIDE;
  virtual int MaxTapGapTimeInMs() OVERRIDE;
  virtual void DropStashedTapDown() OVERRIDE;
  virtual void ForwardStashedTapDown() OVERRIDE;

  GestureEventFilter* gesture_event_filter_;

  typedef scoped_ptr<GestureEventWithLatencyInfo> ScopedGestureEvent;
  ScopedGestureEvent stashed_tap_down_;
  ScopedGestureEvent stashed_show_press_;

  
  scoped_ptr<TapSuppressionController> controller_;

  DISALLOW_COPY_AND_ASSIGN(TouchscreenTapSuppressionController);
};

}  

#endif  
