// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_POWER_POWER_EVENT_OBSERVER_H_
#define ASH_SYSTEM_CHROMEOS_POWER_POWER_EVENT_OBSERVER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "chromeos/dbus/power_manager_client.h"
#include "chromeos/dbus/session_manager_client.h"

namespace ash {
namespace internal {

class ASH_EXPORT PowerEventObserver
    : public chromeos::PowerManagerClient::Observer,
      public chromeos::SessionManagerClient::Observer {
 public:
  
  PowerEventObserver();
  virtual ~PowerEventObserver();

  
  virtual void BrightnessChanged(int level, bool user_initiated) OVERRIDE;
  virtual void SuspendImminent() OVERRIDE;
  virtual void SystemResumed(const base::TimeDelta& sleep_duration) OVERRIDE;

  
  virtual void ScreenIsLocked() OVERRIDE;
  virtual void ScreenIsUnlocked() OVERRIDE;

  
  bool screen_locked_;

  
  
  base::Closure screen_lock_callback_;

 private:
  DISALLOW_COPY_AND_ASSIGN(PowerEventObserver);
};

}  
}  

#endif  
