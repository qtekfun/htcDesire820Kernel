// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TEST_LAP_TIMER_H_
#define CC_TEST_LAP_TIMER_H_

#include "base/time/time.h"

namespace cc {

class LapTimer {
 public:
  LapTimer(int warmup_laps, base::TimeDelta time_limit, int check_interval);
  
  void Reset();
  
  void Start();
  
  bool IsWarmedUp();
  
  
  
  void NextLap();
  
  
  bool HasTimeLimitExpired();
  
  float MsPerLap();
  
  float LapsPerSecond();
  
  int NumLaps();

 private:
  base::TimeTicks start_time_;
  base::TimeDelta accumulator_;
  int num_laps_;
  int warmup_laps_;
  int remaining_warmups_;
  base::TimeDelta time_limit_;
  int check_interval_;
  bool accumulated_;

  DISALLOW_COPY_AND_ASSIGN(LapTimer);
};

}  

#endif  
