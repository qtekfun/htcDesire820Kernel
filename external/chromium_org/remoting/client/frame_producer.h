// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_FRAME_PRODUCER_H_
#define REMOTING_CLIENT_FRAME_PRODUCER_H_

#include "base/callback_forward.h"

namespace webrtc {
class DesktopFrame;
class DesktopRect;
class DesktopRegion;
class DesktopSize;
}  

namespace remoting {

class FrameProducer {
 public:
  FrameProducer() {}

  
  
  
  
  
  
  virtual void DrawBuffer(webrtc::DesktopFrame* buffer) = 0;

  
  
  
  virtual void InvalidateRegion(const webrtc::DesktopRegion& region) = 0;

  
  
  virtual void RequestReturnBuffers(const base::Closure& done) = 0;

  
  
  virtual void SetOutputSizeAndClip(const webrtc::DesktopSize& view_size,
                                    const webrtc::DesktopRect& clip_area) = 0;

  
  virtual const webrtc::DesktopRegion* GetBufferShape() = 0;

 protected:
  virtual ~FrameProducer() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(FrameProducer);
};

}  

#endif  
