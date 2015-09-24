// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_WORKER_THREAD_TASK_RUNNER_H_
#define CONTENT_CHILD_WORKER_THREAD_TASK_RUNNER_H_

#include "base/task_runner.h"

namespace content {

class WorkerThreadTaskRunner : public base::TaskRunner {
 public:
  explicit WorkerThreadTaskRunner(int worker_thread_id);

  
  
  static scoped_refptr<WorkerThreadTaskRunner> current();

  
  virtual bool PostDelayedTask(const tracked_objects::Location& from_here,
                               const base::Closure& task,
                               base::TimeDelta delay) OVERRIDE;
  virtual bool RunsTasksOnCurrentThread() const OVERRIDE;

 protected:
  virtual ~WorkerThreadTaskRunner();

 private:
  const int worker_thread_id_;
};

}  

#endif  
