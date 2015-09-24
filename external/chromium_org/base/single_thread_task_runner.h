// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SINGLE_THREAD_TASK_RUNNER_H_
#define BASE_SINGLE_THREAD_TASK_RUNNER_H_

#include "base/base_export.h"
#include "base/sequenced_task_runner.h"

namespace base {

class BASE_EXPORT SingleThreadTaskRunner : public SequencedTaskRunner {
 public:
  
  bool BelongsToCurrentThread() const {
    return RunsTasksOnCurrentThread();
  }

 protected:
  virtual ~SingleThreadTaskRunner() {}
};

}  

#endif  
