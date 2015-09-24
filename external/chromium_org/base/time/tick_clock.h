// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TICK_CLOCK_H_
#define BASE_TICK_CLOCK_H_

#include "base/base_export.h"
#include "base/time/time.h"

namespace base {

class BASE_EXPORT TickClock {
 public:
  virtual ~TickClock();

  
  
  
  
  virtual TimeTicks NowTicks() = 0;
};

}  

#endif  
