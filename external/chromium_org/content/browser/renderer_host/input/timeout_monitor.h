// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_INPUT_TIMEOUT_MONITOR_H_
#define CONTENT_BROWSER_RENDERER_HOST_INPUT_TIMEOUT_MONITOR_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT TimeoutMonitor {
 public:
  typedef base::Closure TimeoutHandler;

  explicit TimeoutMonitor(const TimeoutHandler& timeout_handler);
  ~TimeoutMonitor();

  
  
  
  void Start(base::TimeDelta delay);

  void Restart(base::TimeDelta delay);
  void Stop();
  bool IsRunning() const;

 private:
  void CheckTimedOut();

  TimeoutHandler timeout_handler_;

  
  
  base::Time time_when_considered_timed_out_;

  
  base::OneShotTimer<TimeoutMonitor> timeout_timer_;

  DISALLOW_COPY_AND_ASSIGN(TimeoutMonitor);
};

}  

#endif  
