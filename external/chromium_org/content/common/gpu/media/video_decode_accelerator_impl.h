// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_VIDEO_DECODE_ACCELERATOR_IMPL_H_
#define CONTENT_COMMON_GPU_MEDIA_VIDEO_DECODE_ACCELERATOR_IMPL_H_

#include "content/common/content_export.h"
#include "media/video/video_decode_accelerator.h"

namespace content {

class CONTENT_EXPORT VideoDecodeAcceleratorImpl
    : public media::VideoDecodeAccelerator {
 public:
  VideoDecodeAcceleratorImpl();
  virtual ~VideoDecodeAcceleratorImpl();

  
  
  
  
  
  
  virtual bool CanDecodeOnIOThread();
};

}  

#endif  
