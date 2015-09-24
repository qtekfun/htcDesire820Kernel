// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_MEDIA_WEB_VIDEO_RENDERER_H_
#define WEBKIT_GLUE_MEDIA_WEB_VIDEO_RENDERER_H_

#include "media/base/video_frame.h"
#include "media/filters/video_renderer_base.h"
#include "webkit/glue/webmediaplayer_impl.h"

namespace webkit_glue {

class WebVideoRenderer : public media::VideoRendererBase {
 public:
  WebVideoRenderer() : media::VideoRendererBase() {}
  virtual ~WebVideoRenderer() {}

  
  virtual void SetWebMediaPlayerImplProxy(WebMediaPlayerImpl::Proxy* proxy) = 0;

  
  
  
  
  
  
  virtual void SetRect(const gfx::Rect& rect) = 0;

  
  
  
  
  virtual void Paint(SkCanvas* canvas,
                     const gfx::Rect& dest_rect) = 0;

  
  
  
  
  
  
  virtual void GetCurrentFrame(scoped_refptr<media::VideoFrame>* frame_out) {}
  virtual void PutCurrentFrame(scoped_refptr<media::VideoFrame> frame) {}

 private:
  DISALLOW_COPY_AND_ASSIGN(WebVideoRenderer);
};

}  

#endif  
