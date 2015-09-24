// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_LAYERS_DRAW_PROPERTIES_H_
#define CC_LAYERS_DRAW_PROPERTIES_H_

#include "base/memory/scoped_ptr.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/transform.h"

namespace cc {

template <typename LayerType>
struct CC_EXPORT DrawProperties {
  DrawProperties()
      : opacity(0.f),
        opacity_is_animating(false),
        screen_space_opacity_is_animating(false),
        target_space_transform_is_animating(false),
        screen_space_transform_is_animating(false),
        can_use_lcd_text(false),
        is_clipped(false),
        render_target(NULL),
        contents_scale_x(1.f),
        contents_scale_y(1.f),
        num_descendants_that_draw_content(0),
        num_unclipped_descendants(0),
        layer_or_descendant_has_copy_request(false),
        has_child_with_a_scroll_parent(false),
        sorted_for_recursion(false),
        index_of_first_descendants_addition(0),
        num_descendants_added(0),
        index_of_first_render_surface_layer_list_addition(0),
        num_render_surfaces_added(0) {}

  
  
  gfx::Transform target_space_transform;

  
  gfx::Transform screen_space_transform;

  
  
  
  float opacity;

  
  
  
  
  bool opacity_is_animating;
  bool screen_space_opacity_is_animating;
  bool target_space_transform_is_animating;
  bool screen_space_transform_is_animating;

  
  bool can_use_lcd_text;

  
  bool is_clipped;

  
  
  
  LayerType* render_target;

  
  scoped_ptr<typename LayerType::RenderSurfaceType> render_surface;

  
  gfx::Rect visible_content_rect;

  
  
  gfx::Rect drawable_content_rect;

  
  
  gfx::Rect clip_rect;

  
  
  
  
  float contents_scale_x;
  float contents_scale_y;
  gfx::Size content_bounds;

  
  int num_descendants_that_draw_content;

  
  
  int num_unclipped_descendants;

  
  
  bool layer_or_descendant_has_copy_request;

  
  
  
  
  bool has_child_with_a_scroll_parent;

  
  
  bool sorted_for_recursion;

  
  
  
  size_t index_of_first_descendants_addition;
  size_t num_descendants_added;
  size_t index_of_first_render_surface_layer_list_addition;
  size_t num_render_surfaces_added;
};

}  

#endif  
