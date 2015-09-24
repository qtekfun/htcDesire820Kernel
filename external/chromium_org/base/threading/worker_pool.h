// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_WORKER_POOL_H_
#define BASE_THREADING_WORKER_POOL_H_

#include "base/base_export.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"

class Task;

namespace tracked_objects {
class Location;
}  

namespace base {

class TaskRunner;

class BASE_EXPORT WorkerPool {
 public:
  
  
  
  
  static bool PostTask(const tracked_objects::Location& from_here,
                       const base::Closure& task, bool task_is_slow);

  
  
  
  static bool PostTaskAndReply(const tracked_objects::Location& from_here,
                               const Closure& task,
                               const Closure& reply,
                               bool task_is_slow);

  
  
  
  
  static bool RunsTasksOnCurrentThread();

  
  
  static const scoped_refptr<TaskRunner>& GetTaskRunner(bool task_is_slow);
};

}  

#endif  
