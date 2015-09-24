// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_TOUCH_POINT_H_
#define PPAPI_CPP_TOUCH_POINT_H_

#include "ppapi/c/ppb_input_event.h"
#include "ppapi/cpp/input_event.h"
#include "ppapi/cpp/point.h"

namespace pp {

class TouchPoint {
 public:
  TouchPoint() : touch_point_(PP_MakeTouchPoint()) {}

  TouchPoint(const PP_TouchPoint& point) : touch_point_(point) {}

  
  
  
  
  
  
  uint32_t id() const { return touch_point_.id; }

  
  FloatPoint position() const {
    return pp::FloatPoint(touch_point_.position);
  }

  
  
  FloatPoint radii() const { return pp::FloatPoint(touch_point_.radius); }

  
  
  float rotation_angle() const { return touch_point_.rotation_angle; }

  
  
  
  
  float pressure() const { return touch_point_.pressure; }

 private:
  PP_TouchPoint touch_point_;
};

}  

#endif  
