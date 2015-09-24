// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SIMPLE_TEST_TICK_CLOCK_H_
#define BASE_SIMPLE_TEST_TICK_CLOCK_H_

#include "base/compiler_specific.h"
#include "base/synchronization/lock.h"
#include "base/time/tick_clock.h"
#include "base/time/time.h"

namespace base {

class SimpleTestTickClock : public TickClock {
 public:
  
  SimpleTestTickClock();
  virtual ~SimpleTestTickClock();

  virtual TimeTicks NowTicks() OVERRIDE;

  
  void Advance(TimeDelta delta);

 private:
  
  Lock lock_;

  TimeTicks now_ticks_;
};

}  

#endif  
