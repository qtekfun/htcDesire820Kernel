// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_SCOPED_LAYER_ANIMATION_SETTINGS_H_
#define UI_COMPOSITOR_SCOPED_LAYER_ANIMATION_SETTINGS_H_

#include <set>

#include "base/memory/scoped_vector.h"
#include "base/time/time.h"

#include "ui/compositor/compositor_export.h"
#include "ui/compositor/layer_animator.h"
#include "ui/gfx/animation/tween.h"

namespace ui {

class ImplicitAnimationObserver;
class LayerAnimationObserver;
class InvertingObserver;

class COMPOSITOR_EXPORT ScopedLayerAnimationSettings {
 public:
  explicit ScopedLayerAnimationSettings(LayerAnimator* animator);
  virtual ~ScopedLayerAnimationSettings();

  void AddObserver(ImplicitAnimationObserver* observer);

  void SetTransitionDuration(base::TimeDelta duration);
  base::TimeDelta GetTransitionDuration() const;

  
  
  
  
  void LockTransitionDuration();

  void SetTweenType(gfx::Tween::Type tween_type);
  gfx::Tween::Type GetTweenType() const;

  void SetPreemptionStrategy(LayerAnimator::PreemptionStrategy strategy);
  LayerAnimator::PreemptionStrategy GetPreemptionStrategy() const;

  
  void SetInverselyAnimatedBaseLayer(Layer* base);

  
  
  
  void AddInverselyAnimatedLayer(Layer* inverse_layer);

 private:
  LayerAnimator* animator_;
  bool old_is_transition_duration_locked_;
  base::TimeDelta old_transition_duration_;
  gfx::Tween::Type old_tween_type_;
  LayerAnimator::PreemptionStrategy old_preemption_strategy_;
  std::set<ImplicitAnimationObserver*> observers_;
  scoped_ptr<InvertingObserver> inverse_observer_;

  DISALLOW_COPY_AND_ASSIGN(ScopedLayerAnimationSettings);
};

}  

#endif  
