// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TIMER_HI_RES_TIMER_MANAGER_H_
#define BASE_TIMER_HI_RES_TIMER_MANAGER_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/power_monitor/power_observer.h"

namespace base {

class BASE_EXPORT HighResolutionTimerManager : public base::PowerObserver {
 public:
  HighResolutionTimerManager();
  virtual ~HighResolutionTimerManager();

  
  virtual void OnPowerStateChange(bool on_battery_power) OVERRIDE;

  
  bool hi_res_clock_available() const { return hi_res_clock_available_; }

 private:
  
  void UseHiResClock(bool use);

  bool hi_res_clock_available_;

  DISALLOW_COPY_AND_ASSIGN(HighResolutionTimerManager);
};

}  

#endif  
