// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_LAYER_ANIMATION_OBSERVER_H_
#define UI_COMPOSITOR_LAYER_ANIMATION_OBSERVER_H_

#include <map>
#include <set>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/compositor/compositor_export.h"
#include "ui/compositor/layer_animation_element.h"

namespace ui {

class LayerAnimationSequence;
class ScopedLayerAnimationSettings;
class ImplicitAnimationObserver;

class COMPOSITOR_EXPORT LayerAnimationObserver  {
 public:
  
  virtual void OnLayerAnimationEnded(
      LayerAnimationSequence* sequence) = 0;

  
  
  virtual void OnLayerAnimationAborted(
      LayerAnimationSequence* sequence) = 0;

  
  virtual void OnLayerAnimationScheduled(
      LayerAnimationSequence* sequence) = 0;

 protected:
  typedef std::set<LayerAnimationSequence*> AttachedSequences;

  LayerAnimationObserver();
  virtual ~LayerAnimationObserver();

  
  
  
  
  
  virtual bool RequiresNotificationWhenAnimatorDestroyed() const;

  
  virtual void OnAttachedToSequence(LayerAnimationSequence* sequence);

  
  virtual void OnDetachedFromSequence(LayerAnimationSequence* sequence);

  
  void StopObserving();

  const AttachedSequences& attached_sequences() const {
    return attached_sequences_;
  }

 private:
  friend class LayerAnimationSequence;

  
  void AttachedToSequence(LayerAnimationSequence* sequence);

  
  
  void DetachedFromSequence(LayerAnimationSequence* sequence,
                            bool send_notification);

  AttachedSequences attached_sequences_;
};

class COMPOSITOR_EXPORT ImplicitAnimationObserver
    : public LayerAnimationObserver {
 public:
  ImplicitAnimationObserver();
  virtual ~ImplicitAnimationObserver();

  
  virtual void OnImplicitAnimationsScheduled() {}

  virtual void OnImplicitAnimationsCompleted() = 0;

 protected:
  
  
  void StopObservingImplicitAnimations();

  
  
  
  bool WasAnimationAbortedForProperty(
      LayerAnimationElement::AnimatableProperty property) const;

  
  
  
  bool WasAnimationCompletedForProperty(
      LayerAnimationElement::AnimatableProperty property) const;

 private:
  enum AnimationStatus {
    ANIMATION_STATUS_UNKNOWN,
    ANIMATION_STATUS_COMPLETED,
    ANIMATION_STATUS_ABORTED,
  };

  friend class ScopedLayerAnimationSettings;

  
  virtual void OnLayerAnimationEnded(
      LayerAnimationSequence* sequence) OVERRIDE;
  virtual void OnLayerAnimationAborted(
      LayerAnimationSequence* sequence) OVERRIDE;
  virtual void OnLayerAnimationScheduled(
      LayerAnimationSequence* sequence) OVERRIDE;
  virtual void OnAttachedToSequence(
      LayerAnimationSequence* sequence) OVERRIDE;
  virtual void OnDetachedFromSequence(
      LayerAnimationSequence* sequence) OVERRIDE;

  
  bool active() const { return active_; }
  void SetActive(bool active);

  void CheckCompleted();

  void UpdatePropertyAnimationStatus(LayerAnimationSequence* sequence,
                                     AnimationStatus status);
  AnimationStatus AnimationStatusForProperty(
      LayerAnimationElement::AnimatableProperty property) const;

  bool active_;

  
  
  bool* destroyed_;

  typedef std::map<LayerAnimationElement::AnimatableProperty,
                   AnimationStatus> PropertyAnimationStatusMap;
  PropertyAnimationStatusMap property_animation_status_;

  
  bool first_sequence_scheduled_;
};

}  

#endif  
