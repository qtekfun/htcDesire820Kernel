// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_MANAGER_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_MANAGER_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/stl_util-inl.h"
#include "chrome/browser/password_manager/password_form_manager.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/ui/login/login_model.h"
#include "content/browser/tab_contents/tab_contents_observer.h"
#include "webkit/glue/password_form.h"
#include "webkit/glue/password_form_dom_manager.h"

class PasswordManagerDelegate;
class PasswordManagerTest;
class PasswordFormManager;
class PrefService;

class PasswordManager : public LoginModel,
                        public TabContentsObserver {
 public:
  static void RegisterUserPrefs(PrefService* prefs);

  
  PasswordManager(TabContents* tab_contents,
                  PasswordManagerDelegate* delegate);
  virtual ~PasswordManager();

  
  
  void Autofill(const webkit_glue::PasswordForm& form_for_autofill,
                const webkit_glue::PasswordFormMap& best_matches,
                const webkit_glue::PasswordForm* const preferred_match,
                bool wait_for_username) const;

  
  virtual void SetObserver(LoginModelObserver* observer);

  
  
  
  void ProvisionallySavePassword(webkit_glue::PasswordForm form);

  
  virtual void DidStopLoading();
  virtual void DidNavigateAnyFramePostCommit(
      const NavigationController::LoadCommittedDetails& details,
      const ViewHostMsg_FrameNavigate_Params& params);
  virtual bool OnMessageReceived(const IPC::Message& message);

  void OnPasswordFormsFound(
      const std::vector<webkit_glue::PasswordForm>& forms);
  void OnPasswordFormsVisible(
      const std::vector<webkit_glue::PasswordForm>& visible_forms);

 private:
  FRIEND_TEST_ALL_PREFIXES(PasswordManagerTest, FormSeenThenLeftPage);

  
  
  
  
  
  
  
  
  
  
  
  
  

  
  void ClearProvisionalSave();

  
  
  
  void DidNavigate();

  typedef std::vector<PasswordFormManager*> LoginManagers;
  LoginManagers pending_login_managers_;

  
  
  STLElementDeleter<LoginManagers> login_managers_deleter_;

  
  
  
  
  
  
  scoped_ptr<PasswordFormManager> provisional_save_manager_;

  
  
  PasswordManagerDelegate* delegate_;

  
  LoginModelObserver* observer_;

  
  
  BooleanPrefMember password_manager_enabled_;

  DISALLOW_COPY_AND_ASSIGN(PasswordManager);
};

#endif  
