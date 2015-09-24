// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILE_MANAGER_VOLUME_MANAGER_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_FILE_MANAGER_VOLUME_MANAGER_OBSERVER_H_

#include <string>

#include "chromeos/dbus/cros_disks_client.h"
#include "chromeos/disks/disk_mount_manager.h"

namespace file_manager {

struct VolumeInfo;

class VolumeManagerObserver {
 public:
  virtual ~VolumeManagerObserver() {}

  
  virtual void OnDiskAdded(
      const chromeos::disks::DiskMountManager::Disk& disk, bool mounting) = 0;

  
  virtual void OnDiskRemoved(
      const chromeos::disks::DiskMountManager::Disk& disk) = 0;

  
  virtual void OnDeviceAdded(const std::string& device_path) = 0;

  
  virtual void OnDeviceRemoved(const std::string& device_path) = 0;

  
  virtual void OnVolumeMounted(chromeos::MountError error_code,
                               const VolumeInfo& volume_info,
                               bool is_remounting) = 0;

  
  virtual void OnVolumeUnmounted(chromeos::MountError error_code,
                                 const VolumeInfo& volume_info) = 0;

  
  virtual void OnFormatStarted(
      const std::string& device_path, bool success) = 0;

  
  virtual void OnFormatCompleted(
      const std::string& device_path, bool success) = 0;
};

}  

#endif  
