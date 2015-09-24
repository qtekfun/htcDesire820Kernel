// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREEN_LOCKER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREEN_LOCKER_H_

#include <string>

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/login/help_app_launcher.h"
#include "chrome/browser/chromeos/login/login_status_consumer.h"
#include "chrome/browser/chromeos/login/screen_locker_delegate.h"
#include "chrome/browser/chromeos/login/user.h"
#include "ui/base/accelerators/accelerator.h"

namespace content {
class WebUI;
}

namespace gfx {
class Image;
}

namespace chromeos {

class Authenticator;
class LoginFailure;
class ScreenlockIconProvider;

namespace test {
class ScreenLockerTester;
class ScreenLockerViewsTester;
class WebUIScreenLockerTester;
}  

class ScreenLocker : public LoginStatusConsumer {
 public:
  explicit ScreenLocker(const UserList& users);

  
  static ScreenLocker* default_screen_locker() {
    return screen_locker_;
  }

  bool locked() const { return locked_; }

  
  void Init();

  
  virtual void OnLoginFailure(const chromeos::LoginFailure& error) OVERRIDE;
  virtual void OnLoginSuccess(const UserContext& user_context) OVERRIDE;

  
  
  void UnlockOnLoginSuccess();

  
  void Authenticate(const UserContext& user_context);

  
  
  
  void AuthenticateByPassword(const std::string& password);

  
  void ClearErrors();

  
  void EnableInput();

  
  void Signout();

  
  void ShowBannerMessage(const std::string& message);

  
  void ShowUserPodButton(const std::string& username,
                         const gfx::Image& icon,
                         const base::Closure& click_callback);

  
  
  
  void ShowErrorMessage(int error_msg_id,
                        HelpAppLauncher::HelpTopic help_topic_id,
                        bool sign_out_only);

  
  ScreenLockerDelegate* delegate() const { return delegate_.get(); }

  
  const UserList& users() const { return users_; }

  
  
  void SetLoginStatusConsumer(chromeos::LoginStatusConsumer* consumer);

  
  
  content::WebUI* GetAssociatedWebUI();

  
  
  
  static void InitClass();

  
  static void Show();

  
  static void Hide();

  
  static test::ScreenLockerTester* GetTester();

 private:
  friend class base::DeleteHelper<ScreenLocker>;
  friend class test::ScreenLockerTester;
  friend class test::ScreenLockerViewsTester;
  friend class test::WebUIScreenLockerTester;
  friend class ScreenLockerDelegate;

  struct AuthenticationParametersCapture {
    UserContext user_context;
  };

  virtual ~ScreenLocker();

  
  void SetAuthenticator(Authenticator* authenticator);

  
  void ScreenLockReady();

  
  static void ScheduleDeletion();

  
  bool IsUserLoggedIn(const std::string& username);

  
  scoped_ptr<ScreenLockerDelegate> delegate_;

  
  UserList users_;

  
  scoped_refptr<Authenticator> authenticator_;

  
  
  
  bool locked_;

  
  
  static ScreenLocker* screen_locker_;

  
  base::Time start_time_;
  
  base::Time authentication_start_time_;

  
  
  LoginStatusConsumer* login_status_consumer_;

  
  int incorrect_passwords_count_;

  
  
  scoped_ptr<AuthenticationParametersCapture> authentication_capture_;

  
  scoped_ptr<ScreenlockIconProvider> screenlock_icon_provider_;

  base::WeakPtrFactory<ScreenLocker> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ScreenLocker);
};

}  

#endif  
