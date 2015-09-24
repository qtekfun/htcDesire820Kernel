// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_INPUT_SYNTHETIC_GESTURE_PARAMS_H_
#define CONTENT_COMMON_INPUT_SYNTHETIC_GESTURE_PARAMS_H_

#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"

namespace content {

struct CONTENT_EXPORT SyntheticGestureParams {
  SyntheticGestureParams();
  SyntheticGestureParams(const SyntheticGestureParams& other);
  virtual ~SyntheticGestureParams();

  
  
  
  enum GestureSourceType {
    DEFAULT_INPUT,
    TOUCH_INPUT,
    MOUSE_INPUT,
    GESTURE_SOURCE_TYPE_MAX = MOUSE_INPUT
  };
  GestureSourceType gesture_source_type;

  enum GestureType {
    SMOOTH_SCROLL_GESTURE,
    PINCH_GESTURE,
    TAP_GESTURE,
    SYNTHETIC_GESTURE_TYPE_MAX = TAP_GESTURE
  };
  virtual GestureType GetGestureType() const = 0;
};

}  

#endif  
