// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_SIGNIN_TRACKER_H_
#define CHROME_BROWSER_SIGNIN_SIGNIN_TRACKER_H_

#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_types.h"
#include "google_apis/gaia/google_service_auth_error.h"
#include "google_apis/gaia/oauth2_token_service.h"

class Profile;

class SigninTracker : public content::NotificationObserver,
                      public OAuth2TokenService::Observer {
 public:
  class Observer {
   public:
    
    virtual void SigninFailed(const GoogleServiceAuthError& error) = 0;

    
    virtual void SigninSuccess() = 0;
  };

  
  
  
  SigninTracker(Profile* profile, Observer* observer);
  virtual ~SigninTracker();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnRefreshTokenAvailable(const std::string& account_id) OVERRIDE;
  virtual void OnRefreshTokenRevoked(const std::string& account_id) OVERRIDE;

 private:
  
  void Initialize();

  
  Profile* profile_;

  
  Observer* observer_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(SigninTracker);
};

#endif  
