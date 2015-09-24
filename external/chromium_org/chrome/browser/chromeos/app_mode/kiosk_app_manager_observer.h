// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_APP_MODE_KIOSK_APP_MANAGER_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_APP_MODE_KIOSK_APP_MANAGER_OBSERVER_H_

#include <string>

namespace chromeos {

class KioskAppManagerObserver {
 public:
  
  virtual void OnKioskAppDataChanged(const std::string& app_id) {}

  
  virtual void OnKioskAppDataLoadFailure(const std::string& app_id) {}

  
  virtual void OnKioskAppsSettingsChanged() {}

 protected:
  virtual ~KioskAppManagerObserver() {}
};

}  

#endif  
