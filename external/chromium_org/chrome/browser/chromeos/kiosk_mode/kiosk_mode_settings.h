// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_KIOSK_MODE_KIOSK_MODE_SETTINGS_H_
#define CHROME_BROWSER_CHROMEOS_KIOSK_MODE_KIOSK_MODE_SETTINGS_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/policy/app_pack_updater.h"
#include "chrome/browser/chromeos/settings/device_settings_service.h"

namespace base {
template <typename T> struct DefaultLazyInstanceTraits;
}

namespace chromeos {

class KioskModeSettings {
 public:
  static KioskModeSettings* Get();

  
  virtual bool IsKioskModeEnabled();

  
  
  virtual void Initialize(const base::Closure& notify_initialized);
  virtual bool is_initialized() const;

  
  
  virtual void GetScreensaverPath(
      policy::AppPackUpdater::ScreenSaverUpdateCallback callback) const;

  
  virtual base::TimeDelta GetScreensaverTimeout() const;

  
  
  
  
  
  virtual base::TimeDelta GetIdleLogoutTimeout() const;

  
  virtual base::TimeDelta GetIdleLogoutWarningDuration() const;

  static const int kMaxIdleLogoutTimeout;
  static const int kMinIdleLogoutTimeout;

  static const int kMaxIdleLogoutWarningDuration;
  static const int kMinIdleLogoutWarningDuration;

 protected:
  
  KioskModeSettings();
  virtual ~KioskModeSettings();

 private:
  friend struct base::DefaultLazyInstanceTraits<KioskModeSettings>;
  friend class KioskModeSettingsTest;

  
  
  void VerifyModeIsKnown(DeviceSettingsService::OwnershipStatus status);

  bool is_initialized_;
  bool is_kiosk_mode_;

  
  void set_initialized(bool value) { is_initialized_ = value; }

  std::string screensaver_id_;
  std::string screensaver_path_;
  base::TimeDelta screensaver_timeout_;
  base::TimeDelta idle_logout_timeout_;
  base::TimeDelta idle_logout_warning_duration_;

  DISALLOW_COPY_AND_ASSIGN(KioskModeSettings);
};

}  

#endif  
