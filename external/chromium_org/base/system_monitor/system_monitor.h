// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SYSTEM_MONITOR_SYSTEM_MONITOR_H_
#define BASE_SYSTEM_MONITOR_SYSTEM_MONITOR_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list_threadsafe.h"
#include "build/build_config.h"

namespace base {

class BASE_EXPORT SystemMonitor {
 public:
  
  enum DeviceType {
    DEVTYPE_AUDIO_CAPTURE,  
    DEVTYPE_VIDEO_CAPTURE,  
    DEVTYPE_UNKNOWN,  
  };

  
  
  SystemMonitor();
  ~SystemMonitor();

  
  static SystemMonitor* Get();

  class BASE_EXPORT DevicesChangedObserver {
   public:
    
    
    virtual void OnDevicesChanged(DeviceType device_type) {}

   protected:
    virtual ~DevicesChangedObserver() {}
  };

  
  
  
  void AddDevicesChangedObserver(DevicesChangedObserver* obs);

  
  
  
  void RemoveDevicesChangedObserver(DevicesChangedObserver* obs);

  
  
  

  
  void ProcessDevicesChanged(DeviceType device_type);

 private:
  
  void NotifyDevicesChanged(DeviceType device_type);

  scoped_refptr<ObserverListThreadSafe<DevicesChangedObserver> >
      devices_changed_observer_list_;

  DISALLOW_COPY_AND_ASSIGN(SystemMonitor);
};

}  

#endif  
