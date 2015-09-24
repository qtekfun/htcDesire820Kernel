// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TREES_OCCLUSION_TRACKER_H_
#define CC_TREES_OCCLUSION_TRACKER_H_

#include <vector>

#include "base/basictypes.h"
#include "cc/base/cc_export.h"
#include "cc/base/region.h"
#include "cc/layers/layer_iterator.h"
#include "ui/gfx/rect.h"

namespace cc {
class OverdrawMetrics;
class LayerImpl;
class RenderSurfaceImpl;
class Layer;
class RenderSurface;

template <typename LayerType, typename RenderSurfaceType>
class CC_EXPORT OcclusionTrackerBase {
 public:
  OcclusionTrackerBase(gfx::Rect screen_space_clip_rect,
                       bool record_metrics_for_frame);
  ~OcclusionTrackerBase();

  
  
  void EnterLayer(const LayerIteratorPosition<LayerType>& layer_iterator);
  
  
  void LeaveLayer(const LayerIteratorPosition<LayerType>& layer_iterator);

  
  
  
  
  bool Occluded(const LayerType* render_target,
                gfx::Rect content_rect,
                const gfx::Transform& draw_transform,
                bool impl_draw_transform_is_unknown) const;

  
  
  
  
  gfx::Rect UnoccludedContentRect(
      const LayerType* render_target,
      gfx::Rect content_rect,
      const gfx::Transform& draw_transform,
      bool impl_draw_transform_is_unknown) const;

  
  
  
  gfx::Rect UnoccludedContributingSurfaceContentRect(
      const LayerType* layer,
      bool for_replica,
      gfx::Rect content_rect) const;

  
  OverdrawMetrics* overdraw_metrics() const {
    return overdraw_metrics_.get();
  }

  
  Region ComputeVisibleRegionInScreen() const {
    DCHECK(!stack_.back().target->parent());
    return SubtractRegions(screen_space_clip_rect_,
                           stack_.back().occlusion_from_inside_target);
  }

  void set_minimum_tracking_size(gfx::Size size) {
    minimum_tracking_size_ = size;
  }

  
  void set_occluding_screen_space_rects_container(
      std::vector<gfx::Rect>* rects) {
    occluding_screen_space_rects_ = rects;
  }
  void set_non_occluding_screen_space_rects_container(
      std::vector<gfx::Rect>* rects) {
    non_occluding_screen_space_rects_ = rects;
  }

 protected:
  struct StackObject {
    StackObject() : target(0) {}
    explicit StackObject(const LayerType* target) : target(target) {}
    const LayerType* target;
    Region occlusion_from_outside_target;
    Region occlusion_from_inside_target;
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  std::vector<StackObject> stack_;

 private:
  
  
  void EnterRenderTarget(const LayerType* new_target);

  
  
  
  void FinishedRenderTarget(const LayerType* finished_target);

  
  
  
  
  void LeaveToRenderTarget(const LayerType* new_target);

  
  void MarkOccludedBehindLayer(const LayerType* layer);

  gfx::Rect screen_space_clip_rect_;
  scoped_ptr<class OverdrawMetrics> overdraw_metrics_;
  gfx::Size minimum_tracking_size_;

  
  std::vector<gfx::Rect>* occluding_screen_space_rects_;
  std::vector<gfx::Rect>* non_occluding_screen_space_rects_;

  DISALLOW_COPY_AND_ASSIGN(OcclusionTrackerBase);
};

typedef OcclusionTrackerBase<Layer, RenderSurface> OcclusionTracker;
typedef OcclusionTrackerBase<LayerImpl, RenderSurfaceImpl> OcclusionTrackerImpl;
#if !defined(COMPILER_MSVC)
extern template class OcclusionTrackerBase<Layer, RenderSurface>;
extern template class OcclusionTrackerBase<LayerImpl, RenderSurfaceImpl>;
#endif

}  

#endif  
