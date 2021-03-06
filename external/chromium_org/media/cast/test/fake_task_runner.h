// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_TEST_FAKE_TASK_RUNNER_H_
#define MEDIA_CAST_TEST_FAKE_TASK_RUNNER_H_

#include <map>

#include "base/basictypes.h"
#include "base/task_runner.h"
#include "base/test/simple_test_tick_clock.h"
#include "base/test/test_pending_task.h"

namespace media {
namespace cast {
namespace test {

typedef base::TestPendingTask PostedTask;

class FakeTaskRunner : public base::TaskRunner {
 public:
  explicit FakeTaskRunner(base::SimpleTestTickClock* clock);

  void RunTasks();

  
  virtual bool PostDelayedTask(const tracked_objects::Location& from_here,
                               const base::Closure& task,
                               base::TimeDelta delay) OVERRIDE;

  virtual bool RunsTasksOnCurrentThread() const OVERRIDE;

 protected:
  virtual ~FakeTaskRunner();

 private:
  base::SimpleTestTickClock* const clock_;
  std::multimap<base::TimeTicks, PostedTask> tasks_;

  DISALLOW_COPY_AND_ASSIGN(FakeTaskRunner);
};

}  
}  
}  

#endif  
