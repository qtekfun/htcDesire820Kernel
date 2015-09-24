// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_CROS_DISKS_CLIENT_H_
#define CHROMEOS_DBUS_CROS_DISKS_CLIENT_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "chromeos/dbus/dbus_client_implementation_type.h"

namespace base {
class FilePath;
}

namespace dbus {
class Response;
}

namespace chromeos {

enum MountType {
  MOUNT_TYPE_INVALID,
  MOUNT_TYPE_DEVICE,
  MOUNT_TYPE_ARCHIVE,
};

enum DeviceType {
  DEVICE_TYPE_UNKNOWN,
  DEVICE_TYPE_USB,  
  DEVICE_TYPE_SD,  
  DEVICE_TYPE_OPTICAL_DISC,  
  DEVICE_TYPE_MOBILE,  
  DEVICE_TYPE_DVD,  
};

enum MountError {
  MOUNT_ERROR_NONE = 0,
  MOUNT_ERROR_UNKNOWN = 1,
  MOUNT_ERROR_INTERNAL = 2,
  MOUNT_ERROR_INVALID_ARGUMENT = 3,
  MOUNT_ERROR_INVALID_PATH = 4,
  MOUNT_ERROR_PATH_ALREADY_MOUNTED = 5,
  MOUNT_ERROR_PATH_NOT_MOUNTED = 6,
  MOUNT_ERROR_DIRECTORY_CREATION_FAILED = 7,
  MOUNT_ERROR_INVALID_MOUNT_OPTIONS = 8,
  MOUNT_ERROR_INVALID_UNMOUNT_OPTIONS = 9,
  MOUNT_ERROR_INSUFFICIENT_PERMISSIONS = 10,
  MOUNT_ERROR_MOUNT_PROGRAM_NOT_FOUND = 11,
  MOUNT_ERROR_MOUNT_PROGRAM_FAILED = 12,
  MOUNT_ERROR_INVALID_DEVICE_PATH = 100,
  MOUNT_ERROR_UNKNOWN_FILESYSTEM = 101,
  MOUNT_ERROR_UNSUPPORTED_FILESYSTEM = 102,
  MOUNT_ERROR_INVALID_ARCHIVE = 201,
  MOUNT_ERROR_NOT_AUTHENTICATED = 601,
  MOUNT_ERROR_PATH_UNMOUNTED = 901,
  
  
};

enum FormatError {
  FORMAT_ERROR_NONE,
  FORMAT_ERROR_UNKNOWN,
  FORMAT_ERROR_INTERNAL,
  FORMAT_ERROR_INVALID_DEVICE_PATH,
  FORMAT_ERROR_DEVICE_BEING_FORMATTED,
  FORMAT_ERROR_UNSUPPORTED_FILESYSTEM,
  FORMAT_ERROR_FORMAT_PROGRAM_NOT_FOUND,
  FORMAT_ERROR_FORMAT_PROGRAM_FAILED,
  FORMAT_ERROR_DEVICE_NOT_ALLOWED,
};

enum MountEventType {
  CROS_DISKS_DISK_ADDED,
  CROS_DISKS_DISK_REMOVED,
  CROS_DISKS_DISK_CHANGED,
  CROS_DISKS_DEVICE_ADDED,
  CROS_DISKS_DEVICE_REMOVED,
  CROS_DISKS_DEVICE_SCANNED,
  CROS_DISKS_FORMATTING_FINISHED,
};

enum UnmountOptions {
  UNMOUNT_OPTIONS_NONE,
  UNMOUNT_OPTIONS_LAZY,  
};

class CHROMEOS_EXPORT DiskInfo {
 public:
  DiskInfo(const std::string& device_path, dbus::Response* response);
  ~DiskInfo();

  
  const std::string& device_path() const { return device_path_; }

  
  const std::string& mount_path() const { return mount_path_; }

  
  
  const std::string& system_path() const { return system_path_; }

  
  bool is_drive() const { return is_drive_; }

  
  bool has_media() const { return has_media_; }

  
  bool on_boot_device() const { return on_boot_device_; }

  
  const std::string& file_path() const { return file_path_; }

  
  const std::string& label() const { return label_; }

  
  const std::string& vendor_id() const { return vendor_id_; }

  
  const std::string& vendor_name() const { return vendor_name_; }

  
  const std::string& product_id() const { return product_id_; }

  
  const std::string& product_name() const { return product_name_; }

  
  const std::string& drive_label() const { return drive_model_; }

  
  DeviceType device_type() const { return device_type_; }

  
  uint64 total_size_in_bytes() const { return total_size_in_bytes_; }

  
  bool is_read_only() const { return is_read_only_; }

  
  bool is_hidden() const { return is_hidden_; }

  
  const std::string& uuid() const { return uuid_; }

 private:
  void InitializeFromResponse(dbus::Response* response);

  std::string device_path_;
  std::string mount_path_;
  std::string system_path_;
  bool is_drive_;
  bool has_media_;
  bool on_boot_device_;

  std::string file_path_;
  std::string label_;
  std::string vendor_id_;
  std::string vendor_name_;
  std::string product_id_;
  std::string product_name_;
  std::string drive_model_;
  DeviceType device_type_;
  uint64 total_size_in_bytes_;
  bool is_read_only_;
  bool is_hidden_;
  std::string uuid_;
};

class CHROMEOS_EXPORT CrosDisksClient : public DBusClient {
 public:
  
  
  typedef base::Callback<void(const std::vector<std::string>& device_paths)
                         > EnumerateAutoMountableDevicesCallback;

  
  
  typedef base::Callback<void(bool format_succeeded)> FormatDeviceCallback;

  
  
  typedef base::Callback<void(const DiskInfo& disk_info)
                         > GetDevicePropertiesCallback;

  
  
  
  
  
  typedef base::Callback<void(MountError error_code,
                              const std::string& source_path,
                              MountType mount_type,
                              const std::string& mount_path)
                         > MountCompletedHandler;

  
  
  
  typedef base::Callback<void(MountEventType event_type,
                              const std::string& device_path)
                         > MountEventHandler;

  virtual ~CrosDisksClient();

  
  
  
  
  
  
  
  
  
  virtual void Mount(const std::string& source_path,
                     const std::string& source_format,
                     const std::string& mount_label,
                     const base::Closure& callback,
                     const base::Closure& error_callback) = 0;

  
  
  virtual void Unmount(const std::string& device_path,
                       UnmountOptions options,
                       const base::Closure& callback,
                       const base::Closure& error_callback) = 0;

  
  
  virtual void EnumerateAutoMountableDevices(
      const EnumerateAutoMountableDevicesCallback& callback,
      const base::Closure& error_callback) = 0;

  
  
  virtual void FormatDevice(const std::string& device_path,
                            const std::string& filesystem,
                            const FormatDeviceCallback& callback,
                            const base::Closure& error_callback) = 0;

  
  
  virtual void GetDeviceProperties(const std::string& device_path,
                                   const GetDevicePropertiesCallback& callback,
                                   const base::Closure& error_callback) = 0;

  
  
  
  virtual void SetUpConnections(
      const MountEventHandler& mount_event_handler,
      const MountCompletedHandler& mount_completed_handler) = 0;

  
  
  static CrosDisksClient* Create(DBusClientImplementationType type);

  
  static base::FilePath GetArchiveMountPoint();

  
  static base::FilePath GetRemovableDiskMountPoint();

 protected:
  
  CrosDisksClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(CrosDisksClient);
};

}  

#endif  
