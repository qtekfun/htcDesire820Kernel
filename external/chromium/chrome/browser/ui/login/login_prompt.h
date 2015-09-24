// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_LOGIN_LOGIN_PROMPT_H_
#define CHROME_BROWSER_UI_LOGIN_LOGIN_PROMPT_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/password_manager/password_manager.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

namespace net {
class AuthChallengeInfo;
class URLRequest;
}  

class ConstrainedWindow;
class GURL;
class RenderViewHostDelegate;

class LoginHandler : public base::RefCountedThreadSafe<LoginHandler>,
                     public LoginModelObserver,
                     public NotificationObserver {
 public:
  LoginHandler(net::AuthChallengeInfo* auth_info, net::URLRequest* request);
  virtual ~LoginHandler();

  
  
  static LoginHandler* Create(net::AuthChallengeInfo* auth_info,
                              net::URLRequest* request);

  
  virtual void BuildViewForPasswordManager(PasswordManager* manager,
                                           const string16& explanation) = 0;

  
  
  void SetPasswordForm(const webkit_glue::PasswordForm& form);
  void SetPasswordManager(PasswordManager* password_manager);

  
  TabContents* GetTabContentsForLogin() const;

  
  RenderViewHostDelegate* GetRenderViewHostDelegate() const;

  
  
  void SetAuth(const string16& username, const string16& password);

  
  
  void CancelAuth();

  
  
  void OnRequestCancelled();

  
  
  
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  const net::AuthChallengeInfo* auth_info() const { return auth_info_.get(); }

  
  bool WasAuthHandled() const;

 protected:
  void SetModel(LoginModel* model);

  void SetDialog(ConstrainedWindow* dialog);

  
  void NotifyAuthNeeded();

  
  void ReleaseSoon();

 private:
  
  void AddObservers();

  
  void RemoveObservers();

  
  void NotifyAuthSupplied(const string16& username,
                          const string16& password);

  
  void NotifyAuthCancelled();

  
  
  bool TestAndSetAuthHandled();

  
  void SetAuthDeferred(const string16& username,
                       const string16& password);

  
  void CancelAuthDeferred();

  
  void CloseContentsDeferred();

  
  bool handled_auth_;
  mutable base::Lock handled_auth_lock_;

  
  
  ConstrainedWindow* dialog_;

  
  scoped_refptr<net::AuthChallengeInfo> auth_info_;

  
  
  net::URLRequest* request_;

  
  
  
  
  webkit_glue::PasswordForm password_form_;

  
  
  
  PasswordManager* password_manager_;

  
  int render_process_host_id_;
  int tab_contents_id_;

  
  
  LoginModel* login_model_;

  
  NotificationRegistrar registrar_;
};

class LoginNotificationDetails {
 public:
  explicit LoginNotificationDetails(LoginHandler* handler)
      : handler_(handler) {}
  LoginHandler* handler() const { return handler_; }

 private:
  LoginNotificationDetails() {}

  LoginHandler* handler_;  

  DISALLOW_COPY_AND_ASSIGN(LoginNotificationDetails);
};

class AuthSuppliedLoginNotificationDetails : public LoginNotificationDetails {
 public:
  AuthSuppliedLoginNotificationDetails(LoginHandler* handler,
                                       const string16& username,
                                       const string16& password)
      : LoginNotificationDetails(handler),
        username_(username),
        password_(password) {}
  const string16& username() const { return username_; }
  const string16& password() const { return password_; }

 private:
  
  const string16 username_;

  
  const string16 password_;

  DISALLOW_COPY_AND_ASSIGN(AuthSuppliedLoginNotificationDetails);
};

LoginHandler* CreateLoginPrompt(net::AuthChallengeInfo* auth_info,
                                net::URLRequest* request);

void ResetLoginHandlerForRequest(net::URLRequest* request);

std::string GetSignonRealm(const GURL& url,
                           const net::AuthChallengeInfo& auth_info);

#endif  
