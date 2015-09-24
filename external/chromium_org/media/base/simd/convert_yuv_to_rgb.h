// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_SIMD_CONVERT_YUV_TO_RGB_H_
#define MEDIA_BASE_SIMD_CONVERT_YUV_TO_RGB_H_

#include "base/basictypes.h"
#include "media/base/yuv_convert.h"

namespace media {


MEDIA_EXPORT void ConvertYUVToRGB32_C(const uint8* yplane,
                                      const uint8* uplane,
                                      const uint8* vplane,
                                      uint8* rgbframe,
                                      int width,
                                      int height,
                                      int ystride,
                                      int uvstride,
                                      int rgbstride,
                                      YUVType yuv_type);

MEDIA_EXPORT void ConvertYUVToRGB32Row_C(const uint8* yplane,
                                         const uint8* uplane,
                                         const uint8* vplane,
                                         uint8* rgbframe,
                                         ptrdiff_t width);

MEDIA_EXPORT void ConvertYUVAToARGB_C(const uint8* yplane,
                                      const uint8* uplane,
                                      const uint8* vplane,
                                      const uint8* aplane,
                                      uint8* rgbframe,
                                      int width,
                                      int height,
                                      int ystride,
                                      int uvstride,
                                      int avstride,
                                      int rgbstride,
                                      YUVType yuv_type);

MEDIA_EXPORT void ConvertYUVAToARGBRow_C(const uint8* yplane,
                                         const uint8* uplane,
                                         const uint8* vplane,
                                         const uint8* aplane,
                                         uint8* rgbframe,
                                         ptrdiff_t width);

MEDIA_EXPORT void ConvertYUVToRGB32_SSE(const uint8* yplane,
                                        const uint8* uplane,
                                        const uint8* vplane,
                                        uint8* rgbframe,
                                        int width,
                                        int height,
                                        int ystride,
                                        int uvstride,
                                        int rgbstride,
                                        YUVType yuv_type);

MEDIA_EXPORT void ConvertYUVToRGB32_MMX(const uint8* yplane,
                                        const uint8* uplane,
                                        const uint8* vplane,
                                        uint8* rgbframe,
                                        int width,
                                        int height,
                                        int ystride,
                                        int uvstride,
                                        int rgbstride,
                                        YUVType yuv_type);

MEDIA_EXPORT void ConvertYUVAToARGB_MMX(const uint8* yplane,
                                        const uint8* uplane,
                                        const uint8* vplane,
                                        const uint8* aplane,
                                        uint8* rgbframe,
                                        int width,
                                        int height,
                                        int ystride,
                                        int uvstride,
                                        int avstride,
                                        int rgbstride,
                                        YUVType yuv_type);

MEDIA_EXPORT void ScaleYUVToRGB32Row_C(const uint8* y_buf,
                                       const uint8* u_buf,
                                       const uint8* v_buf,
                                       uint8* rgb_buf,
                                       ptrdiff_t width,
                                       ptrdiff_t source_dx);

MEDIA_EXPORT void LinearScaleYUVToRGB32Row_C(const uint8* y_buf,
                                             const uint8* u_buf,
                                             const uint8* v_buf,
                                             uint8* rgb_buf,
                                             ptrdiff_t width,
                                             ptrdiff_t source_dx);

MEDIA_EXPORT void LinearScaleYUVToRGB32RowWithRange_C(const uint8* y_buf,
                                                      const uint8* u_buf,
                                                      const uint8* v_buf,
                                                      uint8* rgb_buf,
                                                      int dest_width,
                                                      int source_x,
                                                      int source_dx);

}  

extern "C" {


MEDIA_EXPORT void ConvertYUVToRGB32Row_MMX(const uint8* yplane,
                                           const uint8* uplane,
                                           const uint8* vplane,
                                           uint8* rgbframe,
                                           ptrdiff_t width);

MEDIA_EXPORT void ConvertYUVAToARGBRow_MMX(const uint8* yplane,
                                           const uint8* uplane,
                                           const uint8* vplane,
                                           const uint8* aplane,
                                           uint8* rgbframe,
                                           ptrdiff_t width);

MEDIA_EXPORT void ConvertYUVToRGB32Row_SSE(const uint8* yplane,
                                           const uint8* uplane,
                                           const uint8* vplane,
                                           uint8* rgbframe,
                                           ptrdiff_t width);

MEDIA_EXPORT void ScaleYUVToRGB32Row_MMX(const uint8* y_buf,
                                         const uint8* u_buf,
                                         const uint8* v_buf,
                                         uint8* rgb_buf,
                                         ptrdiff_t width,
                                         ptrdiff_t source_dx);

MEDIA_EXPORT void ScaleYUVToRGB32Row_SSE(const uint8* y_buf,
                                         const uint8* u_buf,
                                         const uint8* v_buf,
                                         uint8* rgb_buf,
                                         ptrdiff_t width,
                                         ptrdiff_t source_dx);

MEDIA_EXPORT void ScaleYUVToRGB32Row_SSE2_X64(const uint8* y_buf,
                                              const uint8* u_buf,
                                              const uint8* v_buf,
                                              uint8* rgb_buf,
                                              ptrdiff_t width,
                                              ptrdiff_t source_dx);

MEDIA_EXPORT void LinearScaleYUVToRGB32Row_MMX(const uint8* y_buf,
                                               const uint8* u_buf,
                                               const uint8* v_buf,
                                               uint8* rgb_buf,
                                               ptrdiff_t width,
                                               ptrdiff_t source_dx);

MEDIA_EXPORT void LinearScaleYUVToRGB32Row_SSE(const uint8* y_buf,
                                               const uint8* u_buf,
                                               const uint8* v_buf,
                                               uint8* rgb_buf,
                                               ptrdiff_t width,
                                               ptrdiff_t source_dx);

MEDIA_EXPORT void LinearScaleYUVToRGB32Row_MMX_X64(const uint8* y_buf,
                                                   const uint8* u_buf,
                                                   const uint8* v_buf,
                                                   uint8* rgb_buf,
                                                   ptrdiff_t width,
                                                   ptrdiff_t source_dx);

}  

#endif  
