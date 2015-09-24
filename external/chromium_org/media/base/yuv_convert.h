// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_YUV_CONVERT_H_
#define MEDIA_BASE_YUV_CONVERT_H_

#include "base/basictypes.h"
#include "media/base/media_export.h"

#if defined(ARCH_CPU_X86_FAMILY) && \
    !(defined(ARCH_CPU_X86_64) && defined(COMPILER_MSVC))
#define MEDIA_MMX_INTRINSICS_AVAILABLE
#endif

namespace media {

enum YUVType {
  YV16 = 0,  
  YV12 = 1,  
};

enum Rotate {
  ROTATE_0,           
  ROTATE_90,          
  ROTATE_180,         
  ROTATE_270,         
  MIRROR_ROTATE_0,    
  MIRROR_ROTATE_90,   
  MIRROR_ROTATE_180,  
  MIRROR_ROTATE_270,  
};

enum ScaleFilter {
  FILTER_NONE = 0,        
  FILTER_BILINEAR_H = 1,  
  FILTER_BILINEAR_V = 2,  
  FILTER_BILINEAR = 3,    
};

MEDIA_EXPORT void InitializeCPUSpecificYUVConversions();

MEDIA_EXPORT void ConvertYUVToRGB32(const uint8* yplane,
                                    const uint8* uplane,
                                    const uint8* vplane,
                                    uint8* rgbframe,
                                    int width,
                                    int height,
                                    int ystride,
                                    int uvstride,
                                    int rgbstride,
                                    YUVType yuv_type);

MEDIA_EXPORT void ConvertYUVAToARGB(const uint8* yplane,
                                    const uint8* uplane,
                                    const uint8* vplane,
                                    const uint8* aplane,
                                    uint8* rgbframe,
                                    int width,
                                    int height,
                                    int ystride,
                                    int uvstride,
                                    int astride,
                                    int rgbstride,
                                    YUVType yuv_type);

MEDIA_EXPORT void ScaleYUVToRGB32(const uint8* yplane,
                                  const uint8* uplane,
                                  const uint8* vplane,
                                  uint8* rgbframe,
                                  int source_width,
                                  int source_height,
                                  int width,
                                  int height,
                                  int ystride,
                                  int uvstride,
                                  int rgbstride,
                                  YUVType yuv_type,
                                  Rotate view_rotate,
                                  ScaleFilter filter);

MEDIA_EXPORT void ScaleYUVToRGB32WithRect(const uint8* yplane,
                                          const uint8* uplane,
                                          const uint8* vplane,
                                          uint8* rgbframe,
                                          int source_width,
                                          int source_height,
                                          int dest_width,
                                          int dest_height,
                                          int dest_rect_left,
                                          int dest_rect_top,
                                          int dest_rect_right,
                                          int dest_rect_bottom,
                                          int ystride,
                                          int uvstride,
                                          int rgbstride);

MEDIA_EXPORT void ConvertRGB32ToYUV(const uint8* rgbframe,
                                    uint8* yplane,
                                    uint8* uplane,
                                    uint8* vplane,
                                    int width,
                                    int height,
                                    int rgbstride,
                                    int ystride,
                                    int uvstride);

MEDIA_EXPORT void ConvertRGB24ToYUV(const uint8* rgbframe,
                                    uint8* yplane,
                                    uint8* uplane,
                                    uint8* vplane,
                                    int width,
                                    int height,
                                    int rgbstride,
                                    int ystride,
                                    int uvstride);

MEDIA_EXPORT void ConvertYUY2ToYUV(const uint8* src,
                                   uint8* yplane,
                                   uint8* uplane,
                                   uint8* vplane,
                                   int width,
                                   int height);

MEDIA_EXPORT void ConvertNV21ToYUV(const uint8* src,
                                   uint8* yplane,
                                   uint8* uplane,
                                   uint8* vplane,
                                   int width,
                                   int height);

MEDIA_EXPORT void EmptyRegisterState();

}  

#endif  
