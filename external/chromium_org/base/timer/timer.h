// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_TIMER_TIMER_H_
#define BASE_TIMER_TIMER_H_


#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/callback.h"
#include "base/location.h"
#include "base/time/time.h"

namespace base {

class BaseTimerTaskInternal;

class BASE_EXPORT Timer {
 public:
  
  
  
  Timer(bool retain_user_task, bool is_repeating);

  
  Timer(const tracked_objects::Location& posted_from,
        TimeDelta delay,
        const base::Closure& user_task,
        bool is_repeating);

  virtual ~Timer();

  
  bool IsRunning() const {
    return is_running_;
  }

  
  TimeDelta GetCurrentDelay() const {
    return delay_;
  }

  
  
  void Start(const tracked_objects::Location& posted_from,
             TimeDelta delay,
             const base::Closure& user_task);

  
  
  void Stop();

  
  
  void Reset();

  const base::Closure& user_task() const { return user_task_; }
  const TimeTicks& desired_run_time() const { return desired_run_time_; }

 protected:
  
  
  void SetTaskInfo(const tracked_objects::Location& posted_from,
                   TimeDelta delay,
                   const base::Closure& user_task);

 private:
  friend class BaseTimerTaskInternal;

  
  
  
  void PostNewScheduledTask(TimeDelta delay);

  
  
  void AbandonScheduledTask();

  
  void RunScheduledTask();

  
  void StopAndAbandon() {
    Stop();
    AbandonScheduledTask();
  }

  
  
  BaseTimerTaskInternal* scheduled_task_;

  
  tracked_objects::Location posted_from_;
  
  TimeDelta delay_;
  
  base::Closure user_task_;

  
  
  
  TimeTicks scheduled_run_time_;

  
  
  
  
  
  
  
  TimeTicks desired_run_time_;

  
  int thread_id_;

  
  
  const bool is_repeating_;

  
  const bool retain_user_task_;

  
  bool is_running_;

  DISALLOW_COPY_AND_ASSIGN(Timer);
};

template <class Receiver, bool kIsRepeating>
class BaseTimerMethodPointer : public Timer {
 public:
  typedef void (Receiver::*ReceiverMethod)();

  
  
  
  
  
  using Timer::Start;

  BaseTimerMethodPointer() : Timer(kIsRepeating, kIsRepeating) {}

  
  
  
  void Start(const tracked_objects::Location& posted_from,
             TimeDelta delay,
             Receiver* receiver,
             ReceiverMethod method) {
    Timer::Start(posted_from, delay,
                 base::Bind(method, base::Unretained(receiver)));
  }
};

template <class Receiver>
class OneShotTimer : public BaseTimerMethodPointer<Receiver, false> {};

template <class Receiver>
class RepeatingTimer : public BaseTimerMethodPointer<Receiver, true> {};

template <class Receiver>
class DelayTimer : protected Timer {
 public:
  typedef void (Receiver::*ReceiverMethod)();

  DelayTimer(const tracked_objects::Location& posted_from,
             TimeDelta delay,
             Receiver* receiver,
             ReceiverMethod method)
      : Timer(posted_from, delay,
              base::Bind(method, base::Unretained(receiver)),
              false) {}

  void Reset() { Timer::Reset(); }
};

}  

#endif  
