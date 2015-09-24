// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_GLUE_TASK_PUMP_H_
#define JINGLE_GLUE_TASK_PUMP_H_

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "third_party/libjingle/source/talk/base/taskrunner.h"

namespace jingle_glue {

class TaskPump : public talk_base::TaskRunner, public base::NonThreadSafe {
 public:
  TaskPump();

  virtual ~TaskPump();

  
  virtual void WakeTasks() OVERRIDE;
  virtual int64 CurrentTime() OVERRIDE;

  
  
  void Stop();

 private:
  void CheckAndRunTasks();

  bool posted_wake_;
  bool stopped_;

  base::WeakPtrFactory<TaskPump> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(TaskPump);
};

}  

#endif  
