// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREEN_LOCK_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREEN_LOCK_VIEW_H_
#pragma once

#include "chrome/browser/chromeos/login/helper.h"
#include "chrome/browser/chromeos/login/user_view.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "views/controls/textfield/textfield_controller.h"
#include "views/view.h"

namespace views {
class ImageView;
}  

namespace chromeos {

class ScreenLocker;
class UserView;

namespace test {
class ScreenLockerTester;
}  

class ScreenLockView : public ThrobberHostView,
                       public views::TextfieldController,
                       public NotificationObserver,
                       public UserView::Delegate {
 public:
  explicit ScreenLockView(ScreenLocker* screen_locker);
  virtual ~ScreenLockView();

  void Init();

  
  void ClearAndSetFocusToPassword();

  
  void SetSignoutEnabled(bool enabled);

  
  gfx::Rect GetPasswordBoundsRelativeTo(const views::View* view);

  
  virtual void SetEnabled(bool enabled);
  virtual void Layout();
  virtual gfx::Size GetPreferredSize();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void ContentsChanged(views::Textfield* sender,
                               const string16& new_contents);
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const views::KeyEvent& keystroke);

  
  virtual void OnSignout();
  virtual bool IsUserSelected() const { return true; }

 private:
  friend class test::ScreenLockerTester;

  UserView* user_view_;

  
  views::Textfield* password_field_;

  
  ScreenLocker* screen_locker_;

  NotificationRegistrar registrar_;

  
  views::View* main_;

  
  views::View* username_;

  DISALLOW_COPY_AND_ASSIGN(ScreenLockView);
};

}  

#endif  
