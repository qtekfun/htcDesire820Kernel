// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_STARTUP_METRIC_UTILS_STARTUP_METRIC_UTILS_H_
#define COMPONENTS_STARTUP_METRIC_UTILS_STARTUP_METRIC_UTILS_H_

#include <string>

#include "base/time/time.h"


namespace startup_metric_utils {

bool WasNonBrowserUIDisplayed();

void SetNonBrowserUIDisplayed();

void RecordMainEntryPointTime();

void RecordExeMainEntryTime();

#if defined(OS_ANDROID)
void RecordSavedMainEntryPointTime(const base::Time& entry_point_time);
#endif 

void OnBrowserStartupComplete(bool is_first_run);

void OnInitialPageLoadComplete();

class ScopedSlowStartupUMA {
 public:
  explicit ScopedSlowStartupUMA(const std::string& histogram_name)
      : start_time_(base::TimeTicks::Now()),
        histogram_name_(histogram_name) {}

  ~ScopedSlowStartupUMA();

 private:
  const base::TimeTicks start_time_;
  const std::string histogram_name_;

  DISALLOW_COPY_AND_ASSIGN(ScopedSlowStartupUMA);
};

}  

#endif  
