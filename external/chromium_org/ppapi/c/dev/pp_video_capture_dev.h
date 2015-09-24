/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PP_VIDEO_CAPTURE_DEV_H_
#define PPAPI_C_DEV_PP_VIDEO_CAPTURE_DEV_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"



struct PP_VideoCaptureDeviceInfo_Dev {
  uint32_t width;
  uint32_t height;
  uint32_t frames_per_second;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_VideoCaptureDeviceInfo_Dev, 12);

typedef enum {
  PP_VIDEO_CAPTURE_STATUS_STOPPED = 0,
  PP_VIDEO_CAPTURE_STATUS_STARTING = 1,
  PP_VIDEO_CAPTURE_STATUS_STARTED = 2,
  PP_VIDEO_CAPTURE_STATUS_PAUSED = 3,
  PP_VIDEO_CAPTURE_STATUS_STOPPING = 4
} PP_VideoCaptureStatus_Dev;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_VideoCaptureStatus_Dev, 4);

#endif  

