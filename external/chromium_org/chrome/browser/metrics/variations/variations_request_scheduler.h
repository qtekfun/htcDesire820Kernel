// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_VARIATIONS_VARIATIONS_REQUEST_SCHEDULER_H_
#define CHROME_BROWSER_METRICS_VARIATIONS_VARIATIONS_REQUEST_SCHEDULER_H_

#include "base/bind.h"
#include "base/gtest_prod_util.h"
#include "base/time/time.h"
#include "base/timer/timer.h"

class PrefService;

namespace chrome_variations {

class VariationsRequestScheduler {
 public:
  virtual ~VariationsRequestScheduler();

  
  virtual void Start();

  
  virtual void Reset();

  
  
  void ScheduleFetchShortly();

  
  static VariationsRequestScheduler* Create(const base::Closure& task,
                                            PrefService* local_state);

 protected:
  
  explicit VariationsRequestScheduler(const base::Closure& task);

  
  base::Closure task() const;

 private:
  FRIEND_TEST_ALL_PREFIXES(VariationsRequestSchedulerTest,
                           ScheduleFetchShortly);

  
  base::Closure task_;

  
  
  
  base::RepeatingTimer<VariationsRequestScheduler> timer_;

  
  base::OneShotTimer<VariationsRequestScheduler> one_shot_timer_;

  DISALLOW_COPY_AND_ASSIGN(VariationsRequestScheduler);
};

}  

#endif  
