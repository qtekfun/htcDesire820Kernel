// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_BASE_PLUGIN_THREAD_TASK_RUNNER_H_
#define REMOTING_BASE_PLUGIN_THREAD_TASK_RUNNER_H_

#include <set>

#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/pending_task.h"
#include "base/single_thread_task_runner.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/platform_thread.h"
#include "base/time/time.h"

namespace remoting {

class PluginThreadTaskRunner : public base::SingleThreadTaskRunner {
 public:
  class Delegate {
   public:
    virtual ~Delegate();

    virtual bool RunOnPluginThread(
        base::TimeDelta delay, void(function)(void*), void* data) = 0;
  };

  
  PluginThreadTaskRunner(Delegate* delegate);

  
  
  
  
  
  void DetachAndRunShutdownLoop();

  
  
  
  
  void Quit();

  
  virtual bool PostDelayedTask(
      const tracked_objects::Location& from_here,
      const base::Closure& task,
      base::TimeDelta delay) OVERRIDE;
  virtual bool PostNonNestableDelayedTask(
      const tracked_objects::Location& from_here,
      const base::Closure& task,
      base::TimeDelta delay) OVERRIDE;
  virtual bool RunsTasksOnCurrentThread() const OVERRIDE;

 protected:
  virtual ~PluginThreadTaskRunner();

 private:
  

  
  void PostRunTasks();

  

  
  
  void PostDelayedRunTasks(base::TimeTicks when);

  
  
  void ProcessIncomingTasks();

  
  void RunDelayedTasks(base::TimeTicks when);

  
  void RunDueTasks(base::TimeTicks now);

  
  void RunTasks();

  static void TaskSpringboard(void* data);

  const base::PlatformThreadId plugin_thread_id_;

  
  
  base::WaitableEvent event_;

  base::Lock lock_;

  

  
  
  Delegate* delegate_;

  
  base::TaskQueue incoming_queue_;

  
  int next_sequence_num_;

  
  bool quit_received_;

  

  
  base::DelayedTaskQueue delayed_queue_;

  
  std::set<base::TimeTicks> scheduled_timers_;

  
  bool stopped_;

  DISALLOW_COPY_AND_ASSIGN(PluginThreadTaskRunner);
};

}  

#endif  
