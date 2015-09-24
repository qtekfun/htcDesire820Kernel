// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_BROWSER_UDEV_LINUX_H_
#define CONTENT_BROWSER_UDEV_LINUX_H_

#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/message_loop/message_pump_libevent.h"

extern "C" {
struct udev;
struct udev_device;
struct udev_monitor;
}

namespace content {

class UdevLinux : public base::MessagePumpLibevent::Watcher {
 public:
  typedef base::Callback<void(udev_device*)> UdevNotificationCallback;

  
  
  struct UdevMonitorFilter {
    UdevMonitorFilter(const char* subsystem_in, const char* devtype_in)
        : subsystem(subsystem_in),
          devtype(devtype_in) {
    }
    const char* subsystem;
    const char* devtype;
  };

  
  
  UdevLinux(const std::vector<UdevMonitorFilter>& filters,
            const UdevNotificationCallback& callback);
  virtual ~UdevLinux();


  
  udev* udev_handle();

 private:
  
  virtual void OnFileCanReadWithoutBlocking(int fd) OVERRIDE;
  virtual void OnFileCanWriteWithoutBlocking(int fd) OVERRIDE;

  
  
  udev* udev_;
  udev_monitor* monitor_;
  int monitor_fd_;
  base::MessagePumpLibevent::FileDescriptorWatcher monitor_watcher_;
  UdevNotificationCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(UdevLinux);
};

}  

#endif  
