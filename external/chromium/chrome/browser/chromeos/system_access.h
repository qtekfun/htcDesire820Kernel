// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SYSTEM_ACCESS_H_
#define CHROME_BROWSER_CHROMEOS_SYSTEM_ACCESS_H_
#pragma once

#include <string>

#include "unicode/timezone.h"

namespace chromeos {

class SystemAccess {
 public:
  class Observer {
   public:
    
    virtual void TimezoneChanged(const icu::TimeZone& timezone) = 0;
  };

  static SystemAccess* GetInstance();

  
  virtual const icu::TimeZone& GetTimezone() = 0;

  
  virtual void SetTimezone(const icu::TimeZone& timezone) = 0;

  
  
  
  virtual bool GetMachineStatistic(const std::string& name,
                                   std::string* result) = 0;

  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

 protected:
  virtual ~SystemAccess() {}
};

}  

#endif  
