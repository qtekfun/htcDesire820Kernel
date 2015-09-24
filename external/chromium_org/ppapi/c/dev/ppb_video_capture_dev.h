/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_VIDEO_CAPTURE_DEV_H_
#define PPAPI_C_DEV_PPB_VIDEO_CAPTURE_DEV_H_

#include "ppapi/c/dev/pp_video_capture_dev.h"
#include "ppapi/c/dev/ppb_device_ref_dev.h"
#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_VIDEOCAPTURE_DEV_INTERFACE_0_2 "PPB_VideoCapture(Dev);0.2"
#define PPB_VIDEOCAPTURE_DEV_INTERFACE_0_3 "PPB_VideoCapture(Dev);0.3"
#define PPB_VIDEOCAPTURE_DEV_INTERFACE PPB_VIDEOCAPTURE_DEV_INTERFACE_0_3



struct PPB_VideoCapture_Dev_0_3 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsVideoCapture)(PP_Resource video_capture);
  int32_t (*EnumerateDevices)(PP_Resource video_capture,
                              struct PP_ArrayOutput output,
                              struct PP_CompletionCallback callback);
  int32_t (*MonitorDeviceChange)(PP_Resource video_capture,
                                 PP_MonitorDeviceChangeCallback callback,
                                 void* user_data);
  int32_t (*Open)(PP_Resource video_capture,
                  PP_Resource device_ref,
                  const struct PP_VideoCaptureDeviceInfo_Dev* requested_info,
                  uint32_t buffer_count,
                  struct PP_CompletionCallback callback);
  int32_t (*StartCapture)(PP_Resource video_capture);
  int32_t (*ReuseBuffer)(PP_Resource video_capture, uint32_t buffer);
  int32_t (*StopCapture)(PP_Resource video_capture);
  void (*Close)(PP_Resource video_capture);
};

typedef struct PPB_VideoCapture_Dev_0_3 PPB_VideoCapture_Dev;

struct PPB_VideoCapture_Dev_0_2 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsVideoCapture)(PP_Resource video_capture);
  int32_t (*EnumerateDevices)(PP_Resource video_capture,
                              PP_Resource* devices,
                              struct PP_CompletionCallback callback);
  int32_t (*Open)(PP_Resource video_capture,
                  PP_Resource device_ref,
                  const struct PP_VideoCaptureDeviceInfo_Dev* requested_info,
                  uint32_t buffer_count,
                  struct PP_CompletionCallback callback);
  int32_t (*StartCapture)(PP_Resource video_capture);
  int32_t (*ReuseBuffer)(PP_Resource video_capture, uint32_t buffer);
  int32_t (*StopCapture)(PP_Resource video_capture);
  void (*Close)(PP_Resource video_capture);
};

#endif  

