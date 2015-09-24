// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_POWER_POWER_STATUS_H_
#define ASH_SYSTEM_CHROMEOS_POWER_POWER_STATUS_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "chromeos/dbus/power_manager/power_supply_properties.pb.h"
#include "chromeos/dbus/power_manager_client.h"
#include "ui/gfx/image/image_skia.h"

namespace ash {
namespace internal {

class ASH_EXPORT PowerStatus : public chromeos::PowerManagerClient::Observer {
 public:
  
  enum IconSet {
    ICON_LIGHT,
    ICON_DARK
  };

  
  
  class Observer {
   public:
    
    virtual void OnPowerStatusChanged() = 0;

   protected:
    virtual ~Observer() {}
  };

  
  
  
  static const int kMaxBatteryTimeToDisplaySec;

  
  static void Initialize();

  
  static void Shutdown();

  
  static bool IsInitialized();

  
  static PowerStatus* Get();

  
  
  
  static bool ShouldDisplayBatteryTime(const base::TimeDelta& time);

  
  
  
  
  static void SplitTimeIntoHoursAndMinutes(const base::TimeDelta& time,
                                           int* hours,
                                           int* minutes);

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  void RequestStatusUpdate();

  
  bool IsBatteryPresent() const;

  
  
  bool IsBatteryFull() const;

  
  
  
  
  
  bool IsBatteryCharging() const;

  
  
  bool IsBatteryDischargingOnLinePower() const;

  
  
  double GetBatteryPercent() const;

  
  
  int GetRoundedBatteryPercent() const;

  
  
  
  bool IsBatteryTimeBeingCalculated() const;

  
  
  
  base::TimeDelta GetBatteryTimeToEmpty() const;
  base::TimeDelta GetBatteryTimeToFull() const;

  
  bool IsLinePowerConnected() const;

  
  bool IsMainsChargerConnected() const;

  
  
  bool IsUsbChargerConnected() const;

  
  bool IsOriginalSpringChargerConnected() const;

  
  gfx::ImageSkia GetBatteryImage(IconSet icon_set) const;

  
  base::string16 GetAccessibleNameString() const;

  
  void SetProtoForTesting(const power_manager::PowerSupplyProperties& proto);

 protected:
  PowerStatus();
  virtual ~PowerStatus();

 private:
  
  virtual void PowerChanged(
      const power_manager::PowerSupplyProperties& proto) OVERRIDE;

  ObserverList<Observer> observers_;

  
  power_manager::PowerSupplyProperties proto_;

  DISALLOW_COPY_AND_ASSIGN(PowerStatus);
};

}  
}  

#endif  
