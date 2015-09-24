// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_COMMON_CANCELABLE_TASK_TRACKER_H_
#define CHROME_COMMON_CANCELABLE_TASK_TRACKER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"

namespace base {
class CancellationFlag;
class TaskRunner;
}  

namespace tracked_objects {
class Location;
}  

class CancelableTaskTracker {
 public:
  
  typedef int64 TaskId;
  static const TaskId kBadTaskId;

  typedef base::Callback<bool()> IsCanceledCallback;

  CancelableTaskTracker();

  
  ~CancelableTaskTracker();

  TaskId PostTask(base::TaskRunner* task_runner,
                  const tracked_objects::Location& from_here,
                  const base::Closure& task);

  TaskId PostTaskAndReply(base::TaskRunner* task_runner,
                          const tracked_objects::Location& from_here,
                          const base::Closure& task,
                          const base::Closure& reply);

  
  
  
  
  
  
  
  
  
  TaskId NewTrackedTaskId(IsCanceledCallback* is_canceled_cb);

  
  
  
  
  
  
  
  void TryCancel(TaskId id);

  
  
  void TryCancelAll();

  
  
  bool HasTrackedTasks() const;

 private:
  void Track(TaskId id, base::CancellationFlag* flag);
  void Untrack(TaskId id);

  base::hash_map<TaskId, base::CancellationFlag*> task_flags_;
  base::WeakPtrFactory<CancelableTaskTracker> weak_factory_;

  TaskId next_id_;
  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(CancelableTaskTracker);
};

#endif  
