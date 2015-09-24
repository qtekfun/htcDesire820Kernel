// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_BASE_RATE_COUNTER_H_
#define REMOTING_BASE_RATE_COUNTER_H_

#include <queue>
#include <utility>

#include "base/basictypes.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"

namespace remoting {

class RateCounter : public base::NonThreadSafe {
 public:
  
  explicit RateCounter(base::TimeDelta time_window);
  virtual ~RateCounter();

  
  void Record(int64 value);

  
  
  double Rate();

  
  void SetCurrentTimeForTest(base::Time current_time);

 private:
  
  typedef std::pair<base::Time, int64> DataPoint;

  
  void EvictOldDataPoints(base::Time current_time);

  
  base::Time CurrentTime() const;

  
  const base::TimeDelta time_window_;

  
  std::queue<DataPoint> data_points_;

  
  int64 sum_;

  
  base::Time current_time_for_test_;

  DISALLOW_COPY_AND_ASSIGN(RateCounter);
};

}  

#endif  
