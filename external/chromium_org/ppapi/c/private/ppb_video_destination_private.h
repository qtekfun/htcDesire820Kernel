/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_VIDEO_DESTINATION_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_VIDEO_DESTINATION_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/private/pp_video_frame_private.h"

#define PPB_VIDEODESTINATION_PRIVATE_INTERFACE_0_1 \
    "PPB_VideoDestination_Private;0.1"
#define PPB_VIDEODESTINATION_PRIVATE_INTERFACE \
    PPB_VIDEODESTINATION_PRIVATE_INTERFACE_0_1



struct PPB_VideoDestination_Private_0_1 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsVideoDestination)(PP_Resource resource);
  int32_t (*Open)(PP_Resource destination,
                  struct PP_Var stream_url,
                  struct PP_CompletionCallback callback);
  int32_t (*PutFrame)(PP_Resource destination,
                      const struct PP_VideoFrame_Private* frame);
  void (*Close)(PP_Resource destination);
};

typedef struct PPB_VideoDestination_Private_0_1 PPB_VideoDestination_Private;

#endif  

