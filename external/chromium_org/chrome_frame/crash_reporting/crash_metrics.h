// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_FRAME_CRASH_REPORTING_CRASH_METRICS_H_
#define CHROME_FRAME_CRASH_REPORTING_CRASH_METRICS_H_

#include "base/basictypes.h"
#include "base/lazy_instance.h"
#include "base/threading/thread_local.h"

class CrashMetricsReporter {
 public:
  enum Metric {
    NAVIGATION_COUNT,
    CRASH_COUNT,
    CHROME_FRAME_NAVIGATION_COUNT,
    SESSION_ID,
    CHANNEL_ERROR_COUNT,
    LAST_METRIC,
  };
  
  static CrashMetricsReporter* GetInstance();

  
  
  
  
  int GetMetric(Metric metric);
  bool SetMetric(Metric metric, int value);
  int IncrementMetric(Metric metric);

  
  void RecordCrashMetrics();

  bool active() const {
    return active_;
  }

  void set_active(bool active) {
    active_ = active;
  }

 private:
  friend struct base::DefaultLazyInstanceTraits<CrashMetricsReporter>;

  CrashMetricsReporter()
      : active_(false) {}
  virtual ~CrashMetricsReporter() {}

  
  bool active_;

  static wchar_t* g_metric_names[LAST_METRIC];

  DISALLOW_COPY_AND_ASSIGN(CrashMetricsReporter);
};

#endif  
