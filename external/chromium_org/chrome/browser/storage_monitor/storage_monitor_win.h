// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STORAGE_MONITOR_STORAGE_MONITOR_WIN_H_
#define CHROME_BROWSER_STORAGE_MONITOR_STORAGE_MONITOR_WIN_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/storage_monitor/storage_monitor.h"

namespace base {
class FilePath;
}

class PortableDeviceWatcherWin;
class TestStorageMonitorWin;
class VolumeMountWatcherWin;

class StorageMonitorWin : public StorageMonitor {
 public:
  virtual ~StorageMonitorWin();

  
  virtual void Init() OVERRIDE;

  
  virtual bool GetStorageInfoForPath(const base::FilePath& path,
                                     StorageInfo* device_info) const OVERRIDE;
  virtual bool GetMTPStorageInfoFromDeviceId(
      const std::string& storage_device_id,
      base::string16* device_location,
      base::string16* storage_object_id) const OVERRIDE;

  virtual void EjectDevice(
      const std::string& device_id,
      base::Callback<void(EjectStatus)> callback) OVERRIDE;

 private:
  class PortableDeviceNotifications;
  friend class TestStorageMonitorWin;
  friend StorageMonitor* StorageMonitor::Create();

  
  
  
  StorageMonitorWin(VolumeMountWatcherWin* volume_mount_watcher,
                    PortableDeviceWatcherWin* portable_device_watcher);

  
  
  bool GetDeviceInfo(const base::FilePath& device_path,
                     StorageInfo* info) const;

  static LRESULT CALLBACK WndProcThunk(HWND hwnd, UINT message, WPARAM wparam,
                                       LPARAM lparam);

  LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam,
                           LPARAM lparam);

  void OnDeviceChange(UINT event_type, LPARAM data);

  
  ATOM window_class_;

  
  HMODULE instance_;
  HWND window_;

  
  scoped_ptr<VolumeMountWatcherWin> volume_mount_watcher_;

  
  
  scoped_ptr<PortableDeviceWatcherWin> portable_device_watcher_;

  DISALLOW_COPY_AND_ASSIGN(StorageMonitorWin);
};

#endif  
