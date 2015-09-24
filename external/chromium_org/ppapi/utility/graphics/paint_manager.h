// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_UTILITY_GRAPHICS_PAINT_MANAGER_H_
#define PPAPI_UTILITY_GRAPHICS_PAINT_MANAGER_H_

#include <vector>

#include "ppapi/cpp/graphics_2d.h"
#include "ppapi/utility/completion_callback_factory.h"
#include "ppapi/utility/graphics/paint_aggregator.h"


namespace pp {

class Graphics2D;
class Instance;
class Point;
class Rect;

class PaintManager {
 public:
  class Client {
   public:
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual bool OnPaint(Graphics2D& graphics,
                         const std::vector<Rect>& paint_rects,
                         const Rect& paint_bounds) = 0;

   protected:
    
    virtual ~Client() {}
  };

  
  
  
  PaintManager();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  PaintManager(Instance* instance, Client* client, bool is_always_opaque);

  
  ~PaintManager();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void Initialize(Instance* instance, Client* client, bool is_always_opaque);

  
  
  
  
  
  
  
  
  
  void set_max_redundant_paint_to_scroll_area(float area) {
    aggregator_.set_max_redundant_paint_to_scroll_area(area);
  }

  
  
  
  
  
  
  
  
  
  void set_max_paint_rects(size_t max_rects) {
    aggregator_.set_max_paint_rects(max_rects);
  }

  
  
  
  
  
  
  
  
  
  
  
  
  void SetSize(const Size& new_size);

  
  
  
  
  
  
  const Graphics2D& graphics() const { return graphics_; }

  
  
  
  
  
  
  Graphics2D& graphics() { return graphics_; }

  
  void Invalidate();

  
  
  
  void InvalidateRect(const Rect& rect);

  
  
  
  
  
  void ScrollRect(const Rect& clip_rect, const Point& amount);

  
  
  
  
  
  
  Size GetEffectiveSize() const;

 private:
  
  PaintManager(const PaintManager&);
  PaintManager& operator=(const PaintManager&);

  
  
  
  
  void EnsureCallbackPending();

  
  void DoPaint();

  
  void OnFlushComplete(int32_t result);

  
  
  void OnManualCallbackComplete(int32_t);

  Instance* instance_;

  
  Client* client_;

  bool is_always_opaque_;

  CompletionCallbackFactory<PaintManager> callback_factory_;

  
  
  Graphics2D graphics_;

  PaintAggregator aggregator_;

  
  bool manual_callback_pending_;
  bool flush_pending_;

  
  
  
  bool has_pending_resize_;
  Size pending_size_;
};

}  

#endif  
