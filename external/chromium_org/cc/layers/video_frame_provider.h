// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_LAYERS_VIDEO_FRAME_PROVIDER_H_
#define CC_LAYERS_VIDEO_FRAME_PROVIDER_H_

#include "base/memory/ref_counted.h"

namespace media {
class VideoFrame;
}

namespace cc {

class VideoFrameProvider {
 public:
  virtual ~VideoFrameProvider() {}

  class Client {
   public:
    
    
    
    virtual void StopUsingProvider() = 0;

    
    
    virtual void DidReceiveFrame() = 0;

    
    virtual void DidUpdateMatrix(const float* matrix) = 0;

   protected:
    virtual ~Client() {}
  };

  
  
  virtual void SetVideoFrameProviderClient(Client* client) = 0;

  
  
  
  
  virtual scoped_refptr<media::VideoFrame> GetCurrentFrame() = 0;

  
  
  
  
  virtual void PutCurrentFrame(
      const scoped_refptr<media::VideoFrame>& frame) = 0;
};

}  

#endif  
