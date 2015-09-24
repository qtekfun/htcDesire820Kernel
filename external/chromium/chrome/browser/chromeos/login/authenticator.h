// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_AUTHENTICATOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_AUTHENTICATOR_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/chromeos/login/login_status_consumer.h"
#include "chrome/common/net/gaia/gaia_auth_consumer.h"

class Profile;

namespace chromeos {

class Authenticator : public base::RefCountedThreadSafe<Authenticator> {
 public:
  
  static const char kSpecialCaseDomain[];

  explicit Authenticator(LoginStatusConsumer* consumer);
  virtual ~Authenticator();

  
  
  
  
  
  virtual bool AuthenticateToLogin(Profile* profile,
                                   const std::string& username,
                                   const std::string& password,
                                   const std::string& login_token,
                                   const std::string& login_captcha) = 0;

  
  
  
  
  virtual bool AuthenticateToUnlock(const std::string& username,
                                    const std::string& password) = 0;

  
  virtual void LoginOffTheRecord() = 0;

  
  
  
  
  virtual void OnLoginSuccess(
      const GaiaAuthConsumer::ClientLoginResult& credentials,
      bool request_pending) = 0;

  
  virtual void OnLoginFailure(const LoginFailure& error) = 0;

  
  
  
  
  
  
  
  
  virtual void RecoverEncryptedData(
      const std::string& old_password,
      const GaiaAuthConsumer::ClientLoginResult& credentials) = 0;

  
  
  
  virtual void ResyncEncryptedData(
      const GaiaAuthConsumer::ClientLoginResult& credentials) = 0;

  
  virtual void RetryAuth(Profile* profile,
                         const std::string& username,
                         const std::string& password,
                         const std::string& login_token,
                         const std::string& login_captcha) = 0;

  
  
  
  
  
  static std::string Canonicalize(const std::string& email_address);

 protected:
  LoginStatusConsumer* consumer_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Authenticator);
};

}  

#endif  
