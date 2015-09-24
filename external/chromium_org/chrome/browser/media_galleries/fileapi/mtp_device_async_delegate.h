// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_MTP_DEVICE_ASYNC_DELEGATE_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_MTP_DEVICE_ASYNC_DELEGATE_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/platform_file.h"
#include "webkit/browser/fileapi/async_file_util.h"

namespace base {
class FilePath;
}

class MTPDeviceAsyncDelegate {
 public:
  
  typedef base::Callback<
      void(const base::PlatformFileInfo& file_info)> GetFileInfoSuccessCallback;

  
  typedef base::Callback<
      void(const fileapi::AsyncFileUtil::EntryList& file_list,
           bool has_more)> ReadDirectorySuccessCallback;

  
  
  typedef base::Callback<
      void(base::PlatformFileError error)> ErrorCallback;

  
  typedef base::Callback<
      void(const base::PlatformFileInfo& file_info,
           const base::FilePath& local_path)> CreateSnapshotFileSuccessCallback;

  
  
  virtual void GetFileInfo(
      const base::FilePath& file_path,
      const GetFileInfoSuccessCallback& success_callback,
      const ErrorCallback& error_callback) = 0;

  
  
  virtual void ReadDirectory(
      const base::FilePath& root,
      const ReadDirectorySuccessCallback& success_callback,
      const ErrorCallback& error_callback) = 0;

  
  
  virtual void CreateSnapshotFile(
      const base::FilePath& device_file_path,
      const base::FilePath& local_path,
      const CreateSnapshotFileSuccessCallback& success_callback,
      const ErrorCallback& error_callback) = 0;

  
  
  
  
  
  
  
  
  virtual void CancelPendingTasksAndDeleteDelegate() = 0;

 protected:
  
  virtual ~MTPDeviceAsyncDelegate() {}
};

typedef base::Callback<void(MTPDeviceAsyncDelegate*)>
    CreateMTPDeviceAsyncDelegateCallback;

void CreateMTPDeviceAsyncDelegate(
    const base::FilePath::StringType& device_location,
    const CreateMTPDeviceAsyncDelegateCallback& callback);

#endif  
