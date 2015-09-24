// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_METRICS_REPORTING_SCHEDULER_H_
#define CHROME_BROWSER_METRICS_METRICS_REPORTING_SCHEDULER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"

class MetricsReportingScheduler {
 public:
  explicit MetricsReportingScheduler(const base::Closure& upload_callback);
  ~MetricsReportingScheduler();

  
  
  void Start();

  
  void Stop();

  
  void InitTaskComplete();

  
  void UploadFinished(bool server_is_healthy, bool more_logs_remaining);

  
  
  void UploadCancelled();

  
  void SetUploadIntervalForTesting(base::TimeDelta interval);

 private:
  
  
  void TriggerUpload();

  
  void ScheduleNextUpload();

  
  
  void BackOffUploadInterval();

  
  const base::Closure upload_callback_;

  base::OneShotTimer<MetricsReportingScheduler> upload_timer_;

  
  
  base::TimeDelta upload_interval_;

  
  
  bool running_;

  
  bool callback_pending_;

  
  bool init_task_complete_;

  
  
  bool waiting_for_init_task_complete_;

  DISALLOW_COPY_AND_ASSIGN(MetricsReportingScheduler);
};

#endif  
