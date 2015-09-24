// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_AUTHENTICATOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_AUTHENTICATOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/chromeos/login/login_status_consumer.h"
#include "google_apis/gaia/gaia_auth_consumer.h"

class Profile;

namespace chromeos {

struct UserContext;

class Authenticator : public base::RefCountedThreadSafe<Authenticator> {
 public:
  explicit Authenticator(LoginStatusConsumer* consumer);

  
  
  virtual void CompleteLogin(Profile* profile,
                             const UserContext& user_context) = 0;

  
  
  
  virtual void AuthenticateToLogin(Profile* profile,
                                   const UserContext& user_context) = 0;

  
  
  
  virtual void AuthenticateToUnlock(const UserContext& user_context) = 0;

  
  virtual void LoginAsLocallyManagedUser(
      const UserContext& user_context) = 0;

  
  virtual void LoginRetailMode() = 0;

  
  virtual void LoginOffTheRecord() = 0;

  
  virtual void LoginAsPublicAccount(const std::string& username) = 0;

  
  
  virtual void LoginAsKioskAccount(const std::string& app_user_id) = 0;

  
  virtual void OnRetailModeLoginSuccess() = 0;

  
  virtual void OnLoginSuccess() = 0;

  
  virtual void OnLoginFailure(const LoginFailure& error) = 0;

  
  
  
  
  
  
  
  virtual void RecoverEncryptedData(
      const std::string& old_password) = 0;

  
  
  virtual void ResyncEncryptedData() = 0;

  
  
  Profile* authentication_profile() { return authentication_profile_; }

  
  void SetConsumer(LoginStatusConsumer* consumer);

 protected:
  virtual ~Authenticator();

  LoginStatusConsumer* consumer_;
  Profile* authentication_profile_;

 private:
  friend class base::RefCountedThreadSafe<Authenticator>;

  DISALLOW_COPY_AND_ASSIGN(Authenticator);
};

}  

#endif  
