// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SYNC_ONE_CLICK_SIGNIN_SYNC_STARTER_H_
#define CHROME_BROWSER_UI_SYNC_ONE_CLICK_SIGNIN_SYNC_STARTER_H_

#include <string>

#include "base/callback_forward.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/signin/signin_tracker.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/host_desktop.h"
#include "chrome/browser/ui/sync/profile_signin_confirmation_helper.h"
#include "content/public/browser/web_contents_observer.h"

class Browser;
class ProfileSyncService;

namespace content {
class WebContents;
}  

class OneClickSigninSyncStarter : public SigninTracker::Observer,
                                  public chrome::BrowserListObserver,
                                  public content::WebContentsObserver {
 public:
  enum StartSyncMode {
    
    
    SYNC_WITH_DEFAULT_SETTINGS,

    
    
    
    CONFIGURE_SYNC_FIRST,

    
    
    
    SHOW_SETTINGS_WITHOUT_CONFIGURE,

    
    UNDO_SYNC
  };

  enum ConfirmationRequired {
    
    
    NO_CONFIRMATION,

    
    
    CONFIRM_UNTRUSTED_SIGNIN,

    
    CONFIRM_AFTER_SIGNIN
  };

  
  enum SyncSetupResult {
    SYNC_SETUP_SUCCESS,
    SYNC_SETUP_FAILURE
  };

  typedef base::Callback<void(SyncSetupResult)> Callback;

  
  
  
  
  
  
  
  
  OneClickSigninSyncStarter(Profile* profile,
                            Browser* browser,
                            const std::string& session_index,
                            const std::string& email,
                            const std::string& password,
                            const std::string& oauth_code,
                            StartSyncMode start_mode,
                            content::WebContents* web_contents,
                            ConfirmationRequired display_confirmation,
                            Callback callback);

  
  virtual void OnBrowserRemoved(Browser* browser) OVERRIDE;

 private:
  friend class OneClickSigninSyncStarterTest;
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninSyncStarterTest,
                           CallbackSigninFailed);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninSyncStarterTest,
                           CallbackSigninSucceeded);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninSyncStarterTest,
                           CallbackNull);

  virtual ~OneClickSigninSyncStarter();

  
  
  void Initialize(Profile* profile, Browser* browser);

  
  virtual void SigninFailed(const GoogleServiceAuthError& error) OVERRIDE;
  virtual void SigninSuccess() OVERRIDE;

#if defined(ENABLE_CONFIGURATION_POLICY)
  
  class SigninDialogDelegate
    : public ui::ProfileSigninConfirmationDelegate {
   public:
    SigninDialogDelegate(
        base::WeakPtr<OneClickSigninSyncStarter> sync_starter);
    virtual ~SigninDialogDelegate();
    virtual void OnCancelSignin() OVERRIDE;
    virtual void OnContinueSignin() OVERRIDE;
    virtual void OnSigninWithNewProfile() OVERRIDE;
   private:
    base::WeakPtr<OneClickSigninSyncStarter> sync_starter_;
  };
  friend class SigninDialogDelegate;

  
  
  void OnRegisteredForPolicy(const std::string& dm_token,
                             const std::string& client_id);

  
  
  void OnPolicyFetchComplete(bool success);

  
  
  void CreateNewSignedInProfile();

  
  
  void LoadPolicyWithCachedCredentials();

  
  
  void CompleteInitForNewProfile(chrome::HostDesktopType desktop_type,
                                 Profile* profile,
                                 Profile::CreateStatus status);

#endif  

  
  void CancelSigninAndDelete();

  
  
  void ConfirmSignin(const std::string& oauth_token);

  
  
  void ConfirmAndSignin();

  
  
  
  void UntrustedSigninConfirmed(StartSyncMode response);

  
  
  
  
  
  ProfileSyncService* GetProfileSyncService();

  void FinishProfileSyncServiceSetup();

  
  
  
  
  void ShowSettingsPage(bool configure_sync);

  
  
  void ShowSettingsPageInWebContents(content::WebContents* contents,
                                     const std::string& sub_page);

  
  
  void DisplayFinalConfirmationBubble(const base::string16& custom_message);

  
  
  
  void EnsureBrowser();

  Profile* profile_;
  Browser* browser_;
  scoped_ptr<SigninTracker> signin_tracker_;
  StartSyncMode start_mode_;
  chrome::HostDesktopType desktop_type_;
  bool force_same_tab_navigation_;
  ConfirmationRequired confirmation_required_;

  
  Callback sync_setup_completed_callback_;

#if defined(ENABLE_CONFIGURATION_POLICY)
  
  
  std::string dm_token_;
  std::string client_id_;
#endif

  base::WeakPtrFactory<OneClickSigninSyncStarter> weak_pointer_factory_;

  DISALLOW_COPY_AND_ASSIGN(OneClickSigninSyncStarter);
};


#endif  
