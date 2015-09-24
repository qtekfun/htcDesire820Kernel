// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PERFORMANCE_MONITOR_STARTUP_TIMER_H_
#define CHROME_BROWSER_PERFORMANCE_MONITOR_STARTUP_TIMER_H_

#include "base/time/time.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace performance_monitor {

class StartupTimer : public content::NotificationObserver {
 public:
  
  
  enum StartupType {
    STARTUP_NORMAL,
    STARTUP_TEST
  };

  StartupTimer();
  virtual ~StartupTimer();

  
  
  bool SignalStartupComplete(StartupType startup_type);

  
  
  
  
  static void PauseTimer();
  static void UnpauseTimer();

  
  
  
  
  
  
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  static void SetElapsedSessionRestoreTime(
      const base::TimeDelta& elapsed_session_restore_time);

 private:
  
  void InsertElapsedStartupTime();
  void InsertElapsedSessionRestoreTime();

  
  
  base::TimeTicks startup_begin_;

  
  
  base::TimeTicks pause_started_;

  
  base::TimeDelta total_pause_;

  
  
  
  StartupType startup_type_;

  
  base::TimeDelta elapsed_startup_time_;

  
  
  
  
  
  
  
  
  std::vector<base::TimeDelta> elapsed_session_restore_times_;

  
  bool performance_monitor_initialized_;

  content::NotificationRegistrar registrar_;

  
  static StartupTimer* g_startup_timer_;

  DISALLOW_COPY_AND_ASSIGN(StartupTimer);
};

}  

#endif  
