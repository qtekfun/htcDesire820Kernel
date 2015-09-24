/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PP_VIDEO_FRAME_PRIVATE_H_
#define PPAPI_C_PRIVATE_PP_VIDEO_FRAME_PRIVATE_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"



struct PP_VideoFrame_Private {
  PP_TimeTicks timestamp;
  PP_Resource image_data;
  int32_t padding;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_VideoFrame_Private, 16);

#endif  

