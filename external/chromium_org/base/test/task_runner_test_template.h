// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

       

#ifndef BASE_TEST_TASK_RUNNER_TEST_TEMPLATE_H_
#define BASE_TEST_TASK_RUNNER_TEST_TEMPLATE_H_

#include <cstddef>
#include <map>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "base/task_runner.h"
#include "base/threading/thread.h"
#include "base/tracked_objects.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {

namespace internal {

class TaskTracker : public RefCountedThreadSafe<TaskTracker> {
 public:
  TaskTracker();

  
  
  
  Closure WrapTask(const Closure& task, int i);

  std::map<int, int> GetTaskRunCounts() const;

  
  void WaitForCompletedTasks(int count);

 private:
  friend class RefCountedThreadSafe<TaskTracker>;

  ~TaskTracker();

  void RunTask(const Closure& task, int i);

  mutable Lock lock_;
  std::map<int, int> task_run_counts_;
  int task_runs_;
  ConditionVariable task_runs_cv_;

  DISALLOW_COPY_AND_ASSIGN(TaskTracker);
};

}  

template <typename TaskRunnerTestDelegate>
class TaskRunnerTest : public testing::Test {
 protected:
  TaskRunnerTest() : task_tracker_(new internal::TaskTracker()) {}

  const scoped_refptr<internal::TaskTracker> task_tracker_;
  TaskRunnerTestDelegate delegate_;
};

TYPED_TEST_CASE_P(TaskRunnerTest);


TYPED_TEST_P(TaskRunnerTest, Basic) {
  std::map<int, int> expected_task_run_counts;

  this->delegate_.StartTaskRunner();
  scoped_refptr<TaskRunner> task_runner = this->delegate_.GetTaskRunner();
  
  for (int i = 0; i < 20; ++i) {
    const Closure& ith_task = this->task_tracker_->WrapTask(Closure(), i);
    for (int j = 0; j < i + 1; ++j) {
      task_runner->PostTask(FROM_HERE, ith_task);
      ++expected_task_run_counts[i];
    }
  }
  this->delegate_.StopTaskRunner();

  EXPECT_EQ(expected_task_run_counts,
            this->task_tracker_->GetTaskRunCounts());
}

TYPED_TEST_P(TaskRunnerTest, Delayed) {
  if (!this->delegate_.TaskRunnerHandlesNonZeroDelays()) {
    DLOG(INFO) << "This TaskRunner doesn't handle non-zero delays; skipping";
    return;
  }

  std::map<int, int> expected_task_run_counts;
  int expected_total_tasks = 0;

  this->delegate_.StartTaskRunner();
  scoped_refptr<TaskRunner> task_runner = this->delegate_.GetTaskRunner();
  
  for (int i = 0; i < 20; ++i) {
    const Closure& ith_task = this->task_tracker_->WrapTask(Closure(), i);
    for (int j = 0; j < i + 1; ++j) {
      task_runner->PostDelayedTask(
          FROM_HERE, ith_task, base::TimeDelta::FromMilliseconds(j));
      ++expected_task_run_counts[i];
      ++expected_total_tasks;
    }
  }
  this->task_tracker_->WaitForCompletedTasks(expected_total_tasks);
  this->delegate_.StopTaskRunner();

  EXPECT_EQ(expected_task_run_counts,
            this->task_tracker_->GetTaskRunCounts());
}

namespace internal {

void ExpectRunsTasksOnCurrentThread(
    bool expected_value,
    const scoped_refptr<TaskRunner>& task_runner);

}  

TYPED_TEST_P(TaskRunnerTest, RunsTasksOnCurrentThread) {
  std::map<int, int> expected_task_run_counts;

  Thread thread("Non-task-runner thread");
  ASSERT_TRUE(thread.Start());
  this->delegate_.StartTaskRunner();

  scoped_refptr<TaskRunner> task_runner = this->delegate_.GetTaskRunner();
  
  
  for (int i = 0; i < 20; ++i) {
    const Closure& ith_task_runner_task =
        this->task_tracker_->WrapTask(
            Bind(&internal::ExpectRunsTasksOnCurrentThread,
                 true, task_runner),
            i);
    const Closure& ith_non_task_runner_task =
        this->task_tracker_->WrapTask(
            Bind(&internal::ExpectRunsTasksOnCurrentThread,
                 false, task_runner),
            i);
    for (int j = 0; j < i + 1; ++j) {
      task_runner->PostTask(FROM_HERE, ith_task_runner_task);
      thread.message_loop()->PostTask(FROM_HERE, ith_non_task_runner_task);
      expected_task_run_counts[i] += 2;
    }
  }

  this->delegate_.StopTaskRunner();
  thread.Stop();

  EXPECT_EQ(expected_task_run_counts,
            this->task_tracker_->GetTaskRunCounts());
}

REGISTER_TYPED_TEST_CASE_P(
    TaskRunnerTest, Basic, Delayed, RunsTasksOnCurrentThread);

}  

#endif  
