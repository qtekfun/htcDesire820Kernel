// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_STORAGE_MONITOR_STORAGE_MONITOR_LINUX_H_
#define CHROME_BROWSER_STORAGE_MONITOR_STORAGE_MONITOR_LINUX_H_

#if defined(OS_CHROMEOS)
#error "Use the ChromeOS-specific implementation instead."
#endif

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/files/file_path_watcher.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/storage_monitor/mtab_watcher_linux.h"
#include "chrome/browser/storage_monitor/storage_monitor.h"
#include "content/public/browser/browser_thread.h"

class MediaTransferProtocolDeviceObserverLinux;

class StorageMonitorLinux : public StorageMonitor,
                            public MtabWatcherLinux::Delegate {
 public:
  
  
  
  explicit StorageMonitorLinux(const base::FilePath& mtab_file_path);
  virtual ~StorageMonitorLinux();

  
  virtual void Init() OVERRIDE;

 protected:
  
  typedef base::Callback<scoped_ptr<StorageInfo>(
      const base::FilePath& device_path,
      const base::FilePath& mount_point)> GetDeviceInfoCallback;

  void SetGetDeviceInfoCallbackForTest(
      const GetDeviceInfoCallback& get_device_info_callback);

  void SetMediaTransferProtocolManagerForTest(
      device::MediaTransferProtocolManager* test_manager);

  
  virtual void UpdateMtab(
      const MtabWatcherLinux::MountPointDeviceMap& new_mtab) OVERRIDE;

 private:
  
  
  struct MountPointInfo {
    base::FilePath mount_device;
    StorageInfo storage_info;
  };

  
  struct MtabWatcherLinuxDeleter {
    void operator()(MtabWatcherLinux* mtab_watcher) {
      content::BrowserThread::DeleteSoon(content::BrowserThread::FILE,
                                         FROM_HERE, mtab_watcher);
    }
  };

  
  typedef std::map<base::FilePath, MountPointInfo> MountMap;

  
  
  
  typedef std::map<base::FilePath, bool> ReferencedMountPoint;

  
  
  
  typedef std::map<base::FilePath, ReferencedMountPoint> MountPriorityMap;

  
  virtual bool GetStorageInfoForPath(const base::FilePath& path,
                                     StorageInfo* device_info) const OVERRIDE;
  virtual void EjectDevice(const std::string& device_id,
                           base::Callback<void(EjectStatus)> callback) OVERRIDE;
  virtual device::MediaTransferProtocolManager*
      media_transfer_protocol_manager() OVERRIDE;

  
  void OnMtabWatcherCreated(MtabWatcherLinux* watcher);

  bool IsDeviceAlreadyMounted(const base::FilePath& mount_device) const;

  
  
  void HandleDeviceMountedMultipleTimes(const base::FilePath& mount_device,
                                        const base::FilePath& mount_point);

  
  void AddNewMount(const base::FilePath& mount_device,
                   scoped_ptr<StorageInfo> storage_info);

  
  const base::FilePath mtab_path_;

  
  
  GetDeviceInfoCallback get_device_info_callback_;

  
  
  
  MountMap mount_info_map_;

  
  
  
  
  MountPriorityMap mount_priority_map_;

  scoped_ptr<device::MediaTransferProtocolManager>
      media_transfer_protocol_manager_;
  scoped_ptr<MediaTransferProtocolDeviceObserverLinux>
      media_transfer_protocol_device_observer_;

  scoped_ptr<MtabWatcherLinux, MtabWatcherLinuxDeleter> mtab_watcher_;

  base::WeakPtrFactory<StorageMonitorLinux> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(StorageMonitorLinux);
};

#endif  
