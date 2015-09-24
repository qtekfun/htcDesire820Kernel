// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEBUG_TRACE_EVENT_SYSTEM_STATS_MONITOR_H_
#define BASE_DEBUG_TRACE_EVENT_SYSTEM_STATS_MONITOR_H_

#include "base/base_export.h"
#include "base/debug/trace_event_impl.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/process/process_metrics.h"
#include "base/timer/timer.h"

namespace base {

class SingleThreadTaskRunner;

namespace debug {

class BASE_EXPORT TraceEventSystemStatsMonitor
    : public TraceLog::EnabledStateObserver {
 public:
  
  static const int kSamplingIntervalMilliseconds = 2000;

  
  
  explicit TraceEventSystemStatsMonitor(
      scoped_refptr<SingleThreadTaskRunner> task_runner);

  virtual ~TraceEventSystemStatsMonitor();

  
  virtual void OnTraceLogEnabled() OVERRIDE;
  virtual void OnTraceLogDisabled() OVERRIDE;

  
  void DumpSystemStats();

 private:
  FRIEND_TEST_ALL_PREFIXES(TraceSystemStatsMonitorTest,
                           TraceEventSystemStatsMonitor);

  bool IsTimerRunningForTest() const;

  void StartProfiling();

  void StopProfiling();

  
  scoped_refptr<SingleThreadTaskRunner> task_runner_;

  
  RepeatingTimer<TraceEventSystemStatsMonitor> dump_timer_;

  WeakPtrFactory<TraceEventSystemStatsMonitor> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(TraceEventSystemStatsMonitor);
};

BASE_EXPORT void AppendSystemProfileAsTraceFormat(const SystemMetrics&
                                                  system_stats,
                                                  std::string* output);

}  
}  

#endif  
