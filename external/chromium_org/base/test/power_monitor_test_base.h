// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_POWER_MONITOR_TEST_BASE_H_
#define BASE_TEST_POWER_MONITOR_TEST_BASE_H_

#include "base/message_loop/message_loop.h"
#include "base/power_monitor/power_monitor.h"
#include "base/power_monitor/power_monitor_source.h"

namespace base {

class PowerMonitorTestSource : public PowerMonitorSource {
 public:
  PowerMonitorTestSource();
  virtual ~PowerMonitorTestSource();

  void GeneratePowerStateEvent(bool on_battery_power);
  void GenerateSuspendEvent();
  void GenerateResumeEvent();

 protected:
  virtual bool IsOnBatteryPowerImpl() OVERRIDE;

  bool test_on_battery_power_;
  MessageLoop message_loop_;
};

class PowerMonitorTestObserver : public PowerObserver {
 public:
  PowerMonitorTestObserver();
  virtual ~PowerMonitorTestObserver();

  
  virtual void OnPowerStateChange(bool on_battery_power) OVERRIDE;
  virtual void OnSuspend() OVERRIDE;
  virtual void OnResume() OVERRIDE;

  
  bool last_power_state() { return last_power_state_; }
  int power_state_changes() { return power_state_changes_; }
  int suspends() { return suspends_; }
  int resumes() { return resumes_; }

 private:
  bool last_power_state_; 
  int power_state_changes_;  
  int suspends_;  
  int resumes_;  
};

}  

#endif  
