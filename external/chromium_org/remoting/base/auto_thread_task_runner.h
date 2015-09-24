// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_BASE_AUTO_THREAD_TASK_RUNNER_H_
#define REMOTING_BASE_AUTO_THREAD_TASK_RUNNER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/single_thread_task_runner.h"

namespace remoting {

class AutoThreadTaskRunner : public base::SingleThreadTaskRunner {
 public:
  
  
  
  AutoThreadTaskRunner(scoped_refptr<base::SingleThreadTaskRunner> task_runner,
                       const base::Closure& stop_task);

  
  virtual bool PostDelayedTask(
      const tracked_objects::Location& from_here,
      const base::Closure& task,
      base::TimeDelta delay) OVERRIDE;
  virtual bool PostNonNestableDelayedTask(
      const tracked_objects::Location& from_here,
      const base::Closure& task,
      base::TimeDelta delay) OVERRIDE;
  virtual bool RunsTasksOnCurrentThread() const OVERRIDE;

 private:
  virtual ~AutoThreadTaskRunner();

  
  base::Closure stop_task_;

  
  scoped_refptr<base::SingleThreadTaskRunner> task_runner_;

  DISALLOW_COPY_AND_ASSIGN(AutoThreadTaskRunner);
};

}  

#endif  
