// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_BASE_UTIL_H_
#define REMOTING_BASE_UTIL_H_

#include <string>

#include "media/base/video_frame.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_geometry.h"

namespace remoting {

std::string GetTimestampString();

int CalculateRGBOffset(int x, int y, int stride);

int CalculateYOffset(int x, int y, int stride);
int CalculateUVOffset(int x, int y, int stride);

void ConvertAndScaleYUVToRGB32Rect(
    const uint8* source_yplane,
    const uint8* source_uplane,
    const uint8* source_vplane,
    int source_ystride,
    int source_uvstride,
    const webrtc::DesktopSize& source_size,
    const webrtc::DesktopRect& source_buffer_rect,
    uint8* dest_buffer,
    int dest_stride,
    const webrtc::DesktopSize& dest_size,
    const webrtc::DesktopRect& dest_buffer_rect,
    const webrtc::DesktopRect& dest_rect);

void ConvertRGB32ToYUVWithRect(const uint8* rgb_plane,
                               uint8* y_plane,
                               uint8* u_plane,
                               uint8* v_plane,
                               int x,
                               int y,
                               int width,
                               int height,
                               int rgb_stride,
                               int y_stride,
                               int uv_stride);

int RoundToTwosMultiple(int x);

webrtc::DesktopRect AlignRect(const webrtc::DesktopRect& rect);

webrtc::DesktopRect ScaleRect(const webrtc::DesktopRect& rect,
                              const webrtc::DesktopSize& in_size,
                              const webrtc::DesktopSize& out_size);

void CopyRGB32Rect(const uint8* source_buffer,
                   int source_stride,
                   const webrtc::DesktopRect& source_buffer_rect,
                   uint8* dest_buffer,
                   int dest_stride,
                   const webrtc::DesktopRect& dest_buffer_rect,
                   const webrtc::DesktopRect& dest_rect);

std::string ReplaceLfByCrLf(const std::string& in);

std::string ReplaceCrLfByLf(const std::string& in);

bool StringIsUtf8(const char* data, size_t length);

bool DoesRectContain(const webrtc::DesktopRect& a,
                     const webrtc::DesktopRect& b);

}  

#endif  
