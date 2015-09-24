// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ANIMATION_MULTI_ANIMATION_H_
#define UI_GFX_ANIMATION_MULTI_ANIMATION_H_

#include <vector>

#include "ui/gfx/animation/animation.h"
#include "ui/gfx/animation/tween.h"

namespace gfx {

class GFX_EXPORT MultiAnimation : public Animation {
 public:
  
  
  
  
  
  
  
  
  
  
  
  struct Part {
    Part() : time_ms(0), start_time_ms(0), end_time_ms(0), type(Tween::ZERO) {}
    Part(int time_ms, Tween::Type type)
        : time_ms(time_ms),
          start_time_ms(0),
          end_time_ms(time_ms),
          type(type) {}

    int time_ms;
    int start_time_ms;
    int end_time_ms;
    Tween::Type type;
  };

  typedef std::vector<Part> Parts;

  MultiAnimation(const Parts& parts, base::TimeDelta timer_interval);
  virtual ~MultiAnimation();

  
  static base::TimeDelta GetDefaultTimerInterval();

  
  
  void set_continuous(bool continuous) { continuous_ = continuous; }

  
  
  virtual double GetCurrentValue() const OVERRIDE;

  
  size_t current_part_index() const { return current_part_index_; }

 protected:
  
  virtual void Step(base::TimeTicks time_now) OVERRIDE;
  virtual void SetStartTime(base::TimeTicks start_time) OVERRIDE;

 private:
  
  
  
  const Part& GetPart(int* time_ms, size_t* part_index);

  
  const Parts parts_;

  
  const int cycle_time_ms_;

  
  double current_value_;

  
  size_t current_part_index_;

  
  bool continuous_;

  DISALLOW_COPY_AND_ASSIGN(MultiAnimation);
};

}  

#endif  
