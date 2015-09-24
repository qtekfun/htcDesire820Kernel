// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_RATE_LIMITER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_RATE_LIMITER_H_

#include <queue>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/cancelable_callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "components/policy/policy_export.h"

namespace base {
class SequencedTaskRunner;
class TickClock;
}

namespace policy {

class POLICY_EXPORT RateLimiter : public base::NonThreadSafe {
 public:
  
  
  
  RateLimiter(size_t max_requests,
              const base::TimeDelta& duration,
              const base::Closure& callback,
              scoped_refptr<base::SequencedTaskRunner> task_runner,
              scoped_ptr<base::TickClock> clock);
  ~RateLimiter();

  
  
  
  
  void PostRequest();

 private:
  const size_t max_requests_;
  const base::TimeDelta duration_;
  base::Closure callback_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  scoped_ptr<base::TickClock> clock_;

  std::queue<base::TimeTicks> invocation_times_;
  base::CancelableClosure delayed_callback_;

  DISALLOW_COPY_AND_ASSIGN(RateLimiter);
};

}  

#endif  
