// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_LAYER_ANIMATION_SEQUENCE_H_
#define UI_COMPOSITOR_LAYER_ANIMATION_SEQUENCE_H_

#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "ui/compositor/compositor_export.h"
#include "ui/compositor/layer_animation_element.h"

namespace ui {

class LayerAnimationDelegate;
class LayerAnimationObserver;

class COMPOSITOR_EXPORT LayerAnimationSequence
    : public base::SupportsWeakPtr<LayerAnimationSequence> {
 public:
  LayerAnimationSequence();
  
  explicit LayerAnimationSequence(LayerAnimationElement* element);
  virtual ~LayerAnimationSequence();

  
  
  
  void set_start_time(base::TimeTicks start_time) { start_time_ = start_time; }
  base::TimeTicks start_time() const { return start_time_; }

  
  
  
  void set_waiting_for_group_start(bool waiting) {
    waiting_for_group_start_ = waiting;
  }
  bool waiting_for_group_start() { return waiting_for_group_start_; }

  
  
  
  
  void Start(LayerAnimationDelegate* delegate);

  
  
  void Progress(base::TimeTicks now, LayerAnimationDelegate* delegate);

  
  
  bool IsFinished(base::TimeTicks time);

  
  
  void ProgressToEnd(LayerAnimationDelegate* delegate);

  
  
  void GetTargetValue(LayerAnimationElement::TargetValue* target) const;

  
  void Abort(LayerAnimationDelegate* delegate);

  
  const LayerAnimationElement::AnimatableProperties& properties() const {
    return properties_;
  }

  
  
  void AddElement(LayerAnimationElement* element);

  
  void set_is_cyclic(bool is_cyclic) { is_cyclic_ = is_cyclic; }
  bool is_cyclic() const { return is_cyclic_; }

  
  
  bool HasConflictingProperty(
      const LayerAnimationElement::AnimatableProperties& other) const;

  
  bool IsFirstElementThreaded() const;

  
  
  
  int animation_group_id() const { return animation_group_id_; }
  void set_animation_group_id(int id) { animation_group_id_ = id; }

  
  
  void AddObserver(LayerAnimationObserver* observer);
  void RemoveObserver(LayerAnimationObserver* observer);

  
  void OnThreadedAnimationStarted(const cc::AnimationEvent& event);

  
  void OnScheduled();

  
  void OnAnimatorDestroyed();

  
  
  double last_progressed_fraction() const { return last_progressed_fraction_; }

  size_t size() const;

  LayerAnimationElement* FirstElement() const;

 private:
  friend class LayerAnimatorTestController;

  typedef std::vector<linked_ptr<LayerAnimationElement> > Elements;

  FRIEND_TEST_ALL_PREFIXES(LayerAnimatorTest,
                           ObserverReleasedBeforeAnimationSequenceEnds);

  
  void NotifyScheduled();

  
  void NotifyEnded();

  
  void NotifyAborted();

  
  LayerAnimationElement* CurrentElement() const;

  
  LayerAnimationElement::AnimatableProperties properties_;

  
  Elements elements_;

  
  bool is_cyclic_;

  
  size_t last_element_;
  base::TimeTicks last_start_;

  
  base::TimeTicks start_time_;

  
  
  bool waiting_for_group_start_;

  
  
  
  
  int animation_group_id_;

  
  ObserverList<LayerAnimationObserver> observers_;

  
  
  double last_progressed_fraction_;

  base::WeakPtrFactory<LayerAnimationSequence> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(LayerAnimationSequence);
};

}  

#endif  
