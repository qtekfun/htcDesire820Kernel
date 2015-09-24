// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CODEC_VIDEO_ENCODER_H_
#define REMOTING_CODEC_VIDEO_ENCODER_H_

#include "base/memory/scoped_ptr.h"

namespace webrtc {
class DesktopFrame;
}  

namespace remoting {

class VideoPacket;

class VideoEncoder {
 public:
  virtual ~VideoEncoder() {}

  
  virtual scoped_ptr<VideoPacket> Encode(const webrtc::DesktopFrame& frame) = 0;
};

}  

#endif  
