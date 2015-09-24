// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PAINT_AGGREGATOR_H_
#define CONTENT_RENDERER_PAINT_AGGREGATOR_H_

#include <vector>

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/vector2d.h"

namespace content {

class CONTENT_EXPORT PaintAggregator {
 public:
  
  
  
  
  
  
  
  struct CONTENT_EXPORT PendingUpdate {
    PendingUpdate();
    ~PendingUpdate();

    
    
    gfx::Rect GetScrollDamage() const;

    
    gfx::Rect GetPaintBounds() const;

    gfx::Vector2d scroll_delta;
    gfx::Rect scroll_rect;
    std::vector<gfx::Rect> paint_rects;
  };

  
  
  bool HasPendingUpdate() const;
  void ClearPendingUpdate();

  
  void PopPendingUpdate(PendingUpdate* update);

  
  void InvalidateRect(const gfx::Rect& rect);

  
  void ScrollRect(const gfx::Vector2d& delta, const gfx::Rect& clip_rect);

 private:
  gfx::Rect ScrollPaintRect(const gfx::Rect& paint_rect,
                            const gfx::Vector2d& delta) const;
  bool ShouldInvalidateScrollRect(const gfx::Rect& rect) const;
  void InvalidateScrollRect();
  void CombinePaintRects();

  PendingUpdate update_;
};

}  

#endif  
