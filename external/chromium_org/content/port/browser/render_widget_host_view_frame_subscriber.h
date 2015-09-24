// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PORT_BROWSER_RENDER_WIDGET_HOST_VIEW_FRAME_SUBSCRIBER_H_
#define CONTENT_PORT_BROWSER_RENDER_WIDGET_HOST_VIEW_FRAME_SUBSCRIBER_H_

#include "base/callback.h"
#include "base/time/time.h"

namespace gfx {
class Rect;
class Size;
}  

namespace media {
class VideoFrame;
}  

namespace content {

class RenderWidgetHostViewFrameSubscriber {
 public:
  virtual ~RenderWidgetHostViewFrameSubscriber() {}

  
  
  
  
  
  
  
  
  
  typedef base::Callback<void(
      base::Time ,
      bool )> DeliverFrameCallback;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool ShouldCaptureFrame(
      base::Time present_time,
      scoped_refptr<media::VideoFrame>* storage,
      DeliverFrameCallback* callback) = 0;
};

}  

#endif  
