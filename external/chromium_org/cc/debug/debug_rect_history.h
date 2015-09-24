// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_DEBUG_DEBUG_RECT_HISTORY_H_
#define CC_DEBUG_DEBUG_RECT_HISTORY_H_

#include <vector>
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "cc/layers/layer_lists.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/rect_f.h"

namespace cc {

class LayerImpl;
class LayerTreeDebugState;

enum DebugRectType {
  PAINT_RECT_TYPE,
  PROPERTY_CHANGED_RECT_TYPE,
  SURFACE_DAMAGE_RECT_TYPE,
  SCREEN_SPACE_RECT_TYPE,
  REPLICA_SCREEN_SPACE_RECT_TYPE,
  OCCLUDING_RECT_TYPE,
  NONOCCLUDING_RECT_TYPE,
  TOUCH_EVENT_HANDLER_RECT_TYPE,
  WHEEL_EVENT_HANDLER_RECT_TYPE,
  NON_FAST_SCROLLABLE_RECT_TYPE,
  ANIMATION_BOUNDS_RECT_TYPE,
};

struct DebugRect {
  DebugRect(DebugRectType new_type, gfx::RectF new_rect)
      : type(new_type), rect(new_rect) {}

  DebugRectType type;
  gfx::RectF rect;
};

class DebugRectHistory {
 public:
  static scoped_ptr<DebugRectHistory> Create();

  ~DebugRectHistory();

  
  
  void SaveDebugRectsForCurrentFrame(
      LayerImpl* root_layer,
      const LayerImplList& render_surface_layer_list,
      const std::vector<gfx::Rect>& occluding_screen_space_rects,
      const std::vector<gfx::Rect>& non_occluding_screen_space_rects,
      const LayerTreeDebugState& debug_state);

  const std::vector<DebugRect>& debug_rects() { return debug_rects_; }

 private:
  DebugRectHistory();

  void SavePaintRects(LayerImpl* layer);
  void SavePropertyChangedRects(
      const LayerImplList& render_surface_layer_list);
  void SaveSurfaceDamageRects(
      const LayerImplList& render_surface_layer_list);
  void SaveScreenSpaceRects(
      const LayerImplList& render_surface_layer_list);
  void SaveOccludingRects(
      const std::vector<gfx::Rect>& occluding_screen_space_rects);
  void SaveNonOccludingRects(
      const std::vector<gfx::Rect>& non_occluding_screen_space_rects);
  void SaveTouchEventHandlerRects(LayerImpl* layer);
  void SaveTouchEventHandlerRectsCallback(LayerImpl* layer);
  void SaveWheelEventHandlerRects(LayerImpl* layer);
  void SaveWheelEventHandlerRectsCallback(LayerImpl* layer);
  void SaveNonFastScrollableRects(LayerImpl* layer);
  void SaveNonFastScrollableRectsCallback(LayerImpl* layer);
  void SaveLayerAnimationBoundsRects(
      const LayerImplList& render_surface_layer_list);

  std::vector<DebugRect> debug_rects_;

  DISALLOW_COPY_AND_ASSIGN(DebugRectHistory);
};

}  

#endif  
