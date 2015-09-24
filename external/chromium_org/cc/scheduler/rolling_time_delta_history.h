// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_SCHEDULER_ROLLING_TIME_DELTA_HISTORY_H_
#define CC_SCHEDULER_ROLLING_TIME_DELTA_HISTORY_H_

#include <deque>
#include <set>

#include "base/time/time.h"
#include "cc/base/cc_export.h"

namespace cc {

class CC_EXPORT RollingTimeDeltaHistory {
 public:
  explicit RollingTimeDeltaHistory(size_t max_size);

  ~RollingTimeDeltaHistory();

  void InsertSample(base::TimeDelta time);

  void Clear();

  
  
  base::TimeDelta Percentile(double percent) const;

 private:
  typedef std::multiset<base::TimeDelta> TimeDeltaMultiset;

  TimeDeltaMultiset sample_set_;
  std::deque<TimeDeltaMultiset::iterator> chronological_sample_deque_;
  size_t max_size_;

  DISALLOW_COPY_AND_ASSIGN(RollingTimeDeltaHistory);
};

}  

#endif  
