// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_FRAME_CONSUMER_H_
#define REMOTING_CLIENT_FRAME_CONSUMER_H_

#include "base/basictypes.h"

namespace webrtc {
class DesktopFrame;
class DesktopRect;
class DesktopRegion;
class DesktopSize;
class DesktopVector;
}  

namespace remoting {

class FrameConsumer {
 public:

  
  enum PixelFormat {
    FORMAT_BGRA,  
    FORMAT_RGBA,  
  };

  
  
  
  
  
  
  
  
  virtual void ApplyBuffer(const webrtc::DesktopSize& view_size,
                           const webrtc::DesktopRect& clip_area,
                           webrtc::DesktopFrame* buffer,
                           const webrtc::DesktopRegion& region) = 0;

  
  
  
  virtual void ReturnBuffer(webrtc::DesktopFrame* buffer) = 0;

  
  virtual void SetSourceSize(const webrtc::DesktopSize& source_size,
                             const webrtc::DesktopVector& dpi) = 0;

  
  virtual PixelFormat GetPixelFormat() = 0;

 protected:
  FrameConsumer() {}
  virtual ~FrameConsumer() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(FrameConsumer);
};

}  

#endif  
