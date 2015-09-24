// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CODEC_VIDEO_DECODER_VERBATIM_H_
#define REMOTING_CODEC_VIDEO_DECODER_VERBATIM_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/codec/video_decoder.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_geometry.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_region.h"

namespace remoting {

class VideoDecoderVerbatim : public VideoDecoder {
 public:
  virtual ~VideoDecoderVerbatim();

  VideoDecoderVerbatim();

  
  virtual void Initialize(const webrtc::DesktopSize& screen_size) OVERRIDE;
  virtual bool DecodePacket(const VideoPacket& packet) OVERRIDE;
  virtual void Invalidate(const webrtc::DesktopSize& view_size,
                          const webrtc::DesktopRegion& region) OVERRIDE;
  virtual void RenderFrame(const webrtc::DesktopSize& view_size,
                           const webrtc::DesktopRect& clip_area,
                           uint8* image_buffer,
                           int image_stride,
                           webrtc::DesktopRegion* output_region) OVERRIDE;
  virtual const webrtc::DesktopRegion* GetImageShape() OVERRIDE;

 private:
  
  webrtc::DesktopRegion updated_region_;

  
  webrtc::DesktopSize screen_size_;

  
  scoped_ptr<uint8[]> screen_buffer_;

  DISALLOW_COPY_AND_ASSIGN(VideoDecoderVerbatim);
};

}  

#endif  
