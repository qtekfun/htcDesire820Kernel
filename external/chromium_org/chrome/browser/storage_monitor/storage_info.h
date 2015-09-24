// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STORAGE_MONITOR_STORAGE_INFO_H_
#define CHROME_BROWSER_STORAGE_MONITOR_STORAGE_INFO_H_

#include "base/files/file_path.h"
#include "base/strings/string16.h"

struct StorageInfo {
 public:
  enum Type {
    
    REMOVABLE_MASS_STORAGE_WITH_DCIM,
    
    REMOVABLE_MASS_STORAGE_NO_DCIM,
    
    FIXED_MASS_STORAGE,
    
    MTP_OR_PTP,
    
    MAC_IMAGE_CAPTURE,
    
    ITUNES,
    
    PICASA,
    
    IPHOTO,
  };

  StorageInfo();
  
  StorageInfo(const std::string& device_id_in,
              const base::string16& device_name,
              const base::FilePath::StringType& device_location,
              const base::string16& label,
              const base::string16& vendor,
              const base::string16& model,
              uint64 size_in_bytes);
  ~StorageInfo();

  
  
  
  static std::string MakeDeviceId(Type type, const std::string& unique_id);

  
  
  static bool CrackDeviceId(const std::string& device_id,
                            Type* type, std::string* unique_id);

  
  
  static bool IsMediaDevice(const std::string& device_id);

  
  
  static bool IsRemovableDevice(const std::string& device_id);

  
  
  static bool IsMassStorageDevice(const std::string& device_id);

  static bool IsITunesDevice(const std::string& device_id);

  static bool IsPicasaDevice(const std::string& device_id);

  static bool IsIPhotoDevice(const std::string& device_id);

  const std::string& device_id() const { return device_id_; }
  const base::string16& name() const { return name_; }
  const base::FilePath::StringType& location() const { return location_; }
  const base::string16& storage_label() const { return storage_label_; }
  const base::string16& vendor_name() const { return vendor_name_; }
  const base::string16& model_name() const { return model_name_; }
  uint64 total_size_in_bytes() const { return total_size_in_bytes_; }

  void set_device_id(const std::string& device_id) { device_id_ = device_id; }
  void set_name(const base::string16& name) { name_ = name; }
  void set_location(const base::FilePath::StringType& location) {
    location_ = location;
  }

 private:
  
  
  
  
  std::string device_id_;

  
  base::string16 name_;

  
  base::FilePath::StringType location_;

  
  
  base::string16 storage_label_;

  
  
  base::string16 vendor_name_;

  
  
  base::string16 model_name_;

  
  
  uint64 total_size_in_bytes_;
};

#endif  
