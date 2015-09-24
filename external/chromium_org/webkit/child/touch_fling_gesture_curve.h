// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_CHILD_TOUCH_FLING_GESTURE_CURVE_H_
#define WEBKIT_CHILD_TOUCH_FLING_GESTURE_CURVE_H_

#include "third_party/WebKit/public/platform/WebFloatPoint.h"
#include "third_party/WebKit/public/platform/WebFloatSize.h"
#include "third_party/WebKit/public/platform/WebGestureCurve.h"
#include "third_party/WebKit/public/platform/WebSize.h"
#include "webkit/child/webkit_child_export.h"

namespace blink {
class WebGestureCurveTarget;
}

namespace webkit_glue {

class TouchFlingGestureCurve : public blink::WebGestureCurve {
 public:

  WEBKIT_CHILD_EXPORT static WebGestureCurve* Create(
      const blink::WebFloatPoint& initial_velocity,
      float p0, float p1, float p2,
      const blink::WebSize& cumulativeScroll);

 virtual bool apply(double monotonicTime,
                    blink::WebGestureCurveTarget*) OVERRIDE;

 private:
  TouchFlingGestureCurve(const blink::WebFloatPoint& initial_velocity,
                         float p0,
                         float p1,
                         float p2,
                         const blink::WebSize& cumulativeScroll);
  virtual ~TouchFlingGestureCurve();

  blink::WebFloatPoint displacement_ratio_;
  blink::WebFloatSize cumulative_scroll_;
  float coefficients_[3];
  float time_offset_;
  float curve_duration_;
  float position_offset_;

  DISALLOW_COPY_AND_ASSIGN(TouchFlingGestureCurve);
};

} 

#endif 
