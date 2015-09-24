// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STORAGE_MONITOR_VOLUME_MOUNT_WATCHER_WIN_H_
#define CHROME_BROWSER_STORAGE_MONITOR_VOLUME_MOUNT_WATCHER_WIN_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner.h"
#include "base/strings/string16.h"
#include "base/threading/sequenced_worker_pool.h"
#include "chrome/browser/storage_monitor/storage_info.h"
#include "chrome/browser/storage_monitor/storage_monitor.h"

class TestVolumeMountWatcherWin;

class VolumeMountWatcherWin {
 public:
  VolumeMountWatcherWin();
  virtual ~VolumeMountWatcherWin();

  
  
  
  static base::FilePath DriveNumberToFilePath(int drive_number);

  void Init();

  
  
  
  bool GetDeviceInfo(const base::FilePath& device_path,
                     StorageInfo* info) const;

  
  
  void OnWindowMessage(UINT event_type, LPARAM data);

  
  
  void SetNotifications(StorageMonitor::Receiver* notifications);

  void EjectDevice(const std::string& device_id,
                   base::Callback<void(StorageMonitor::EjectStatus)> callback);

 protected:
  typedef base::Callback<bool(const base::FilePath&,
                              StorageInfo*)> GetDeviceDetailsCallbackType;

  typedef base::Callback<std::vector<base::FilePath>(void)>
      GetAttachedDevicesCallbackType;

  
  void HandleDeviceAttachEventOnUIThread(
      const base::FilePath& device_path,
      const StorageInfo& info);

  
  void HandleDeviceDetachEventOnUIThread(const base::string16& device_location);

  
  void AddDevicesOnUIThread(std::vector<base::FilePath> removable_devices);

  
  
  static void RetrieveInfoForDeviceAndAdd(
      const base::FilePath& device_path,
      const GetDeviceDetailsCallbackType& get_device_details_callback,
      base::WeakPtr<VolumeMountWatcherWin> volume_watcher);

  
  virtual void DeviceCheckComplete(const base::FilePath& device_path);

  virtual GetAttachedDevicesCallbackType GetAttachedDevicesCallback() const;
  virtual GetDeviceDetailsCallbackType GetDeviceDetailsCallback() const;

  
  
  
  scoped_refptr<base::SequencedWorkerPool> device_info_worker_pool_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;

 private:
  friend class TestVolumeMountWatcherWin;

  
  
  typedef std::map<base::FilePath, StorageInfo> MountPointDeviceMetadataMap;

  
  
  
  std::set<base::FilePath> pending_device_checks_;

  
  
  MountPointDeviceMetadataMap device_metadata_;

  base::WeakPtrFactory<VolumeMountWatcherWin> weak_factory_;

  
  
  StorageMonitor::Receiver* notifications_;

  DISALLOW_COPY_AND_ASSIGN(VolumeMountWatcherWin);
};

#endif  
