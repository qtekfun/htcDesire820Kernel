// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_QUADS_DRAW_QUAD_H_
#define CC_QUADS_DRAW_QUAD_H_

#include "base/callback.h"
#include "cc/base/cc_export.h"
#include "cc/quads/shared_quad_state.h"
#include "cc/resources/resource_provider.h"

namespace base {
class Value;
class DictionaryValue;
}

namespace cc {

class CC_EXPORT DrawQuad {
 public:
  enum Material {
    INVALID,
    CHECKERBOARD,
    DEBUG_BORDER,
    IO_SURFACE_CONTENT,
    PICTURE_CONTENT,
    RENDER_PASS,
    TEXTURE_CONTENT,
    SOLID_COLOR,
    TILED_CONTENT,
    YUV_VIDEO_CONTENT,
    STREAM_VIDEO_CONTENT,
  };

  virtual ~DrawQuad();

  scoped_ptr<DrawQuad> Copy(
      const SharedQuadState* copied_shared_quad_state) const;

  
  const gfx::Transform& quadTransform() const {
    return shared_quad_state->content_to_target_transform;
  }
  gfx::Rect visibleContentRect() const {
    return shared_quad_state->visible_content_rect;
  }
  gfx::Rect clipRect() const { return shared_quad_state->clip_rect; }
  bool isClipped() const { return shared_quad_state->is_clipped; }
  float opacity() const { return shared_quad_state->opacity; }

  Material material;

  
  
  gfx::Rect rect;

  
  
  gfx::Rect opaque_rect;

  
  
  gfx::Rect visible_rect;

  
  
  
  bool needs_blending;

  
  
  
  const SharedQuadState* shared_quad_state;

  bool IsDebugQuad() const { return material == DEBUG_BORDER; }

  bool ShouldDrawWithBlending() const {
    if (needs_blending || shared_quad_state->opacity < 1.0f)
      return true;
    if (visible_rect.IsEmpty())
      return false;
    return !opaque_rect.Contains(visible_rect);
  }

  typedef ResourceProvider::ResourceId ResourceId;
  typedef base::Callback<ResourceId(ResourceId)> ResourceIteratorCallback;
  virtual void IterateResources(const ResourceIteratorCallback& callback) = 0;

  
  
  bool IsLeftEdge() const { return !rect.x(); }

  
  
  bool IsTopEdge() const { return !rect.y(); }

  
  
  bool IsRightEdge() const {
    return rect.right() == shared_quad_state->content_bounds.width();
  }

  
  
  bool IsBottomEdge() const {
    return rect.bottom() == shared_quad_state->content_bounds.height();
  }

  
  
  bool IsEdge() const {
    return IsLeftEdge() || IsTopEdge() || IsRightEdge() || IsBottomEdge();
  }

  scoped_ptr<base::Value> AsValue() const;

 protected:
  DrawQuad();

  void SetAll(const SharedQuadState* shared_quad_state,
              Material material,
              gfx::Rect rect,
              gfx::Rect opaque_rect,
              gfx::Rect visible_rect,
              bool needs_blending);
  virtual void ExtendValue(base::DictionaryValue* value) const = 0;
};

}  

#endif  
