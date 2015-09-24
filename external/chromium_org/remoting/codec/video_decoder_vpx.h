// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CODEC_VIDEO_DECODER_VPX_H_
#define REMOTING_CODEC_VIDEO_DECODER_VPX_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/codec/scoped_vpx_codec.h"
#include "remoting/codec/video_decoder.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_geometry.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_region.h"

typedef struct vpx_image vpx_image_t;

namespace remoting {

class VideoDecoderVpx : public VideoDecoder {
 public:
  
  static scoped_ptr<VideoDecoderVpx> CreateForVP8();
  static scoped_ptr<VideoDecoderVpx> CreateForVP9();

  virtual ~VideoDecoderVpx();

  
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
  explicit VideoDecoderVpx(ScopedVpxCodec codec);

  
  
  
  void UpdateImageShapeRegion(webrtc::DesktopRegion* new_desktop_shape);

  ScopedVpxCodec codec_;

  
  vpx_image_t* last_image_;

  
  webrtc::DesktopRegion updated_region_;

  
  webrtc::DesktopSize screen_size_;

  
  webrtc::DesktopRegion desktop_shape_;

  
  webrtc::DesktopRegion transparent_region_;

  DISALLOW_COPY_AND_ASSIGN(VideoDecoderVpx);
};

}  

#endif  
