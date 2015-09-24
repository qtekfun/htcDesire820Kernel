// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILEAPI_WORKER_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILEAPI_WORKER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "webkit/common/blob/scoped_file.h"

namespace base {
class FilePath;
}  

namespace fileapi {
struct DirectoryEntry;
}  

namespace drive {

class FileSystemInterface;

namespace fileapi_internal {

typedef base::Callback<FileSystemInterface*()> FileSystemGetter;

typedef base::Callback<
    void(base::PlatformFileError result)> StatusCallback;
typedef base::Callback<
    void(base::PlatformFileError result,
         const base::PlatformFileInfo& file_info)> GetFileInfoCallback;
typedef base::Callback<
    void(base::PlatformFileError result,
         const std::vector<fileapi::DirectoryEntry>& file_list,
         bool has_more)> ReadDirectoryCallback;
typedef base::Callback<
    void(base::PlatformFileError result,
         const base::PlatformFileInfo& file_info,
         const base::FilePath& snapshot_file_path,
         webkit_blob::ScopedFile::ScopeOutPolicy scope_out_policy)>
    CreateSnapshotFileCallback;
typedef base::Callback<
    void(base::PlatformFileError result,
         const base::FilePath& snapshot_file_path,
         const base::Closure& close_callback)>
    CreateWritableSnapshotFileCallback;
typedef base::Callback<
    void(base::PlatformFileError result,
         base::PlatformFile platform_file,
         const base::Closure& close_callback)> OpenFileCallback;

void RunFileSystemCallback(
    const FileSystemGetter& file_system_getter,
    const base::Callback<void(FileSystemInterface*)>& callback,
    const base::Closure& error_callback);

void GetFileInfo(const base::FilePath& file_path,
                 const GetFileInfoCallback& callback,
                 FileSystemInterface* file_system);

void Copy(const base::FilePath& src_file_path,
          const base::FilePath& dest_file_path,
          bool preserve_last_modified,
          const StatusCallback& callback,
          FileSystemInterface* file_system);

void Move(const base::FilePath& src_file_path,
          const base::FilePath& dest_file_path,
          bool preserve_last_modified,
          const StatusCallback& callback,
          FileSystemInterface* file_system);


void CopyInForeignFile(const base::FilePath& src_foreign_file_path,
                       const base::FilePath& dest_file_path,
                       const StatusCallback& callback,
                       FileSystemInterface* file_system);

void ReadDirectory(const base::FilePath& file_path,
                   const ReadDirectoryCallback& callback,
                   FileSystemInterface* file_system);

void Remove(const base::FilePath& file_path,
            bool is_recursive,
            const StatusCallback& callback,
            FileSystemInterface* file_system);

void CreateDirectory(const base::FilePath& file_path,
                     bool is_exclusive,
                     bool is_recursive,
                     const StatusCallback& callback,
                     FileSystemInterface* file_system);

void CreateFile(const base::FilePath& file_path,
                bool is_exclusive,
                const StatusCallback& callback,
                FileSystemInterface* file_system);

void Truncate(const base::FilePath& file_path,
              int64 length,
              const StatusCallback& callback,
              FileSystemInterface* file_system);

void CreateSnapshotFile(const base::FilePath& file_path,
                        const CreateSnapshotFileCallback& callback,
                        FileSystemInterface* file_system);

void CreateWritableSnapshotFile(
    const base::FilePath& file_path,
    const CreateWritableSnapshotFileCallback& callback,
    FileSystemInterface* file_system);

void OpenFile(const base::FilePath& file_path,
              int file_flags,
              const OpenFileCallback& callback,
              FileSystemInterface* file_system);

void TouchFile(const base::FilePath& file_path,
               const base::Time& last_access_time,
               const base::Time& last_modified_time,
               const StatusCallback& callback,
               FileSystemInterface* file_system);

}  
}  

#endif  
