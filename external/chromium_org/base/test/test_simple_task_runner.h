// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_TEST_SIMPLE_TASK_RUNNER_H_
#define BASE_TEST_TEST_SIMPLE_TASK_RUNNER_H_

#include <deque>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/single_thread_task_runner.h"
#include "base/test/test_pending_task.h"
#include "base/threading/thread_checker.h"

namespace base {

class TimeDelta;

class TestSimpleTaskRunner : public SingleThreadTaskRunner {
 public:
  TestSimpleTaskRunner();

  
  virtual bool PostDelayedTask(const tracked_objects::Location& from_here,
                               const Closure& task,
                               TimeDelta delay) OVERRIDE;
  virtual bool PostNonNestableDelayedTask(
      const tracked_objects::Location& from_here,
      const Closure& task,
      TimeDelta delay) OVERRIDE;

  virtual bool RunsTasksOnCurrentThread() const OVERRIDE;

  const std::deque<TestPendingTask>& GetPendingTasks() const;
  bool HasPendingTask() const;
  base::TimeDelta NextPendingTaskDelay() const;

  
  void ClearPendingTasks();

  
  
  void RunPendingTasks();

  
  void RunUntilIdle();

 protected:
  virtual ~TestSimpleTaskRunner();

 private:
  ThreadChecker thread_checker_;
  std::deque<TestPendingTask> pending_tasks_;

  DISALLOW_COPY_AND_ASSIGN(TestSimpleTaskRunner);
};

}  

#endif  
