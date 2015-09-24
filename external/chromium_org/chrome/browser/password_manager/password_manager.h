// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_MANAGER_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_MANAGER_H_

#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "base/prefs/pref_member.h"
#include "base/stl_util.h"
#include "chrome/browser/password_manager/password_form_manager.h"
#include "chrome/browser/ui/login/login_model.h"
#include "components/autofill/core/common/password_form.h"
#include "components/autofill/core/common/password_form_fill_data.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

class PasswordManagerDelegate;
class PasswordManagerTest;
class PasswordFormManager;
class PrefRegistrySimple;

namespace user_prefs {
class PrefRegistrySyncable;
}

class PasswordManager : public LoginModel,
                        public content::WebContentsObserver,
                        public content::WebContentsUserData<PasswordManager> {
 public:
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);
#if defined(OS_WIN)
  static void RegisterLocalPrefs(PrefRegistrySimple* registry);
#endif
  static void CreateForWebContentsAndDelegate(
      content::WebContents* contents,
      PasswordManagerDelegate* delegate);
  virtual ~PasswordManager();

  typedef base::Callback<void(const autofill::PasswordForm&)>
      PasswordSubmittedCallback;

  
  
  
  void AddSubmissionCallback(const PasswordSubmittedCallback& callback);

  
  
  
  bool IsSavingEnabled() const;

  
  
  virtual void Autofill(const autofill::PasswordForm& form_for_autofill,
                        const autofill::PasswordFormMap& best_matches,
                        const autofill::PasswordForm& preferred_match,
                        bool wait_for_username) const;

  
  virtual void AddObserver(LoginModelObserver* observer) OVERRIDE;
  virtual void RemoveObserver(LoginModelObserver* observer) OVERRIDE;

  
  void SetFormHasGeneratedPassword(const autofill::PasswordForm& form);

  
  
  
  
  
  void ProvisionallySavePassword(const autofill::PasswordForm& form);

  
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  
  void OnPasswordFormsParsed(
      const std::vector<autofill::PasswordForm>& forms);
  void OnPasswordFormsRendered(
      const std::vector<autofill::PasswordForm>& visible_forms);

 protected:
  
  PasswordManager(content::WebContents* web_contents,
                  PasswordManagerDelegate* delegate);

  
  virtual void OnPasswordFormSubmitted(
      const autofill::PasswordForm& password_form);

 private:
  friend class content::WebContentsUserData<PasswordManager>;

  enum ProvisionalSaveFailure {
    SAVING_DISABLED,
    EMPTY_PASSWORD,
    NO_MATCHING_FORM,
    MATCHING_NOT_COMPLETE,
    FORM_BLACKLISTED,
    INVALID_FORM,
    AUTOCOMPLETE_OFF,
    MAX_FAILURE_VALUE
  };

  
  
  void RecordFailure(ProvisionalSaveFailure failure,
                     const std::string& form_origin);

  
  
  
  
  
  
  void PossiblyInitializeUsernamesExperiment(
      const autofill::PasswordFormMap& matches) const;

  
  
  bool OtherPossibleUsernamesEnabled() const;

  
  
  bool ShouldShowSavePasswordInfoBar() const;

  
  
  
  
  
  
  
  
  
  
  
  
  

  ScopedVector<PasswordFormManager> pending_login_managers_;

  
  
  
  
  
  
  scoped_ptr<PasswordFormManager> provisional_save_manager_;

  
  
  PasswordManagerDelegate* const delegate_;

  
  
  BooleanPrefMember password_manager_enabled_;

  
  
  mutable ObserverList<LoginModelObserver> observers_;

  
  std::vector<PasswordSubmittedCallback> submission_callbacks_;

  DISALLOW_COPY_AND_ASSIGN(PasswordManager);
};

#endif  
