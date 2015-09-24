// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STORAGE_MONITOR_TEST_PORTABLE_DEVICE_WATCHER_WIN_H_
#define CHROME_BROWSER_STORAGE_MONITOR_TEST_PORTABLE_DEVICE_WATCHER_WIN_H_

#include <string>

#include "base/strings/string16.h"
#include "chrome/browser/storage_monitor/portable_device_watcher_win.h"

class TestPortableDeviceWatcherWin : public PortableDeviceWatcherWin {
 public:
  
  static const char16 kMTPDeviceWithMultipleStorages[];
  static const char16 kMTPDeviceWithInvalidInfo[];
  static const char16 kMTPDeviceWithValidInfo[];

  
  static const char kStorageUniqueIdA[];

  TestPortableDeviceWatcherWin();
  virtual ~TestPortableDeviceWatcherWin();

  
  
  
  static std::string GetMTPStorageUniqueId(
      const base::string16& pnp_device_id,
      const base::string16& storage_object_id);

  
  
  static PortableDeviceWatcherWin::StorageObjectIDs GetMTPStorageObjectIds(
      const base::string16& pnp_device_id);

  
  
  static void GetMTPStorageDetails(const base::string16& pnp_device_id,
                                   const base::string16& storage_object_id,
                                   base::string16* device_location,
                                   std::string* unique_id,
                                   base::string16* name);

  
  
  static PortableDeviceWatcherWin::StorageObjects GetDeviceStorageObjects(
      const base::string16& pnp_device_id);

  
  void set_use_dummy_mtp_storage_info(bool use_dummy_info) {
    use_dummy_mtp_storage_info_ = use_dummy_info;
  }

 private:
  
  virtual void EnumerateAttachedDevices() OVERRIDE;
  virtual void HandleDeviceAttachEvent(
      const base::string16& pnp_device_id) OVERRIDE;
  virtual bool GetMTPStorageInfoFromDeviceId(
      const std::string& storage_device_id,
      base::string16* device_location,
      base::string16* storage_object_id) const OVERRIDE;

  
  
  bool use_dummy_mtp_storage_info_;

  DISALLOW_COPY_AND_ASSIGN(TestPortableDeviceWatcherWin);
};

#endif  
