// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_GOOGLE_AUTHENTICATOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_GOOGLE_AUTHENTICATOR_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/cros/cros_library.h"
#include "chrome/browser/chromeos/cros/cryptohome_library.h"
#include "chrome/browser/chromeos/login/authenticator.h"
#include "chrome/common/net/gaia/gaia_auth_consumer.h"
#include "chrome/common/net/gaia/gaia_auth_fetcher.h"


class Profile;
class GoogleServiceAuthError;
class LoginFailure;

namespace base {
class Lock;
}

namespace chromeos {

class GoogleAuthenticatorTest;
class LoginStatusConsumer;
class UserManager;

class GoogleAuthenticator : public Authenticator, public GaiaAuthConsumer {
 public:
  explicit GoogleAuthenticator(LoginStatusConsumer* consumer);
  virtual ~GoogleAuthenticator();

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool AuthenticateToLogin(Profile* profile,
                           const std::string& username,
                           const std::string& password,
                           const std::string& login_token,
                           const std::string& login_captcha);

  
  
  
  
  bool AuthenticateToUnlock(const std::string& username,
                            const std::string& password);

  
  
  void LoginOffTheRecord();

  
  void set_system_salt(const chromeos::CryptohomeBlob& new_salt) {
    system_salt_ = new_salt;
  }
  void set_username(const std::string& fake_user) { username_ = fake_user; }
  void set_password(const std::string& fake_pass) { password_ = fake_pass; }
  void set_password_hash(const std::string& fake_hash) {
    ascii_hash_ = fake_hash;
  }
  void set_user_manager(UserManager* new_manager) {
    user_manager_ = new_manager;
  }
  void SetLocalaccount(const std::string& new_name);

  
  
  void OnLoginSuccess(const GaiaAuthConsumer::ClientLoginResult& credentials,
                      bool request_pending);
  void CheckOffline(const LoginFailure& error);
  void CheckLocalaccount(const LoginFailure& error);
  void OnLoginFailure(const LoginFailure& error);

  
  void RecoverEncryptedData(
      const std::string& old_password,
      const GaiaAuthConsumer::ClientLoginResult& credentials);
  void ResyncEncryptedData(
      const GaiaAuthConsumer::ClientLoginResult& credentials);
  void RetryAuth(Profile* profile,
                 const std::string& username,
                 const std::string& password,
                 const std::string& login_token,
                 const std::string& login_captcha);

  
  virtual void OnClientLoginFailure(
      const GoogleServiceAuthError& error);
  virtual void OnClientLoginSuccess(
      const GaiaAuthConsumer::ClientLoginResult& credentials);

 private:

  
  void LoadSystemSalt();

  
  
  
  
  
  
  void LoadLocalaccount(const std::string& filename);

  
  std::string HashPassword(const std::string& password);

  
  std::string SaltAsAscii();

  
  
  void PrepareClientLoginAttempt(const std::string& password,
                                 const std::string& login_token,
                                 const std::string& login_captcha);
  
  void ClearClientLoginAttempt();

  
  
  
  
  
   void TryClientLogin();

  
  
  void CancelClientLogin();


  
  
  
  static bool BinaryToHex(const std::vector<unsigned char>& binary,
                          const unsigned int binary_len,
                          char* hex_string,
                          const unsigned int len);

  void set_hosted_policy(GaiaAuthFetcher::HostedAccountsSetting policy) {
    hosted_policy_ = policy;
  }

  
  static const char kFormatCaptcha[];

  
  
  static const char kSecondFactor[];

  
  static const char kLocalaccountFile[];

  
  scoped_ptr<GaiaAuthFetcher> gaia_authenticator_;

  
  UserManager* user_manager_;

  
  static const int kClientLoginTimeoutMs;

  
  static const int kLocalaccountRetryIntervalMs;

  
  GaiaAuthFetcher::HostedAccountsSetting hosted_policy_;

  std::string username_;
  
  std::string password_;
  std::string login_token_;
  std::string login_captcha_;

  std::string ascii_hash_;
  chromeos::CryptohomeBlob system_salt_;
  bool unlock_;  
  bool try_again_;  

  std::string localaccount_;
  bool checked_for_localaccount_;  
  base::Lock localaccount_lock_;  

  friend class GoogleAuthenticatorTest;
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest, SaltToAscii);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest, CheckTwoFactorResponse);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest, CheckNormalErrorCode);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest, EmailAddressNoOp);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest, EmailAddressIgnoreCaps);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest,
                           EmailAddressIgnoreDomainCaps);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest,
                           EmailAddressIgnoreOneUsernameDot);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest,
                           EmailAddressIgnoreManyUsernameDots);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest,
                           EmailAddressIgnoreConsecutiveUsernameDots);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest,
                           EmailAddressDifferentOnesRejected);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest,
                           EmailAddressIgnorePlusSuffix);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest,
                           EmailAddressIgnoreMultiPlusSuffix);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest, ReadSaltOnlyOnce);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest, LocalaccountLogin);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest, ReadLocalaccount);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest, ReadLocalaccountTrailingWS);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest, ReadNoLocalaccount);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest, LoginNetFailure);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest, LoginDenied);
  FRIEND_TEST_ALL_PREFIXES(GoogleAuthenticatorTest, TwoFactorLogin);

  DISALLOW_COPY_AND_ASSIGN(GoogleAuthenticator);
};

}  

#endif  
