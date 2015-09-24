// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_MEDIA_TRANSFER_PROTOCOL_MEDIA_TRANSFER_PROTOCOL_MANAGER_H_
#define DEVICE_MEDIA_TRANSFER_PROTOCOL_MEDIA_TRANSFER_PROTOCOL_MANAGER_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "build/build_config.h"

#if !defined(OS_LINUX)
#error "Only used on Linux and ChromeOS"
#endif

class MtpFileEntry;
class MtpStorageInfo;

namespace base {
class SequencedTaskRunner;
}

namespace device {

class MediaTransferProtocolManager {
 public:
  
  
  
  typedef base::Callback<void(const std::string& handle,
                              bool error)> OpenStorageCallback;

  
  
  typedef base::Callback<void(bool error)> CloseStorageCallback;

  
  
  
  typedef base::Callback<void(const std::vector<MtpFileEntry>& file_entries,
                              bool error)> ReadDirectoryCallback;

  
  
  
  typedef base::Callback<void(const std::string& data,
                              bool error)> ReadFileCallback;

  
  
  
  typedef base::Callback<void(const MtpFileEntry& file_entry,
                              bool error)> GetFileInfoCallback;

  
  
  class Observer {
   public:
    virtual ~Observer() {}

    
    virtual void StorageChanged(bool is_attached,
                                const std::string& storage_name) = 0;
  };

  virtual ~MediaTransferProtocolManager() {}

  
  virtual void AddObserver(Observer* observer) = 0;

  
  virtual void RemoveObserver(Observer* observer) = 0;

  
  virtual const std::vector<std::string> GetStorages() const = 0;

  
  
  virtual const MtpStorageInfo* GetStorageInfo(
      const std::string& storage_name) const = 0;

  
  virtual void OpenStorage(const std::string& storage_name,
                           const std::string& mode,
                           const OpenStorageCallback& callback) = 0;

  
  virtual void CloseStorage(const std::string& storage_handle,
                            const CloseStorageCallback& callback) = 0;

  
  
  virtual void ReadDirectoryByPath(const std::string& storage_handle,
                                   const std::string& path,
                                   const ReadDirectoryCallback& callback) = 0;

  
  
  virtual void ReadDirectoryById(const std::string& storage_handle,
                                 uint32 file_id,
                                 const ReadDirectoryCallback& callback) = 0;

  
  
  virtual void ReadFileChunkByPath(const std::string& storage_handle,
                                   const std::string& path,
                                   uint32 offset,
                                   uint32 count,
                                   const ReadFileCallback& callback) = 0;

  
  
  virtual void ReadFileChunkById(const std::string& storage_handle,
                                 uint32 file_id,
                                 uint32 offset,
                                 uint32 count,
                                 const ReadFileCallback& callback) = 0;

  
  virtual void GetFileInfoByPath(const std::string& storage_handle,
                                 const std::string& path,
                                 const GetFileInfoCallback& callback) = 0;

  
  
  virtual void GetFileInfoById(const std::string& storage_handle,
                               uint32 file_id,
                               const GetFileInfoCallback& callback) = 0;

  
  
  
  
  
  static MediaTransferProtocolManager* Initialize(
      scoped_refptr<base::SequencedTaskRunner> task_runner);
};

}  

#endif  
