// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_SYNCABLE_FILE_OPERATION_RUNNER_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_SYNCABLE_FILE_OPERATION_RUNNER_H_

#include <list>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/sync_file_system/local/local_file_sync_status.h"
#include "webkit/browser/fileapi/file_system_url.h"

namespace fileapi {
class FileSystemURL;
}

namespace sync_file_system {

class SyncableFileOperationRunner
    : public base::NonThreadSafe,
      public base::SupportsWeakPtr<SyncableFileOperationRunner>,
      public LocalFileSyncStatus::Observer {
 public:
  
  class Task {
   public:
    Task() {}
    virtual ~Task() {}

    
    virtual void Run() = 0;
    virtual void Cancel() = 0;

   protected:
    
    virtual const std::vector<fileapi::FileSystemURL>& target_paths() const = 0;

   private:
    friend class SyncableFileOperationRunner;
    bool IsRunnable(LocalFileSyncStatus* status) const;
    void Start(LocalFileSyncStatus* status);
    static void CancelAndDelete(Task* task);

    DISALLOW_COPY_AND_ASSIGN(Task);
  };

  SyncableFileOperationRunner(int64 max_inflight_tasks,
                              LocalFileSyncStatus* sync_status);
  virtual ~SyncableFileOperationRunner();

  
  virtual void OnSyncEnabled(const fileapi::FileSystemURL& url) OVERRIDE;
  virtual void OnWriteEnabled(const fileapi::FileSystemURL& url) OVERRIDE;

  
  
  
  
  
  
  void PostOperationTask(scoped_ptr<Task> task);

  
  void RunNextRunnableTask();

  
  
  void OnOperationCompleted(
      const std::vector<fileapi::FileSystemURL>& target_paths);

  LocalFileSyncStatus* sync_status() const { return sync_status_; }

  int64 num_pending_tasks() const {
    return static_cast<int64>(pending_tasks_.size());
  }

  int64 num_inflight_tasks() const { return num_inflight_tasks_; }

 private:
  
  bool ShouldStartMoreTasks() const;

  
  LocalFileSyncStatus* sync_status_;

  std::list<Task*> pending_tasks_;

  const int64 max_inflight_tasks_;
  int64 num_inflight_tasks_;

  DISALLOW_COPY_AND_ASSIGN(SyncableFileOperationRunner);
};

}  

#endif  
