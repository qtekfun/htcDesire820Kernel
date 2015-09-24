// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_SCREEN_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_SCREEN_OBSERVER_H_

#include <string>

namespace chromeos {

class ErrorScreen;
class WizardScreen;

class ScreenObserver {
 public:
  
  
  
  
  
  enum ExitCodes {
    
    NETWORK_CONNECTED,
    
    CONNECTION_FAILED,
    UPDATE_INSTALLED,
    UPDATE_NOUPDATE,
    UPDATE_ERROR_CHECKING_FOR_UPDATE,
    UPDATE_ERROR_UPDATING,
    USER_IMAGE_SELECTED,
    EULA_ACCEPTED,
    EULA_BACK,
    ENTERPRISE_ENROLLMENT_COMPLETED,
    ENTERPRISE_AUTO_MAGIC_ENROLLMENT_COMPLETED,
    ENTERPRISE_ENROLLMENT_BACK,
    RESET_CANCELED,
    KIOSK_AUTOLAUNCH_CANCELED,
    KIOSK_AUTOLAUNCH_CONFIRMED,
    KIOSK_ENABLE_COMPLETED,
    TERMS_OF_SERVICE_DECLINED,
    TERMS_OF_SERVICE_ACCEPTED,
    WRONG_HWID_WARNING_SKIPPED,
    EXIT_CODES_COUNT  
  };

  
  virtual void OnExit(ExitCodes exit_code) = 0;

  
  virtual void ShowCurrentScreen() = 0;

  
  
  virtual void OnSetUserNamePassword(const std::string& username,
                                     const std::string& password) = 0;

  
  virtual void SetUsageStatisticsReporting(bool val) = 0;
  virtual bool GetUsageStatisticsReporting() const = 0;

  virtual ErrorScreen* GetErrorScreen() = 0;
  virtual void ShowErrorScreen() = 0;
  virtual void HideErrorScreen(WizardScreen* parent_screen) = 0;

 protected:
  virtual ~ScreenObserver() {}
};

}  

#endif  
