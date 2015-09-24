// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_DEBUG_OVERDRAW_METRICS_H_
#define CC_DEBUG_OVERDRAW_METRICS_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace gfx {
class Rect;
class Transform;
}

namespace cc {
class LayerTreeHost;
class LayerTreeHostImpl;

class OverdrawMetrics {
 public:
  static scoped_ptr<OverdrawMetrics> Create(bool record_metrics_for_frame) {
    return make_scoped_ptr(new OverdrawMetrics(record_metrics_for_frame));
  }

  

  
  
  void DidPaint(gfx::Rect painted_rect);
  
  
  
  void DidCullTilesForUpload(int count);
  
  void DidUpload(const gfx::Transform& transform_to_target,
                 gfx::Rect upload_rect,
                 gfx::Rect opaque_rect);
  
  void DidUseContentsTextureMemoryBytes(size_t contents_texture_use_bytes);
  
  
  void DidUseRenderSurfaceTextureMemoryBytes(size_t render_surface_use_bytes);

  

  
  void DidCullForDrawing(const gfx::Transform& transform_to_target,
                         gfx::Rect before_cull_rect,
                         gfx::Rect after_cull_rect);
  
  void DidDraw(const gfx::Transform& transform_to_target,
               gfx::Rect after_cull_rect,
               gfx::Rect opaque_rect);

  void RecordMetrics(const LayerTreeHost* layer_tree_host) const;
  void RecordMetrics(const LayerTreeHostImpl* layer_tree_host_impl) const;

  
  float pixels_drawn_opaque() const { return pixels_drawn_opaque_; }
  float pixels_drawn_translucent() const { return pixels_drawn_translucent_; }
  float pixels_culled_for_drawing() const { return pixels_culled_for_drawing_; }
  float pixels_painted() const { return pixels_painted_; }
  float pixels_uploaded_opaque() const { return pixels_uploaded_opaque_; }
  float pixels_uploaded_translucent() const {
    return pixels_uploaded_translucent_;
  }
  int tiles_culled_for_upload() const { return tiles_culled_for_upload_; }

 private:
  enum MetricsType {
    UpdateAndCommit,
    DrawingToScreen
  };

  explicit OverdrawMetrics(bool record_metrics_for_frame);

  template <typename LayerTreeHostType>
  void RecordMetricsInternal(MetricsType metrics_type,
                             const LayerTreeHostType* layer_tree_host) const;

  
  bool record_metrics_for_frame_;

  

  
  float pixels_painted_;
  
  float pixels_uploaded_opaque_;
  
  float pixels_uploaded_translucent_;
  
  int tiles_culled_for_upload_;
  
  uint64 contents_texture_use_bytes_;
  
  uint64 render_surface_texture_use_bytes_;

  

  
  
  float pixels_drawn_opaque_;
  
  float pixels_drawn_translucent_;
  
  float pixels_culled_for_drawing_;

  DISALLOW_COPY_AND_ASSIGN(OverdrawMetrics);
};

}  

#endif  
