// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_CHANGE_PROCESSOR_H_

#include "base/callback_forward.h"
#include "chrome/browser/sync_file_system/sync_callbacks.h"

namespace fileapi {
class FileSystemURL;
}

namespace sync_file_system {

class FileChange;

class LocalChangeProcessor {
 public:
  LocalChangeProcessor() {}
  virtual ~LocalChangeProcessor() {}

  
  
  
  
  
  
  
  virtual void ApplyLocalChange(
      const FileChange& change,
      const base::FilePath& local_file_path,
      const SyncFileMetadata& local_file_metadata,
      const fileapi::FileSystemURL& url,
      const SyncStatusCallback& callback) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(LocalChangeProcessor);
};

}  

#endif  
