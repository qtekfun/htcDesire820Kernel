// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_FORM_MANAGER_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_FORM_MANAGER_H_
#pragma once

#include <string>
#include <vector>

#include "build/build_config.h"

#include "base/stl_util-inl.h"
#include "chrome/browser/password_manager/password_store_consumer.h"
#include "webkit/glue/password_form.h"

class PasswordManager;
class PasswordStore;
class Profile;

class PasswordFormManager : public PasswordStoreConsumer {
 public:
  
  
  
  
  
  
  PasswordFormManager(Profile* profile,
                      PasswordManager* password_manager,
                      const webkit_glue::PasswordForm& observed_form,
                      bool ssl_valid);
  virtual ~PasswordFormManager();

  
  bool DoesManage(const webkit_glue::PasswordForm& form) const;

  
  void FetchMatchingLoginsFromPasswordStore();

  
  
  
  
  
  
  
  
  
  
  
  
  bool HasCompletedMatching();

  
  bool IsBlacklisted();

  
  
  
  bool IsNewLogin();

  
  
  bool HasValidPasswordForm();

  
  void OnRequestDone(
      int handle, const std::vector<webkit_glue::PasswordForm*>& result);

  
  virtual void OnPasswordStoreRequestDone(
      CancelableRequestProvider::Handle handle,
      const std::vector<webkit_glue::PasswordForm*>& result);

  
  
  void PermanentlyBlacklist();

  
  
  
  void ProvisionallySave(const webkit_glue::PasswordForm& credentials);

  
  
  
  void Save();

  
  
  void SubmitPassed();
  void SubmitFailed();

 private:
  friend class PasswordFormManagerTest;

  
  
  
  
  
  
  enum ManagerAction {
    kManagerActionNone = 0,
    kManagerActionAutofilled,
    kManagerActionBlacklisted,
    kManagerActionDisabled,
    kManagerActionMax
  };

  
  
  
  
  
  
  enum UserAction {
    kUserActionNone = 0,
    kUserActionChoose,
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

  
  
  bool IgnoreResult(const webkit_glue::PasswordForm& form) const;

  
  
  
  
  void SaveAsNewLogin(bool reset_preferred_login);

  
  
  int ScoreResult(const webkit_glue::PasswordForm& form) const;

  
  
  
  
  
  void UpdateLogin();

  
  
  
  void UpdatePreferredLoginState(PasswordStore* password_store);

  
  
  int GetActionsTaken();

  
  
  
  webkit_glue::PasswordFormMap best_matches_;

  
  STLValueDeleter<webkit_glue::PasswordFormMap> best_matches_deleter_;

  
  webkit_glue::PasswordForm observed_form_;

  
  
  std::vector<std::string> form_path_tokens_;

  
  
  webkit_glue::PasswordForm pending_credentials_;

  
  
  bool is_new_login_;

  
  const PasswordManager* const password_manager_;

  
  CancelableRequestProvider::Handle pending_login_query_;

  
  
  
  
  const webkit_glue::PasswordForm* preferred_match_;

  typedef enum {
    PRE_MATCHING_PHASE,      
                             
    MATCHING_PHASE,          
                             
    POST_MATCHING_PHASE      
                             
  } PasswordFormManagerState;

  
  
  
  PasswordFormManagerState state_;

  
  Profile* profile_;

  
  
  
  ManagerAction manager_action_;
  UserAction user_action_;
  SubmitResult submit_result_;

  DISALLOW_COPY_AND_ASSIGN(PasswordFormManager);
};
#endif  
