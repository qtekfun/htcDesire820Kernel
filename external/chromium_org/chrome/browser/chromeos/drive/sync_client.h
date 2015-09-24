// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_SYNC_CLIENT_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_SYNC_CLIENT_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "chrome/browser/chromeos/drive/file_system/update_operation.h"

namespace base {
class SequencedTaskRunner;
}

namespace drive {

class FileCacheEntry;
class JobScheduler;
class ResourceEntry;
struct ClientContext;

namespace file_system {
class DownloadOperation;
class OperationObserver;
class UpdateOperation;
}

namespace internal {

class EntryUpdatePerformer;
class FileCache;
class ResourceMetadata;

class SyncClient {
 public:
  SyncClient(base::SequencedTaskRunner* blocking_task_runner,
             file_system::OperationObserver* observer,
             JobScheduler* scheduler,
             ResourceMetadata* metadata,
             FileCache* cache,
             const base::FilePath& temporary_file_directory);
  virtual ~SyncClient();

  
  void AddFetchTask(const std::string& local_id);

  
  void RemoveFetchTask(const std::string& local_id);

  
  void AddUploadTask(const ClientContext& context, const std::string& local_id);

  
  void AddUpdateTask(const std::string& local_id);

  
  
  
  void StartProcessingBacklog();

  
  
  
  void StartCheckingExistingPinnedFiles();

  
  void set_delay_for_testing(const base::TimeDelta& delay) {
    delay_ = delay;
  }

  
  void StartSyncLoop();

 private:
  
  enum SyncType {
    FETCH,  
    UPLOAD,  
    UPDATE,  
  };

  
  enum SyncState {
    PENDING,
    RUNNING,
  };

  struct SyncTask {
    SyncTask();
    ~SyncTask();
    SyncState state;
    base::Closure task;
    bool should_run_again;
  };

  typedef std::map<std::pair<SyncType, std::string>, SyncTask> SyncTasks;

  
  void AddFetchTaskInternal(const std::string& local_id,
                            const base::TimeDelta& delay);

  
  void AddUploadTaskInternal(
      const ClientContext& context,
      const std::string& local_id,
      file_system::UpdateOperation::ContentCheckMode content_check_mode,
      const base::TimeDelta& delay);

  
  void AddUpdateTaskInternal(const std::string& local_id,
                             const base::TimeDelta& delay);

  
  void AddTask(const SyncTasks::key_type& key,
               const SyncTask& task,
               const base::TimeDelta& delay);

  
  void StartTask(const SyncTasks::key_type& key);

  
  void OnGetLocalIdsOfBacklog(const std::vector<std::string>* to_fetch,
                              const std::vector<std::string>* to_upload,
                              const std::vector<std::string>* to_update);

  
  void AddFetchTasks(const std::vector<std::string>* local_ids);

  
  bool OnTaskComplete(SyncType type, const std::string& local_id);

  
  
  void OnFetchFileComplete(const std::string& local_id,
                           FileError error,
                           const base::FilePath& local_path,
                           scoped_ptr<ResourceEntry> entry);

  
  
  void OnUploadFileComplete(const std::string& local_id, FileError error);

  
  void OnUpdateComplete(const std::string& local_id, FileError error);

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  ResourceMetadata* metadata_;
  FileCache* cache_;

  
  scoped_ptr<file_system::DownloadOperation> download_operation_;

  
  scoped_ptr<file_system::UpdateOperation> update_operation_;

  
  scoped_ptr<EntryUpdatePerformer> entry_update_performer_;

  
  SyncTasks tasks_;

  
  base::TimeDelta delay_;

  
  base::TimeDelta long_delay_;

  
  
  base::WeakPtrFactory<SyncClient> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(SyncClient);
};

}  
}  

#endif  
