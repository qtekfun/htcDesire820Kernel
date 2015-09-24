// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SIMPLE_TEST_CLOCK_H_
#define BASE_SIMPLE_TEST_CLOCK_H_

#include "base/compiler_specific.h"
#include "base/synchronization/lock.h"
#include "base/time/clock.h"
#include "base/time/time.h"

namespace base {

class SimpleTestClock : public Clock {
 public:
  
  SimpleTestClock();
  virtual ~SimpleTestClock();

  virtual Time Now() OVERRIDE;

  
  void Advance(TimeDelta delta);

  
  void SetNow(Time now);

 private:
  
  Lock lock_;

  Time now_;
};

}  

#endif  
