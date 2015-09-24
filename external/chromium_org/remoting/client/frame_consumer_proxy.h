// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_CLIENT_FRAME_CONSUMER_PROXY_H_
#define REMOTING_CLIENT_FRAME_CONSUMER_PROXY_H_

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "remoting/client/frame_consumer.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace remoting {

class FrameConsumerProxy
    : public base::RefCountedThreadSafe<FrameConsumerProxy>,
      public FrameConsumer {
 public:
  
  
  FrameConsumerProxy(scoped_refptr<base::SingleThreadTaskRunner> task_runner,
                     const base::WeakPtr<FrameConsumer>& frame_consumer);

  
  virtual void ApplyBuffer(const webrtc::DesktopSize& view_size,
                           const webrtc::DesktopRect& clip_area,
                           webrtc::DesktopFrame* buffer,
                           const webrtc::DesktopRegion& region) OVERRIDE;
  virtual void ReturnBuffer(webrtc::DesktopFrame* buffer) OVERRIDE;
  virtual void SetSourceSize(const webrtc::DesktopSize& source_size,
                             const webrtc::DesktopVector& dpi) OVERRIDE;
  virtual PixelFormat GetPixelFormat() OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<FrameConsumerProxy>;
  virtual ~FrameConsumerProxy();

  base::WeakPtr<FrameConsumer> frame_consumer_;
  scoped_refptr<base::SingleThreadTaskRunner> task_runner_;

  PixelFormat pixel_format_;

  DISALLOW_COPY_AND_ASSIGN(FrameConsumerProxy);
};

}  

#endif  
