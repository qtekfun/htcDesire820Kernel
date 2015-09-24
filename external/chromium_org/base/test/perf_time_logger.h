// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_PERF_TIME_LOGGER_H_
#define BASE_TEST_PERF_TIME_LOGGER_H_

#include <string>

#include "base/basictypes.h"
#include "base/timer/elapsed_timer.h"

namespace base {

class PerfTimeLogger {
 public:
  explicit PerfTimeLogger(const char* test_name);
  ~PerfTimeLogger();

  void Done();

 private:
  bool logged_;
  std::string test_name_;
  ElapsedTimer timer_;

  DISALLOW_COPY_AND_ASSIGN(PerfTimeLogger);
};

}  

#endif  
