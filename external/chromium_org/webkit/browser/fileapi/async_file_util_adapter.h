// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_ASYNC_FILE_UTIL_ADAPTER_H_
#define WEBKIT_BROWSER_FILEAPI_ASYNC_FILE_UTIL_ADAPTER_H_

#include "base/memory/scoped_ptr.h"
#include "webkit/browser/fileapi/async_file_util.h"

namespace fileapi {

class FileSystemFileUtil;

class WEBKIT_STORAGE_BROWSER_EXPORT AsyncFileUtilAdapter
    : public NON_EXPORTED_BASE(AsyncFileUtil) {
 public:
  
  
  
  
  explicit AsyncFileUtilAdapter(FileSystemFileUtil* sync_file_util);

  virtual ~AsyncFileUtilAdapter();

  FileSystemFileUtil* sync_file_util() {
    return sync_file_util_.get();
  }

  
  virtual void CreateOrOpen(
      scoped_ptr<FileSystemOperationContext> context,
      const FileSystemURL& url,
      int file_flags,
      const CreateOrOpenCallback& callback) OVERRIDE;
  virtual void EnsureFileExists(
      scoped_ptr<FileSystemOperationContext> context,
      const FileSystemURL& url,
      const EnsureFileExistsCallback& callback) OVERRIDE;
  virtual void CreateDirectory(
      scoped_ptr<FileSystemOperationContext> context,
      const FileSystemURL& url,
      bool exclusive,
      bool recursive,
      const StatusCallback& callback) OVERRIDE;
  virtual void GetFileInfo(
      scoped_ptr<FileSystemOperationContext> context,
      const FileSystemURL& url,
      const GetFileInfoCallback& callback) OVERRIDE;
  virtual void ReadDirectory(
      scoped_ptr<FileSystemOperationContext> context,
      const FileSystemURL& url,
      const ReadDirectoryCallback& callback) OVERRIDE;
  virtual void Touch(
      scoped_ptr<FileSystemOperationContext> context,
      const FileSystemURL& url,
      const base::Time& last_access_time,
      const base::Time& last_modified_time,
      const StatusCallback& callback) OVERRIDE;
  virtual void Truncate(
      scoped_ptr<FileSystemOperationContext> context,
      const FileSystemURL& url,
      int64 length,
      const StatusCallback& callback) OVERRIDE;
  virtual void CopyFileLocal(
      scoped_ptr<FileSystemOperationContext> context,
      const FileSystemURL& src_url,
      const FileSystemURL& dest_url,
      CopyOrMoveOption option,
      const CopyFileProgressCallback& progress_callback,
      const StatusCallback& callback) OVERRIDE;
  virtual void MoveFileLocal(
      scoped_ptr<FileSystemOperationContext> context,
      const FileSystemURL& src_url,
      const FileSystemURL& dest_url,
      CopyOrMoveOption option,
      const StatusCallback& callback) OVERRIDE;
  virtual void CopyInForeignFile(
      scoped_ptr<FileSystemOperationContext> context,
      const base::FilePath& src_file_path,
      const FileSystemURL& dest_url,
      const StatusCallback& callback) OVERRIDE;
  virtual void DeleteFile(
      scoped_ptr<FileSystemOperationContext> context,
      const FileSystemURL& url,
      const StatusCallback& callback) OVERRIDE;
  virtual void DeleteDirectory(
      scoped_ptr<FileSystemOperationContext> context,
      const FileSystemURL& url,
      const StatusCallback& callback) OVERRIDE;
  virtual void DeleteRecursively(
      scoped_ptr<FileSystemOperationContext> context,
      const FileSystemURL& url,
      const StatusCallback& callback) OVERRIDE;
  virtual void CreateSnapshotFile(
      scoped_ptr<FileSystemOperationContext> context,
      const FileSystemURL& url,
      const CreateSnapshotFileCallback& callback) OVERRIDE;

 private:
  scoped_ptr<FileSystemFileUtil> sync_file_util_;

  DISALLOW_COPY_AND_ASSIGN(AsyncFileUtilAdapter);
};

}  

#endif  
