// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CODEC_VIDEO_ENCODER_VPX_H_
#define REMOTING_CODEC_VIDEO_ENCODER_VPX_H_

#include "base/callback.h"
#include "remoting/codec/scoped_vpx_codec.h"
#include "remoting/codec/video_encoder.h"

typedef struct vpx_image vpx_image_t;

namespace webrtc {
class DesktopRegion;
class DesktopSize;
}  

namespace remoting {

class VideoEncoderVpx : public VideoEncoder {
 public:
  
  static scoped_ptr<VideoEncoderVpx> CreateForVP8();

  virtual ~VideoEncoderVpx();

  
  virtual scoped_ptr<VideoPacket> Encode(
      const webrtc::DesktopFrame& frame) OVERRIDE;

 private:
  typedef base::Callback<ScopedVpxCodec(const webrtc::DesktopSize&)>
      InitializeCodecCallback;

  VideoEncoderVpx(const InitializeCodecCallback& init_codec);

  
  bool Initialize(const webrtc::DesktopSize& size);

  
  
  void PrepareImage(const webrtc::DesktopFrame& frame,
                    webrtc::DesktopRegion* updated_region);

  
  
  void PrepareActiveMap(const webrtc::DesktopRegion& updated_region);

  InitializeCodecCallback init_codec_;

  ScopedVpxCodec codec_;
  scoped_ptr<vpx_image_t> image_;
  scoped_ptr<uint8[]> active_map_;
  int active_map_width_;
  int active_map_height_;
  int last_timestamp_;

  
  scoped_ptr<uint8[]> yuv_image_;

  DISALLOW_COPY_AND_ASSIGN(VideoEncoderVpx);
};

}  

#endif  
