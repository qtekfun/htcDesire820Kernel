// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_BASE_RUNNING_AVERAGE_H_
#define REMOTING_BASE_RUNNING_AVERAGE_H_

#include <deque>

#include "base/basictypes.h"
#include "base/synchronization/lock.h"

namespace remoting {

class RunningAverage {
 public:
  
  explicit RunningAverage(int window_size);
  virtual ~RunningAverage();

  
  void Record(int64 value);

  
  double Average();

 private:
  
  
  const size_t window_size_;

  
  base::Lock lock_;

  
  std::deque<int64> data_points_;

  
  int64 sum_;

  DISALLOW_COPY_AND_ASSIGN(RunningAverage);
};

}  

#endif  
