// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_MEDIA_STREAM_CLIENT_H_
#define CONTENT_RENDERER_MEDIA_MEDIA_STREAM_CLIENT_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "content/renderer/media/video_frame_provider.h"

class GURL;

namespace content {

class MediaStreamAudioRenderer;

class MediaStreamClient {
 public:
  
  virtual bool IsMediaStream(const GURL& url) = 0;

  virtual scoped_refptr<VideoFrameProvider> GetVideoFrameProvider(
      const GURL& url,
      const base::Closure& error_cb,
      const VideoFrameProvider::RepaintCB& repaint_cb) = 0;

  virtual scoped_refptr<MediaStreamAudioRenderer> GetAudioRenderer(
      const GURL& url) = 0;

 protected:
  virtual ~MediaStreamClient() {}
};

}  

#endif  
