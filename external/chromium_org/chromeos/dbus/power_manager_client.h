// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_POWER_MANAGER_CLIENT_H_
#define CHROMEOS_DBUS_POWER_MANAGER_CLIENT_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/time/time.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "chromeos/dbus/dbus_client_implementation_type.h"
#include "third_party/cros_system_api/dbus/service_constants.h"

namespace power_manager {
class PowerManagementPolicy;
class PowerSupplyProperties;
}

namespace chromeos {

typedef base::Callback<void(double)> GetScreenBrightnessPercentCallback;

class CHROMEOS_EXPORT PowerManagerClient : public DBusClient {
 public:
  
  class Observer {
   public:
    virtual ~Observer() {}

    
    virtual void PowerManagerRestarted() {}

    
    
    
    virtual void BrightnessChanged(int level, bool user_initiated) {}

    
    
    
    
    
    virtual void PeripheralBatteryStatusReceived(const std::string& path,
                                                 const std::string& name,
                                                 int level) {}

    
    
    
    virtual void PowerChanged(
        const power_manager::PowerSupplyProperties& proto) {}

    
    
    
    
    
    
    
    virtual void SuspendImminent() {}

    
    virtual void PowerButtonEventReceived(bool down,
                                          const base::TimeTicks& timestamp) {}

    
    virtual void LidEventReceived(bool open,
                                  const base::TimeTicks& timestamp) {}

    
    virtual void SystemResumed(const base::TimeDelta& sleep_duration) {}

    
    virtual void IdleActionImminent() {}

    
    
    virtual void IdleActionDeferred() {}
  };

  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;
  virtual bool HasObserver(Observer* observer) = 0;

  
  
  virtual void DecreaseScreenBrightness(bool allow_off) = 0;

  
  virtual void IncreaseScreenBrightness() = 0;

  
  
  virtual void SetScreenBrightnessPercent(double percent, bool gradual) = 0;

  
  
  virtual void GetScreenBrightnessPercent(
      const GetScreenBrightnessPercentCallback& callback) = 0;

  
  virtual void DecreaseKeyboardBrightness() = 0;

  
  virtual void IncreaseKeyboardBrightness() = 0;

  
  
  virtual void RequestStatusUpdate() = 0;

  
  virtual void RequestRestart() = 0;

  
  virtual void RequestShutdown() = 0;

  
  
  virtual void NotifyUserActivity(power_manager::UserActivityType type) = 0;

  
  
  virtual void NotifyVideoActivity(bool is_fullscreen) = 0;

  
  virtual void SetPolicy(
      const power_manager::PowerManagementPolicy& policy) = 0;

  
  
  
  virtual void SetIsProjecting(bool is_projecting) = 0;

  
  
  virtual base::Closure GetSuspendReadinessCallback() = 0;

  
  
  virtual int GetNumPendingSuspendReadinessCallbacks() = 0;

  
  static PowerManagerClient* Create(DBusClientImplementationType type);

  virtual ~PowerManagerClient();

 protected:
  
  PowerManagerClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(PowerManagerClient);
};

}  

#endif  
