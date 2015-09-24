// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_ANIMATION_ANIMATION_H_
#define CC_ANIMATION_ANIMATION_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "cc/base/cc_export.h"

namespace cc {

class AnimationCurve;

class CC_EXPORT Animation {
 public:
  
  
  
  
  
  
  
  
  enum RunState {
    WaitingForTargetAvailability = 0,
    WaitingForDeletion,
    Starting,
    Running,
    Paused,
    Finished,
    Aborted,
    
    RunStateEnumSize
  };

  enum TargetProperty {
    Transform = 0,
    Opacity,
    Filter,
    BackgroundColor,
    ScrollOffset,
    
    TargetPropertyEnumSize
  };

  static scoped_ptr<Animation> Create(scoped_ptr<AnimationCurve> curve,
                                      int animation_id,
                                      int group_id,
                                      TargetProperty target_property);

  virtual ~Animation();

  int id() const { return id_; }
  int group() const { return group_; }
  TargetProperty target_property() const { return target_property_; }

  RunState run_state() const { return run_state_; }
  void SetRunState(RunState run_state, double monotonic_time);

  
  
  
  int iterations() const { return iterations_; }
  void set_iterations(int n) { iterations_ = n; }

  double start_time() const { return start_time_; }
  void set_start_time(double monotonic_time) { start_time_ = monotonic_time; }
  bool has_set_start_time() const { return !!start_time_; }

  double time_offset() const { return time_offset_; }
  void set_time_offset(double monotonic_time) { time_offset_ = monotonic_time; }

  void Suspend(double monotonic_time);
  void Resume(double monotonic_time);

  
  
  bool alternates_direction() const { return alternates_direction_; }
  void set_alternates_direction(bool alternates) {
    alternates_direction_ = alternates;
  }

  bool IsFinishedAt(double monotonic_time) const;
  bool is_finished() const {
    return run_state_ == Finished ||
        run_state_ == Aborted ||
        run_state_ == WaitingForDeletion;
  }

  AnimationCurve* curve() { return curve_.get(); }
  const AnimationCurve* curve() const { return curve_.get(); }

  
  
  
  bool needs_synchronized_start_time() const {
    return needs_synchronized_start_time_;
  }
  void set_needs_synchronized_start_time(bool needs_synchronized_start_time) {
    needs_synchronized_start_time_ = needs_synchronized_start_time;
  }

  
  
  bool received_finished_event() const {
    return received_finished_event_;
  }
  void set_received_finished_event(bool received_finished_event) {
    received_finished_event_ = received_finished_event;
  }

  
  
  double TrimTimeToCurrentIteration(double monotonic_time) const;

  scoped_ptr<Animation> Clone() const;
  scoped_ptr<Animation> CloneAndInitialize(RunState initial_run_state,
                                           double start_time) const;
  bool is_controlling_instance() const { return is_controlling_instance_; }

  void PushPropertiesTo(Animation* other) const;

  void set_is_impl_only(bool is_impl_only) { is_impl_only_ = is_impl_only; }
  bool is_impl_only() const { return is_impl_only_; }

 private:
  Animation(scoped_ptr<AnimationCurve> curve,
            int animation_id,
            int group_id,
            TargetProperty target_property);

  scoped_ptr<AnimationCurve> curve_;

  
  int id_;

  
  
  
  
  
  int group_;

  TargetProperty target_property_;
  RunState run_state_;
  int iterations_;
  double start_time_;
  bool alternates_direction_;

  
  
  
  
  double time_offset_;

  bool needs_synchronized_start_time_;
  bool received_finished_event_;

  
  
  
  bool suspended_;

  
  
  
  
  double pause_time_;
  double total_paused_time_;

  
  
  
  
  
  
  
  bool is_controlling_instance_;

  bool is_impl_only_;

  DISALLOW_COPY_AND_ASSIGN(Animation);
};

}  

#endif  
