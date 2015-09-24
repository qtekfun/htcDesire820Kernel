// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_MAC_MTP_DEVICE_DELEGATE_IMPL_MAC_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_MAC_MTP_DEVICE_DELEGATE_IMPL_MAC_H_

#include <list>
#include <map>
#include <vector>

#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "chrome/browser/media_galleries/fileapi/mtp_device_async_delegate.h"

class MTPDeviceDelegateImplMac : public MTPDeviceAsyncDelegate {
 public:
  MTPDeviceDelegateImplMac(const std::string& device_id,
                           const base::FilePath::StringType& synthetic_path);

  
  
  
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

  
  
  virtual void ItemAdded(const std::string& name,
                         const base::PlatformFileInfo& info);
  virtual void NoMoreItems();
  virtual void DownloadedFile(const std::string& name,
                              base::PlatformFileError error);

  
  
  
  void ReadDirectoryTimeout(const base::FilePath& root);

 private:
  class DeviceListener;

  virtual ~MTPDeviceDelegateImplMac();

  
  void GetFileInfoImpl(const base::FilePath& file_path,
                       base::PlatformFileInfo* file_info,
                       base::PlatformFileError* error);

  
  void ReadDirectoryImpl(
      const base::FilePath& root,
      const ReadDirectorySuccessCallback& success_callback,
      const ErrorCallback& error_callback);

  
  void DownloadFile(
      const base::FilePath& device_file_path,
      const base::FilePath& local_path,
      const CreateSnapshotFileSuccessCallback& success_callback,
      const ErrorCallback& error_callback);

  
  
  void CancelAndDelete();

  
  void CancelDownloads();

  
  
  void NotifyReadDir();

  std::string device_id_;
  base::FilePath root_path_;

  
  scoped_ptr<DeviceListener> camera_interface_;

  
  base::hash_map<base::FilePath::StringType,
                 base::PlatformFileInfo> file_info_;

  
  std::vector<base::FilePath> file_paths_;

  
  bool received_all_files_;

  struct ReadFileRequest {
    ReadFileRequest();
    ReadFileRequest(const std::string& request_file,
                    const base::FilePath& snapshot_filename,
                    CreateSnapshotFileSuccessCallback success_cb,
                    ErrorCallback error_cb);
    ~ReadFileRequest();

    std::string request_file;
    base::FilePath snapshot_file;
    CreateSnapshotFileSuccessCallback success_callback;
    ErrorCallback error_callback;
  };

  typedef std::list<ReadFileRequest> ReadFileTransactionList;

  struct ReadDirectoryRequest {
    ReadDirectoryRequest(const base::FilePath& dir,
                         ReadDirectorySuccessCallback success_cb,
                         ErrorCallback error_cb);
    ~ReadDirectoryRequest();

    base::FilePath directory;
    ReadDirectorySuccessCallback success_callback;
    ErrorCallback error_callback;
  };

  typedef std::list<ReadDirectoryRequest> ReadDirTransactionList;

  ReadFileTransactionList read_file_transactions_;
  ReadDirTransactionList read_dir_transactions_;

  base::WeakPtrFactory<MTPDeviceDelegateImplMac> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(MTPDeviceDelegateImplMac);
};

#endif  
