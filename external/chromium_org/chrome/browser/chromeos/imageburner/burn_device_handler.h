// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_IMAGEBURNER_BURN_DEVICE_HANDLER_H_
#define CHROME_BROWSER_CHROMEOS_IMAGEBURNER_BURN_DEVICE_HANDLER_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "chromeos/dbus/cros_disks_client.h"
#include "chromeos/disks/disk_mount_manager.h"

namespace chromeos {
namespace imageburner {

class BurnDeviceHandler : public disks::DiskMountManager::Observer {
 public:
  
  typedef base::Callback<void(const disks::DiskMountManager::Disk& disk)>
      DiskCallback;

  
  
  explicit BurnDeviceHandler(disks::DiskMountManager* disk_mount_manager);
  virtual ~BurnDeviceHandler();

  
  
  
  
  
  
  void SetCallbacks(const DiskCallback& add_callback,
                    const DiskCallback& remove_callback);

  
  std::vector<disks::DiskMountManager::Disk> GetBurnableDevices();

  
  virtual void OnDiskEvent(
      disks::DiskMountManager::DiskEvent event,
      const disks::DiskMountManager::Disk* disk) OVERRIDE;
  virtual void OnDeviceEvent(
      disks::DiskMountManager::DeviceEvent event,
      const std::string& device_path) OVERRIDE;
  virtual void OnMountEvent(
      disks::DiskMountManager::MountEvent event,
      MountError error_code,
      const disks::DiskMountManager::MountPointInfo& mount_info) OVERRIDE;
  virtual void OnFormatEvent(
      disks::DiskMountManager::FormatEvent event,
      FormatError error_code,
      const std::string& device_path) OVERRIDE;

 private:
  disks::DiskMountManager* disk_mount_manager_;  
  DiskCallback add_callback_;
  DiskCallback remove_callback_;

  DISALLOW_COPY_AND_ASSIGN(BurnDeviceHandler);
};

}  
}  

#endif  
