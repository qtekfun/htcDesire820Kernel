// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILE_MANAGER_VOLUME_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_FILE_MANAGER_VOLUME_MANAGER_H_

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/chromeos/drive/drive_integration_service.h"
#include "chromeos/disks/disk_mount_manager.h"
#include "chromeos/dbus/cros_disks_client.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class Profile;

namespace chromeos {
class PowerManagerClient;
}  

namespace content {
class BrowserContext;
}  

namespace drive {
class DriveIntegrationService;
}  

namespace file_manager {

class MountedDiskMonitor;
class VolumeManagerObserver;

enum VolumeType {
  VOLUME_TYPE_GOOGLE_DRIVE,
  VOLUME_TYPE_DOWNLOADS_DIRECTORY,
  VOLUME_TYPE_REMOVABLE_DISK_PARTITION,
  VOLUME_TYPE_MOUNTED_ARCHIVE_FILE,
};

struct VolumeInfo {
  VolumeInfo();
  ~VolumeInfo();

  
  std::string volume_id;

  
  VolumeType type;

  
  chromeos::DeviceType device_type;

  
  
  
  base::FilePath source_path;

  
  
  
  
  
  base::FilePath mount_path;

  
  chromeos::disks::MountCondition mount_condition;

  
  
  base::FilePath system_path_prefix;

  
  
  std::string drive_label;

  
  bool is_parent;

  
  bool is_read_only;
};

class VolumeManager : public BrowserContextKeyedService,
                      public drive::DriveIntegrationServiceObserver,
                      public chromeos::disks::DiskMountManager::Observer {
 public:
  VolumeManager(Profile* profile,
                drive::DriveIntegrationService* drive_integration_service,
                chromeos::PowerManagerClient* power_manager_client,
                chromeos::disks::DiskMountManager* disk_mount_manager);
  virtual ~VolumeManager();

  
  static VolumeManager* Get(content::BrowserContext* context);

  
  void Initialize();

  
  virtual void Shutdown() OVERRIDE;

  
  void AddObserver(VolumeManagerObserver* observer);

  
  void RemoveObserver(VolumeManagerObserver* observer);

  
  std::vector<VolumeInfo> GetVolumeInfoList() const;

  
  // result is written into |result|. Returns false otherwise.
  bool FindVolumeInfoById(const std::string& volume_id,
                          VolumeInfo* result) const;

  
  virtual void OnFileSystemMounted() OVERRIDE;
  virtual void OnFileSystemBeingUnmounted() OVERRIDE;

  
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

  
  void OnExternalStorageDisabledChanged();

 private:
  Profile* profile_;
  drive::DriveIntegrationService* drive_integration_service_;
  chromeos::disks::DiskMountManager* disk_mount_manager_;
  scoped_ptr<MountedDiskMonitor> mounted_disk_monitor_;
  PrefChangeRegistrar pref_change_registrar_;
  ObserverList<VolumeManagerObserver> observers_;
  DISALLOW_COPY_AND_ASSIGN(VolumeManager);
};

}  

#endif  
