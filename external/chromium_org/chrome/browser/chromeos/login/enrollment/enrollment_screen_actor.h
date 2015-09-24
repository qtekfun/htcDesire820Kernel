// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_ENROLLMENT_ENROLLMENT_SCREEN_ACTOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_ENROLLMENT_ENROLLMENT_SCREEN_ACTOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "chrome/browser/chromeos/policy/enrollment_status_chromeos.h"

class GoogleServiceAuthError;

namespace chromeos {

class EnrollmentScreenActor {
 public:
  
  
  enum UIError {
    
    UI_ERROR_DOMAIN_MISMATCH,
    
    UI_ERROR_AUTO_ENROLLMENT_BAD_MODE,
    
    UI_ERROR_FATAL,
  };

  
  
  class Controller {
   public:
    virtual ~Controller() {}

    virtual void OnLoginDone(const std::string& user) = 0;
    virtual void OnAuthError(const GoogleServiceAuthError& error) = 0;
    virtual void OnOAuthTokenAvailable(const std::string& oauth_token) = 0;
    virtual void OnRetry() = 0;
    virtual void OnCancel() = 0;
    virtual void OnConfirmationClosed() = 0;
  };

  virtual ~EnrollmentScreenActor() {}

  
  virtual void SetParameters(Controller* controller,
                             bool is_auto_enrollment,
                             bool can_exit_enrollment,
                             const std::string& user) = 0;

  
  virtual void PrepareToShow() = 0;

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  virtual void FetchOAuthToken() = 0;

  
  
  virtual void ResetAuth(const base::Closure& callback) = 0;

  
  virtual void ShowSigninScreen() = 0;

  
  virtual void ShowEnrollmentSpinnerScreen() = 0;

  
  virtual void ShowLoginSpinnerScreen() = 0;

  
  virtual void ShowAuthError(const GoogleServiceAuthError& error) = 0;

  
  virtual void ShowUIError(UIError error) = 0;

  
  virtual void ShowEnrollmentStatus(policy::EnrollmentStatus status) = 0;
};

}  

#endif  
