// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_PAGE_LOAD_HISTOGRAMS_H_
#define CHROME_RENDERER_PAGE_LOAD_HISTOGRAMS_H_

#include "base/basictypes.h"
#include "content/public/renderer/render_view_observer.h"

namespace content {
class DocumentState;
}

class PageLoadHistograms : public content::RenderViewObserver {
 public:
  explicit PageLoadHistograms(content::RenderView* render_view);

 private:
  
  virtual void FrameWillClose(blink::WebFrame* frame) OVERRIDE;
  virtual void ClosePage() OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void Dump(blink::WebFrame* frame);

  void ResetCrossFramePropertyAccess();

  void LogPageLoadTime(const content::DocumentState* load_times,
                       const blink::WebDataSource* ds) const;

  
  
  int cross_origin_access_count_;
  int same_origin_access_count_;

  DISALLOW_COPY_AND_ASSIGN(PageLoadHistograms);
};

#endif  
