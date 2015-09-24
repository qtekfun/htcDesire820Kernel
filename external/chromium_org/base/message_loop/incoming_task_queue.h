// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_INCOMING_TASK_QUEUE_H_
#define BASE_MESSAGE_LOOP_INCOMING_TASK_QUEUE_H_

#include "base/base_export.h"
#include "base/memory/ref_counted.h"
#include "base/pending_task.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"

namespace base {

class MessageLoop;
class WaitableEvent;

namespace internal {

class BASE_EXPORT IncomingTaskQueue
    : public RefCountedThreadSafe<IncomingTaskQueue> {
 public:
  explicit IncomingTaskQueue(MessageLoop* message_loop);

  
  
  
  
  
  
  
  bool AddToIncomingQueue(const tracked_objects::Location& from_here,
                          const Closure& task,
                          TimeDelta delay,
                          bool nestable);

  
  
  
  bool TryAddToIncomingQueue(const tracked_objects::Location& from_here,
                             const Closure& task);

  
  
  bool IsHighResolutionTimerEnabledForTesting();

  
  bool IsIdleForTesting();

  
  
  void LockWaitUnLockForTesting(WaitableEvent* caller_wait,
                                WaitableEvent* caller_signal);

  
  
  void ReloadWorkQueue(TaskQueue* work_queue);

  
  void WillDestroyCurrentMessageLoop();

 private:
  friend class RefCountedThreadSafe<IncomingTaskQueue>;
  virtual ~IncomingTaskQueue();

  
  TimeTicks CalculateDelayedRuntime(TimeDelta delay);

  
  
  
  
  bool PostPendingTask(PendingTask* pending_task);

#if defined(OS_WIN)
  TimeTicks high_resolution_timer_expiration_;
#endif

  
  
  base::Lock incoming_queue_lock_;

  
  
  
  TaskQueue incoming_queue_;

  
  MessageLoop* message_loop_;

  
  int next_sequence_num_;

  DISALLOW_COPY_AND_ASSIGN(IncomingTaskQueue);
};

}  
}  

#endif  
