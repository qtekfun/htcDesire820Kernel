// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_SYNC_SETUP_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_SYNC_SETUP_HANDLER_H_

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "chrome/browser/sync/sync_startup_tracker.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "chrome/browser/ui/webui/signin/login_ui_service.h"

class LoginUIService;
class ProfileManager;
class ProfileSyncService;
class SigninManagerBase;

namespace content {
class WebContents;
}

class SyncSetupHandler : public options::OptionsPageUIHandler,
                         public SyncStartupTracker::Observer,
                         public LoginUIService::LoginUI {
 public:
  
  explicit SyncSetupHandler(ProfileManager* profile_manager);
  virtual ~SyncSetupHandler();

  
  virtual void GetLocalizedValues(base::DictionaryValue* localized_strings)
      OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void SyncStartupCompleted() OVERRIDE;
  virtual void SyncStartupFailed() OVERRIDE;

  
  virtual void FocusUI() OVERRIDE;
  virtual void CloseUI() OVERRIDE;

  static void GetStaticLocalizedValues(
      base::DictionaryValue* localized_strings,
      content::WebUI* web_ui);

  
  void OpenSyncSetup();

  
  
  
  void OpenConfigureSync();

  
  void CloseSyncSetup();

 protected:
  friend class SyncSetupHandlerTest;
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerTest, DisplayBasicLogin);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerTest,
                           DisplayConfigureWithBackendDisabledAndCancel);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerTest, SelectCustomEncryption);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerTest, ShowSyncSetupWhenNotSignedIn);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerTest, SuccessfullySetPassphrase);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerTest, TestSyncEverything);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerTest, TestSyncNothing);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerTest, TestSyncAllManually);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerTest, TestPassphraseStillRequired);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerTest, TestSyncIndividualTypes);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerTest, TurnOnEncryptAll);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerTest, UnsuccessfullySetPassphrase);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerNonCrosTest,
                           UnrecoverableErrorInitializingSync);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerNonCrosTest,
                           GaiaErrorInitializingSync);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerNonCrosTest, HandleCaptcha);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerNonCrosTest, HandleGaiaAuthFailure);
  FRIEND_TEST_ALL_PREFIXES(SyncSetupHandlerNonCrosTest,
                           SubmitAuthWithInvalidUsername);

  bool is_configuring_sync() const { return configuring_sync_; }

  
  
  
  
  
  
  virtual void DisplayConfigureSync(bool show_advanced, bool passphrase_failed);

  
  void ConfigureSyncDone();

  
  
  ProfileSyncService* GetSyncService() const;

  
  LoginUIService* GetLoginUIService() const;

 private:
  
  void OnDidClosePage(const base::ListValue* args);
  void HandleConfigure(const base::ListValue* args);
  void HandlePassphraseEntry(const base::ListValue* args);
  void HandlePassphraseCancel(const base::ListValue* args);
  void HandleShowSetupUI(const base::ListValue* args);
  void HandleDoSignOutOnAuthError(const base::ListValue* args);
  void HandleStartSignin(const base::ListValue* args);
  void HandleStopSyncing(const base::ListValue* args);
  void HandleCloseTimeout(const base::ListValue* args);
#if !defined(OS_CHROMEOS)
  
  void DisplayGaiaLogin();

  
  
  virtual void DisplayGaiaLoginInNewTabOrWindow();
#endif

  
  
  virtual Profile* GetProfile() const;

  
  
  bool PrepareSyncSetup();

  
  
  
  
  void DisplaySpinner();

  
  void DisplayTimeout();

  
  bool IsActiveLogin() const;

  
  bool IsExistingWizardPresent();

  
  bool FocusExistingWizardIfPresent();

  
  scoped_ptr<SyncStartupTracker> sync_startup_tracker_;

  
  
  
  bool configuring_sync_;

  
  ProfileManager* const profile_manager_;

  
  
  scoped_ptr<base::OneShotTimer<SyncSetupHandler> > backend_start_timer_;

  DISALLOW_COPY_AND_ASSIGN(SyncSetupHandler);
};

#endif  
