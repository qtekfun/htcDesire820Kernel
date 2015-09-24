/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_VIDEO_SOURCE_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_VIDEO_SOURCE_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/private/pp_video_frame_private.h"

#define PPB_VIDEOSOURCE_PRIVATE_INTERFACE_0_1 "PPB_VideoSource_Private;0.1"
#define PPB_VIDEOSOURCE_PRIVATE_INTERFACE PPB_VIDEOSOURCE_PRIVATE_INTERFACE_0_1



struct PPB_VideoSource_Private_0_1 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsVideoSource)(PP_Resource resource);
  int32_t (*Open)(PP_Resource source,
                  struct PP_Var stream_url,
                  struct PP_CompletionCallback callback);
  int32_t (*GetFrame)(PP_Resource source,
                      struct PP_VideoFrame_Private* frame,
                      struct PP_CompletionCallback callback);
  void (*Close)(PP_Resource source);
};

typedef struct PPB_VideoSource_Private_0_1 PPB_VideoSource_Private;

#endif  

