// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_WIN_MTP_DEVICE_DELEGATE_IMPL_WIN_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_WIN_MTP_DEVICE_DELEGATE_IMPL_WIN_H_

#include <queue>

#include "base/callback.h"
#include "base/location.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "base/strings/string16.h"
#include "base/win/scoped_comptr.h"
#include "chrome/browser/media_galleries/fileapi/mtp_device_async_delegate.h"
#include "webkit/browser/fileapi/async_file_util.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
}

class SnapshotFileDetails;
struct SnapshotRequestInfo;

class MTPDeviceDelegateImplWin : public MTPDeviceAsyncDelegate {
 public:
  
  struct StorageDeviceInfo {
    StorageDeviceInfo(const base::string16& pnp_device_id,
                      const base::string16& registered_device_path,
                      const base::string16& storage_object_id);

    
    
    const base::string16 pnp_device_id;

    
    
    
    const base::string16 registered_device_path;

    
    
    const base::string16 storage_object_id;
  };

 private:
  friend void OnGetStorageInfoCreateDelegate(
      const base::string16& device_location,
      const CreateMTPDeviceAsyncDelegateCallback& callback,
      base::string16* pnp_device_id,
      base::string16* storage_object_id,
      bool succeeded);

  enum InitializationState {
    UNINITIALIZED = 0,
    PENDING_INIT,
    INITIALIZED
  };

  
  struct PendingTaskInfo {
    PendingTaskInfo(const tracked_objects::Location& location,
                    const base::Callback<base::PlatformFileError(void)>& task,
                    const base::Callback<void(base::PlatformFileError)>& reply);

    const tracked_objects::Location location;
    const base::Callback<base::PlatformFileError(void)> task;
    const base::Callback<void(base::PlatformFileError)> reply;
  };

  
  
  MTPDeviceDelegateImplWin(const base::string16& registered_device_path,
                           const base::string16& pnp_device_id,
                           const base::string16& storage_object_id);

  
  virtual ~MTPDeviceDelegateImplWin();

  
  virtual void GetFileInfo(
      const base::FilePath& file_path,
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

  
  
  
  void WriteDataChunkIntoSnapshotFile();

  
  void ProcessNextPendingRequest();

  
  
  
  void OnInitCompleted(bool succeeded);

  
  
  
  
  
  
  
  
  void OnGetFileInfo(const GetFileInfoSuccessCallback& success_callback,
                     const ErrorCallback& error_callback,
                     base::PlatformFileInfo* file_info,
                     base::PlatformFileError error);

  
  
  
  
  
  
  
  
  void OnDidReadDirectory(const ReadDirectorySuccessCallback& success_callback,
                          const ErrorCallback& error_callback,
                          fileapi::AsyncFileUtil::EntryList* file_list,
                          base::PlatformFileError error);

  
  
  
  
  
  
  
  
  
  void OnGetFileStream(scoped_ptr<SnapshotFileDetails> file_details,
                       base::PlatformFileError error);

  
  // |bytes_written| specifies the number of bytes written into the
  
  
  // If the write operation succeeds, |bytes_written| is set to a non-zero
  
  
  // If the write operation fails, |bytes_written| is set to zero.
  void OnWroteDataChunkIntoSnapshotFile(
      const base::FilePath& snapshot_file_path,
      DWORD bytes_written);

  
  InitializationState init_state_;

  
  scoped_refptr<base::SequencedTaskRunner> media_task_runner_;

  
  
  StorageDeviceInfo storage_device_info_;

  
  // written to the snapshot file, optimal data transfer size, source file
  
  
  
  
  
  
  scoped_ptr<SnapshotFileDetails> current_snapshot_details_;

  
  
  std::queue<PendingTaskInfo> pending_tasks_;

  
  bool task_in_progress_;

  
  base::WeakPtrFactory<MTPDeviceDelegateImplWin> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(MTPDeviceDelegateImplWin);
};

#endif  
