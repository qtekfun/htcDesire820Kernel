/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPP_VIDEO_CAPTURE_DEV_H_
#define PPAPI_C_DEV_PPP_VIDEO_CAPTURE_DEV_H_

#include "ppapi/c/dev/pp_video_capture_dev.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPP_VIDEO_CAPTURE_DEV_INTERFACE_0_1 "PPP_VideoCapture(Dev);0.1"
#define PPP_VIDEO_CAPTURE_DEV_INTERFACE PPP_VIDEO_CAPTURE_DEV_INTERFACE_0_1



struct PPP_VideoCapture_Dev_0_1 {
  void (*OnDeviceInfo)(PP_Instance instance,
                       PP_Resource video_capture,
                       const struct PP_VideoCaptureDeviceInfo_Dev* info,
                       uint32_t buffer_count,
                       const PP_Resource buffers[]);
  void (*OnStatus)(PP_Instance instance,
                   PP_Resource video_capture,
                   uint32_t status);
  void (*OnError)(PP_Instance instance,
                  PP_Resource video_capture,
                  uint32_t error_code);
  void (*OnBufferReady)(PP_Instance instance,
                        PP_Resource video_capture,
                        uint32_t buffer);
};

typedef struct PPP_VideoCapture_Dev_0_1 PPP_VideoCapture_Dev;

#endif  

