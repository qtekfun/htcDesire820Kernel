// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_LOCAL_FILE_SYNC_CONTEXT_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_LOCAL_FILE_SYNC_CONTEXT_H_

#include <deque>
#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/timer/timer.h"
#include "chrome/browser/sync_file_system/local/local_file_sync_status.h"
#include "chrome/browser/sync_file_system/sync_callbacks.h"
#include "chrome/browser/sync_file_system/sync_status_code.h"
#include "url/gurl.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace fileapi {
class FileSystemContext;
class FileSystemURL;
}

namespace webkit_blob {
class ScopedFile;
}

namespace sync_file_system {

class FileChange;
class LocalFileChangeTracker;
struct LocalFileSyncInfo;
class LocalOriginChangeObserver;
class RootDeleteHelper;
class SyncableFileOperationRunner;

class LocalFileSyncContext
    : public base::RefCountedThreadSafe<LocalFileSyncContext>,
      public LocalFileSyncStatus::Observer {
 public:
  enum SyncMode {
    SYNC_EXCLUSIVE,
    SYNC_SNAPSHOT,
  };

  typedef base::Callback<void(
      SyncStatusCode status,
      const LocalFileSyncInfo& sync_file_info,
      webkit_blob::ScopedFile snapshot)>
          LocalFileSyncInfoCallback;

  typedef base::Callback<void(SyncStatusCode status,
                              bool has_pending_changes)>
      HasPendingLocalChangeCallback;

  LocalFileSyncContext(const base::FilePath& base_path,
                       base::SingleThreadTaskRunner* ui_task_runner,
                       base::SingleThreadTaskRunner* io_task_runner);

  
  
  
  
  void MaybeInitializeFileSystemContext(
      const GURL& source_url,
      fileapi::FileSystemContext* file_system_context,
      const SyncStatusCallback& callback);

  
  
  void ShutdownOnUIThread();

  
  
  
  void GetFileForLocalSync(fileapi::FileSystemContext* file_system_context,
                           const LocalFileSyncInfoCallback& callback);

  
  
  
  
  void ClearChangesForURL(fileapi::FileSystemContext* file_system_context,
                          const fileapi::FileSystemURL& url,
                          const base::Closure& done_callback);

  
  
  
  
  
  
  void FinalizeSnapshotSync(
      fileapi::FileSystemContext* file_system_context,
      const fileapi::FileSystemURL& url,
      SyncStatusCode sync_finish_status,
      const base::Closure& done_callback);

  
  
  void FinalizeExclusiveSync(
      fileapi::FileSystemContext* file_system_context,
      const fileapi::FileSystemURL& url,
      bool clear_local_changes,
      const base::Closure& done_callback);

  
  // If the target |url| is being written and cannot start sync it
  
  
  
  
  
  
  
  
  
  
  
  
  
  void PrepareForSync(fileapi::FileSystemContext* file_system_context,
                      const fileapi::FileSystemURL& url,
                      SyncMode sync_mode,
                      const LocalFileSyncInfoCallback& callback);

  
  
  
  
  
  
  
  
  
  void RegisterURLForWaitingSync(const fileapi::FileSystemURL& url,
                                 const base::Closure& on_syncable_callback);

  
  
  void ApplyRemoteChange(
      fileapi::FileSystemContext* file_system_context,
      const FileChange& change,
      const base::FilePath& local_path,
      const fileapi::FileSystemURL& url,
      const SyncStatusCallback& callback);

  
  void RecordFakeLocalChange(
      fileapi::FileSystemContext* file_system_context,
      const fileapi::FileSystemURL& url,
      const FileChange& change,
      const SyncStatusCallback& callback);

  
  void GetFileMetadata(
      fileapi::FileSystemContext* file_system_context,
      const fileapi::FileSystemURL& url,
      const SyncFileMetadataCallback& callback);

  
  
  void HasPendingLocalChanges(
      fileapi::FileSystemContext* file_system_context,
      const fileapi::FileSystemURL& url,
      const HasPendingLocalChangeCallback& callback);

  
  void AddOriginChangeObserver(LocalOriginChangeObserver* observer);
  void RemoveOriginChangeObserver(LocalOriginChangeObserver* observer);

  
  base::WeakPtr<SyncableFileOperationRunner> operation_runner() const;

  
  LocalFileSyncStatus* sync_status() const;

  
  
  void set_mock_notify_changes_duration_in_sec(int duration) {
    mock_notify_changes_duration_in_sec_ = duration;
  }

 protected:
  
  virtual void OnSyncEnabled(const fileapi::FileSystemURL& url) OVERRIDE;
  virtual void OnWriteEnabled(const fileapi::FileSystemURL& url) OVERRIDE;

 private:
  typedef base::Callback<void(base::PlatformFileError result)> StatusCallback;
  typedef std::deque<SyncStatusCallback> StatusCallbackQueue;
  friend class base::RefCountedThreadSafe<LocalFileSyncContext>;
  friend class CannedSyncableFileSystem;

  virtual ~LocalFileSyncContext();

  void ShutdownOnIOThread();

  
  
  
  void ScheduleNotifyChangesUpdatedOnIOThread();

  
  void NotifyAvailableChangesOnIOThread();

  
  void NotifyAvailableChanges(const std::set<GURL>& origins);

  
  void InitializeFileSystemContextOnIOThread(
      const GURL& source_url,
      fileapi::FileSystemContext* file_system_context,
      const GURL& ,
      const std::string& ,
      base::PlatformFileError error);
  SyncStatusCode InitializeChangeTrackerOnFileThread(
      scoped_ptr<LocalFileChangeTracker>* tracker_ptr,
      fileapi::FileSystemContext* file_system_context,
      std::set<GURL>* origins_with_changes);
  void DidInitializeChangeTrackerOnIOThread(
      scoped_ptr<LocalFileChangeTracker>* tracker_ptr,
      const GURL& source_url,
      fileapi::FileSystemContext* file_system_context,
      std::set<GURL>* origins_with_changes,
      SyncStatusCode status);
  void DidInitialize(
      const GURL& source_url,
      fileapi::FileSystemContext* file_system_context,
      SyncStatusCode status);

  
  void GetNextURLsForSyncOnFileThread(
      fileapi::FileSystemContext* file_system_context,
      std::deque<fileapi::FileSystemURL>* urls);
  void TryPrepareForLocalSync(
      fileapi::FileSystemContext* file_system_context,
      std::deque<fileapi::FileSystemURL>* urls,
      const LocalFileSyncInfoCallback& callback);
  void DidTryPrepareForLocalSync(
      fileapi::FileSystemContext* file_system_context,
      std::deque<fileapi::FileSystemURL>* remaining_urls,
      const LocalFileSyncInfoCallback& callback,
      SyncStatusCode status,
      const LocalFileSyncInfo& sync_file_info,
      webkit_blob::ScopedFile snapshot);

  
  void DidGetWritingStatusForSync(
      fileapi::FileSystemContext* file_system_context,
      SyncStatusCode status,
      const fileapi::FileSystemURL& url,
      SyncMode sync_mode,
      const LocalFileSyncInfoCallback& callback);

  
  
  
  
  
  
  
  void ClearSyncFlagOnIOThread(const fileapi::FileSystemURL& url,
                               bool for_snapshot_sync);
  void FinalizeSnapshotSyncOnIOThread(const fileapi::FileSystemURL& url);

  void HandleRemoteDelete(
      fileapi::FileSystemContext* file_system_context,
      const fileapi::FileSystemURL& url,
      const SyncStatusCallback& callback);
  void HandleRemoteAddOrUpdate(
      fileapi::FileSystemContext* file_system_context,
      const FileChange& change,
      const base::FilePath& local_path,
      const fileapi::FileSystemURL& url,
      const SyncStatusCallback& callback);
  void DidRemoveExistingEntryForRemoteAddOrUpdate(
      fileapi::FileSystemContext* file_system_context,
      const FileChange& change,
      const base::FilePath& local_path,
      const fileapi::FileSystemURL& url,
      const SyncStatusCallback& callback,
      base::PlatformFileError error);

  
  void DidApplyRemoteChange(
      const fileapi::FileSystemURL& url,
      const SyncStatusCallback& callback_on_ui,
      base::PlatformFileError file_error);

  void DidGetFileMetadata(
      const SyncFileMetadataCallback& callback,
      base::PlatformFileError file_error,
      const base::PlatformFileInfo& file_info);

  base::TimeDelta NotifyChangesDuration();

  void DidCreateDirectoryForCopyIn(
      fileapi::FileSystemContext* file_system_context,
      const base::FilePath& local_file_path,
      const fileapi::FileSystemURL& dest_url,
      const StatusCallback& callback,
      base::PlatformFileError error);

  const base::FilePath local_base_path_;

  scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> io_task_runner_;

  
  bool shutdown_on_ui_;  
  bool shutdown_on_io_;  

  
  scoped_ptr<SyncableFileOperationRunner> operation_runner_;

  
  
  scoped_ptr<LocalFileSyncStatus> sync_status_;

  
  
  
  std::set<fileapi::FileSystemContext*> file_system_contexts_;

  
  std::map<fileapi::FileSystemContext*, StatusCallbackQueue>
      pending_initialize_callbacks_;

  
  
  fileapi::FileSystemURL url_waiting_sync_on_io_;
  base::Closure url_syncable_callback_;

  
  base::Time last_notified_changes_;
  scoped_ptr<base::OneShotTimer<LocalFileSyncContext> > timer_on_io_;
  std::set<GURL> origins_with_pending_changes_;

  
  
  scoped_ptr<RootDeleteHelper> root_delete_helper_;

  ObserverList<LocalOriginChangeObserver> origin_change_observers_;

  int mock_notify_changes_duration_in_sec_;

  DISALLOW_COPY_AND_ASSIGN(LocalFileSyncContext);
};

}  

#endif  
