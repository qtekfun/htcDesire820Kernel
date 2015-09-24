// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PPB_FULLSCREEN_CONTAINER_IMPL_H_
#define CONTENT_RENDERER_PEPPER_PPB_FULLSCREEN_CONTAINER_IMPL_H_

namespace blink {
class WebLayer;
struct WebCursorInfo;
struct WebRect;
}  

namespace content {

class FullscreenContainer {
 public:
  
  virtual void Invalidate() = 0;

  
  virtual void InvalidateRect(const blink::WebRect&) = 0;

  
  virtual void ScrollRect(int dx, int dy, const blink::WebRect&) = 0;

  
  
  virtual void Destroy() = 0;

  
  virtual void DidChangeCursor(const blink::WebCursorInfo& cursor) = 0;

  virtual void SetLayer(blink::WebLayer* layer) = 0;

 protected:
  virtual ~FullscreenContainer() {}
};

}  

#endif  
