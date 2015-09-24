// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_SYSTEM_CLOCK_OBSERVER_H_
#define ASH_SYSTEM_CHROMEOS_SYSTEM_CLOCK_OBSERVER_H_

#include "chromeos/dbus/system_clock_client.h"
#include "chromeos/settings/timezone_settings.h"

namespace ash {
namespace internal {

class SystemClockObserver
    : public chromeos::SystemClockClient::Observer,
      public chromeos::system::TimezoneSettings::Observer {
 public:
  SystemClockObserver();
  virtual ~SystemClockObserver();

  
  virtual void SystemClockUpdated() OVERRIDE;

  
  virtual void TimezoneChanged(const icu::TimeZone& timezone) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(SystemClockObserver);
};

}  
}  

#endif  
