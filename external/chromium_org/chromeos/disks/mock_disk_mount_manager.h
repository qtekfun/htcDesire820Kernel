// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DISKS_MOCK_DISK_MOUNT_MANAGER_H_
#define CHROMEOS_DISKS_MOCK_DISK_MOUNT_MANAGER_H_

#include <string>

#include "base/observer_list.h"
#include "chromeos/dbus/cros_disks_client.h"
#include "chromeos/disks/disk_mount_manager.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace chromeos {
namespace disks {

class MockDiskMountManager : public DiskMountManager {
 public:
  MockDiskMountManager();
  virtual ~MockDiskMountManager();

  
  MOCK_METHOD0(Init, void(void));
  MOCK_METHOD1(AddObserver, void(DiskMountManager::Observer*));
  MOCK_METHOD1(RemoveObserver, void(DiskMountManager::Observer*));
  MOCK_CONST_METHOD0(disks, const DiskMountManager::DiskMap&(void));
  MOCK_CONST_METHOD1(FindDiskBySourcePath,
                     const DiskMountManager::Disk*(const std::string&));
  MOCK_CONST_METHOD0(mount_points,
                     const DiskMountManager::MountPointMap&(void));
  MOCK_METHOD0(RequestMountInfoRefresh, void(void));
  MOCK_METHOD4(MountPath, void(const std::string&, const std::string&,
                               const std::string&, MountType));
  MOCK_METHOD3(UnmountPath, void(const std::string&,
                                 UnmountOptions,
                                 const DiskMountManager::UnmountPathCallback&));
  MOCK_METHOD1(FormatMountedDevice, void(const std::string&));
  MOCK_METHOD2(
      UnmountDeviceRecursively,
      void(const std::string&,
           const DiskMountManager::UnmountDeviceRecursivelyCallbackType&));

  
  void NotifyDeviceInsertEvents();

  
  void NotifyDeviceRemoveEvents();

  
  void SetupDefaultReplies();

  
  
  void CreateDiskEntryForMountDevice(
      const DiskMountManager::MountPointInfo& mount_info,
      const std::string& device_id,
      const std::string& device_label,
      const std::string& vendor_name,
      const std::string& product_name,
      DeviceType device_type,
      uint64 total_size_in_bytes);

  
  
  void RemoveDiskEntryForMountDevice(
      const DiskMountManager::MountPointInfo& mount_info);

 private:
  
  void AddObserverInternal(DiskMountManager::Observer* observer);

  
  void RemoveObserverInternal(DiskMountManager::Observer* observer);

  
  const DiskMountManager::DiskMap& disksInternal() const { return disks_; }

  const DiskMountManager::MountPointMap& mountPointsInternal() const;

  
  const DiskMountManager::Disk* FindDiskBySourcePathInternal(
      const std::string& source_path) const;

  
  void NotifyDeviceChanged(DeviceEvent event,
                           const std::string& path);

  
  void NotifyDiskChanged(DiskEvent event,
                         const DiskMountManager::Disk* disk);

  
  ObserverList<DiskMountManager::Observer> observers_;

  
  DiskMountManager::DiskMap disks_;

  
  DiskMountManager::MountPointMap mount_points_;

  DISALLOW_COPY_AND_ASSIGN(MockDiskMountManager);
};

}  
}  

#endif  
