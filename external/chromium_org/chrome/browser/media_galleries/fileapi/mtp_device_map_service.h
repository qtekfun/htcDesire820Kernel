// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_MTP_DEVICE_MAP_SERVICE_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_MTP_DEVICE_MAP_SERVICE_H_

#include <map>

#include "base/files/file_path.h"
#include "base/lazy_instance.h"
#include "base/threading/thread_checker.h"

class MTPDeviceAsyncDelegate;

class MTPDeviceMapService {
 public:
  static MTPDeviceMapService* GetInstance();

  
  
  
  
  MTPDeviceAsyncDelegate* GetMTPDeviceAsyncDelegate(
      const std::string& filesystem_id);

  
  void RegisterMTPFileSystem(
    const base::FilePath::StringType& device_location,
    const std::string& fsid);

  
  
  
  void RevokeMTPFileSystem(const std::string& fsid);

 private:
  friend struct base::DefaultLazyInstanceTraits<MTPDeviceMapService>;

  
  
  
  void AddAsyncDelegate(const base::FilePath::StringType& device_location,
                        MTPDeviceAsyncDelegate* delegate);

  
  
  
  void RemoveAsyncDelegate(const base::FilePath::StringType& device_location);

  
  
  typedef std::map<base::FilePath::StringType, MTPDeviceAsyncDelegate*>
      AsyncDelegateMap;

  
  typedef std::map<std::string, base::FilePath::StringType>
      MTPDeviceFileSystemMap;

  
  
  typedef std::map<const base::FilePath::StringType, int>
      MTPDeviceUsageMap;


  
  MTPDeviceMapService();
  ~MTPDeviceMapService();

  
  AsyncDelegateMap async_delegate_map_;

  MTPDeviceFileSystemMap mtp_device_map_;

  MTPDeviceUsageMap mtp_device_usage_map_;

  DISALLOW_COPY_AND_ASSIGN(MTPDeviceMapService);
};

#endif  
