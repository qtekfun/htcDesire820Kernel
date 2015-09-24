// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_WORKER_POOL_POSIX_H_
#define BASE_THREADING_WORKER_POOL_POSIX_H_
#pragma once

#include <queue>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "base/threading/platform_thread.h"

class Task;

namespace base {

class PosixDynamicThreadPool
    : public RefCountedThreadSafe<PosixDynamicThreadPool> {
 public:
  class PosixDynamicThreadPoolPeer;

  
  
  PosixDynamicThreadPool(const std::string& name_prefix,
                         int idle_seconds_before_exit);
  ~PosixDynamicThreadPool();

  
  
  void Terminate();

  
  
  void PostTask(Task* task);

  
  
  Task* WaitForTask();

 private:
  friend class PosixDynamicThreadPoolPeer;

  const std::string name_prefix_;
  const int idle_seconds_before_exit_;

  Lock lock_;  

  
  
  
  ConditionVariable tasks_available_cv_;
  int num_idle_threads_;
  std::queue<Task*> tasks_;
  bool terminated_;
  
  
  scoped_ptr<ConditionVariable> num_idle_threads_cv_;

  DISALLOW_COPY_AND_ASSIGN(PosixDynamicThreadPool);
};

}  

#endif  
