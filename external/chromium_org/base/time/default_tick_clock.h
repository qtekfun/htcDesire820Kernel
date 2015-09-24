// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEFAULT_TICK_CLOCK_H_
#define BASE_DEFAULT_TICK_CLOCK_H_

#include "base/base_export.h"
#include "base/compiler_specific.h"
#include "base/time/tick_clock.h"

namespace base {

class BASE_EXPORT DefaultTickClock : public TickClock {
 public:
  virtual ~DefaultTickClock();

  
  virtual TimeTicks NowTicks() OVERRIDE;
};

}  

#endif  
