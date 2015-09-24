// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_SYNC_TASK_MANAGER_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_SYNC_TASK_MANAGER_H_

#include <queue>
#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/sync_file_system/sync_callbacks.h"
#include "chrome/browser/sync_file_system/sync_status_code.h"
#include "chrome/browser/sync_file_system/sync_task.h"

namespace tracked_objects {
class Location;
}

namespace sync_file_system {

class SyncTaskManager
    : public base::NonThreadSafe,
      public base::SupportsWeakPtr<SyncTaskManager> {
 public:
  class TaskToken;
  typedef base::Callback<void(const SyncStatusCallback& callback)> Task;

  enum Priority {
    PRIORITY_LOW,
    PRIORITY_MED,
    PRIORITY_HIGH,
  };

  class Client {
   public:
    virtual ~Client() {}

    
    virtual void MaybeScheduleNextTask() = 0;

    
    virtual void NotifyLastOperationStatus(
        SyncStatusCode last_operation_status,
        bool last_operation_used_network) = 0;
  };

  explicit SyncTaskManager(base::WeakPtr<Client> client);
  virtual ~SyncTaskManager();

  
  
  
  void Initialize(SyncStatusCode status);

  
  void ScheduleTask(const Task& task,
                    const SyncStatusCallback& callback);
  void ScheduleSyncTask(scoped_ptr<SyncTask> task,
                        const SyncStatusCallback& callback);

  
  void ScheduleTaskAtPriority(const Task& task,
                              Priority priority,
                              const SyncStatusCallback& callback);
  void ScheduleSyncTaskAtPriority(scoped_ptr<SyncTask> task,
                                  Priority priority,
                                  const SyncStatusCallback& callback);

  
  
  bool ScheduleTaskIfIdle(const Task& task, const SyncStatusCallback& callback);
  bool ScheduleSyncTaskIfIdle(scoped_ptr<SyncTask> task,
                              const SyncStatusCallback& callback);

  void NotifyTaskDone(scoped_ptr<TaskToken> token,
                      SyncStatusCode status);

 private:
  struct PendingTask {
    base::Closure task;
    Priority priority;
    int64 seq;

    PendingTask();
    PendingTask(const base::Closure& task, Priority pri, int seq);
    ~PendingTask();
  };

  struct PendingTaskComparator {
    bool operator()(const PendingTask& left,
                    const PendingTask& right) const;
  };

  
  scoped_ptr<TaskToken> GetToken(const tracked_objects::Location& from_here);

  
  
  SyncStatusCallback CreateCompletionCallback(
      scoped_ptr<TaskToken> token,
      const SyncStatusCallback& callback);

  void PushPendingTask(const base::Closure& closure, Priority priority);

  base::WeakPtr<Client> client_;

  SyncStatusCode last_operation_status_;
  scoped_ptr<SyncTask> running_task_;
  SyncStatusCallback current_callback_;

  std::priority_queue<PendingTask, std::vector<PendingTask>,
                      PendingTaskComparator> pending_tasks_;
  int64 pending_task_seq_;

  
  
  
  
  
  scoped_ptr<TaskToken> token_;

  DISALLOW_COPY_AND_ASSIGN(SyncTaskManager);
};

}  

#endif  
