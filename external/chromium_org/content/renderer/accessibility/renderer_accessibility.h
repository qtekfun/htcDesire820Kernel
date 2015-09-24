// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_ACCESSIBILITY_RENDERER_ACCESSIBILITY_H_
#define CONTENT_RENDERER_ACCESSIBILITY_RENDERER_ACCESSIBILITY_H_

#include "content/common/accessibility_messages.h"
#include "content/public/renderer/render_view_observer.h"
#include "third_party/WebKit/public/web/WebAXObject.h"

namespace blink {
class WebDocument;
};

namespace content {
class RenderViewImpl;

class CONTENT_EXPORT RendererAccessibility : public RenderViewObserver {
 public:
  explicit RendererAccessibility(RenderViewImpl* render_view);
  virtual ~RendererAccessibility();

  
  virtual void HandleWebAccessibilityEvent(
      const blink::WebAXObject& obj, blink::WebAXEvent event) = 0;

 protected:
  
  
  blink::WebDocument GetMainDocument();

#ifndef NDEBUG
  const std::string AccessibilityEventToString(blink::WebAXEvent event);
#endif

  
  RenderViewImpl* render_view_;

  
  bool logging_;

  DISALLOW_COPY_AND_ASSIGN(RendererAccessibility);
};

}  

#endif  
