// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_DEVICE_MEDIA_ASYNC_FILE_UTIL_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_DEVICE_MEDIA_ASYNC_FILE_UTIL_H_

#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "webkit/browser/fileapi/async_file_util.h"
#include "webkit/common/blob/shareable_file_reference.h"

namespace base {
class SequencedTaskRunner;
class Time;
}

namespace fileapi {
class FileSystemOperationContext;
class FileSystemURL;
}

class DeviceMediaAsyncFileUtil : public fileapi::AsyncFileUtil {
 public:
  virtual ~DeviceMediaAsyncFileUtil();

  
  
  
  static DeviceMediaAsyncFileUtil* Create(const base::FilePath& profile_path);

  
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
  
  explicit DeviceMediaAsyncFileUtil(const base::FilePath& profile_path);

  
  
  
  void OnDidGetFileInfo(
      const AsyncFileUtil::GetFileInfoCallback& callback,
      const base::PlatformFileInfo& file_info);

  
  
  
  void OnGetFileInfoError(
      const AsyncFileUtil::GetFileInfoCallback& callback,
      base::PlatformFileError error);

  
  
  
  
  
  
  
  
  
  
  
  void OnDidReadDirectory(
      const AsyncFileUtil::ReadDirectoryCallback& callback,
      const AsyncFileUtil::EntryList& file_list,
      bool has_more);

  
  
  
  void OnReadDirectoryError(
      const AsyncFileUtil::ReadDirectoryCallback& callback,
      base::PlatformFileError error);

  
  
  
  void OnDidCreateSnapshotFile(
      const AsyncFileUtil::CreateSnapshotFileCallback& callback,
      base::SequencedTaskRunner* media_task_runner,
      const base::PlatformFileInfo& file_info,
      const base::FilePath& platform_path);

  
  
  void OnDidCheckMedia(
      const AsyncFileUtil::CreateSnapshotFileCallback& callback,
      const base::PlatformFileInfo& file_info,
      scoped_refptr<webkit_blob::ShareableFileReference> platform_file,
      base::PlatformFileError error);

  
  
  void OnCreateSnapshotFileError(
      const AsyncFileUtil::CreateSnapshotFileCallback& callback,
      base::PlatformFileError error);

  
  
  
  
  
  
  void OnSnapshotFileCreatedRunTask(
      scoped_ptr<fileapi::FileSystemOperationContext> context,
      const AsyncFileUtil::CreateSnapshotFileCallback& callback,
      const fileapi::FileSystemURL& url,
      base::FilePath* snapshot_file_path);

  
  const base::FilePath profile_path_;

  
  base::WeakPtrFactory<DeviceMediaAsyncFileUtil> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DeviceMediaAsyncFileUtil);
};

#endif  
