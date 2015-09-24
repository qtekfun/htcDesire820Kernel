// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ANIMATION_ANIMATION_CONTAINER_H_
#define UI_GFX_ANIMATION_ANIMATION_CONTAINER_H_

#include <set>

#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "ui/gfx/gfx_export.h"

namespace gfx {

class AnimationContainerElement;
class AnimationContainerObserver;

class GFX_EXPORT AnimationContainer
    : public base::RefCounted<AnimationContainer> {
 public:
  AnimationContainer();

  
  
  
  void Start(AnimationContainerElement* animation);

  
  
  
  
  void Stop(AnimationContainerElement* animation);

  void set_observer(AnimationContainerObserver* observer) {
    observer_ = observer;
  }

  
  base::TimeTicks last_tick_time() const { return last_tick_time_; }

  
  bool is_running() const { return !elements_.empty(); }

 private:
  friend class base::RefCounted<AnimationContainer>;

  typedef std::set<AnimationContainerElement*> Elements;

  ~AnimationContainer();

  
  void Run();

  
  void SetMinTimerInterval(base::TimeDelta delta);

  
  base::TimeDelta GetMinInterval();

  
  
  
  
  base::TimeTicks last_tick_time_;

  
  Elements elements_;

  
  base::TimeDelta min_timer_interval_;

  base::RepeatingTimer<AnimationContainer> timer_;

  AnimationContainerObserver* observer_;

  DISALLOW_COPY_AND_ASSIGN(AnimationContainer);
};

}  

#endif  
