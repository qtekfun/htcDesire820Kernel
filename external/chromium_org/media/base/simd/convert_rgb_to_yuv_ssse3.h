// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_SIMD_CONVERT_RGB_TO_YUV_SSSE3_H_
#define MEDIA_BASE_SIMD_CONVERT_RGB_TO_YUV_SSSE3_H_

#ifdef __cplusplus
extern "C" {
#endif



void ConvertRGBToYUVRow_SSSE3(const uint8* rgb,
                              uint8* y,
                              uint8* u,
                              uint8* v,
                              ptrdiff_t width);

void ConvertARGBToYUVRow_SSSE3(const uint8* argb,
                               uint8* y,
                               uint8* u,
                               uint8* v,
                               ptrdiff_t width);

#ifdef __cplusplus
}
#endif

#endif  
