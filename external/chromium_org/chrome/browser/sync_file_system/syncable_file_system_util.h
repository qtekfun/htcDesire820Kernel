// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_SYNCABLE_FILE_SYSTEM_UTIL_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_SYNCABLE_FILE_SYSTEM_UTIL_H_

#include <string>

#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "webkit/browser/fileapi/file_system_url.h"

namespace fileapi {
class FileSystemContext;
class FileSystemURL;
}

namespace tracked_objects {
class Location;
}

namespace sync_file_system {

void RegisterSyncableFileSystem();

void RevokeSyncableFileSystem();

GURL GetSyncableFileSystemRootURI(const GURL& origin);

fileapi::FileSystemURL
CreateSyncableFileSystemURL(const GURL& origin, const base::FilePath& path);

fileapi::FileSystemURL CreateSyncableFileSystemURLForSync(
    fileapi::FileSystemContext* file_system_context,
    const fileapi::FileSystemURL& syncable_url);

bool SerializeSyncableFileSystemURL(
    const fileapi::FileSystemURL& url, std::string* serialized_url);

bool DeserializeSyncableFileSystemURL(
    const std::string& serialized_url, fileapi::FileSystemURL* url);

void SetEnableSyncFSDirectoryOperation(bool flag);

bool IsSyncFSDirectoryOperationEnabled();

bool IsSyncFSDirectoryOperationEnabled(const GURL& origin);

bool IsV2Enabled();

bool IsV2EnabledForOrigin(const GURL& origin);

base::FilePath GetSyncFileSystemDir(const base::FilePath& profile_base_dir);

class ScopedEnableSyncFSDirectoryOperation {
 public:
  ScopedEnableSyncFSDirectoryOperation();
  ~ScopedEnableSyncFSDirectoryOperation();

 private:
  bool was_enabled_;

  DISALLOW_COPY_AND_ASSIGN(ScopedEnableSyncFSDirectoryOperation);
};

void RunSoon(const tracked_objects::Location& from_here,
             const base::Closure& callback);

}  

#endif  
