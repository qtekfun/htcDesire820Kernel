// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_RENDERER_PAGE_CLICK_TRACKER_H_
#define COMPONENTS_AUTOFILL_CONTENT_RENDERER_PAGE_CLICK_TRACKER_H_

#include <vector>

#include "base/basictypes.h"
#include "content/public/renderer/render_view_observer.h"
#include "third_party/WebKit/public/web/WebDOMEventListener.h"
#include "third_party/WebKit/public/web/WebNode.h"

namespace autofill {

class PageClickListener;

class PageClickTracker : public content::RenderViewObserver,
                         public blink::WebDOMEventListener {
 public:
  
  
  PageClickTracker(content::RenderView* render_view,
                   PageClickListener* listener);
  virtual ~PageClickTracker();

 private:
  
  virtual void DidFinishDocumentLoad(blink::WebFrame* frame) OVERRIDE;
  virtual void FrameDetached(blink::WebFrame* frame) OVERRIDE;
  virtual void DidHandleMouseEvent(const blink::WebMouseEvent& event) OVERRIDE;

  
  virtual void handleEvent(const blink::WebDOMEvent& event);

  
  
  void HandleTextFieldMaybeLosingFocus(
      const blink::WebNode& newly_clicked_node);

  
  blink::WebNode last_node_clicked_;

  
  bool was_focused_;

  
  std::vector<blink::WebFrame*> tracked_frames_;

  
  PageClickListener* listener_;

  DISALLOW_COPY_AND_ASSIGN(PageClickTracker);
};

}  

#endif  
