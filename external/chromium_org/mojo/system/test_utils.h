// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_TEST_UTILS_H_
#define MOJO_SYSTEM_TEST_UTILS_H_

#include <stdint.h>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/task_runner.h"
#include "base/time/time.h"

namespace tracked_objects {
class Location;
}

namespace mojo {
namespace system {
namespace test {

class Stopwatch {
 public:
  Stopwatch() {}
  ~Stopwatch() {}

  void Start() {
    start_time_ = base::TimeTicks::HighResNow();
  }

  int64_t Elapsed() {
    return (base::TimeTicks::HighResNow() - start_time_).InMicroseconds();
  }

 private:
  base::TimeTicks start_time_;

  DISALLOW_COPY_AND_ASSIGN(Stopwatch);
};

void PostTaskAndWait(scoped_refptr<base::TaskRunner> task_runner,
                     const tracked_objects::Location& from_here,
                     const base::Closure& task);

}  
}  
}  

#endif  
