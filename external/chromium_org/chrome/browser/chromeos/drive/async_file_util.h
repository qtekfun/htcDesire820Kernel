// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_ASYNC_FILE_UTIL_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_ASYNC_FILE_UTIL_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "webkit/browser/fileapi/async_file_util.h"

namespace drive {

class FileSystemInterface;

namespace internal {

class AsyncFileUtil : public fileapi::AsyncFileUtil {
 public:
  
  
  
  
  typedef base::Callback<FileSystemInterface*()> FileSystemGetter;

  explicit AsyncFileUtil(const FileSystemGetter& file_system_getter);
  virtual ~AsyncFileUtil();

  
  virtual void CreateOrOpen(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const fileapi::FileSystemURL& url,
      int file_flags,
      const CreateOrOpenCallback& callback) OVERRIDE;
  virtual void EnsureFileExists(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const fileapi::FileSystemURL& url,
      const EnsureFileExistsCallback& callback) OVERRIDE;
  virtual void CreateDirectory(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const fileapi::FileSystemURL& url,
      bool exclusive,
      bool recursive,
      const StatusCallback& callback) OVERRIDE;
  virtual void GetFileInfo(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const fileapi::FileSystemURL& url,
      const GetFileInfoCallback& callback) OVERRIDE;
  virtual void ReadDirectory(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const fileapi::FileSystemURL& url,
      const ReadDirectoryCallback& callback) OVERRIDE;
  virtual void Touch(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const fileapi::FileSystemURL& url,
      const base::Time& last_access_time,
      const base::Time& last_modified_time,
      const StatusCallback& callback) OVERRIDE;
  virtual void Truncate(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const fileapi::FileSystemURL& url,
      int64 length,
      const StatusCallback& callback) OVERRIDE;
  virtual void CopyFileLocal(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const fileapi::FileSystemURL& src_url,
      const fileapi::FileSystemURL& dest_url,
      CopyOrMoveOption option,
      const CopyFileProgressCallback& progress_callback,
      const StatusCallback& callback) OVERRIDE;
  virtual void MoveFileLocal(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const fileapi::FileSystemURL& src_url,
      const fileapi::FileSystemURL& dest_url,
      CopyOrMoveOption option,
      const StatusCallback& callback) OVERRIDE;
  virtual void CopyInForeignFile(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const base::FilePath& src_file_path,
      const fileapi::FileSystemURL& dest_url,
      const StatusCallback& callback) OVERRIDE;
  virtual void DeleteFile(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const fileapi::FileSystemURL& url,
      const StatusCallback& callback) OVERRIDE;
  virtual void DeleteDirectory(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const fileapi::FileSystemURL& url,
      const StatusCallback& callback) OVERRIDE;
  virtual void DeleteRecursively(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const fileapi::FileSystemURL& url,
      const StatusCallback& callback) OVERRIDE;
  virtual void CreateSnapshotFile(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const fileapi::FileSystemURL& url,
      const CreateSnapshotFileCallback& callback) OVERRIDE;

 private:
  FileSystemGetter file_system_getter_;

  DISALLOW_COPY_AND_ASSIGN(AsyncFileUtil);
};

}  
}  

#endif  