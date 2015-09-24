// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_MOUNT_LIBRARY_H_
#define CHROME_BROWSER_CHROMEOS_CROS_MOUNT_LIBRARY_H_
#pragma once

#include <string>
#include <map>

#include "base/memory/singleton.h"
#include "base/observer_list.h"
#include "base/time.h"
#include "third_party/cros/chromeos_mount.h"

namespace chromeos {

typedef enum MountLibraryEventType {
  MOUNT_DISK_ADDED,
  MOUNT_DISK_REMOVED,
  MOUNT_DISK_CHANGED,
  MOUNT_DISK_MOUNTED,
  MOUNT_DISK_UNMOUNTED,
  MOUNT_DEVICE_ADDED,
  MOUNT_DEVICE_REMOVED,
  MOUNT_DEVICE_SCANNED
} MountLibraryEventType;

class MountLibrary {
 public:
  
  class Disk {
   public:
    Disk(const std::string& device_path,
         const std::string& mount_path,
         const std::string& system_path,
         const std::string& file_path,
         const std::string& device_label,
         const std::string& drive_label,
         const std::string& parent_path,
         DeviceType device_type,
         uint64 total_size,
         bool is_parent,
         bool is_read_only,
         bool has_media,
         bool on_boot_device);
    
    const std::string& device_path() const { return device_path_; }
    
    const std::string&  mount_path() const { return mount_path_; }
    
    const std::string& system_path() const { return system_path_; }
    
    const std::string& file_path() const { return file_path_; }
    
    const std::string& device_label() const { return device_label_; }
    
    const std::string& drive_label() const { return drive_label_; }
    
    const std::string& parent_path() const { return parent_path_; }
    
    DeviceType device_type() const { return device_type_; }
    
    uint64 total_size() const { return total_size_; }
    
    bool is_parent() const { return is_parent_; }
    
    bool is_read_only() const { return is_read_only_; }
    
    bool has_media() const { return has_media_; }
    
    bool on_boot_device() const { return on_boot_device_; }

    void set_mount_path(const char* mount_path) { mount_path_ = mount_path; }
    void clear_mount_path() { mount_path_.clear(); }

   private:
    std::string device_path_;
    std::string mount_path_;
    std::string system_path_;
    std::string file_path_;
    std::string device_label_;
    std::string drive_label_;
    std::string parent_path_;
    DeviceType device_type_;
    uint64 total_size_;
    bool is_parent_;
    bool is_read_only_;
    bool has_media_;
    bool on_boot_device_;
  };
  typedef std::map<std::string, Disk*> DiskMap;

  class Observer {
   public:
    virtual ~Observer() {}
    
    virtual void DiskChanged(MountLibraryEventType event,
                             const Disk* disk) = 0;
    virtual void DeviceChanged(MountLibraryEventType event,
                               const std::string& device_path ) = 0;
  };

  virtual ~MountLibrary() {}
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;
  virtual const DiskMap& disks() const = 0;

  virtual void RequestMountInfoRefresh() = 0;
  virtual void MountPath(const char* device_path) = 0;
  virtual void UnmountPath(const char* device_path) = 0;

  
  
  static MountLibrary* GetImpl(bool stub);
};

}  

#endif  
