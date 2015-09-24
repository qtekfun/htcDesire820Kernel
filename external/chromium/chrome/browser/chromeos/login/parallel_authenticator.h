// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_PARALLEL_AUTHENTICATOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_PARALLEL_AUTHENTICATOR_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/cros/cros_library.h"
#include "chrome/browser/chromeos/cros/cryptohome_library.h"
#include "chrome/browser/chromeos/login/authenticator.h"
#include "chrome/browser/chromeos/login/auth_attempt_state.h"
#include "chrome/browser/chromeos/login/auth_attempt_state_resolver.h"
#include "chrome/browser/chromeos/login/test_attempt_state.h"
#include "chrome/browser/chromeos/login/cryptohome_op.h"
#include "chrome/browser/chromeos/login/online_attempt.h"
#include "chrome/common/net/gaia/gaia_auth_consumer.h"

class GaiaAuthFetcher;
class LoginFailure;
class Profile;

namespace base {
class Lock;
}

namespace chromeos {

class LoginStatusConsumer;
class ParallelAuthenticator;
class ResolveChecker;

class ParallelAuthenticator : public Authenticator,
                              public AuthAttemptStateResolver {
 public:
  enum AuthState {
    CONTINUE,        
    NO_MOUNT,        
    FAILED_MOUNT,    
    FAILED_REMOVE,   
    FAILED_TMPFS,    
    CREATE_NEW,      
    RECOVER_MOUNT,   
    POSSIBLE_PW_CHANGE,  
    NEED_NEW_PW,     
    NEED_OLD_PW,     
    HAVE_NEW_PW,     
    OFFLINE_LOGIN,   
    ONLINE_LOGIN,    
    UNLOCK,          
    LOCAL_LOGIN,     
    ONLINE_FAILED,   
    LOGIN_FAILED     
  };

  explicit ParallelAuthenticator(LoginStatusConsumer* consumer);
  virtual ~ParallelAuthenticator();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool AuthenticateToLogin(Profile* profile,
                           const std::string& username,
                           const std::string& password,
                           const std::string& login_token,
                           const std::string& login_captcha);

  
  
  
  
  bool AuthenticateToUnlock(const std::string& username,
                            const std::string& password);

  
  
  void LoginOffTheRecord();

  
  
  void OnLoginSuccess(const GaiaAuthConsumer::ClientLoginResult& credentials,
                      bool request_pending);
  void OnOffTheRecordLoginSuccess();
  void OnPasswordChangeDetected(
      const GaiaAuthConsumer::ClientLoginResult& credentials);
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

  
  void CheckLocalaccount(const LoginFailure& error);

  
  
  
  
  
  
  void Resolve();

 private:
  
  
  
  AuthState ResolveState();

  
  
  
  
  AuthState ResolveReauthState();

  
  
  
  
  AuthState ResolveCryptohomeFailureState();

  
  
  
  
  AuthState ResolveCryptohomeSuccessState();

  
  
  
  
  
  AuthState ResolveOnlineFailureState(AuthState offline_state);

  
  
  
  
  AuthState ResolveOnlineSuccessState(AuthState offline_state);

  
  void set_attempt_state(TestAttemptState* new_state) {  
    current_state_.reset(new_state);
  }

  
  
  
  void ResyncRecoverHelper(CryptohomeOp* to_initiate);

  
  void LoadSystemSalt();

  
  
  
  
  
  
  void LoadLocalaccount(const std::string& filename);

  void SetLocalaccount(const std::string& new_name);

  
  std::string HashPassword(const std::string& password);

  
  std::string SaltAsAscii();

  
  
  
  static bool BinaryToHex(const std::vector<unsigned char>& binary,
                          const unsigned int binary_len,
                          char* hex_string,
                          const unsigned int len);

  
  static const char kLocalaccountFile[];

  
  static const int kClientLoginTimeoutMs;

  
  static const int kLocalaccountRetryIntervalMs;

  
  scoped_ptr<GaiaAuthFetcher> gaia_authenticator_;

  
  
  scoped_ptr<AuthAttemptState> reauth_state_;

  scoped_ptr<AuthAttemptState> current_state_;
  scoped_refptr<OnlineAttempt> current_online_;
  scoped_refptr<CryptohomeOp> mounter_;
  scoped_refptr<CryptohomeOp> key_migrator_;
  scoped_refptr<CryptohomeOp> data_remover_;
  scoped_refptr<CryptohomeOp> guest_mounter_;
  scoped_refptr<CryptohomeOp> key_checker_;

  std::string ascii_hash_;
  chromeos::CryptohomeBlob system_salt_;

  
  
  
  
  bool already_reported_success_;
  base::Lock success_lock_;  

  
  std::string localaccount_;
  bool checked_for_localaccount_;  
  base::Lock localaccount_lock_;  

  friend class ResolveChecker;
  friend class ParallelAuthenticatorTest;
  FRIEND_TEST_ALL_PREFIXES(ParallelAuthenticatorTest, SaltToAscii);
  FRIEND_TEST_ALL_PREFIXES(ParallelAuthenticatorTest, ReadLocalaccount);
  FRIEND_TEST_ALL_PREFIXES(ParallelAuthenticatorTest,
                           ReadLocalaccountTrailingWS);
  FRIEND_TEST_ALL_PREFIXES(ParallelAuthenticatorTest, ReadNoLocalaccount);
  DISALLOW_COPY_AND_ASSIGN(ParallelAuthenticator);
};

}  

#endif  
