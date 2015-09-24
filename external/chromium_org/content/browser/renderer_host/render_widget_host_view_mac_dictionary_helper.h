// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_MAC_DICTIONARY_HELPER_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_MAC_DICTIONARY_HELPER_H_

#include "base/basictypes.h"
#include "ui/gfx/vector2d.h"

namespace content {

class RenderWidgetHostView;
class RenderWidgetHostViewMac;
class RenderWidgetHostViewPort;

class RenderWidgetHostViewMacDictionaryHelper {
 public:
  explicit RenderWidgetHostViewMacDictionaryHelper(
      RenderWidgetHostViewPort* view);

  
  
  
  
  void SetTargetView(RenderWidgetHostView* target_view);
  void set_offset(const gfx::Vector2d& offset) { offset_ = offset; }

  
  
  void ShowDefinitionForSelection();

 private:
  
  RenderWidgetHostViewMac* view_;
  
  
  RenderWidgetHostViewMac* target_view_;
  
  gfx::Vector2d offset_;

  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHostViewMacDictionaryHelper);
};

}  

#endif  
