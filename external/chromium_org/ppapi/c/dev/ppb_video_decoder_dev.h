/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_VIDEO_DECODER_DEV_H_
#define PPAPI_C_DEV_PPB_VIDEO_DECODER_DEV_H_

#include "ppapi/c/dev/pp_video_dev.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_VIDEODECODER_DEV_INTERFACE_0_16 "PPB_VideoDecoder(Dev);0.16"
#define PPB_VIDEODECODER_DEV_INTERFACE PPB_VIDEODECODER_DEV_INTERFACE_0_16



struct PPB_VideoDecoder_Dev_0_16 {
  PP_Resource (*Create)(PP_Instance instance,
                        PP_Resource context,
                        PP_VideoDecoder_Profile profile);
  PP_Bool (*IsVideoDecoder)(PP_Resource resource);
  int32_t (*Decode)(PP_Resource video_decoder,
                    const struct PP_VideoBitstreamBuffer_Dev* bitstream_buffer,
                    struct PP_CompletionCallback callback);
  void (*AssignPictureBuffers)(PP_Resource video_decoder,
                               uint32_t no_of_buffers,
                               const struct PP_PictureBuffer_Dev buffers[]);
  void (*ReusePictureBuffer)(PP_Resource video_decoder,
                             int32_t picture_buffer_id);
  int32_t (*Flush)(PP_Resource video_decoder,
                   struct PP_CompletionCallback callback);
  int32_t (*Reset)(PP_Resource video_decoder,
                   struct PP_CompletionCallback callback);
  void (*Destroy)(PP_Resource video_decoder);
};

typedef struct PPB_VideoDecoder_Dev_0_16 PPB_VideoDecoder_Dev;

#endif  

