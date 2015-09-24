// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PORT_BROWSER_RENDER_VIEW_HOST_DELEGATE_VIEW_H_
#define CONTENT_PORT_BROWSER_RENDER_VIEW_HOST_DELEGATE_VIEW_H_

#include <vector>

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/common/drag_event_source_info.h"
#include "third_party/WebKit/public/web/WebDragOperation.h"

class SkBitmap;

namespace gfx {
class ImageSkia;
class Rect;
class Vector2d;
}

namespace content {
struct ContextMenuParams;
struct DropData;
struct MenuItem;

class CONTENT_EXPORT RenderViewHostDelegateView {
 public:
  
  
  virtual void ShowContextMenu(const ContextMenuParams& params) {}

  
  
  
  virtual void ShowPopupMenu(const gfx::Rect& bounds,
                             int item_height,
                             double item_font_size,
                             int selected_item,
                             const std::vector<MenuItem>& items,
                             bool right_aligned,
                             bool allow_multiple_selection) = 0;

  
  
  
  
  
  virtual void StartDragging(const DropData& drop_data,
                             blink::WebDragOperationsMask allowed_ops,
                             const gfx::ImageSkia& image,
                             const gfx::Vector2d& image_offset,
                             const DragEventSourceInfo& event_info) {}

  
  
  virtual void UpdateDragCursor(blink::WebDragOperation operation) {}

  
  virtual void GotFocus() {}

  
  
  
  virtual void TakeFocus(bool reverse) {}

 protected:
  virtual ~RenderViewHostDelegateView() {}
};

}  

#endif  
