// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_POWER_TRAY_POWER_H_
#define ASH_SYSTEM_CHROMEOS_POWER_TRAY_POWER_H_

#include "ash/system/chromeos/power/power_status.h"
#include "ash/system/tray/system_tray_item.h"

class SkBitmap;

namespace gfx {
class Image;
class ImageSkia;
}

namespace message_center {
class MessageCenter;
}

namespace ash {
namespace internal {

namespace tray {
class PowerNotificationView;
class PowerTrayView;
}

class ASH_EXPORT TrayPower : public SystemTrayItem,
                             public PowerStatus::Observer {
 public:
  
  enum NotificationState {
    NOTIFICATION_NONE,

    
    NOTIFICATION_LOW_POWER,

    
    NOTIFICATION_CRITICAL,
  };

  
  static const int kCriticalMinutes;
  static const int kLowPowerMinutes;
  static const int kNoWarningMinutes;

  
  static const int kCriticalPercentage;
  static const int kLowPowerPercentage;
  static const int kNoWarningPercentage;

  TrayPower(SystemTray* system_tray,
            message_center::MessageCenter* message_center);
  virtual ~TrayPower();

 private:
  friend class TrayPowerTest;

  
  
  enum ChargerType{
    UNKNOWN_CHARGER,
    MAINS_CHARGER,
    USB_CHARGER,
    UNCONFIRMED_SPRING_CHARGER,
    SAFE_SPRING_CHARGER,
    CHARGER_TYPE_COUNT,
  };

  
  virtual views::View* CreateTrayView(user::LoginStatus status) OVERRIDE;
  virtual views::View* CreateDefaultView(user::LoginStatus status) OVERRIDE;
  virtual views::View* CreateNotificationView(
      user::LoginStatus status) OVERRIDE;
  virtual void DestroyTrayView() OVERRIDE;
  virtual void DestroyDefaultView() OVERRIDE;
  virtual void DestroyNotificationView() OVERRIDE;
  virtual void UpdateAfterLoginStatusChange(user::LoginStatus status) OVERRIDE;
  virtual void UpdateAfterShelfAlignmentChange(
      ShelfAlignment alignment) OVERRIDE;

  
  virtual void OnPowerStatusChanged() OVERRIDE;

  
  
  bool MaybeShowUsbChargerNotification();

  
  bool UpdateNotificationState();
  bool UpdateNotificationStateForRemainingTime();
  bool UpdateNotificationStateForRemainingPercentage();

  
  void RecordChargerType();

  message_center::MessageCenter* message_center_;  
  tray::PowerTrayView* power_tray_;
  tray::PowerNotificationView* notification_view_;
  NotificationState notification_state_;

  
  
  bool usb_charger_was_connected_;

  
  bool line_power_was_connected_;

  DISALLOW_COPY_AND_ASSIGN(TrayPower);
};

}  
}  

#endif  
