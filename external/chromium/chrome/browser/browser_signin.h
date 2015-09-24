// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSER_SIGNIN_H_
#define CHROME_BROWSER_BROWSER_SIGNIN_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/common/net/gaia/google_service_auth_error.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class BrowserSigninHtml;
class Profile;
class ProfileSyncService;
class TabContents;

class BrowserSignin : public NotificationObserver {
 public:
  explicit BrowserSignin(Profile* profile);
  virtual ~BrowserSignin();

  
  class SigninDelegate {
   public:
    virtual ~SigninDelegate() {}

    
    virtual void OnLoginSuccess() = 0;

    
    virtual void OnLoginFailure(const GoogleServiceAuthError& error) = 0;
  };

  
  
  
  
  
  
  
  
  
  
  
  
  virtual void RequestSignin(TabContents* tab_contents,
                             const string16& suggested_email,
                             const string16& login_message,
                             SigninDelegate* delegate);

  
  
  
  virtual std::string GetSignedInUsername() const;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  ProfileSyncService* GetProfileSyncService() const;

  
  void Cancel();

 private:
  
  BrowserSigninHtml* CreateHtmlDialogUI();

  
  void OnLoginFinished();

  
  void RegisterAuthNotifications();

  
  void UnregisterAuthNotifications();

  
  void ShowSigninTabModal(TabContents* tab_contents);

  
  Profile* profile_;

  
  string16 suggested_email_;

  
  string16 login_message_;

  
  SigninDelegate* delegate_;

  
  
  BrowserSigninHtml* html_dialog_ui_delegate_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(BrowserSignin);
};


#endif  
