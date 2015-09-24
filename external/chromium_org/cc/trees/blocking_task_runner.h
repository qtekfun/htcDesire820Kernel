// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TREES_BLOCKING_TASK_RUNNER_H_
#define CC_TREES_BLOCKING_TASK_RUNNER_H_

#include <vector>

#include "base/location.h"
#include "base/memory/ref_counted.h"
#include "base/single_thread_task_runner.h"
#include "base/synchronization/lock.h"
#include "cc/base/cc_export.h"

namespace cc {

class CC_EXPORT BlockingTaskRunner
    : public base::RefCountedThreadSafe<BlockingTaskRunner> {
 public:
  
  
  static scoped_refptr<BlockingTaskRunner> current();

  
  
  
  class CC_EXPORT CapturePostTasks {
   public:
    CapturePostTasks();
    ~CapturePostTasks();

   private:
    scoped_refptr<BlockingTaskRunner> blocking_runner_;

    DISALLOW_COPY_AND_ASSIGN(CapturePostTasks);
  };

  
  
  bool BelongsToCurrentThread() {
    return task_runner_->BelongsToCurrentThread();
  }

  
  
  
  
  bool PostTask(const tracked_objects::Location& from_here,
                const base::Closure& task);

 private:
  friend class base::RefCountedThreadSafe<BlockingTaskRunner>;

  explicit BlockingTaskRunner(
      scoped_refptr<base::SingleThreadTaskRunner> task_runner);
  virtual ~BlockingTaskRunner();

  void SetCapture(bool capture);

  scoped_refptr<base::SingleThreadTaskRunner> task_runner_;

  base::Lock lock_;
  int capture_;
  std::vector<base::Closure> captured_tasks_;
};

}  

#endif  
