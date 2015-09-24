// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_SEQUENCED_TASK_RUNNER_TEST_TEMPLATE_H_
#define BASE_SEQUENCED_TASK_RUNNER_TEST_TEMPLATE_H_

#include <cstddef>
#include <iosfwd>
#include <vector>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/sequenced_task_runner.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {

namespace internal {

struct TaskEvent {
  enum Type { POST, START, END };
  TaskEvent(int i, Type type);
  int i;
  Type type;
};

class SequencedTaskTracker : public RefCountedThreadSafe<SequencedTaskTracker> {
 public:
  SequencedTaskTracker();

  
  void PostWrappedNonNestableTask(
      const scoped_refptr<SequencedTaskRunner>& task_runner,
      const Closure& task);

  
  void PostWrappedNestableTask(
      const scoped_refptr<SequencedTaskRunner>& task_runner,
      const Closure& task);

  
  void PostWrappedDelayedNonNestableTask(
      const scoped_refptr<SequencedTaskRunner>& task_runner,
      const Closure& task,
      TimeDelta delay);

  
  void PostNonNestableTasks(
      const scoped_refptr<SequencedTaskRunner>& task_runner,
      int task_count);

  const std::vector<TaskEvent>& GetTaskEvents() const;

  
  void WaitForCompletedTasks(int count);

 private:
  friend class RefCountedThreadSafe<SequencedTaskTracker>;

  ~SequencedTaskTracker();

  
  void RunTask(const Closure& task, int task_i);

  
  
  void TaskPosted(int i);

  
  void TaskStarted(int i);

  
  void TaskEnded(int i);

  
  Lock lock_;

  
  std::vector<TaskEvent> events_;

  
  
  int next_post_i_;

  
  int task_end_count_;
  ConditionVariable task_end_cv_;

  DISALLOW_COPY_AND_ASSIGN(SequencedTaskTracker);
};

void PrintTo(const TaskEvent& event, std::ostream* os);

::testing::AssertionResult CheckNonNestableInvariants(
    const std::vector<TaskEvent>& events,
    int task_count);

}  

template <typename TaskRunnerTestDelegate>
class SequencedTaskRunnerTest : public testing::Test {
 protected:
  SequencedTaskRunnerTest()
      : task_tracker_(new internal::SequencedTaskTracker()) {}

  const scoped_refptr<internal::SequencedTaskTracker> task_tracker_;
  TaskRunnerTestDelegate delegate_;
};

TYPED_TEST_CASE_P(SequencedTaskRunnerTest);

TYPED_TEST_P(SequencedTaskRunnerTest, SequentialNonNestable) {
  const int kTaskCount = 1000;

  this->delegate_.StartTaskRunner();
  const scoped_refptr<SequencedTaskRunner> task_runner =
      this->delegate_.GetTaskRunner();

  this->task_tracker_->PostWrappedNonNestableTask(
      task_runner, Bind(&PlatformThread::Sleep, TimeDelta::FromSeconds(1)));
  for (int i = 1; i < kTaskCount; ++i) {
    this->task_tracker_->PostWrappedNonNestableTask(task_runner, Closure());
  }

  this->delegate_.StopTaskRunner();

  EXPECT_TRUE(CheckNonNestableInvariants(this->task_tracker_->GetTaskEvents(),
                                         kTaskCount));
}

TYPED_TEST_P(SequencedTaskRunnerTest, SequentialNestable) {
  const int kTaskCount = 1000;

  this->delegate_.StartTaskRunner();
  const scoped_refptr<SequencedTaskRunner> task_runner =
      this->delegate_.GetTaskRunner();

  this->task_tracker_->PostWrappedNestableTask(
      task_runner,
      Bind(&PlatformThread::Sleep, TimeDelta::FromSeconds(1)));
  for (int i = 1; i < kTaskCount; ++i) {
    this->task_tracker_->PostWrappedNestableTask(task_runner, Closure());
  }

  this->delegate_.StopTaskRunner();

  EXPECT_TRUE(CheckNonNestableInvariants(this->task_tracker_->GetTaskEvents(),
                                         kTaskCount));
}

TYPED_TEST_P(SequencedTaskRunnerTest, SequentialDelayedNonNestable) {
  
  if (!this->delegate_.TaskRunnerHandlesNonZeroDelays()) {
    DLOG(INFO) << "This SequencedTaskRunner doesn't handle "
        "non-zero delays; skipping";
    return;
  }

  const int kTaskCount = 20;
  const int kDelayIncrementMs = 50;

  this->delegate_.StartTaskRunner();
  const scoped_refptr<SequencedTaskRunner> task_runner =
      this->delegate_.GetTaskRunner();

  for (int i = 0; i < kTaskCount; ++i) {
    this->task_tracker_->PostWrappedDelayedNonNestableTask(
        task_runner,
        Closure(),
        TimeDelta::FromMilliseconds(kDelayIncrementMs * i));
  }

  this->task_tracker_->WaitForCompletedTasks(kTaskCount);
  this->delegate_.StopTaskRunner();

  EXPECT_TRUE(CheckNonNestableInvariants(this->task_tracker_->GetTaskEvents(),
                                         kTaskCount));
}

TYPED_TEST_P(SequencedTaskRunnerTest, NonNestablePostFromNonNestableTask) {
  const int kParentCount = 10;
  const int kChildrenPerParent = 10;

  this->delegate_.StartTaskRunner();
  const scoped_refptr<SequencedTaskRunner> task_runner =
      this->delegate_.GetTaskRunner();

  for (int i = 0; i < kParentCount; ++i) {
    Closure task = Bind(
        &internal::SequencedTaskTracker::PostNonNestableTasks,
        this->task_tracker_,
        task_runner,
        kChildrenPerParent);
    this->task_tracker_->PostWrappedNonNestableTask(task_runner, task);
  }

  this->delegate_.StopTaskRunner();

  EXPECT_TRUE(CheckNonNestableInvariants(
      this->task_tracker_->GetTaskEvents(),
      kParentCount * (kChildrenPerParent + 1)));
}

TYPED_TEST_P(SequencedTaskRunnerTest, DelayedTaskBasic) {
  
  if (!this->delegate_.TaskRunnerHandlesNonZeroDelays()) {
    DLOG(INFO) << "This SequencedTaskRunner doesn't handle "
        "non-zero delays; skipping";
    return;
  }

  const int kTaskCount = 1;
  const TimeDelta kDelay = TimeDelta::FromMilliseconds(100);

  this->delegate_.StartTaskRunner();
  const scoped_refptr<SequencedTaskRunner> task_runner =
      this->delegate_.GetTaskRunner();

  Time time_before_run = Time::Now();
  this->task_tracker_->PostWrappedDelayedNonNestableTask(
      task_runner, Closure(), kDelay);
  this->task_tracker_->WaitForCompletedTasks(kTaskCount);
  this->delegate_.StopTaskRunner();
  Time time_after_run = Time::Now();

  EXPECT_TRUE(CheckNonNestableInvariants(this->task_tracker_->GetTaskEvents(),
                                         kTaskCount));
  EXPECT_LE(kDelay, time_after_run - time_before_run);
}

TYPED_TEST_P(SequencedTaskRunnerTest, DelayedTasksSameDelay) {
  
  if (!this->delegate_.TaskRunnerHandlesNonZeroDelays()) {
    DLOG(INFO) << "This SequencedTaskRunner doesn't handle "
        "non-zero delays; skipping";
    return;
  }

  const int kTaskCount = 2;
  const TimeDelta kDelay = TimeDelta::FromMilliseconds(100);

  this->delegate_.StartTaskRunner();
  const scoped_refptr<SequencedTaskRunner> task_runner =
      this->delegate_.GetTaskRunner();

  this->task_tracker_->PostWrappedDelayedNonNestableTask(
      task_runner, Closure(), kDelay);
  this->task_tracker_->PostWrappedDelayedNonNestableTask(
      task_runner, Closure(), kDelay);
  this->task_tracker_->WaitForCompletedTasks(kTaskCount);
  this->delegate_.StopTaskRunner();

  EXPECT_TRUE(CheckNonNestableInvariants(this->task_tracker_->GetTaskEvents(),
                                         kTaskCount));
}

TYPED_TEST_P(SequencedTaskRunnerTest, DelayedTaskAfterLongTask) {
  
  if (!this->delegate_.TaskRunnerHandlesNonZeroDelays()) {
    DLOG(INFO) << "This SequencedTaskRunner doesn't handle "
        "non-zero delays; skipping";
    return;
  }

  const int kTaskCount = 2;

  this->delegate_.StartTaskRunner();
  const scoped_refptr<SequencedTaskRunner> task_runner =
      this->delegate_.GetTaskRunner();

  this->task_tracker_->PostWrappedNonNestableTask(
      task_runner, base::Bind(&PlatformThread::Sleep,
                              TimeDelta::FromMilliseconds(50)));
  this->task_tracker_->PostWrappedDelayedNonNestableTask(
      task_runner, Closure(), TimeDelta::FromMilliseconds(10));
  this->task_tracker_->WaitForCompletedTasks(kTaskCount);
  this->delegate_.StopTaskRunner();

  EXPECT_TRUE(CheckNonNestableInvariants(this->task_tracker_->GetTaskEvents(),
                                         kTaskCount));
}

TYPED_TEST_P(SequencedTaskRunnerTest, DelayedTaskAfterManyLongTasks) {
  
  if (!this->delegate_.TaskRunnerHandlesNonZeroDelays()) {
    DLOG(INFO) << "This SequencedTaskRunner doesn't handle "
        "non-zero delays; skipping";
    return;
  }

  const int kTaskCount = 11;

  this->delegate_.StartTaskRunner();
  const scoped_refptr<SequencedTaskRunner> task_runner =
      this->delegate_.GetTaskRunner();

  for (int i = 0; i < kTaskCount - 1; i++) {
    this->task_tracker_->PostWrappedNonNestableTask(
        task_runner, base::Bind(&PlatformThread::Sleep,
                                TimeDelta::FromMilliseconds(50)));
  }
  this->task_tracker_->PostWrappedDelayedNonNestableTask(
      task_runner, Closure(), TimeDelta::FromMilliseconds(10));
  this->task_tracker_->WaitForCompletedTasks(kTaskCount);
  this->delegate_.StopTaskRunner();

  EXPECT_TRUE(CheckNonNestableInvariants(this->task_tracker_->GetTaskEvents(),
                                         kTaskCount));
}



REGISTER_TYPED_TEST_CASE_P(SequencedTaskRunnerTest,
                           SequentialNonNestable,
                           SequentialNestable,
                           SequentialDelayedNonNestable,
                           NonNestablePostFromNonNestableTask,
                           DelayedTaskBasic,
                           DelayedTasksSameDelay,
                           DelayedTaskAfterLongTask,
                           DelayedTaskAfterManyLongTasks);

}  

#endif  
