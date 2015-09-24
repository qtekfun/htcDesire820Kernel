// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_WORKER_POOL_POSIX_H_
#define BASE_THREADING_WORKER_POOL_POSIX_H_

#include <queue>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/location.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/pending_task.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "base/threading/platform_thread.h"
#include "base/tracked_objects.h"

class Task;

namespace base {

class BASE_EXPORT PosixDynamicThreadPool
    : public RefCountedThreadSafe<PosixDynamicThreadPool> {
 public:
  class PosixDynamicThreadPoolPeer;

  
  
  PosixDynamicThreadPool(const std::string& name_prefix,
                         int idle_seconds_before_exit);

  
  
  void Terminate();

  
  void PostTask(const tracked_objects::Location& from_here,
                const Closure& task);

  
  
  PendingTask WaitForTask();

 private:
  friend class RefCountedThreadSafe<PosixDynamicThreadPool>;
  friend class PosixDynamicThreadPoolPeer;

  ~PosixDynamicThreadPool();

  
  
  void AddTask(PendingTask* pending_task);

  const std::string name_prefix_;
  const int idle_seconds_before_exit_;

  Lock lock_;  

  
  
  
  ConditionVariable pending_tasks_available_cv_;
  int num_idle_threads_;
  TaskQueue pending_tasks_;
  bool terminated_;
  
  
  scoped_ptr<ConditionVariable> num_idle_threads_cv_;

  DISALLOW_COPY_AND_ASSIGN(PosixDynamicThreadPool);
};

}  

#endif  
