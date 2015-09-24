// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_UTILITY_GRAPHICS_PAINT_AGGREGATOR_H_
#define PPAPI_UTILITY_GRAPHICS_PAINT_AGGREGATOR_H_

#include <stddef.h>
#include <vector>

#include "ppapi/cpp/point.h"
#include "ppapi/cpp/rect.h"

namespace pp {

class PaintAggregator {
 public:
  struct PaintUpdate {
    
    
    PaintUpdate();

    
    ~PaintUpdate();

    
    
    bool has_scroll;

    
    
    
    
    
    
    Point scroll_delta;

    
    
    
    
    Rect scroll_rect;

    
    
    
    
    std::vector<Rect> paint_rects;

    
    Rect paint_bounds;
  };

  
  PaintAggregator();

  
  
  
  
  
  
  
  
  
  void set_max_redundant_paint_to_scroll_area(float area) {
    max_redundant_paint_to_scroll_area_ = area;
  }

  
  
  
  
  
  
  
  
  void set_max_paint_rects(size_t max_rects) {
    max_paint_rects_ = max_rects;
  }

  
  
  
  
  
  bool HasPendingUpdate() const;

  
  void ClearPendingUpdate();

  
  
  
  PaintUpdate GetPendingUpdate() const;

  
  
  
  void InvalidateRect(const Rect& rect);

  
  
  
  
  void ScrollRect(const Rect& clip_rect, const Point& amount);

 private:
  
  
  
  
  
  
  
  
  
  class InternalPaintUpdate {
   public:
    InternalPaintUpdate();
    ~InternalPaintUpdate();

    
    
    
    Rect GetScrollDamage() const;

    
    
    Rect GetPaintBounds() const;

    Point scroll_delta;
    Rect scroll_rect;

    
    std::vector<Rect> paint_rects;
  };

  Rect ScrollPaintRect(const Rect& paint_rect, const Point& amount) const;
  bool ShouldInvalidateScrollRect(const Rect& rect) const;
  void InvalidateScrollRect();
  void CombinePaintRects();

  InternalPaintUpdate update_;

  
  
  
  
  float max_redundant_paint_to_scroll_area_;

  
  
  
  
  
  
  size_t max_paint_rects_;
};

}  

#endif  
