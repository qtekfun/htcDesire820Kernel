// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_LOCAL_FILE_CHANGE_TRACKER_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_LOCAL_FILE_CHANGE_TRACKER_H_

#include <deque>
#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/sync_file_system/file_change.h"
#include "chrome/browser/sync_file_system/sync_status_code.h"
#include "webkit/browser/fileapi/file_observers.h"
#include "webkit/browser/fileapi/file_system_url.h"

namespace base {
class SequencedTaskRunner;
}

namespace fileapi {
class FileSystemContext;
class FileSystemURL;
}

namespace sync_file_system {

class LocalFileChangeTracker
    : public fileapi::FileUpdateObserver,
      public fileapi::FileChangeObserver {
 public:
  
  
  
  LocalFileChangeTracker(const base::FilePath& base_path,
                         base::SequencedTaskRunner* file_task_runner);
  virtual ~LocalFileChangeTracker();

  
  virtual void OnStartUpdate(const fileapi::FileSystemURL& url) OVERRIDE;
  virtual void OnUpdate(
      const fileapi::FileSystemURL& url, int64 delta) OVERRIDE {}
  virtual void OnEndUpdate(const fileapi::FileSystemURL& url) OVERRIDE;

  
  virtual void OnCreateFile(const fileapi::FileSystemURL& url) OVERRIDE;
  virtual void OnCreateFileFrom(const fileapi::FileSystemURL& url,
                                const fileapi::FileSystemURL& src) OVERRIDE;
  virtual void OnRemoveFile(const fileapi::FileSystemURL& url) OVERRIDE;
  virtual void OnModifyFile(const fileapi::FileSystemURL& url) OVERRIDE;
  virtual void OnCreateDirectory(const fileapi::FileSystemURL& url) OVERRIDE;
  virtual void OnRemoveDirectory(const fileapi::FileSystemURL& url) OVERRIDE;

  
  
  
  
  void GetNextChangedURLs(std::deque<fileapi::FileSystemURL>* urls,
                          int max_urls);

  
  
  void GetChangesForURL(const fileapi::FileSystemURL& url,
                        FileChangeList* changes);

  
  void ClearChangesForURL(const fileapi::FileSystemURL& url);

  
  
  void CreateFreshMirrorForURL(const fileapi::FileSystemURL& url);

  
  void RemoveMirrorAndCommitChangesForURL(const fileapi::FileSystemURL& url);

  
  
  void ResetToMirrorAndCommitChangesForURL(const fileapi::FileSystemURL& url);

  
  
  
  
  void DemoteChangesForURL(const fileapi::FileSystemURL& url);

  
  
  SyncStatusCode Initialize(fileapi::FileSystemContext* file_system_context);

  
  
  
  void ResetForFileSystem(const GURL& origin, fileapi::FileSystemType type);

  
  int64 num_changes() const {
    base::AutoLock lock(num_changes_lock_);
    return num_changes_;
  }

 private:
  class TrackerDB;
  friend class CannedSyncableFileSystem;
  friend class LocalFileChangeTrackerTest;
  friend class LocalFileSyncContext;
  friend class LocalFileSyncContextTest;
  friend class SyncableFileSystemTest;

  struct ChangeInfo {
    ChangeInfo();
    ~ChangeInfo();
    FileChangeList change_list;
    int64 change_seq;
  };

  typedef std::map<fileapi::FileSystemURL, ChangeInfo,
      fileapi::FileSystemURL::Comparator>
          FileChangeMap;
  typedef std::map<int64, fileapi::FileSystemURL> ChangeSeqMap;

  void UpdateNumChanges();

  
  
  
  void GetAllChangedURLs(fileapi::FileSystemURLSet* urls);

  
  void DropAllChanges();

  
  SyncStatusCode MarkDirtyOnDatabase(const fileapi::FileSystemURL& url);
  SyncStatusCode ClearDirtyOnDatabase(const fileapi::FileSystemURL& url);

  SyncStatusCode CollectLastDirtyChanges(
      fileapi::FileSystemContext* file_system_context);
  void RecordChange(const fileapi::FileSystemURL& url,
                    const FileChange& change);

  static void RecordChangeToChangeMaps(const fileapi::FileSystemURL& url,
                                       const FileChange& change,
                                       int change_seq,
                                       FileChangeMap* changes,
                                       ChangeSeqMap* change_seqs);

  bool initialized_;

  scoped_refptr<base::SequencedTaskRunner> file_task_runner_;

  FileChangeMap changes_;
  ChangeSeqMap change_seqs_;

  
  FileChangeMap mirror_changes_;

  scoped_ptr<TrackerDB> tracker_db_;

  
  
  
  int64 current_change_seq_;

  
  int64 num_changes_;
  mutable base::Lock num_changes_lock_;

  DISALLOW_COPY_AND_ASSIGN(LocalFileChangeTracker);
};

}  

#endif  
