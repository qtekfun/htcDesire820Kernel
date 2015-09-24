// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_LIBDISPATCH_SEQUENCED_TASK_RUNNER_H_
#define BASE_MAC_LIBDISPATCH_SEQUENCED_TASK_RUNNER_H_

#include <dispatch/dispatch.h>

#include "base/single_thread_task_runner.h"
#include "base/synchronization/waitable_event.h"

namespace base {
namespace mac {

class BASE_EXPORT LibDispatchTaskRunner : public base::SingleThreadTaskRunner {
 public:
  
  explicit LibDispatchTaskRunner(const char* name);

  
  virtual bool PostDelayedTask(const tracked_objects::Location& from_here,
                               const Closure& task,
                               base::TimeDelta delay) OVERRIDE;
  virtual bool RunsTasksOnCurrentThread() const OVERRIDE;

  
  virtual bool PostNonNestableDelayedTask(
      const tracked_objects::Location& from_here,
      const Closure& task,
      base::TimeDelta delay) OVERRIDE;

  
  
  
  
  void Shutdown();

  
  
  
  
  
  
  dispatch_queue_t GetDispatchQueue() const;

 protected:
  virtual ~LibDispatchTaskRunner();

 private:
  static void Finalizer(void* context);

  dispatch_queue_t queue_;

  
  base::WaitableEvent queue_finalized_;
};

}  
}  

#endif  
