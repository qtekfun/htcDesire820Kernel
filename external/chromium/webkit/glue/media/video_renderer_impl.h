// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_MEDIA_VIDEO_RENDERER_IMPL_H_
#define WEBKIT_GLUE_MEDIA_VIDEO_RENDERER_IMPL_H_

#include "media/base/buffers.h"
#include "media/base/filters.h"
#include "media/filters/video_renderer_base.h"
#include "skia/ext/platform_canvas.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebMediaPlayer.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "webkit/glue/media/web_video_renderer.h"
#include "webkit/glue/webmediaplayer_impl.h"

namespace webkit_glue {

class VideoRendererImpl : public WebVideoRenderer {
 public:
  explicit VideoRendererImpl(bool pts_logging);
  virtual ~VideoRendererImpl();

  
  virtual void SetWebMediaPlayerImplProxy(WebMediaPlayerImpl::Proxy* proxy);
  virtual void SetRect(const gfx::Rect& rect);
  virtual void Paint(SkCanvas* canvas, const gfx::Rect& dest_rect);
  virtual void GetCurrentFrame(scoped_refptr<media::VideoFrame>* frame_out);
  virtual void PutCurrentFrame(scoped_refptr<media::VideoFrame> frame);

 protected:
  
  virtual bool OnInitialize(media::VideoDecoder* decoder);

  
  virtual void OnStop(media::FilterCallback* callback);

  
  virtual void OnFrameAvailable();

 private:
  
  
  bool CanFastPaint(SkCanvas* canvas, const gfx::Rect& dest_rect);

  
  void SlowPaint(media::VideoFrame* video_frame,
                 SkCanvas* canvas,
                 const gfx::Rect& dest_rect);

  
  
  
  void FastPaint(media::VideoFrame* video_frame,
                 SkCanvas* canvas,
                 const gfx::Rect& dest_rect);

  void TransformToSkIRect(const SkMatrix& matrix, const gfx::Rect& src_rect,
                          SkIRect* dest_rect);

  
  scoped_refptr<WebMediaPlayerImpl::Proxy> proxy_;

  
  SkBitmap bitmap_;

  
  
  
  
  
  
  
  media::VideoFrame* last_converted_frame_;
  base::TimeDelta last_converted_timestamp_;

  
  gfx::Size video_size_;

  
  bool pts_logging_;

  DISALLOW_COPY_AND_ASSIGN(VideoRendererImpl);
};

}  

#endif  
