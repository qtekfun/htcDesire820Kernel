// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILE_MANAGER_MOUNTED_DISK_MONITOR_H_
#define CHROME_BROWSER_CHROMEOS_FILE_MANAGER_MOUNTED_DISK_MONITOR_H_

#include <map>
#include <set>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/dbus/power_manager_client.h"
#include "chromeos/disks/disk_mount_manager.h"

namespace file_manager {

class MountedDiskMonitor
    : public chromeos::PowerManagerClient::Observer,
      public chromeos::disks::DiskMountManager::Observer {
 public:
  MountedDiskMonitor(
      chromeos::PowerManagerClient* power_manager_client,
      chromeos::disks::DiskMountManager* disk_mount_manager);
  virtual ~MountedDiskMonitor();

  
  virtual void SuspendImminent() OVERRIDE;
  virtual void SystemResumed(const base::TimeDelta& sleep_duration) OVERRIDE;

  
  virtual void OnDiskEvent(
      chromeos::disks::DiskMountManager::DiskEvent event,
      const chromeos::disks::DiskMountManager::Disk* disk) OVERRIDE;
  virtual void OnDeviceEvent(
      chromeos::disks::DiskMountManager::DeviceEvent event,
      const std::string& device_path) OVERRIDE;
  virtual void OnMountEvent(
      chromeos::disks::DiskMountManager::MountEvent event,
      chromeos::MountError error_code,
      const chromeos::disks::DiskMountManager::MountPointInfo& mount_info)
      OVERRIDE;
  virtual void OnFormatEvent(
      chromeos::disks::DiskMountManager::FormatEvent event,
      chromeos::FormatError error_code,
      const std::string& device_path) OVERRIDE;

  
  
  bool DiskIsRemounting(
      const chromeos::disks::DiskMountManager::Disk& disk) const;

  
  
  void set_resuming_time_span_for_testing(
      const base::TimeDelta& resuming_time_span) {
    resuming_time_span_ = resuming_time_span;
  }

 private:
  
  typedef std::map<std::string, std::string> DiskMap;

  
  typedef std::set<std::string> DiskSet;

  void Reset();

  chromeos::PowerManagerClient* power_manager_client_;
  chromeos::disks::DiskMountManager* disk_mount_manager_;

  bool is_resuming_;
  DiskMap mounted_disks_;
  DiskSet unmounted_while_resuming_;
  base::TimeDelta resuming_time_span_;
  base::WeakPtrFactory<MountedDiskMonitor> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(MountedDiskMonitor);
};

}  

#endif  
