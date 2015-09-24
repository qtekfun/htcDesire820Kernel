// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_SIGNIN_SCREEN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_SIGNIN_SCREEN_HANDLER_H_

#include <set>
#include <string>

#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/login/help_app_launcher.h"
#include "chrome/browser/chromeos/login/login_display.h"
#include "chrome/browser/chromeos/login/screens/error_screen_actor.h"
#include "chrome/browser/chromeos/login/user_manager.h"
#include "chrome/browser/chromeos/login/wallpaper_manager.h"
#include "chrome/browser/chromeos/net/network_portal_detector.h"
#include "chrome/browser/chromeos/settings/cros_settings.h"
#include "chrome/browser/chromeos/system_key_event_listener.h"
#include "chrome/browser/ui/webui/chromeos/login/base_screen_handler.h"
#include "chrome/browser/ui/webui/chromeos/login/gaia_screen_handler.h"
#include "chrome/browser/ui/webui/chromeos/login/network_state_informer.h"
#include "chrome/browser/ui/webui/chromeos/login/oobe_ui.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_ui.h"
#include "net/base/net_errors.h"

namespace base {
class DictionaryValue;
class ListValue;
}

namespace chromeos {

class CaptivePortalWindowProxy;
class CoreOobeActor;
class LocallyManagedUserCreationScreenHandler;
class NativeWindowDelegate;
class User;
struct UserContext;

class LoginScreenContext {
 public:
  LoginScreenContext();
  explicit LoginScreenContext(const base::ListValue* args);

  void set_email(const std::string& email) { email_ = email; }
  const std::string& email() const { return email_; }

  void set_oobe_ui(bool oobe_ui) { oobe_ui_ = oobe_ui; }
  bool oobe_ui() const { return oobe_ui_; }

 private:
  void Init();

  std::string email_;
  bool oobe_ui_;
};

class LoginDisplayWebUIHandler {
 public:
  virtual void ClearAndEnablePassword() = 0;
  virtual void ClearUserPodPassword() = 0;
  virtual void OnLoginSuccess(const std::string& username) = 0;
  virtual void OnUserRemoved(const std::string& username) = 0;
  virtual void OnUserImageChanged(const User& user) = 0;
  virtual void OnPreferencesChanged() = 0;
  virtual void ResetSigninScreenHandlerDelegate() = 0;
  virtual void ShowBannerMessage(const std::string& message) = 0;
  virtual void ShowUserPodButton(const std::string& username,
                                 const std::string& iconURL,
                                 const base::Closure& click_callback) = 0;
  virtual void ShowError(int login_attempts,
                         const std::string& error_text,
                         const std::string& help_link_text,
                         HelpAppLauncher::HelpTopic help_topic_id) = 0;
  virtual void ShowErrorScreen(LoginDisplay::SigninError error_id) = 0;
  virtual void ShowGaiaPasswordChanged(const std::string& username) = 0;
  virtual void ShowSigninUI(const std::string& email) = 0;
  virtual void ShowPasswordChangedDialog(bool show_password_error) = 0;
  
  virtual void ShowSigninScreenForCreds(const std::string& username,
                                        const std::string& password) = 0;
 protected:
  virtual ~LoginDisplayWebUIHandler() {}
};

class SigninScreenHandlerDelegate {
 public:
  
  virtual void CancelPasswordChangedFlow() = 0;

  
  virtual void CancelUserAdding() = 0;

  
  virtual void CreateAccount() = 0;

  
  
  virtual void CompleteLogin(const UserContext& user_context) = 0;

  
  
  virtual void Login(const UserContext& user_context) = 0;

  
  virtual void LoginAsRetailModeUser() = 0;

  
  virtual void LoginAsGuest() = 0;

  
  virtual void LoginAsPublicAccount(const std::string& username) = 0;

  
  
  virtual void MigrateUserData(const std::string& old_password) = 0;

  
  virtual void LoadWallpaper(const std::string& username) = 0;

  
  virtual void LoadSigninWallpaper() = 0;

  
  virtual void OnSigninScreenReady() = 0;

  
  virtual void RemoveUser(const std::string& username) = 0;

  
  
  virtual void ResyncUserData() = 0;

  
  virtual void ShowEnterpriseEnrollmentScreen() = 0;

  
  virtual void ShowKioskEnableScreen() = 0;

  
  virtual void ShowResetScreen() = 0;

  
  virtual void ShowKioskAutolaunchScreen() = 0;

  
  virtual void ShowWrongHWIDScreen() = 0;

  
  virtual void SetWebUIHandler(LoginDisplayWebUIHandler* webui_handler) = 0;

  
  virtual const UserList& GetUsers() const = 0;

  
  virtual bool IsShowGuest() const = 0;

  
  virtual bool IsShowUsers() const = 0;

  
  virtual bool IsShowNewUser() const = 0;

  
  virtual bool IsSigninInProgress() const = 0;

  
  virtual bool IsUserSigninCompleted() const = 0;

  
  
  virtual void SetDisplayEmail(const std::string& email) = 0;

  
  virtual void Signout() = 0;

  
  virtual void LoginAsKioskApp(const std::string& app_id) = 0;

 protected:
  virtual ~SigninScreenHandlerDelegate() {}
};

class SigninScreenHandler
    : public BaseScreenHandler,
      public LoginDisplayWebUIHandler,
      public SystemKeyEventListener::CapsLockObserver,
      public content::NotificationObserver,
      public NetworkStateInformer::NetworkStateInformerObserver,
      public WallpaperManager::Observer {
 public:
  SigninScreenHandler(
      const scoped_refptr<NetworkStateInformer>& network_state_informer,
      ErrorScreenActor* error_screen_actor,
      CoreOobeActor* core_oobe_actor,
      GaiaScreenHandler* gaia_screen_handler);
  virtual ~SigninScreenHandler();

  
  void Show(const LoginScreenContext& context);

  
  void ShowRetailModeLoginSpinner();

  
  
  void SetDelegate(SigninScreenHandlerDelegate* delegate);

  void SetNativeWindowDelegate(NativeWindowDelegate* native_window_delegate);

  
  virtual void OnNetworkReady() OVERRIDE;
  virtual void UpdateState(ErrorScreenActor::ErrorReason reason) OVERRIDE;

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  void set_kiosk_enable_flow_aborted_callback_for_test(
      const base::Closure& callback) {
    kiosk_enable_flow_aborted_callback_for_test_ = callback;
  }

  
  virtual void OnWallpaperAnimationFinished(const std::string& email) OVERRIDE;

 private:
  enum UIState {
    UI_STATE_UNKNOWN = 0,
    UI_STATE_GAIA_SIGNIN,
    UI_STATE_ACCOUNT_PICKER,
  };

  typedef base::hash_set<std::string> WebUIObservers;

  friend class ReportDnsCacheClearedOnUIThread;
  friend class LocallyManagedUserCreationScreenHandler;

  void ShowImpl();

  
  
  
  void UpdateUIState(UIState ui_state, DictionaryValue* params);

  void UpdateStateInternal(ErrorScreenActor::ErrorReason reason,
                           bool force_update);
  void SetupAndShowOfflineMessage(NetworkStateInformer::State state,
                                  ErrorScreenActor::ErrorReason reason);
  void HideOfflineMessage(NetworkStateInformer::State state,
                          ErrorScreenActor::ErrorReason reason);
  void ReloadGaiaScreen();

  
  virtual void DeclareLocalizedValues(LocalizedValuesBuilder* builder) OVERRIDE;
  virtual void Initialize() OVERRIDE;
  virtual gfx::NativeWindow GetNativeWindow() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void ClearAndEnablePassword() OVERRIDE;
  virtual void ClearUserPodPassword() OVERRIDE;
  virtual void OnLoginSuccess(const std::string& username) OVERRIDE;
  virtual void OnUserRemoved(const std::string& username) OVERRIDE;
  virtual void OnUserImageChanged(const User& user) OVERRIDE;
  virtual void OnPreferencesChanged() OVERRIDE;
  virtual void ResetSigninScreenHandlerDelegate() OVERRIDE;
  virtual void ShowBannerMessage(const std::string& message) OVERRIDE;
  virtual void ShowUserPodButton(const std::string& username,
                                 const std::string& iconURL,
                                 const base::Closure& click_callback) OVERRIDE;
  virtual void ShowError(int login_attempts,
                         const std::string& error_text,
                         const std::string& help_link_text,
                         HelpAppLauncher::HelpTopic help_topic_id) OVERRIDE;
  virtual void ShowGaiaPasswordChanged(const std::string& username) OVERRIDE;
  virtual void ShowSigninUI(const std::string& email) OVERRIDE;
  virtual void ShowPasswordChangedDialog(bool show_password_error) OVERRIDE;
  virtual void ShowErrorScreen(LoginDisplay::SigninError error_id) OVERRIDE;
  virtual void ShowSigninScreenForCreds(const std::string& username,
                                        const std::string& password) OVERRIDE;

  
  virtual void OnCapsLockChange(bool enabled) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void ShowSigninScreenIfReady();

  
  
  
  
  
  void LoadAuthExtension(bool force, bool silent_load, bool offline);

  
  
  void UserSettingsChanged();
  void UpdateAddButtonStatus();

  
  void RefocusCurrentPod();

  
  void HandleCompleteAuthentication(const std::string& email,
                                    const std::string& password,
                                    const std::string& auth_code);
  void HandleCompleteLogin(const std::string& typed_email,
                           const std::string& password);
  void HandleGetUsers();
  void HandleAuthenticateUser(const std::string& username,
                              const std::string& password);
  void HandleLaunchDemoUser();
  void HandleLaunchIncognito();
  void HandleLaunchPublicAccount(const std::string& username);
  void HandleOfflineLogin(const base::ListValue* args);
  void HandleShutdownSystem();
  void HandleLoadWallpaper(const std::string& email);
  void HandleRebootSystem();
  void HandleRemoveUser(const std::string& email);
  void HandleShowAddUser(const base::ListValue* args);
  void HandleToggleEnrollmentScreen();
  void HandleToggleKioskEnableScreen();
  void HandleToggleResetScreen();
  void HandleToggleKioskAutolaunchScreen();
  void HandleLaunchHelpApp(double help_topic_id);
  void HandleCreateAccount();
  void HandleAccountPickerReady();
  void HandleWallpaperReady();
  void HandleLoginWebuiReady();
  void HandleSignOutUser();
  void HandleNetworkErrorShown();
  void HandleOpenProxySettings();
  void HandleLoginVisible(const std::string& source);
  void HandleCancelPasswordChangedFlow();
  void HandleCancelUserAdding();
  void HandleMigrateUserData(const std::string& password);
  void HandleResyncUserData();
  void HandleLoginUIStateChanged(const std::string& source, bool new_value);
  void HandleUnlockOnLoginSuccess();
  void HandleLoginScreenUpdate();
  void HandleShowLoadingTimeoutError();
  void HandleUpdateOfflineLogin(bool offline_login_active);
  void HandleShowLocallyManagedUserCreationScreen();
  void HandleFocusPod(const std::string& user_id);
  void HandleLaunchKioskApp(const std::string& app_id);
  void HandleCustomButtonClicked(const std::string& username);

  
  static void FillUserDictionary(User* user,
                                 bool is_owner,
                                 DictionaryValue* user_dict);

  
  void SendUserList(bool animated);

  
  void StartClearingCookies(const base::Closure& on_clear_callback);
  void OnCookiesCleared(base::Closure on_clear_callback);

  
  void StartClearingDnsCache();
  void OnDnsCleared();

  
  
  void MaybePreloadAuthExtension();

  
  
  
  bool AllWhitelistedUsersPresent();

  
  
  void CancelPasswordChangedFlowInternal();

  
  OobeUI::Screen GetCurrentScreen() const;

  
  bool IsGaiaVisible() const;

  
  
  bool IsGaiaHiddenByError() const;

  
  
  bool IsSigninScreenHiddenByError() const;

  
  bool IsGuestSigninAllowed() const;

  
  bool IsOfflineLoginAllowed() const;

  
  void SubmitLoginFormForTest();

  
  void SetUserInputMethod(const std::string& username);

  
  
  
  void ContinueKioskEnableFlow(bool should_auto_enroll);

  
  void OnShowAddUser(const std::string& email);

  GaiaScreenHandler::FrameState FrameState() const;
  net::Error FrameError() const;

  
  UIState ui_state_;

  
  SigninScreenHandlerDelegate* delegate_;

  
  NativeWindowDelegate* native_window_delegate_;

  
  bool show_on_init_;

  
  bool oobe_ui_;

  
  bool focus_stolen_;

  
  bool gaia_silent_load_;

  
  std::string gaia_silent_load_network_;

  
  bool is_account_picker_showing_first_time_;

  
  bool dns_cleared_;

  
  bool dns_clear_task_running_;

  
  bool cookies_cleared_;

  
  scoped_refptr<HelpAppLauncher> help_app_;

  
  scoped_refptr<NetworkStateInformer> network_state_informer_;

  
  std::string email_;
  
  std::set<std::string> password_changed_for_;

  
  std::string test_user_;
  std::string test_pass_;
  bool test_expects_complete_login_;

  base::WeakPtrFactory<SigninScreenHandler> weak_factory_;

  
  bool webui_visible_;
  bool preferences_changed_delayed_;

  ErrorScreenActor* error_screen_actor_;
  CoreOobeActor* core_oobe_actor_;

  bool is_first_update_state_call_;
  bool offline_login_active_;
  NetworkStateInformer::State last_network_state_;

  base::CancelableClosure update_state_closure_;
  base::CancelableClosure connecting_closure_;

  content::NotificationRegistrar registrar_;

  
  
  
  bool has_pending_auth_ui_;

  scoped_ptr<CrosSettings::ObserverSubscription> allow_new_user_subscription_;
  scoped_ptr<CrosSettings::ObserverSubscription> allow_guest_subscription_;

  bool wait_for_auto_enrollment_check_;

  base::Closure kiosk_enable_flow_aborted_callback_for_test_;

  
  std::map<std::string, base::Closure> user_pod_button_callback_map_;

  
  
  GaiaScreenHandler* gaia_screen_handler_;

  DISALLOW_COPY_AND_ASSIGN(SigninScreenHandler);
};

}  

#endif  
