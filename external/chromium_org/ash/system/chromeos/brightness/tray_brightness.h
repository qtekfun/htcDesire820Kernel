// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_BRIGHTNESS_TRAY_BRIGHTNESS_H_
#define ASH_SYSTEM_CHROMEOS_BRIGHTNESS_TRAY_BRIGHTNESS_H_

#include "ash/system/tray/system_tray_item.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/dbus/power_manager_client.h"

namespace ash {
namespace internal {

namespace tray {
class BrightnessView;
}

class TrayBrightness : public SystemTrayItem,
                       public chromeos::PowerManagerClient::Observer {
 public:
  explicit TrayBrightness(SystemTray* system_tray);
  virtual ~TrayBrightness();

 private:
  
  
  void GetInitialBrightness();

  
  
  
  void HandleInitialBrightness(double percent);

  
  virtual views::View* CreateTrayView(user::LoginStatus status) OVERRIDE;
  virtual views::View* CreateDefaultView(user::LoginStatus status) OVERRIDE;
  virtual views::View* CreateDetailedView(user::LoginStatus status) OVERRIDE;
  virtual void DestroyTrayView() OVERRIDE;
  virtual void DestroyDefaultView() OVERRIDE;
  virtual void DestroyDetailedView() OVERRIDE;
  virtual void UpdateAfterLoginStatusChange(user::LoginStatus status) OVERRIDE;
  virtual bool ShouldHideArrow() const OVERRIDE;
  virtual bool ShouldShowLauncher() const OVERRIDE;

  
  virtual void BrightnessChanged(int level, bool user_initiated) OVERRIDE;

  void HandleBrightnessChanged(double percent, bool user_initiated);

  base::WeakPtrFactory<TrayBrightness> weak_ptr_factory_;

  tray::BrightnessView* brightness_view_;

  
  
  
  bool is_default_view_;

  
  
  double current_percent_;

  
  bool got_current_percent_;

  DISALLOW_COPY_AND_ASSIGN(TrayBrightness);
};

}  
}  

#endif  
