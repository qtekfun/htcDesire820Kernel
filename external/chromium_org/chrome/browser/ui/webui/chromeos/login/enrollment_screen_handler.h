// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_ENROLLMENT_SCREEN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_ENROLLMENT_SCREEN_HANDLER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "chrome/browser/browsing_data/browsing_data_remover.h"
#include "chrome/browser/chromeos/login/enrollment/enrollment_screen_actor.h"
#include "chrome/browser/ui/webui/chromeos/login/base_screen_handler.h"

namespace policy {
class PolicyOAuth2TokenFetcher;
}

namespace chromeos {

class EnrollmentScreenHandler
    : public BaseScreenHandler,
      public EnrollmentScreenActor,
      public BrowsingDataRemover::Observer {
 public:
  EnrollmentScreenHandler();
  virtual ~EnrollmentScreenHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void SetParameters(Controller* controller,
                             bool is_auto_enrollment,
                             bool can_exit_enrollment,
                             const std::string& user) OVERRIDE;
  virtual void PrepareToShow() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void FetchOAuthToken() OVERRIDE;
  virtual void ResetAuth(const base::Closure& callback) OVERRIDE;
  virtual void ShowSigninScreen() OVERRIDE;
  virtual void ShowEnrollmentSpinnerScreen() OVERRIDE;
  virtual void ShowLoginSpinnerScreen() OVERRIDE;
  virtual void ShowAuthError(const GoogleServiceAuthError& error) OVERRIDE;
  virtual void ShowEnrollmentStatus(policy::EnrollmentStatus status) OVERRIDE;
  virtual void ShowUIError(UIError error_code) OVERRIDE;

  
  virtual void Initialize() OVERRIDE;
  virtual void DeclareLocalizedValues(LocalizedValuesBuilder* builder) OVERRIDE;

  
  virtual void OnBrowsingDataRemoverDone() OVERRIDE;

 private:
  
  void HandleClose(const std::string& reason);
  void HandleCompleteLogin(const std::string& user);
  void HandleRetry();

  
  void ShowStep(const char* step);

  
  void ShowError(int message_id, bool retry);

  
  void ShowErrorMessage(const std::string& message, bool retry);

  
  void ShowWorking(int message_id);

  
  void OnTokenFetched(const std::string& token,
                      const GoogleServiceAuthError& error);

  
  void DoShow();

  
  Controller* controller_;

  bool show_on_init_;

  
  bool is_auto_enrollment_;

  
  bool can_exit_enrollment_;

  
  bool enrollment_failed_once_;

  
  std::string user_;

  
  
  scoped_ptr<policy::PolicyOAuth2TokenFetcher> oauth_fetcher_;

  
  BrowsingDataRemover* browsing_data_remover_;

  
  std::vector<base::Closure> auth_reset_callbacks_;

  DISALLOW_COPY_AND_ASSIGN(EnrollmentScreenHandler);
};

}  

#endif  
