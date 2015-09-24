// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef BASE_THREADING_WATCHDOG_H_
#define BASE_THREADING_WATCHDOG_H_

#include <string>

#include "base/base_export.h"
#include "base/compiler_specific.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "base/threading/platform_thread.h"
#include "base/time/time.h"

namespace base {

class BASE_EXPORT Watchdog {
 public:
  
  Watchdog(const TimeDelta& duration,
           const std::string& thread_watched_name,
           bool enabled);
  virtual ~Watchdog();

  
  void Cleanup();

  
  
  bool IsJoinable();

  
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
    virtual void ThreadMain() OVERRIDE;
   private:
    void SetThreadName() const;

    Watchdog* watchdog_;
  };

  enum State {ARMED, DISARMED, SHUTDOWN, JOINABLE };

  bool enabled_;

  Lock lock_;  
  ConditionVariable condition_variable_;
  State state_;
  const TimeDelta duration_;  
  const std::string thread_watched_name_;
  PlatformThreadHandle handle_;
  ThreadDelegate delegate_;  

  TimeTicks start_time_;  

  DISALLOW_COPY_AND_ASSIGN(Watchdog);
};

}  

#endif  
