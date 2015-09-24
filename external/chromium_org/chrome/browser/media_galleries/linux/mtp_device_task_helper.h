// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_LINUX_MTP_DEVICE_TASK_HELPER_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_LINUX_MTP_DEVICE_TASK_HELPER_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "chrome/browser/media_galleries/fileapi/mtp_device_async_delegate.h"
#include "device/media_transfer_protocol/mtp_file_entry.pb.h"
#include "webkit/browser/fileapi/async_file_util.h"

class MTPReadFileWorker;
struct SnapshotRequestInfo;

class MTPDeviceTaskHelper {
 public:
  typedef base::Callback<void(bool succeeded)> OpenStorageCallback;

  typedef MTPDeviceAsyncDelegate::GetFileInfoSuccessCallback
      GetFileInfoSuccessCallback;

  typedef base::Callback<void(const fileapi::AsyncFileUtil::EntryList&)>
      ReadDirectorySuccessCallback;

  typedef MTPDeviceAsyncDelegate::ErrorCallback ErrorCallback;

  MTPDeviceTaskHelper();
  ~MTPDeviceTaskHelper();

  
  
  
  
  
  
  void OpenStorage(const std::string& storage_name,
                   const OpenStorageCallback& callback);

  
  
  
  
  
  
  
  
  
  
  void GetFileInfoByPath(
      const std::string& file_path,
      const GetFileInfoSuccessCallback& success_callback,
      const ErrorCallback& error_callback);

  
  
  
  
  
  
  
  
  
  
  void ReadDirectoryByPath(const std::string& dir_path,
                           const ReadDirectorySuccessCallback& success_callback,
                           const ErrorCallback& error_callback);

  
  
  
  
  
  void WriteDataIntoSnapshotFile(
      const SnapshotRequestInfo& request_info,
      const base::PlatformFileInfo& snapshot_file_info);

  
  void CloseStorage() const;

 private:
  
  
  
  
  
  
  
  void OnDidOpenStorage(const OpenStorageCallback& callback,
                        const std::string& device_handle,
                        bool error);

  
  
  
  
  
  
  
  
  
  void OnGetFileInfo(const GetFileInfoSuccessCallback& success_callback,
                     const ErrorCallback& error_callback,
                     const MtpFileEntry& file_entry,
                     bool error) const;

  
  
  
  
  
  
  
  
  void OnDidReadDirectoryByPath(
      const ReadDirectorySuccessCallback& success_callback,
      const ErrorCallback& error_callback,
      const std::vector<MtpFileEntry>& file_entries,
      bool error) const;

  
  
  
  
  void HandleDeviceError(const ErrorCallback& error_callback,
                         base::PlatformFileError error) const;

  
  std::string device_handle_;

  
  scoped_ptr<MTPReadFileWorker> read_file_worker_;

  
  base::WeakPtrFactory<MTPDeviceTaskHelper> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(MTPDeviceTaskHelper);
};

#endif  
