// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREEN_LOCKER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREEN_LOCKER_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "base/time.h"
#include "chrome/browser/chromeos/login/captcha_view.h"
#include "chrome/browser/chromeos/login/login_status_consumer.h"
#include "chrome/browser/chromeos/login/message_bubble.h"
#include "chrome/browser/chromeos/login/user_manager.h"
#include "views/accelerator.h"

namespace gfx {
class Rect;
}  

namespace views {
class View;
class WidgetGtk;
}  

namespace chromeos {

class Authenticator;
class BackgroundView;
class InputEventObserver;
class LockerInputEventObserver;
class MessageBubble;
class MouseEventRelay;
class ScreenLockView;
class LoginFailure;

namespace test {
class ScreenLockerTester;
}  

class ScreenLocker : public LoginStatusConsumer,
                     public MessageBubbleDelegate,
                     public CaptchaView::Delegate,
                     public views::AcceleratorTarget {
 public:
  
  class ScreenLockViewContainer {
   public:
    virtual void SetScreenLockView(views::View* screen_lock_view) = 0;

   protected:
    virtual ~ScreenLockViewContainer() {}
  };

  explicit ScreenLocker(const UserManager::User& user);

  
  static ScreenLocker* default_screen_locker() {
    return screen_locker_;
  }

  
  void Init();

  
  virtual void OnLoginFailure(const chromeos::LoginFailure& error);
  virtual void OnLoginSuccess(const std::string& username,
                              const std::string& password,
                              const GaiaAuthConsumer::ClientLoginResult& result,
                              bool pending_requests);

  
  virtual void BubbleClosing(Bubble* bubble, bool closed_by_escape);
  virtual bool CloseOnEscape() { return true; }
  virtual bool FadeInOnShow() { return false; }
  virtual void OnHelpLinkActivated() {}

  
  virtual void OnCaptchaEntered(const std::string& captcha);

  
  void Authenticate(const string16& password);

  
  void ClearErrors();

  
  void EnableInput();

  
  void Signout();

  
  
  void ShowCaptchaAndErrorMessage(const GURL& captcha_url,
                                  const std::wstring& message);

  
  
  
  void ShowErrorMessage(const std::wstring& message, bool sign_out_only);

  
  void OnGrabInputs();

  
  const UserManager::User& user() const {
    return user_;
  }

  
  views::View* GetViewByID(int id);

  
  
  
  static void InitClass();

  
  static void Show();

  
  static void Hide();

  
  static void UnlockScreenFailed();

  
  static test::ScreenLockerTester* GetTester();

 private:
  friend class DeleteTask<ScreenLocker>;
  friend class test::ScreenLockerTester;
  friend class LockerInputEventObserver;

  virtual ~ScreenLocker();

  
  void SetAuthenticator(Authenticator* authenticator);

  
  void ScreenLockReady();

  
  void OnWindowManagerReady();

  
  
  void ShowErrorBubble(const std::wstring& message,
                       BubbleBorder::ArrowLocation arrow_location);

  
  void StopScreenSaver();

  
  void StartScreenSaver();

  
  virtual bool AcceleratorPressed(const views::Accelerator& accelerator);

  
  CHROMEGTK_CALLBACK_1(ScreenLocker, void, OnClientEvent, GdkEventClient*);

  
  views::WidgetGtk* lock_window_;

  
  views::WidgetGtk* lock_widget_;

  
  ScreenLockView* screen_lock_view_;

  
  BackgroundView* background_view_;

  
  CaptchaView* captcha_view_;

  
  
  
  
  ScreenLockViewContainer* grab_container_;
  ScreenLockViewContainer* background_container_;

  
  
  scoped_ptr<views::View> secondary_view_;

  
  std::wstring postponed_error_message_;

  
  UserManager::User user_;

  
  scoped_refptr<Authenticator> authenticator_;

  
  
  
  
  scoped_ptr<MouseEventRelay> mouse_event_relay_;

  
  
  scoped_ptr<InputEventObserver> input_event_observer_;

  
  
  scoped_ptr<LockerInputEventObserver> locker_input_event_observer_;

  
  MessageBubble* error_info_;

  
  bool drawn_;

  
  bool input_grabbed_;

  
  
  bool unlock_on_input_;

  
  
  
  bool locked_;

  
  
  static ScreenLocker* screen_locker_;

  
  base::Time start_time_;
  
  base::Time authentication_start_time_;

  DISALLOW_COPY_AND_ASSIGN(ScreenLocker);
};

}  

#endif  
