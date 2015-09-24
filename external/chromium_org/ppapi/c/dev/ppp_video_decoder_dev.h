/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPP_VIDEO_DECODER_DEV_H_
#define PPAPI_C_DEV_PPP_VIDEO_DECODER_DEV_H_

#include "ppapi/c/dev/pp_video_dev.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"

#define PPP_VIDEODECODER_DEV_INTERFACE_0_11 "PPP_VideoDecoder(Dev);0.11"
#define PPP_VIDEODECODER_DEV_INTERFACE PPP_VIDEODECODER_DEV_INTERFACE_0_11



struct PPP_VideoDecoder_Dev_0_11 {
  void (*ProvidePictureBuffers)(PP_Instance instance,
                                PP_Resource decoder,
                                uint32_t req_num_of_bufs,
                                const struct PP_Size* dimensions,
                                uint32_t texture_target);
  void (*DismissPictureBuffer)(PP_Instance instance,
                               PP_Resource decoder,
                               int32_t picture_buffer_id);
  void (*PictureReady)(PP_Instance instance,
                       PP_Resource decoder,
                       const struct PP_Picture_Dev* picture);
  void (*NotifyError)(PP_Instance instance,
                      PP_Resource decoder,
                      PP_VideoDecodeError_Dev error);
};

typedef struct PPP_VideoDecoder_Dev_0_11 PPP_VideoDecoder_Dev;

#endif  

