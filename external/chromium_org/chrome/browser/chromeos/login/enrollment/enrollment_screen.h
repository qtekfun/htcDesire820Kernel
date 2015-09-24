// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_ENROLLMENT_ENROLLMENT_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_ENROLLMENT_ENROLLMENT_SCREEN_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/login/enrollment/enrollment_screen_actor.h"
#include "chrome/browser/chromeos/login/screens/wizard_screen.h"
#include "components/policy/core/common/cloud/cloud_policy_constants.h"

namespace chromeos {

class ScreenObserver;

class EnrollmentScreen
    : public WizardScreen,
      public EnrollmentScreenActor::Controller {
 public:
  EnrollmentScreen(ScreenObserver* observer,
                   EnrollmentScreenActor* actor);
  virtual ~EnrollmentScreen();

  void SetParameters(bool is_auto_enrollment,
                     bool can_exit_enrollment,
                     const std::string& enrollment_user);

  
  virtual void PrepareToShow() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual std::string GetName() const OVERRIDE;

  
  virtual void OnLoginDone(const std::string& user) OVERRIDE;
  virtual void OnAuthError(const GoogleServiceAuthError& error) OVERRIDE;
  virtual void OnOAuthTokenAvailable(const std::string& oauth_token) OVERRIDE;
  virtual void OnRetry() OVERRIDE;
  virtual void OnCancel() OVERRIDE;
  virtual void OnConfirmationClosed() OVERRIDE;

  
  EnrollmentScreenActor* GetActor() {
    return actor_;
  }

 private:
  
  void WriteInstallAttributesData();

  
  void RegisterForDevicePolicy(const std::string& token);

  
  
  void ReportEnrollmentStatus(policy::EnrollmentStatus status);

  
  
  
  void UMAFailure(int sample);

  
  void ShowSigninScreen();

  EnrollmentScreenActor* actor_;
  bool is_auto_enrollment_;
  bool can_exit_enrollment_;
  bool enrollment_failed_once_;
  std::string user_;
  int lockbox_init_duration_;
  base::WeakPtrFactory<EnrollmentScreen> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(EnrollmentScreen);
};

}  

#endif  
