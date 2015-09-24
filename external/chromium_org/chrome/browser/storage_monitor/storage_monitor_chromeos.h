// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_STORAGE_MONITOR_STORAGE_MONITOR_CHROMEOS_H_
#define CHROME_BROWSER_STORAGE_MONITOR_STORAGE_MONITOR_CHROMEOS_H_

#if !defined(OS_CHROMEOS)
#error "Should only be used on ChromeOS."
#endif

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/storage_monitor/storage_monitor.h"
#include "chromeos/disks/disk_mount_manager.h"

class MediaTransferProtocolDeviceObserverLinux;

namespace chromeos {

class StorageMonitorCros : public StorageMonitor,
                           public disks::DiskMountManager::Observer {
 public:
  
  
  StorageMonitorCros();
  virtual ~StorageMonitorCros();

  
  
  virtual void Init() OVERRIDE;

 protected:
  void SetMediaTransferProtocolManagerForTest(
      device::MediaTransferProtocolManager* test_manager);

  
  virtual void OnDiskEvent(disks::DiskMountManager::DiskEvent event,
                           const disks::DiskMountManager::Disk* disk) OVERRIDE;
  virtual void OnDeviceEvent(disks::DiskMountManager::DeviceEvent event,
                             const std::string& device_path) OVERRIDE;
  virtual void OnMountEvent(
      disks::DiskMountManager::MountEvent event,
      MountError error_code,
      const disks::DiskMountManager::MountPointInfo& mount_info) OVERRIDE;
  virtual void OnFormatEvent(disks::DiskMountManager::FormatEvent event,
                             FormatError error_code,
                             const std::string& device_path) OVERRIDE;

  
  virtual bool GetStorageInfoForPath(const base::FilePath& path,
                                     StorageInfo* device_info) const OVERRIDE;
  virtual void EjectDevice(
      const std::string& device_id,
      base::Callback<void(EjectStatus)> callback) OVERRIDE;
  virtual device::MediaTransferProtocolManager*
      media_transfer_protocol_manager() OVERRIDE;

 private:
  
  typedef std::map<std::string, StorageInfo> MountMap;

  
  
  void CheckExistingMountPoints();

  
  
  
  void AddMountedPath(const disks::DiskMountManager::MountPointInfo& mount_info,
                      bool has_dcim);

  
  MountMap mount_map_;

  scoped_ptr<device::MediaTransferProtocolManager>
      media_transfer_protocol_manager_;
  scoped_ptr<MediaTransferProtocolDeviceObserverLinux>
      media_transfer_protocol_device_observer_;

  base::WeakPtrFactory<StorageMonitorCros> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(StorageMonitorCros);
};

}  

#endif  
