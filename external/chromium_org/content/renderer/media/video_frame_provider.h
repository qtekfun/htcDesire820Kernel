// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_VIDEO_FRAME_PROVIDER_H_
#define CONTENT_RENDERER_MEDIA_VIDEO_FRAME_PROVIDER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"

namespace media {
class VideoFrame;
}

namespace content {

class CONTENT_EXPORT VideoFrameProvider
    : public base::RefCountedThreadSafe<VideoFrameProvider> {
 public:
  typedef base::Callback<void(const scoped_refptr<media::VideoFrame>&)>
      RepaintCB;

  
  virtual void Start() = 0;

  
  virtual void Stop() = 0;

  
  virtual void Play() = 0;

  
  
  
  virtual void Pause() = 0;

 protected:
  friend class base::RefCountedThreadSafe<VideoFrameProvider>;

  VideoFrameProvider();
  virtual ~VideoFrameProvider();

 private:
  DISALLOW_COPY_AND_ASSIGN(VideoFrameProvider);
};

}  

#endif  
