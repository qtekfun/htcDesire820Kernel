// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_POWER_MONITOR_POWER_MONITOR_SOURCE_H_
#define BASE_POWER_MONITOR_POWER_MONITOR_SOURCE_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list_threadsafe.h"
#include "base/synchronization/lock.h"

namespace base {

class PowerMonitor;

class BASE_EXPORT PowerMonitorSource {
 public:
  PowerMonitorSource();
  virtual ~PowerMonitorSource();

  
  enum PowerEvent {
    POWER_STATE_EVENT,  
    SUSPEND_EVENT,      
    RESUME_EVENT        
  };

  
  bool IsOnBatteryPower();

 protected:
  friend class PowerMonitorTest;

  
  friend void ProcessPowerEventHelper(PowerEvent);

  
  static PowerMonitorSource* Get();

  
  
  static void ProcessPowerEvent(PowerEvent event_id);

  
  
  
  virtual bool IsOnBatteryPowerImpl() = 0;

 private:
  bool on_battery_power_;
  bool suspended_;

  
  
  Lock battery_lock_;

  DISALLOW_COPY_AND_ASSIGN(PowerMonitorSource);
};

}  

#endif  
