// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_GPU_GPU_WATCHDOG_THREAD_H_
#define CONTENT_GPU_GPU_WATCHDOG_THREAD_H_

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/power_monitor/power_observer.h"
#include "base/threading/thread.h"
#include "base/time/time.h"
#include "content/common/gpu/gpu_watchdog.h"

namespace content {

class GpuWatchdogThread : public base::Thread,
                          public GpuWatchdog,
                          public base::PowerObserver,
                          public base::RefCountedThreadSafe<GpuWatchdogThread> {
 public:
  explicit GpuWatchdogThread(int timeout);

  
  bool armed() const { return armed_; }
  void PostAcknowledge();

  
  virtual void CheckArmed() OVERRIDE;

  
  
  void AddPowerObserver();

 protected:
  virtual void Init() OVERRIDE;
  virtual void CleanUp() OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<GpuWatchdogThread>;

  
  
  class GpuWatchdogTaskObserver : public base::MessageLoop::TaskObserver {
   public:
    explicit GpuWatchdogTaskObserver(GpuWatchdogThread* watchdog);
    virtual ~GpuWatchdogTaskObserver();

    
    virtual void WillProcessTask(
        const base::PendingTask& pending_task) OVERRIDE;
    virtual void DidProcessTask(const base::PendingTask& pending_task) OVERRIDE;

   private:
    GpuWatchdogThread* watchdog_;
  };

  virtual ~GpuWatchdogThread();

  void OnAcknowledge();
  void OnCheck(bool after_suspend);
  void DeliberatelyTerminateToRecoverFromHang();

  void OnAddPowerObserver();

  
  virtual void OnSuspend() OVERRIDE;
  virtual void OnResume() OVERRIDE;

#if defined(OS_WIN)
  base::TimeDelta GetWatchedThreadTime();
#endif

  base::MessageLoop* watched_message_loop_;
  base::TimeDelta timeout_;
  volatile bool armed_;
  GpuWatchdogTaskObserver task_observer_;

#if defined(OS_WIN)
  void* watched_thread_handle_;
  base::TimeDelta arm_cpu_time_;
#endif

  
  
  base::Time suspension_timeout_;

  base::WeakPtrFactory<GpuWatchdogThread> weak_factory_;

  bool suspended_;

#if defined(OS_CHROMEOS)
  FILE* tty_file_;
#endif

  DISALLOW_COPY_AND_ASSIGN(GpuWatchdogThread);
};

}  

#endif  
