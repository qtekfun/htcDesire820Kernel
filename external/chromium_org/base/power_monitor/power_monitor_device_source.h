// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_POWER_MONITOR_POWER_MONITOR_DEVICE_SOURCE_H_
#define BASE_POWER_MONITOR_POWER_MONITOR_DEVICE_SOURCE_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list_threadsafe.h"
#include "base/power_monitor/power_monitor_source.h"
#include "base/power_monitor/power_observer.h"

#if defined(OS_WIN)
#include <windows.h>

#define ENABLE_BATTERY_MONITORING 1
#else
#undef ENABLE_BATTERY_MONITORING
#endif  

#if defined(ENABLE_BATTERY_MONITORING)
#include "base/timer/timer.h"
#endif  

#if defined(OS_IOS)
#include <objc/runtime.h>
#endif  

namespace base {

class BASE_EXPORT PowerMonitorDeviceSource : public PowerMonitorSource {
 public:
  PowerMonitorDeviceSource();
  virtual ~PowerMonitorDeviceSource();

#if defined(OS_MACOSX)
  
  
  
  
#if !defined(OS_IOS)
  static void AllocateSystemIOPorts();
#else
  static void AllocateSystemIOPorts() {}
#endif  
#endif  

 private:
#if defined(OS_WIN)
  
  
  class PowerMessageWindow {
   public:
    PowerMessageWindow();
    ~PowerMessageWindow();

   private:
    static LRESULT CALLBACK WndProcThunk(HWND hwnd,
                                         UINT message,
                                         WPARAM wparam,
                                         LPARAM lparam);
    
    HMODULE instance_;
    
    HWND message_hwnd_;
  };
#endif  

#if defined(OS_MACOSX)
  void PlatformInit();
  void PlatformDestroy();
#endif

  
  
  
  virtual bool IsOnBatteryPowerImpl() OVERRIDE;

  
  
  void BatteryCheck();

#if defined(OS_IOS)
  
  std::vector<id> notification_observers_;
#endif

#if defined(ENABLE_BATTERY_MONITORING)
  base::OneShotTimer<PowerMonitorDeviceSource> delayed_battery_check_;
#endif

#if defined(OS_WIN)
  PowerMessageWindow power_message_window_;
#endif

  DISALLOW_COPY_AND_ASSIGN(PowerMonitorDeviceSource);
};

}  

#endif  
