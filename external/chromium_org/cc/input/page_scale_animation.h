// Copyright 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_INPUT_PAGE_SCALE_ANIMATION_H_
#define CC_INPUT_PAGE_SCALE_ANIMATION_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/size.h"
#include "ui/gfx/vector2d_f.h"

namespace cc {
class TimingFunction;

class PageScaleAnimation {
 public:
  
  static scoped_ptr<PageScaleAnimation> Create(
      gfx::Vector2dF start_scroll_offset,
      float start_page_scale_factor,
      gfx::SizeF viewport_size,
      gfx::SizeF root_layer_size,
      scoped_ptr<TimingFunction> timing_function);

  ~PageScaleAnimation();

  
  

  
  void ZoomTo(gfx::Vector2dF target_scroll_offset,
              float target_page_scale_factor,
              double duration);

  
  
  
  
  void ZoomWithAnchor(gfx::Vector2dF anchor,
                      float target_page_scale_factor,
                      double duration);

  
  
  bool IsAnimationStarted() const;
  void StartAnimation(double time);

  
  
  gfx::Vector2dF ScrollOffsetAtTime(double time) const;
  float PageScaleFactorAtTime(double time) const;
  bool IsAnimationCompleteAtTime(double time) const;

  
  
  double start_time() const { return start_time_; }
  double duration() const { return duration_; }
  double end_time() const { return start_time_ + duration_; }
  gfx::Vector2dF target_scroll_offset() const { return target_scroll_offset_; }
  float target_page_scale_factor() const { return target_page_scale_factor_; }

 protected:
  PageScaleAnimation(gfx::Vector2dF start_scroll_offset,
                     float start_page_scale_factor,
                     gfx::SizeF viewport_size,
                     gfx::SizeF root_layer_size,
                     scoped_ptr<TimingFunction> timing_function);

 private:
  void ClampTargetScrollOffset();
  void InferTargetScrollOffsetFromStartAnchor();
  void InferTargetAnchorFromScrollOffsets();

  gfx::SizeF StartViewportSize() const;
  gfx::SizeF TargetViewportSize() const;
  float InterpAtTime(double time) const;
  gfx::SizeF ViewportSizeAt(float interp) const;
  gfx::Vector2dF ScrollOffsetAt(float interp) const;
  gfx::Vector2dF AnchorAt(float interp) const;
  gfx::Vector2dF ViewportRelativeAnchorAt(float interp) const;
  float PageScaleFactorAt(float interp) const;

  float start_page_scale_factor_;
  float target_page_scale_factor_;
  gfx::Vector2dF start_scroll_offset_;
  gfx::Vector2dF target_scroll_offset_;

  gfx::Vector2dF start_anchor_;
  gfx::Vector2dF target_anchor_;

  gfx::SizeF viewport_size_;
  gfx::SizeF root_layer_size_;

  double start_time_;
  double duration_;

  scoped_ptr<TimingFunction> timing_function_;

  DISALLOW_COPY_AND_ASSIGN(PageScaleAnimation);
};

}  

#endif  
