/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PP_VIDEO_DEV_H_
#define PPAPI_C_DEV_PP_VIDEO_DEV_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"



typedef enum {
  PP_VIDEODECODER_PROFILE_UNKNOWN = -1,
  PP_VIDEODECODER_H264PROFILE_NONE = 0,
  PP_VIDEODECODER_H264PROFILE_BASELINE = 1,
  PP_VIDEODECODER_H264PROFILE_MAIN = 2,
  PP_VIDEODECODER_H264PROFILE_EXTENDED = 3,
  PP_VIDEODECODER_H264PROFILE_HIGH = 4,
  PP_VIDEODECODER_H264PROFILE_HIGH10PROFILE = 5,
  PP_VIDEODECODER_H264PROFILE_HIGH422PROFILE = 6,
  PP_VIDEODECODER_H264PROFILE_HIGH444PREDICTIVEPROFILE = 7,
  PP_VIDEODECODER_H264PROFILE_SCALABLEBASELINE = 8,
  PP_VIDEODECODER_H264PROFILE_SCALABLEHIGH = 9,
  PP_VIDEODECODER_H264PROFILE_STEREOHIGH = 10,
  PP_VIDEODECODER_H264PROFILE_MULTIVIEWHIGH = 11,
  PP_VIDEODECODER_VP8PROFILE_MAIN = 12
} PP_VideoDecoder_Profile;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_VideoDecoder_Profile, 4);

struct PP_VideoBitstreamBuffer_Dev {
  int32_t id;
  PP_Resource data;
  uint32_t size;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_VideoBitstreamBuffer_Dev, 12);

struct PP_PictureBuffer_Dev {
  int32_t id;
  struct PP_Size size;
  uint32_t texture_id;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_PictureBuffer_Dev, 16);

struct PP_Picture_Dev {
  int32_t picture_buffer_id;
  int32_t bitstream_buffer_id;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_Picture_Dev, 8);

typedef enum {
  PP_VIDEODECODERERROR_ILLEGAL_STATE = 1,
  PP_VIDEODECODERERROR_INVALID_ARGUMENT = 2,
  PP_VIDEODECODERERROR_UNREADABLE_INPUT = 3,
  PP_VIDEODECODERERROR_PLATFORM_FAILURE = 4
} PP_VideoDecodeError_Dev;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_VideoDecodeError_Dev, 4);

#endif  

