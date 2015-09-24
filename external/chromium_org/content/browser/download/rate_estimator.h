// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOWNLOAD_RATE_ESTIMATOR_H_
#define CONTENT_BROWSER_DOWNLOAD_RATE_ESTIMATOR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT RateEstimator {
 public:
  RateEstimator();
  RateEstimator(base::TimeDelta bucket_time,
                size_t num_buckets,
                base::TimeTicks now);
  ~RateEstimator();

  
  
  void Increment(uint32 count);
  void Increment(uint32 count, base::TimeTicks now);

  
  
  uint64 GetCountPerSecond() const;
  uint64 GetCountPerSecond(base::TimeTicks now) const;

 private:
  void ClearOldBuckets(base::TimeTicks now);
  void ResetBuckets(base::TimeTicks now);

  std::vector<uint32> history_;
  base::TimeDelta bucket_time_;
  size_t oldest_index_;
  size_t bucket_count_;
  base::TimeTicks oldest_time_;
};

}  

#endif  
