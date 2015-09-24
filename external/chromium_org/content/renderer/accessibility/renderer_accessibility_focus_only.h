// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_ACCESSIBILITY_RENDERER_ACCESSIBILITY_FOCUS_ONLY_H_
#define CONTENT_RENDERER_ACCESSIBILITY_RENDERER_ACCESSIBILITY_FOCUS_ONLY_H_

#include "content/renderer/accessibility/renderer_accessibility.h"

namespace content {

class RendererAccessibilityFocusOnly : public RendererAccessibility {
 public:
  explicit RendererAccessibilityFocusOnly(RenderViewImpl* render_view);
  virtual ~RendererAccessibilityFocusOnly();

  
  virtual void HandleWebAccessibilityEvent(
      const blink::WebAXObject& obj, blink::WebAXEvent event) OVERRIDE;

  
  virtual void FocusedNodeChanged(const blink::WebNode& node) OVERRIDE;
  virtual void DidFinishLoad(blink::WebFrame* frame) OVERRIDE;

 private:
  void HandleFocusedNodeChanged(const blink::WebNode& node,
                                bool send_focus_event);

  int next_id_;

  DISALLOW_COPY_AND_ASSIGN(RendererAccessibilityFocusOnly);
};

}  

#endif  
