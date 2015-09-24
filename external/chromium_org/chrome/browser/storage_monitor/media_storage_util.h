// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_STORAGE_MONITOR_MEDIA_STORAGE_UTIL_H_
#define CHROME_BROWSER_STORAGE_MONITOR_MEDIA_STORAGE_UTIL_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"

struct StorageInfo;

class MediaStorageUtil {
 public:
  typedef std::set<std::string > DeviceIdSet;

  
  
  
  
  
  
  static bool HasDcim(const base::FilePath& mount_point);

  
  static bool CanCreateFileSystem(const std::string& device_id,
                                  const base::FilePath& path);

  
  static void FilterAttachedDevices(DeviceIdSet* devices,
                                    const base::Closure& done);

  
  
  static bool GetDeviceInfoFromPath(const base::FilePath& path,
                                    StorageInfo* device_info,
                                    base::FilePath* relative_path);

  
  
  
  static base::FilePath FindDevicePathById(const std::string& device_id);

  
  
  
  static void RecordDeviceInfoHistogram(bool mass_storage,
                                        const std::string& device_uuid,
                                        const base::string16& device_label);

  
  
  static bool IsRemovableStorageAttached(const std::string& id);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(MediaStorageUtil);
};

#endif  
