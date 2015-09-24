// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_WEBUI_SCREEN_LOCKER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_WEBUI_SCREEN_LOCKER_H_

#include <string>

#include "ash/wm/lock_state_observer.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/login/lock_window.h"
#include "chrome/browser/chromeos/login/login_display.h"
#include "chrome/browser/chromeos/login/screen_locker_delegate.h"
#include "chrome/browser/chromeos/login/webui_login_view.h"
#include "chromeos/dbus/power_manager_client.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_observer.h"

namespace content {
class WebUI;
}

namespace chromeos {

class ScreenLocker;
class WebUILoginDisplay;
struct UserContext;

namespace login {
class NetworkStateHelper;
}

namespace test {
class WebUIScreenLockerTester;
}

class WebUIScreenLocker : public WebUILoginView,
                          public LoginDisplay::Delegate,
                          public ScreenLockerDelegate,
                          public LockWindow::Observer,
                          public ash::LockStateObserver,
                          public views::WidgetObserver,
                          public PowerManagerClient::Observer {
 public:
  explicit WebUIScreenLocker(ScreenLocker* screen_locker);

  
  virtual void LockScreen() OVERRIDE;
  virtual void ScreenLockReady() OVERRIDE;
  virtual void OnAuthenticate() OVERRIDE;
  virtual void SetInputEnabled(bool enabled) OVERRIDE;
  virtual void ShowBannerMessage(const std::string& message) OVERRIDE;
  virtual void ShowUserPodButton(const std::string& username,
                                 const std::string& iconURL,
                                 const base::Closure& click_callback) OVERRIDE;
  virtual void ShowErrorMessage(
      int error_msg_id,
      HelpAppLauncher::HelpTopic help_topic_id) OVERRIDE;
  virtual void ClearErrors() OVERRIDE;
  virtual void AnimateAuthenticationSuccess() OVERRIDE;
  virtual gfx::NativeWindow GetNativeWindow() const OVERRIDE;
  virtual content::WebUI* GetAssociatedWebUI() OVERRIDE;
  virtual void OnLockWebUIReady() OVERRIDE;
  virtual void OnLockBackgroundDisplayed() OVERRIDE;

  
  virtual void CancelPasswordChangedFlow() OVERRIDE;
  virtual void CreateAccount() OVERRIDE;
  virtual void CompleteLogin(const UserContext& user_context) OVERRIDE;
  virtual base::string16 GetConnectedNetworkName() OVERRIDE;
  virtual bool IsSigninInProgress() const OVERRIDE;
  virtual void Login(const UserContext& user_context) OVERRIDE;
  virtual void LoginAsRetailModeUser() OVERRIDE;
  virtual void LoginAsGuest() OVERRIDE;
  virtual void MigrateUserData(const std::string& old_password) OVERRIDE;
  virtual void LoginAsPublicAccount(const std::string& username) OVERRIDE;
  virtual void OnSigninScreenReady() OVERRIDE;
  virtual void OnUserSelected(const std::string& username) OVERRIDE;
  virtual void OnStartEnterpriseEnrollment() OVERRIDE;
  virtual void OnStartKioskEnableScreen() OVERRIDE;
  virtual void OnStartDeviceReset() OVERRIDE;
  virtual void OnStartKioskAutolaunchScreen() OVERRIDE;
  virtual void ShowWrongHWIDScreen() OVERRIDE;
  virtual void ResetPublicSessionAutoLoginTimer() OVERRIDE;
  virtual void ResyncUserData() OVERRIDE;
  virtual void SetDisplayEmail(const std::string& email) OVERRIDE;
  virtual void Signout() OVERRIDE;
  virtual void LoginAsKioskApp(const std::string& app_id) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnLockWindowReady() OVERRIDE;

  
  virtual void OnLockStateEvent(
      ash::LockStateObserver::EventType event) OVERRIDE;

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;

  
  virtual void SystemResumed(const base::TimeDelta& sleep_duration) OVERRIDE;
  virtual void LidEventReceived(bool open,
                                const base::TimeTicks& time) OVERRIDE;

  
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;

 private:
  friend class test::WebUIScreenLockerTester;

  virtual ~WebUIScreenLocker();

  
  void FocusUserPod();

  
  views::Widget* lock_window_;

  
  scoped_ptr<WebUILoginDisplay> login_display_;

  
  content::NotificationRegistrar registrar_;

  
  bool lock_ready_;

  
  bool webui_ready_;

  
  base::TimeTicks lock_time_;

  scoped_ptr<login::NetworkStateHelper> network_state_helper_;

  base::WeakPtrFactory<WebUIScreenLocker> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(WebUIScreenLocker);
};

}  

#endif  
