// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CODEC_VIDEO_DECODER_H_
#define REMOTING_CODEC_VIDEO_DECODER_H_

#include "base/basictypes.h"
#include "remoting/proto/video.pb.h"

namespace webrtc {
class DesktopRect;
class DesktopRegion;
class DesktopSize;
}  

namespace remoting {

class VideoDecoder {
 public:
  static const int kBytesPerPixel = 4;

  VideoDecoder() {}
  virtual ~VideoDecoder() {}

  
  
  virtual void Initialize(const webrtc::DesktopSize& screen_size) = 0;

  
  
  virtual bool DecodePacket(const VideoPacket& packet) = 0;

  
  
  
  virtual void Invalidate(const webrtc::DesktopSize& view_size,
                          const webrtc::DesktopRegion& region) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  virtual void RenderFrame(const webrtc::DesktopSize& view_size,
                           const webrtc::DesktopRect& clip_area,
                           uint8* image_buffer,
                           int image_stride,
                           webrtc::DesktopRegion* output_region) = 0;

  
  
  virtual const webrtc::DesktopRegion* GetImageShape() = 0;
};

}  

#endif  
