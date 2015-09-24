// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STORAGE_MONITOR_STORAGE_MONITOR_H_
#define CHROME_BROWSER_STORAGE_MONITOR_STORAGE_MONITOR_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/observer_list_threadsafe.h"
#include "base/strings/string16.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "chrome/browser/storage_monitor/storage_info.h"

class MediaFileSystemRegistryTest;
class MediaGalleriesPlatformAppBrowserTest;
class MediaGalleriesPrivateApiTest;
class RemovableStorageObserver;
class SystemStorageApiTest;
class SystemStorageEjectApiTest;
class TransientDeviceIds;

namespace device {
class MediaTransferProtocolManager;
}

class StorageMonitor {
 public:
  
  class Receiver {
   public:
    virtual ~Receiver();

    virtual void ProcessAttach(const StorageInfo& info) = 0;
    virtual void ProcessDetach(const std::string& id) = 0;
    virtual void MarkInitialized() = 0;
  };

  
  enum EjectStatus {
    EJECT_OK,
    EJECT_IN_USE,
    EJECT_NO_SUCH_DEVICE,
    EJECT_FAILURE
  };

  
  
  static StorageMonitor* Create();

  
  
  
  
  static StorageMonitor* GetInstance();

  virtual ~StorageMonitor();

  
  
  
  
  
  
  
  void EnsureInitialized(base::Closure callback);

  
  bool IsInitialized() const;

  
  
  
  virtual bool GetStorageInfoForPath(
      const base::FilePath& path,
      StorageInfo* device_info) const = 0;

#if defined(OS_WIN)
  
  
  
  
  
  virtual bool GetMTPStorageInfoFromDeviceId(
      const std::string& storage_device_id,
      base::string16* device_location,
      base::string16* storage_object_id) const = 0;
#endif

#if defined(OS_LINUX)
  virtual device::MediaTransferProtocolManager*
      media_transfer_protocol_manager() = 0;
#endif

  
  
  std::vector<StorageInfo> GetAllAvailableStorages() const;

  void AddObserver(RemovableStorageObserver* obs);
  void RemoveObserver(RemovableStorageObserver* obs);

  std::string GetTransientIdForDeviceId(const std::string& device_id);
  std::string GetDeviceIdForTransientId(const std::string& transient_id) const;

  virtual void EjectDevice(
      const std::string& device_id,
      base::Callback<void(EjectStatus)> callback);

 protected:
  friend class ::MediaFileSystemRegistryTest;
  friend class ::MediaGalleriesPlatformAppBrowserTest;
  friend class ::MediaGalleriesPrivateApiTest;
  friend class ::SystemStorageApiTest;
  friend class ::SystemStorageEjectApiTest;

  StorageMonitor();

  virtual Receiver* receiver() const;

  
  virtual void Init() = 0;

  
  
  void MarkInitialized();

 private:
  class ReceiverImpl;
  friend class ReceiverImpl;

  
  typedef std::map<std::string, StorageInfo> StorageMap;

  void ProcessAttach(const StorageInfo& storage);
  void ProcessDetach(const std::string& id);

  scoped_ptr<Receiver> receiver_;

  scoped_refptr<ObserverListThreadSafe<RemovableStorageObserver> >
      observer_list_;

  
  base::ThreadChecker thread_checker_;

  bool initializing_;
  bool initialized_;
  std::vector<base::Closure> on_initialize_callbacks_;

  
  mutable base::Lock storage_lock_;

  
  StorageMap storage_map_;

  scoped_ptr<TransientDeviceIds> transient_device_ids_;
};

#endif  
