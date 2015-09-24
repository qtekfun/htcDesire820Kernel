// Copyright 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_INPUT_INPUT_HANDLER_H_
#define CC_INPUT_INPUT_HANDLER_H_

#include "base/basictypes.h"
#include "base/time/time.h"
#include "cc/base/cc_export.h"
#include "cc/base/swap_promise_monitor.h"
#include "cc/input/scrollbar.h"

namespace gfx {
class Point;
class PointF;
class Vector2d;
class Vector2dF;
}

namespace ui { struct LatencyInfo; }

namespace cc {

class LayerScrollOffsetDelegate;

struct DidOverscrollParams {
  gfx::Vector2dF accumulated_overscroll;
  gfx::Vector2dF latest_overscroll_delta;
  gfx::Vector2dF current_fling_velocity;
};

class CC_EXPORT InputHandlerClient {
 public:
  virtual ~InputHandlerClient() {}

  virtual void WillShutdown() = 0;
  virtual void Animate(base::TimeTicks time) = 0;
  virtual void MainThreadHasStoppedFlinging() = 0;

  
  
  
  virtual void DidOverscroll(const DidOverscrollParams& params) = 0;

 protected:
  InputHandlerClient() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(InputHandlerClient);
};

class CC_EXPORT InputHandler {
 public:
  enum ScrollStatus { ScrollOnMainThread, ScrollStarted, ScrollIgnored };
  enum ScrollInputType { Gesture, Wheel, NonBubblingGesture };

  
  
  
  virtual void BindToClient(InputHandlerClient* client) = 0;

  
  
  
  
  
  virtual ScrollStatus ScrollBegin(gfx::Point viewport_point,
                                   ScrollInputType type) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool ScrollBy(gfx::Point viewport_point,
                        gfx::Vector2dF scroll_delta) = 0;

  virtual bool ScrollVerticallyByPage(
      gfx::Point viewport_point,
      ScrollDirection direction) = 0;

  
  
  virtual ScrollStatus FlingScrollBegin() = 0;

  virtual void NotifyCurrentFlingVelocity(gfx::Vector2dF velocity) = 0;

  virtual void MouseMoveAt(gfx::Point mouse_position) = 0;

  
  
  virtual void ScrollEnd() = 0;

  virtual void SetRootLayerScrollOffsetDelegate(
      LayerScrollOffsetDelegate* root_layer_scroll_offset_delegate) = 0;

  
  
  
  
  
  virtual void OnRootLayerDelegatedScrollOffsetChanged() = 0;

  virtual void PinchGestureBegin() = 0;
  virtual void PinchGestureUpdate(float magnify_delta, gfx::Point anchor) = 0;
  virtual void PinchGestureEnd() = 0;

  virtual void StartPageScaleAnimation(gfx::Vector2d target_offset,
                                       bool anchor_point,
                                       float page_scale,
                                       base::TimeDelta duration) = 0;

  
  virtual void ScheduleAnimation() = 0;

  virtual bool HaveTouchEventHandlersAt(gfx::Point viewport_point) = 0;

  
  
  
  
  
  virtual scoped_ptr<SwapPromiseMonitor> CreateLatencyInfoSwapPromiseMonitor(
      ui::LatencyInfo* latency) = 0;

 protected:
  InputHandler() {}
  virtual ~InputHandler() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(InputHandler);
};

}  

#endif  
