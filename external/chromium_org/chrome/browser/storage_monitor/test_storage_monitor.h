// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STORAGE_MONITOR_TEST_STORAGE_MONITOR_H_
#define CHROME_BROWSER_STORAGE_MONITOR_TEST_STORAGE_MONITOR_H_

#include <string>

#include "chrome/browser/storage_monitor/storage_monitor.h"

class TestStorageMonitor : public StorageMonitor {
 public:
  TestStorageMonitor();
  virtual ~TestStorageMonitor();

  virtual void Init() OVERRIDE;

  void MarkInitialized();

  
  
  
  
  
  static TestStorageMonitor* CreateAndInstall();

  
  
  
  
  static TestStorageMonitor* CreateForBrowserTests();

  
  static void RemoveSingleton();

  
  static void SyncInitialize();

  virtual bool GetStorageInfoForPath(
      const base::FilePath& path,
      StorageInfo* device_info) const OVERRIDE;

#if defined(OS_WIN)
  virtual bool GetMTPStorageInfoFromDeviceId(
      const std::string& storage_device_id,
      base::string16* device_location,
      base::string16* storage_object_id) const OVERRIDE;
#endif

#if defined(OS_LINUX)
  virtual device::MediaTransferProtocolManager*
      media_transfer_protocol_manager() OVERRIDE;
#endif

  virtual Receiver* receiver() const OVERRIDE;

  virtual void EjectDevice(
      const std::string& device_id,
      base::Callback<void(StorageMonitor::EjectStatus)> callback)
      OVERRIDE;

  const std::string& ejected_device() const { return ejected_device_; }

  bool init_called() const { return init_called_; }

 private:
  
  bool init_called_;

  
  std::string ejected_device_;

#if defined(OS_LINUX)
  scoped_ptr<device::MediaTransferProtocolManager>
      media_transfer_protocol_manager_;
#endif
};

#endif  
