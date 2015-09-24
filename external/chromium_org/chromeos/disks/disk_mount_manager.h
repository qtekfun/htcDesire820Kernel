// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DISKS_DISK_MOUNT_MANAGER_H_
#define CHROMEOS_DISKS_DISK_MOUNT_MANAGER_H_

#include <map>

#include "base/callback_forward.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/cros_disks_client.h"

namespace chromeos {
namespace disks {

enum MountCondition {
  MOUNT_CONDITION_NONE,
  MOUNT_CONDITION_UNKNOWN_FILESYSTEM,
  MOUNT_CONDITION_UNSUPPORTED_FILESYSTEM,
};

class CHROMEOS_EXPORT DiskMountManager {
 public:
  
  enum DiskEvent {
    DISK_ADDED,
    DISK_REMOVED,
    DISK_CHANGED,
  };

  enum DeviceEvent {
    DEVICE_ADDED,
    DEVICE_REMOVED,
    DEVICE_SCANNED,
  };

  enum MountEvent {
    MOUNTING,
    UNMOUNTING,
  };

  enum FormatEvent {
    FORMAT_STARTED,
    FORMAT_COMPLETED
  };

  
  class Disk {
   public:
    Disk(const std::string& device_path,
         const std::string& mount_path,
         const std::string& system_path,
         const std::string& file_path,
         const std::string& device_label,
         const std::string& drive_label,
         const std::string& vendor_id,
         const std::string& vendor_name,
         const std::string& product_id,
         const std::string& product_name,
         const std::string& fs_uuid,
         const std::string& system_path_prefix,
         DeviceType device_type,
         uint64 total_size_in_bytes,
         bool is_parent,
         bool is_read_only,
         bool has_media,
         bool on_boot_device,
         bool is_hidden);
    ~Disk();

    
    
    const std::string& device_path() const { return device_path_; }

    
    
    const std::string&  mount_path() const { return mount_path_; }

    
    
    const std::string& system_path() const { return system_path_; }

    
    
    const std::string& file_path() const { return file_path_; }

    
    const std::string& device_label() const { return device_label_; }

    
    
    const std::string& drive_label() const { return drive_label_; }

    
    const std::string& vendor_id() const { return vendor_id_; }

    
    const std::string& vendor_name() const { return vendor_name_; }

    
    const std::string& product_id() const { return product_id_; }

    
    const std::string& product_name() const { return product_name_; }

    
    const std::string& fs_uuid() const { return fs_uuid_; }

    
    
    const std::string& system_path_prefix() const {
      return system_path_prefix_;
    }

    
    DeviceType device_type() const { return device_type_; }

    
    uint64 total_size_in_bytes() const { return total_size_in_bytes_; }

    
    bool is_parent() const { return is_parent_; }

    
    bool is_read_only() const { return is_read_only_; }

    
    bool has_media() const { return has_media_; }

    
    bool on_boot_device() const { return on_boot_device_; }

    
    bool is_hidden() const { return is_hidden_; }

    void set_mount_path(const std::string& mount_path) {
      mount_path_ = mount_path;
    }

    void clear_mount_path() { mount_path_.clear(); }

   private:
    std::string device_path_;
    std::string mount_path_;
    std::string system_path_;
    std::string file_path_;
    std::string device_label_;
    std::string drive_label_;
    std::string vendor_id_;
    std::string vendor_name_;
    std::string product_id_;
    std::string product_name_;
    std::string fs_uuid_;
    std::string system_path_prefix_;
    DeviceType device_type_;
    uint64 total_size_in_bytes_;
    bool is_parent_;
    bool is_read_only_;
    bool has_media_;
    bool on_boot_device_;
    bool is_hidden_;
  };
  typedef std::map<std::string, Disk*> DiskMap;

  
  struct MountPointInfo {
    
    std::string source_path;
    
    std::string mount_path;
    
    MountType mount_type;
    
    MountCondition mount_condition;

    MountPointInfo(const std::string& source,
                   const std::string& mount,
                   const MountType type,
                   MountCondition condition)
        : source_path(source),
          mount_path(mount),
          mount_type(type),
          mount_condition(condition) {
    }
  };

  
  typedef std::map<std::string, MountPointInfo> MountPointMap;

  
  
  typedef base::Callback<void(bool)> UnmountDeviceRecursivelyCallbackType;

  
  typedef base::Callback<void(MountError error_code)> UnmountPathCallback;

  
  class Observer {
   public:
    virtual ~Observer() {}

    
    virtual void OnDiskEvent(DiskEvent event, const Disk* disk) = 0;
    
    virtual void OnDeviceEvent(DeviceEvent event,
                               const std::string& device_path) = 0;
    
    virtual void OnMountEvent(MountEvent event,
                              MountError error_code,
                              const MountPointInfo& mount_info) = 0;
    
    virtual void OnFormatEvent(FormatEvent event,
                               FormatError error_code,
                               const std::string& device_path) = 0;
  };

  virtual ~DiskMountManager() {}

  
  virtual void AddObserver(Observer* observer) = 0;

  
  virtual void RemoveObserver(Observer* observer) = 0;

  
  virtual const DiskMap& disks() const = 0;

  
  virtual const Disk* FindDiskBySourcePath(
      const std::string& source_path) const = 0;

  
  virtual const MountPointMap& mount_points() const = 0;

  
  virtual void RequestMountInfoRefresh() = 0;

  
  
  
  
  virtual void MountPath(const std::string& source_path,
                         const std::string& source_format,
                         const std::string& mount_label,
                         MountType type) = 0;

  
  
  
  
  
  
  virtual void UnmountPath(const std::string& mount_path,
                           UnmountOptions options,
                           const UnmountPathCallback& callback) = 0;

  
  
  virtual void FormatMountedDevice(const std::string& mount_path) = 0;

  
  virtual void UnmountDeviceRecursively(
      const std::string& device_path,
      const UnmountDeviceRecursivelyCallbackType& callback) = 0;

  
  
  virtual bool AddDiskForTest(Disk* disk);
  virtual bool AddMountPointForTest(const MountPointInfo& mount_point);

  
  static std::string MountConditionToString(MountCondition type);

  
  static std::string DeviceTypeToString(DeviceType type);

  
  static void Initialize();

  
  
  
  
  static void InitializeForTesting(DiskMountManager* disk_mount_manager);

  
  static void Shutdown();

  
  
  static DiskMountManager* GetInstance();
};

}  
}  

#endif  
