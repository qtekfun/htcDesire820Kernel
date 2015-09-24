// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STORAGE_MONITOR_PORTABLE_DEVICE_WATCHER_WIN_H_
#define CHROME_BROWSER_STORAGE_MONITOR_PORTABLE_DEVICE_WATCHER_WIN_H_

#include <portabledeviceapi.h>

#include <map>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/storage_monitor/storage_monitor.h"

namespace base {
class SequencedTaskRunner;
}

class TestPortableDeviceWatcherWin;

class PortableDeviceWatcherWin {
 public:
  typedef std::vector<base::string16> StorageObjectIDs;

  struct DeviceStorageObject {
    DeviceStorageObject(const base::string16& temporary_id,
                        const std::string& persistent_id);

    
    
    
    
    base::string16 object_temporary_id;

    
    
    std::string object_persistent_id;
  };
  typedef std::vector<DeviceStorageObject> StorageObjects;

  
  struct DeviceDetails {
    
    base::string16 name;

    
    base::string16 location;

    
    StorageObjects storage_objects;
  };
  typedef std::vector<DeviceDetails> Devices;

  
  
  PortableDeviceWatcherWin();
  virtual ~PortableDeviceWatcherWin();

  
  
  void Init(HWND hwnd);

  
  
  void OnWindowMessage(UINT event_type, LPARAM data);

  
  
  
  
  virtual bool GetMTPStorageInfoFromDeviceId(
      const std::string& storage_device_id,
      base::string16* device_location,
      base::string16* storage_object_id) const;

  
  static base::string16 GetStoragePathFromStorageId(
      const std::string& storage_unique_id);

  
  
  void SetNotifications(StorageMonitor::Receiver* notifications);

  void EjectDevice(const std::string& device_id,
                   base::Callback<void(StorageMonitor::EjectStatus)> callback);

 private:
  friend class TestPortableDeviceWatcherWin;

  
  
  typedef std::map<std::string, StorageInfo> MTPStorageMap;

  
  
  typedef std::map<base::string16, StorageObjects> MTPDeviceMap;

  
  virtual void EnumerateAttachedDevices();
  void OnDidEnumerateAttachedDevices(const Devices* devices,
                                     const bool result);

  
  virtual void HandleDeviceAttachEvent(const base::string16& pnp_device_id);
  void OnDidHandleDeviceAttachEvent(const DeviceDetails* device_details,
                                    const bool result);

  
  void HandleDeviceDetachEvent(const base::string16& pnp_device_id);

  
  HDEVNOTIFY notifications_;

  
  MTPDeviceMap device_map_;

  
  MTPStorageMap storage_map_;

  
  
  scoped_refptr<base::SequencedTaskRunner> media_task_runner_;

  
  base::WeakPtrFactory<PortableDeviceWatcherWin> weak_ptr_factory_;

  
  StorageMonitor::Receiver* storage_notifications_;

  DISALLOW_COPY_AND_ASSIGN(PortableDeviceWatcherWin);
};

#endif  
