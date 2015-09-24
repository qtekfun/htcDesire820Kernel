// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_REMOTE_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_REMOTE_CHANGE_PROCESSOR_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "chrome/browser/sync_file_system/sync_callbacks.h"
#include "chrome/browser/sync_file_system/sync_status_code.h"

namespace base {
class FilePath;
}

namespace fileapi {
class FileSystemURL;
}

namespace sync_file_system {

class FileChange;
class FileChangeList;
class SyncFileMetadata;

class RemoteChangeProcessor {
 public:
  
  
  
  
  
  typedef base::Callback<void(
      SyncStatusCode status,
      const SyncFileMetadata& metadata,
      const FileChangeList& changes)> PrepareChangeCallback;

  RemoteChangeProcessor() {}
  virtual ~RemoteChangeProcessor() {}

  
  
  
  
  
  virtual void PrepareForProcessRemoteChange(
      const fileapi::FileSystemURL& url,
      const PrepareChangeCallback& callback) = 0;

  
  
  
  
  
  
  
  virtual void ApplyRemoteChange(
      const FileChange& change,
      const base::FilePath& local_path,
      const fileapi::FileSystemURL& url,
      const SyncStatusCallback& callback) = 0;

  
  
  
  
  
  virtual void FinalizeRemoteSync(
      const fileapi::FileSystemURL& url,
      bool clear_local_changes,
      const base::Closure& completion_callback) = 0;

  
  
  
  
  
  virtual void RecordFakeLocalChange(
      const fileapi::FileSystemURL& url,
      const FileChange& change,
      const SyncStatusCallback& callback) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(RemoteChangeProcessor);
};

}  

#endif  
