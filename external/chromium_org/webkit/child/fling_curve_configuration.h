// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_CHILD_FLING_CURVE_CONFIGURATION_H_
#define WEBKIT_CHILD_FLING_CURVE_CONFIGURATION_H_

#include <vector>

#include "base/synchronization/lock.h"
#include "third_party/WebKit/public/platform/WebFloatPoint.h"
#include "third_party/WebKit/public/platform/WebSize.h"

namespace blink {
class WebGestureCurve;
}

namespace webkit_glue {

class FlingCurveConfiguration {
 public:
  FlingCurveConfiguration();
  virtual ~FlingCurveConfiguration();

  
  blink::WebGestureCurve* CreateForTouchPad(
      const blink::WebFloatPoint& velocity,
      const blink::WebSize& cumulativeScroll);

  
  blink::WebGestureCurve* CreateForTouchScreen(
      const blink::WebFloatPoint& velocity,
      const blink::WebSize& cumulativeScroll);

  
  void SetCurveParameters(
      const std::vector<float>& new_touchpad,
      const std::vector<float>& new_touchscreen);

 private:
  blink::WebGestureCurve* CreateCore(
    const std::vector<float>& coefs,
    const blink::WebFloatPoint& velocity,
    const blink::WebSize& cumulativeScroll);

  
  base::Lock lock_;
  std::vector<float> touchpad_coefs_;
  std::vector<float> touchscreen_coefs_;

  DISALLOW_COPY_AND_ASSIGN(FlingCurveConfiguration);
};

} 

#endif 
