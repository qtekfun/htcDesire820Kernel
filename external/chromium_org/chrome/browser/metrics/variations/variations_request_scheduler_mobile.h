// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_VARIATIONS_VARIATIONS_REQUEST_SCHEDULER_MOBILE_H_
#define CHROME_BROWSER_METRICS_VARIATIONS_VARIATIONS_REQUEST_SCHEDULER_MOBILE_H_

#include "base/bind.h"
#include "chrome/browser/metrics/variations/variations_request_scheduler.h"

class PrefService;

namespace chrome_variations {

class VariationsRequestSchedulerMobile : public VariationsRequestScheduler {
 public:
  
  
  explicit VariationsRequestSchedulerMobile(const base::Closure& task,
                                            PrefService* local_state);
  virtual ~VariationsRequestSchedulerMobile();

  
  virtual void Start() OVERRIDE;
  virtual void Reset() OVERRIDE;

 private:
  
  PrefService* local_state_;

  DISALLOW_COPY_AND_ASSIGN(VariationsRequestSchedulerMobile);
};

}  

#endif  
