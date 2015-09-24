// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef BASE_THREADING_WATCHDOG_H_
#define BASE_THREADING_WATCHDOG_H_
#pragma once

#include <string>

#include "base/base_api.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "base/threading/platform_thread.h"
#include "base/time.h"

namespace base {

class BASE_API Watchdog {
 public:
  
  Watchdog(const TimeDelta& duration,
           const std::string& thread_watched_name,
           bool enabled);
  virtual ~Watchdog();

  
  void Arm();  
  void ArmSomeTimeDeltaAgo(const TimeDelta& time_delta);
  void ArmAtStartTime(const TimeTicks start_time);

  
  void Disarm();

  
  
  virtual void Alarm();

  
  
  static void ResetStaticData();

 private:
  class ThreadDelegate : public PlatformThread::Delegate {
   public:
    explicit ThreadDelegate(Watchdog* watchdog) : watchdog_(watchdog) {
    }
    virtual void ThreadMain();
   private:
    void SetThreadName() const;

    Watchdog* watchdog_;
  };

  enum State {ARMED, DISARMED, SHUTDOWN };

  bool init_successful_;

  Lock lock_;  
  ConditionVariable condition_variable_;
  State state_;
  const TimeDelta duration_;  
  const std::string thread_watched_name_;
  PlatformThreadHandle handle_;
  ThreadDelegate delegate_;  

  TimeTicks start_time_;  

  
  
  
  
  
  
  static Lock static_lock_;  
  
  static TimeTicks last_debugged_alarm_time_;
  
  static TimeDelta last_debugged_alarm_delay_;

  DISALLOW_COPY_AND_ASSIGN(Watchdog);
};

}  

#endif  
