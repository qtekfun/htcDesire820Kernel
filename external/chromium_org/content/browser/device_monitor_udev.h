// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_BROWSER_DEVICE_MONITOR_UDEV_H_
#define CONTENT_BROWSER_DEVICE_MONITOR_UDEV_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"

extern "C" {
struct udev_device;
}

namespace content {

class UdevLinux;

class DeviceMonitorLinux : public base::MessageLoop::DestructionObserver {
 public:
  DeviceMonitorLinux();
  virtual ~DeviceMonitorLinux();

 private:
  
  
  
  virtual void WillDestroyCurrentMessageLoop() OVERRIDE;

  void Initialize();
  void OnDevicesChanged(udev_device* device);

  scoped_ptr<UdevLinux> udev_;

  DISALLOW_COPY_AND_ASSIGN(DeviceMonitorLinux);
};

}  

#endif  
