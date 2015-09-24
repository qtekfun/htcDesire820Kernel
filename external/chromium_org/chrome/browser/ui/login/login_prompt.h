// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_LOGIN_LOGIN_PROMPT_H_
#define CHROME_BROWSER_UI_LOGIN_LOGIN_PROMPT_H_

#include <string>

#include "base/basictypes.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/password_manager/password_manager.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/resource_dispatcher_host_login_delegate.h"

class GURL;

namespace content {
class RenderViewHostDelegate;
class NotificationRegistrar;
}  

namespace net {
class AuthChallengeInfo;
class HttpNetworkSession;
class URLRequest;
}  

class LoginHandler : public content::ResourceDispatcherHostLoginDelegate,
                     public LoginModelObserver,
                     public content::NotificationObserver {
 public:
  LoginHandler(net::AuthChallengeInfo* auth_info, net::URLRequest* request);

  
  
  static LoginHandler* Create(net::AuthChallengeInfo* auth_info,
                              net::URLRequest* request);

  
  virtual void OnRequestCancelled() OVERRIDE;

  
  virtual void BuildViewForPasswordManager(
      PasswordManager* manager,
      const base::string16& explanation) = 0;

  
  
  void SetPasswordForm(const autofill::PasswordForm& form);
  void SetPasswordManager(PasswordManager* password_manager);

  
  content::WebContents* GetWebContentsForLogin() const;

  
  
  void SetAuth(const base::string16& username, const base::string16& password);

  
  
  void CancelAuth();

  
  
  
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  const net::AuthChallengeInfo* auth_info() const { return auth_info_.get(); }

  
  bool WasAuthHandled() const;

 protected:
  virtual ~LoginHandler();

  void SetModel(LoginModel* model);

  
  void NotifyAuthNeeded();

  
  void ReleaseSoon();

  
  virtual void CloseDialog() = 0;

 private:
  
  void AddObservers();

  
  void RemoveObservers();

  
  void NotifyAuthSupplied(const base::string16& username,
                          const base::string16& password);

  
  void NotifyAuthCancelled();

  
  
  bool TestAndSetAuthHandled();

  
  void SetAuthDeferred(const base::string16& username,
                       const base::string16& password);

  
  void CancelAuthDeferred();

  
  void CloseContentsDeferred();

  
  bool handled_auth_;
  mutable base::Lock handled_auth_lock_;

  
  scoped_refptr<net::AuthChallengeInfo> auth_info_;

  
  
  net::URLRequest* request_;

  
  const net::HttpNetworkSession* http_network_session_;

  
  
  
  
  autofill::PasswordForm password_form_;

  
  
  PasswordManager* password_manager_;

  
  int render_process_host_id_;
  int tab_contents_id_;

  
  
  LoginModel* login_model_;

  
  
  scoped_ptr<content::NotificationRegistrar> registrar_;
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
                                       const base::string16& username,
                                       const base::string16& password)
      : LoginNotificationDetails(handler),
        username_(username),
        password_(password) {}
  const base::string16& username() const { return username_; }
  const base::string16& password() const { return password_; }

 private:
  
  const base::string16 username_;

  
  const base::string16 password_;

  DISALLOW_COPY_AND_ASSIGN(AuthSuppliedLoginNotificationDetails);
};

LoginHandler* CreateLoginPrompt(net::AuthChallengeInfo* auth_info,
                                net::URLRequest* request);

void ResetLoginHandlerForRequest(net::URLRequest* request);

std::string GetSignonRealm(const GURL& url,
                           const net::AuthChallengeInfo& auth_info);

#endif  
