// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STORAGE_MONITOR_MEDIA_TRANSFER_PROTOCOL_DEVICE_OBSERVER_LINUX_H_
#define CHROME_BROWSER_STORAGE_MONITOR_MEDIA_TRANSFER_PROTOCOL_DEVICE_OBSERVER_LINUX_H_

#include <map>
#include <string>

#include "base/strings/string16.h"
#include "chrome/browser/storage_monitor/storage_monitor.h"
#include "device/media_transfer_protocol/media_transfer_protocol_manager.h"

namespace base {
class FilePath;
}

typedef void (*GetStorageInfoFunc)(
    const std::string& storage_name,
    device::MediaTransferProtocolManager* mtp_manager,
    std::string* id,
    base::string16* name,
    std::string* location);

class MediaTransferProtocolDeviceObserverLinux
    : public device::MediaTransferProtocolManager::Observer {
 public:
  MediaTransferProtocolDeviceObserverLinux(
      StorageMonitor::Receiver* receiver,
      device::MediaTransferProtocolManager* mtp_manager);
  virtual ~MediaTransferProtocolDeviceObserverLinux();

  
  
  bool GetStorageInfoForPath(const base::FilePath& path,
                             StorageInfo* storage_info) const;

  void EjectDevice(const std::string& device_id,
                   base::Callback<void(StorageMonitor::EjectStatus)> callback);

 protected:
  
  MediaTransferProtocolDeviceObserverLinux(
      StorageMonitor::Receiver* receiver,
      device::MediaTransferProtocolManager* mtp_manager,
      GetStorageInfoFunc get_storage_info_func);

  
  
  virtual void StorageChanged(bool is_attached,
                              const std::string& storage_name) OVERRIDE;

 private:
  
  typedef std::map<std::string, StorageInfo> StorageLocationToInfoMap;

  
  void EnumerateStorages();

  
  
  bool GetLocationForDeviceId(const std::string& device_id,
                              std::string* location) const;

  
  
  device::MediaTransferProtocolManager* mtp_manager_;

  
  StorageLocationToInfoMap storage_map_;

  
  
  GetStorageInfoFunc get_storage_info_func_;

  
  
  StorageMonitor::Receiver* const notifications_;

  DISALLOW_COPY_AND_ASSIGN(MediaTransferProtocolDeviceObserverLinux);
};

#endif  
