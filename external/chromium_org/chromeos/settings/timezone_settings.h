// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_SETTINGS_TIMEZONE_SETTINGS_H_
#define CHROMEOS_SETTINGS_TIMEZONE_SETTINGS_H_

#include <vector>

#include "base/strings/string16.h"
#include "chromeos/settings/cros_settings_provider.h"
#include "third_party/icu/source/i18n/unicode/timezone.h"

namespace chromeos {
namespace system {

class CHROMEOS_EXPORT TimezoneSettings {
 public:
  class Observer {
   public:
    
    virtual void TimezoneChanged(const icu::TimeZone& timezone) = 0;
   protected:
    virtual ~Observer();
  };

  static TimezoneSettings* GetInstance();

  
  virtual const icu::TimeZone& GetTimezone() = 0;
  virtual string16 GetCurrentTimezoneID() = 0;

  
  virtual void SetTimezone(const icu::TimeZone& timezone) = 0;
  virtual void SetTimezoneFromID(const string16& timezone_id) = 0;

  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  virtual const std::vector<icu::TimeZone*>& GetTimezoneList() const = 0;

  
  static string16 GetTimezoneID(const icu::TimeZone& timezone);

 protected:
  virtual ~TimezoneSettings() {}
};

}  
}  

#endif  
