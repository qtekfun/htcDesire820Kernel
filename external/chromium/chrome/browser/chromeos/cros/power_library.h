// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_POWER_LIBRARY_H_
#define CHROME_BROWSER_CHROMEOS_CROS_POWER_LIBRARY_H_
#pragma once

#include "base/memory/singleton.h"
#include "base/observer_list.h"
#include "base/time.h"
#include "third_party/cros/chromeos_power.h"

namespace chromeos {

class PowerLibrary {
 public:
  class Observer {
   public:
    virtual void PowerChanged(PowerLibrary* obj) = 0;
    virtual void SystemResumed() = 0;
  };
  virtual ~PowerLibrary() {}
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;
  
  virtual bool line_power_on() const = 0;

  
  virtual bool battery_fully_charged() const = 0;

  
  virtual double battery_percentage() const = 0;

  
  virtual bool battery_is_present() const = 0;

  
  virtual base::TimeDelta battery_time_to_empty() const = 0;

  
  virtual base::TimeDelta battery_time_to_full() const = 0;

  
  virtual void EnableScreenLock(bool enable) = 0;

  
  virtual void RequestRestart() = 0;

  
  virtual void RequestShutdown() = 0;

  
  
  static PowerLibrary* GetImpl(bool stub);
};

}  

#endif  
