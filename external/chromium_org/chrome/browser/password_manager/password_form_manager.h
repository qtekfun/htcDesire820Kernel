// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_FORM_MANAGER_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_FORM_MANAGER_H_

#include <string>
#include <vector>

#include "build/build_config.h"

#include "base/stl_util.h"
#include "chrome/browser/password_manager/password_store.h"
#include "chrome/browser/password_manager/password_store_consumer.h"
#include "components/autofill/core/common/password_form.h"

namespace content {
class WebContents;
}  

class PasswordManager;
class Profile;

class PasswordFormManager : public PasswordStoreConsumer {
 public:
  
  
  
  
  
  
  PasswordFormManager(Profile* profile,
                      PasswordManager* password_manager,
                      content::WebContents* web_contents,
                      const autofill::PasswordForm& observed_form,
                      bool ssl_valid);
  virtual ~PasswordFormManager();

  enum ActionMatch {
    ACTION_MATCH_REQUIRED,
    ACTION_MATCH_NOT_REQUIRED
  };

  enum OtherPossibleUsernamesAction {
    ALLOW_OTHER_POSSIBLE_USERNAMES,
    IGNORE_OTHER_POSSIBLE_USERNAMES
  };

  
  
  bool DoesManage(const autofill::PasswordForm& form,
                  ActionMatch action_match) const;

  
  
  
  
  void FetchMatchingLoginsFromPasswordStore(
      PasswordStore::AuthorizationPromptPolicy prompt_policy);

  
  
  
  
  
  
  
  
  
  
  
  
  bool HasCompletedMatching();

  
  bool IsBlacklisted();

  
  
  
  bool IsNewLogin();

  
  
  
  bool IsPendingCredentialsPublicSuffixMatch();

  
  
  bool HasValidPasswordForm();

  
  
  bool HasGeneratedPassword();
  void SetHasGeneratedPassword();

  
  void OnRequestDone(const std::vector<autofill::PasswordForm*>& result);

  
  virtual void OnPasswordStoreRequestDone(
      CancelableRequestProvider::Handle handle,
      const std::vector<autofill::PasswordForm*>& result) OVERRIDE;
  virtual void OnGetPasswordStoreResults(
      const std::vector<autofill::PasswordForm*>& results) OVERRIDE;

  
  
  
  void PermanentlyBlacklist();

  
  
  
  
  
  
  
  void ProvisionallySave(const autofill::PasswordForm& credentials,
                         OtherPossibleUsernamesAction action);

  
  
  
  
  void Save();

  
  
  void SubmitPassed();
  void SubmitFailed();

  
  const base::string16& associated_username() const {
    return pending_credentials_.username_value;
  }

  
  const autofill::PasswordForm pending_credentials() const {
    return pending_credentials_;
  }

  
  const autofill::PasswordFormMap best_matches() const {
    return best_matches_;
  }

  
  const std::string& realm() const {
    return pending_credentials_.signon_realm;
  }

 private:
  friend class PasswordFormManagerTest;

  
  
  
  
  
  
  enum ManagerAction {
    kManagerActionNone = 0,
    kManagerActionAutofilled,
    kManagerActionBlacklisted,
    kManagerActionMax
  };

  
  
  
  
  
  
  
  enum UserAction {
    kUserActionNone = 0,
    kUserActionChoose,
    kUserActionChoosePslMatch,
    kUserActionOverride,
    kUserActionMax
  };

  
  enum SubmitResult {
    kSubmitResultNotSubmitted = 0,
    kSubmitResultFailed,
    kSubmitResultPassed,
    kSubmitResultMax
  };

  
  
  static const int kMaxNumActionsTaken = kManagerActionMax * kUserActionMax *
                                         kSubmitResultMax;

  
  
  bool IgnoreResult(const autofill::PasswordForm& form) const;

  
  
  
  
  void SaveAsNewLogin(bool reset_preferred_login);

  
  
  int ScoreResult(const autofill::PasswordForm& form) const;

  
  
  
  
  
  void UpdateLogin();

  
  
  
  void CheckForAccountCreationForm(const autofill::PasswordForm& pending,
                                   const autofill::PasswordForm& observed);

  
  
  
  void UpdatePreferredLoginState(PasswordStore* password_store);

  
  
  
  bool UpdatePendingCredentialsIfOtherPossibleUsername(
      const base::string16& username);

  
  
  int GetActionsTaken();

  
  
  
  virtual void SendNotBlacklistedToRenderer();

  
  
  void SanitizePossibleUsernames(autofill::PasswordForm* form);

  
  
  
  autofill::PasswordFormMap best_matches_;

  
  STLValueDeleter<autofill::PasswordFormMap> best_matches_deleter_;

  
  autofill::PasswordForm observed_form_;

  
  
  std::vector<std::string> form_path_tokens_;

  
  
  autofill::PasswordForm pending_credentials_;

  
  
  bool is_new_login_;

  
  bool has_generated_password_;

  
  
  base::string16 selected_username_;

  
  const PasswordManager* const password_manager_;

  
  
  
  
  const autofill::PasswordForm* preferred_match_;

  typedef enum {
    PRE_MATCHING_PHASE,      
                             
    MATCHING_PHASE,          
                             
    POST_MATCHING_PHASE      
                             
  } PasswordFormManagerState;

  
  
  
  PasswordFormManagerState state_;

  
  Profile* profile_;

  
  
  content::WebContents* web_contents_;

  
  
  
  ManagerAction manager_action_;
  UserAction user_action_;
  SubmitResult submit_result_;

  DISALLOW_COPY_AND_ASSIGN(PasswordFormManager);
};
#endif  
