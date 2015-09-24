// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_OOBE_DISPLAY_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_OOBE_DISPLAY_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"

namespace chromeos {

class AppLaunchSplashScreenActor;
class EnrollmentScreenActor;
class ErrorScreenActor;
class EulaScreenActor;
class KioskAutolaunchScreenActor;
class KioskEnableScreenActor;
class NetworkScreenActor;
class ResetScreenActor;
class TermsOfServiceScreenActor;
class UpdateScreenActor;
class UserImageScreenActor;
class ViewScreenDelegate;
class WizardScreen;
class WrongHWIDScreenActor;
class LocallyManagedUserCreationScreenHandler;

class OobeDisplay {
 public:
  enum Screen {
    SCREEN_OOBE_NETWORK = 0,
    SCREEN_OOBE_EULA,
    SCREEN_OOBE_UPDATE,
    SCREEN_OOBE_ENROLLMENT,
    SCREEN_GAIA_SIGNIN,
    SCREEN_ACCOUNT_PICKER,
    SCREEN_KIOSK_AUTOLAUNCH,
    SCREEN_KIOSK_ENABLE,
    SCREEN_ERROR_MESSAGE,
    SCREEN_USER_IMAGE_PICKER,
    SCREEN_TPM_ERROR,
    SCREEN_PASSWORD_CHANGED,
    SCREEN_CREATE_MANAGED_USER_DIALOG,
    SCREEN_CREATE_MANAGED_USER_FLOW,
    SCREEN_TERMS_OF_SERVICE,
    SCREEN_WRONG_HWID,
    SCREEN_APP_LAUNCH_SPLASH,
    SCREEN_CONFIRM_PASSWORD,
    SCREEN_MESSAGE_BOX,
    SCREEN_UNKNOWN
  };

  virtual ~OobeDisplay() {}

  
  virtual void ShowScreen(WizardScreen* screen) = 0;

  
  virtual void HideScreen(WizardScreen* screen) = 0;

  
  
  virtual UpdateScreenActor* GetUpdateScreenActor() = 0;
  virtual NetworkScreenActor* GetNetworkScreenActor() = 0;
  virtual EulaScreenActor* GetEulaScreenActor() = 0;
  virtual EnrollmentScreenActor* GetEnrollmentScreenActor() = 0;
  virtual ResetScreenActor* GetResetScreenActor() = 0;
  virtual KioskAutolaunchScreenActor* GetKioskAutolaunchScreenActor() = 0;
  virtual KioskEnableScreenActor* GetKioskEnableScreenActor() = 0;
  virtual TermsOfServiceScreenActor* GetTermsOfServiceScreenActor() = 0;
  virtual UserImageScreenActor* GetUserImageScreenActor() = 0;
  virtual ErrorScreenActor* GetErrorScreenActor() = 0;
  virtual WrongHWIDScreenActor* GetWrongHWIDScreenActor() = 0;
  virtual LocallyManagedUserCreationScreenHandler*
      GetLocallyManagedUserCreationScreenActor() = 0;
  virtual AppLaunchSplashScreenActor* GetAppLaunchSplashScreenActor() = 0;

  
  
  
  virtual bool IsJSReady(const base::Closure& display_is_ready_callback) = 0;
};

}  

#endif  