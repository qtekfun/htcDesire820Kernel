// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_KIOSK_MODE_MOCK_KIOSK_MODE_SETTINGS_H_
#define CHROME_BROWSER_CHROMEOS_KIOSK_MODE_MOCK_KIOSK_MODE_SETTINGS_H_

#include "chrome/browser/chromeos/kiosk_mode/kiosk_mode_settings.h"

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/time/time.h"

namespace base {
template <typename T> struct DefaultLazyInstanceTraits;
}

namespace {

const int kMockIdleLogoutTimeoutMs = 50000;
const int kMockIdleLogoutWarningDurationMs = 1000;

}  

namespace chromeos {

class MockKioskModeSettings : public KioskModeSettings {
 public:
  
  
  MockKioskModeSettings();
  virtual ~MockKioskModeSettings();

  virtual bool IsKioskModeEnabled() OVERRIDE;

  
  virtual void Initialize(const base::Closure& notify_initialized) OVERRIDE;
  virtual bool is_initialized() const OVERRIDE;

  
  virtual base::TimeDelta GetIdleLogoutTimeout() const OVERRIDE;

  
  virtual base::TimeDelta GetIdleLogoutWarningDuration() const OVERRIDE;

 private:
  friend struct base::DefaultLazyInstanceTraits<MockKioskModeSettings>;

  bool is_initialized_;

  DISALLOW_COPY_AND_ASSIGN(MockKioskModeSettings);
};

}  

#endif  
