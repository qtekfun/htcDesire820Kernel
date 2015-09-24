// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_VIDEO_UTIL_H_
#define MEDIA_BASE_VIDEO_UTIL_H_

#include "base/basictypes.h"
#include "media/base/media_export.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

namespace media {

class VideoFrame;

MEDIA_EXPORT gfx::Size GetNaturalSize(const gfx::Size& visible_size,
                                      int aspect_ratio_numerator,
                                      int aspect_ratio_denominator);

MEDIA_EXPORT void CopyYPlane(const uint8* source, int stride, int rows,
                             VideoFrame* frame);
MEDIA_EXPORT void CopyUPlane(const uint8* source, int stride, int rows,
                             VideoFrame* frame);
MEDIA_EXPORT void CopyVPlane(const uint8* source, int stride, int rows,
                             VideoFrame* frame);
MEDIA_EXPORT void CopyAPlane(const uint8* source, int stride, int rows,
                             VideoFrame* frame);

MEDIA_EXPORT void MakeOpaqueAPlane(int stride, int rows, VideoFrame* frame);

MEDIA_EXPORT void CopyPlane(size_t plane, const uint8* source, int stride,
                            int rows, VideoFrame* frame);


MEDIA_EXPORT void FillYUV(VideoFrame* frame, uint8 y, uint8 u, uint8 v);

MEDIA_EXPORT void LetterboxYUV(VideoFrame* frame,
                               const gfx::Rect& view_area);

MEDIA_EXPORT void RotatePlaneByPixels(
    const uint8* src,
    uint8* dest,
    int width,
    int height,
    int rotation,  
    bool flip_vert,
    bool flip_horiz);

MEDIA_EXPORT gfx::Rect ComputeLetterboxRegion(const gfx::Rect& bounds,
                                              const gfx::Size& content);

MEDIA_EXPORT void CopyRGBToVideoFrame(const uint8* source,
                                      int stride,
                                      const gfx::Rect& region_in_frame,
                                      VideoFrame* frame);

}  

#endif  
