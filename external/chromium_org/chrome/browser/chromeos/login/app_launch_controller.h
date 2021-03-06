// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_APP_LAUNCH_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_APP_LAUNCH_CONTROLLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/timer/timer.h"
#include "chrome/browser/chromeos/app_mode/kiosk_app_launch_error.h"
#include "chrome/browser/chromeos/app_mode/kiosk_profile_loader.h"
#include "chrome/browser/chromeos/app_mode/startup_app_launcher.h"
#include "chrome/browser/chromeos/login/app_launch_signin_screen.h"
#include "chrome/browser/chromeos/login/screens/app_launch_splash_screen_actor.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;

namespace chromeos {

class LoginDisplayHost;
class OobeDisplay;
class UserManager;

class AppLaunchController
    : public base::SupportsWeakPtr<AppLaunchController>,
      public AppLaunchSplashScreenActor::Delegate,
      public KioskProfileLoader::Delegate,
      public StartupAppLauncher::Delegate,
      public AppLaunchSigninScreen::Delegate,
      public content::NotificationObserver {
 public:
  typedef base::Callback<bool()> ReturnBoolCallback;

  AppLaunchController(const std::string& app_id,
                      LoginDisplayHost* host,
                      OobeDisplay* oobe_display);

  virtual ~AppLaunchController();

  void StartAppLaunch();

  bool waiting_for_network() { return waiting_for_network_; }
  bool network_wait_timedout() { return network_wait_timedout_; }
  bool showing_network_dialog() { return showing_network_dialog_; }

  
  static void SkipSplashWaitForTesting();
  static void SetNetworkTimeoutCallbackForTesting(base::Closure* callback);
  static void SetNetworkWaitForTesting(int wait_time_secs);
  static void SetCanConfigureNetworkCallbackForTesting(
      ReturnBoolCallback* can_configure_network_callback);
  static void SetNeedOwnerAuthToConfigureNetworkCallbackForTesting(
      ReturnBoolCallback* need_owner_auth_callback);

 private:
  
  class AppWindowWatcher;

  void CleanUp();
  void OnNetworkWaitTimedout();

  
  void OnAppWindowCreated();

  
  bool CanConfigureNetwork();

  
  bool NeedOwnerAuthToConfigureNetwork();

  
  
  void MaybeShowNetworkConfigureUI();

  
  virtual void OnProfileLoaded(Profile* profile) OVERRIDE;
  virtual void OnProfileLoadFailed(KioskAppLaunchError::Error error) OVERRIDE;

  
  virtual void OnConfigureNetwork() OVERRIDE;
  virtual void OnCancelAppLaunch() OVERRIDE;
  virtual void OnNetworkStateChanged(bool online) OVERRIDE;

  
  virtual void InitializeNetwork() OVERRIDE;
  virtual void OnLoadingOAuthFile() OVERRIDE;
  virtual void OnInitializingTokenService() OVERRIDE;
  virtual void OnInstallingApp() OVERRIDE;
  virtual void OnReadyToLaunch() OVERRIDE;
  virtual void OnLaunchSucceeded() OVERRIDE;
  virtual void OnLaunchFailed(KioskAppLaunchError::Error error) OVERRIDE;

  
  virtual void OnOwnerSigninSuccess() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  Profile* profile_;
  const std::string app_id_;
  LoginDisplayHost* host_;
  OobeDisplay* oobe_display_;
  AppLaunchSplashScreenActor* app_launch_splash_screen_actor_;
  scoped_ptr<KioskProfileLoader> kiosk_profile_loader_;
  scoped_ptr<StartupAppLauncher> startup_app_launcher_;
  scoped_ptr<AppLaunchSigninScreen> signin_screen_;
  scoped_ptr<AppWindowWatcher> app_window_watcher_;

  content::NotificationRegistrar registrar_;
  bool webui_visible_;
  bool launcher_ready_;

  base::OneShotTimer<AppLaunchController> network_wait_timer_;
  bool waiting_for_network_;
  bool network_wait_timedout_;
  bool showing_network_dialog_;
  int64 launch_splash_start_time_;

  static bool skip_splash_wait_;
  static int network_wait_time_;
  static base::Closure* network_timeout_callback_;
  static ReturnBoolCallback* can_configure_network_callback_;
  static ReturnBoolCallback* need_owner_auth_to_configure_network_callback_;

  DISALLOW_COPY_AND_ASSIGN(AppLaunchController);
};

}  

#endif  
