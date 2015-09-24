// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_INPUT_TOUCHPAD_TAP_SUPPRESSION_CONTROLLER_H_
#define CONTENT_BROWSER_RENDERER_HOST_INPUT_TOUCHPAD_TAP_SUPPRESSION_CONTROLLER_H_

#include "base/memory/scoped_ptr.h"
#include "content/browser/renderer_host/input/tap_suppression_controller_client.h"
#include "content/common/content_export.h"
#include "content/port/browser/event_with_latency_info.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"

namespace content {

class TapSuppressionController;

class CONTENT_EXPORT TouchpadTapSuppressionControllerClient {
 public:
  virtual ~TouchpadTapSuppressionControllerClient() {}
  virtual void SendMouseEventImmediately(
      const MouseEventWithLatencyInfo& event) = 0;
};

class TouchpadTapSuppressionController : public TapSuppressionControllerClient {
 public:
  
  explicit TouchpadTapSuppressionController(
      TouchpadTapSuppressionControllerClient* client);
  virtual ~TouchpadTapSuppressionController();

  
  void GestureFlingCancel();

  
  
  void GestureFlingCancelAck(bool processed);

  
  
  
  bool ShouldDeferMouseDown(const MouseEventWithLatencyInfo& event);

  
  
  bool ShouldSuppressMouseUp();

 private:
  friend class MockRenderWidgetHost;

  
  virtual int MaxCancelToDownTimeInMs() OVERRIDE;
  virtual int MaxTapGapTimeInMs() OVERRIDE;
  virtual void DropStashedTapDown() OVERRIDE;
  virtual void ForwardStashedTapDown() OVERRIDE;

  TouchpadTapSuppressionControllerClient* client_;
  MouseEventWithLatencyInfo stashed_mouse_down_;

  
  scoped_ptr<TapSuppressionController> controller_;

  DISALLOW_COPY_AND_ASSIGN(TouchpadTapSuppressionController);
};

}  

#endif  
