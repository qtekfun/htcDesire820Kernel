/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PP_TOUCH_POINT_H_
#define PPAPI_C_PP_TOUCH_POINT_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_stdint.h"



struct PP_TouchPoint {
  uint32_t id;
  struct PP_FloatPoint position;
  struct PP_FloatPoint radius;
  float rotation_angle;
  float pressure;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_TouchPoint, 28);


PP_INLINE struct PP_TouchPoint PP_MakeTouchPoint(void) {
  struct PP_TouchPoint result = { 0, {0, 0}, {0, 0}, 0, 0 };
  return result;
}

#endif  

