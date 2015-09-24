// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_LINUX_MTP_DEVICE_DELEGATE_IMPL_LINUX_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_LINUX_MTP_DEVICE_DELEGATE_IMPL_LINUX_H_

#include <queue>

#include "base/callback.h"
#include "base/location.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "chrome/browser/media_galleries/fileapi/mtp_device_async_delegate.h"
#include "webkit/browser/fileapi/async_file_util.h"

namespace base {
class FilePath;
}

struct SnapshotRequestInfo;

class MTPDeviceDelegateImplLinux : public MTPDeviceAsyncDelegate {
 private:
  friend void CreateMTPDeviceAsyncDelegate(
      const std::string&,
      const CreateMTPDeviceAsyncDelegateCallback&);

  enum InitializationState {
    UNINITIALIZED = 0,
    PENDING_INIT,
    INITIALIZED
  };

  
  struct PendingTaskInfo {
    PendingTaskInfo(const tracked_objects::Location& location,
                    const base::Closure& task);
    ~PendingTaskInfo();

    const tracked_objects::Location location;
    const base::Closure task;
  };

  
  
  
  explicit MTPDeviceDelegateImplLinux(const std::string& device_location);

  
  virtual ~MTPDeviceDelegateImplLinux();

  
  virtual void GetFileInfo(const base::FilePath& file_path,
                           const GetFileInfoSuccessCallback& success_callback,
                           const ErrorCallback& error_callback) OVERRIDE;
  virtual void ReadDirectory(
      const base::FilePath& root,
      const ReadDirectorySuccessCallback& success_callback,
      const ErrorCallback& error_callback) OVERRIDE;
  virtual void CreateSnapshotFile(
      const base::FilePath& device_file_path,
      const base::FilePath& local_path,
      const CreateSnapshotFileSuccessCallback& success_callback,
      const ErrorCallback& error_callback) OVERRIDE;
  virtual void CancelPendingTasksAndDeleteDelegate() OVERRIDE;

  
  
  
  
  
  
  
  
  
  void EnsureInitAndRunTask(const PendingTaskInfo& task_info);

  
  
  
  
  
  void WriteDataIntoSnapshotFile(
      const base::PlatformFileInfo& snapshot_file_info);

  
  void ProcessNextPendingRequest();

  
  
  
  
  void OnInitCompleted(bool succeeded);

  
  
  
  void OnDidGetFileInfo(const GetFileInfoSuccessCallback& success_callback,
                        const base::PlatformFileInfo& file_info);

  
  
  
  
  
  
  
  
  
  void OnDidGetFileInfoToReadDirectory(
      const std::string& root,
      const ReadDirectorySuccessCallback& success_callback,
      const ErrorCallback& error_callback,
      const base::PlatformFileInfo& file_info);

  
  
  
  
  
  
  void OnDidGetFileInfoToCreateSnapshotFile(
      scoped_ptr<SnapshotRequestInfo> snapshot_request_info,
      const base::PlatformFileInfo& file_info);

  
  
  
  
  
  void OnDidReadDirectory(const ReadDirectorySuccessCallback& success_callback,
                          const fileapi::AsyncFileUtil::EntryList& file_list);

  
  
  
  
  
  
  void OnDidWriteDataIntoSnapshotFile(
      const base::PlatformFileInfo& snapshot_file_info,
      const base::FilePath& snapshot_file_path);

  
  
  
  
  
  
  void OnWriteDataIntoSnapshotFileError(base::PlatformFileError error);

  
  
  void HandleDeviceFileError(const ErrorCallback& error_callback,
                             base::PlatformFileError error);

  
  InitializationState init_state_;

  
  bool task_in_progress_;

  
  
  const base::FilePath device_path_;

  
  std::string storage_name_;

  
  
  std::queue<PendingTaskInfo> pending_tasks_;

  
  
  
  
  
  scoped_ptr<SnapshotRequestInfo> current_snapshot_request_info_;

  
  base::WeakPtrFactory<MTPDeviceDelegateImplLinux> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(MTPDeviceDelegateImplLinux);
};

#endif  
