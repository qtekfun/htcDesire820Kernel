// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_PARALLEL_AUTHENTICATOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_PARALLEL_AUTHENTICATOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/chromeos/login/auth_attempt_state.h"
#include "chrome/browser/chromeos/login/auth_attempt_state_resolver.h"
#include "chrome/browser/chromeos/login/authenticator.h"
#include "chrome/browser/chromeos/login/test_attempt_state.h"
#include "chrome/browser/chromeos/settings/device_settings_service.h"
#include "google_apis/gaia/gaia_auth_consumer.h"

class LoginFailure;
class Profile;

namespace chromeos {

class LoginStatusConsumer;

class ParallelAuthenticator : public Authenticator,
                              public AuthAttemptStateResolver {
 public:
  enum AuthState {
    CONTINUE = 0,            
    NO_MOUNT = 1,            
    FAILED_MOUNT = 2,        
    FAILED_REMOVE = 3,       
    FAILED_TMPFS = 4,        
    FAILED_TPM = 5,          
    CREATE_NEW = 6,          
    RECOVER_MOUNT = 7,       
    POSSIBLE_PW_CHANGE = 8,  
    NEED_NEW_PW = 9,         
                             
    NEED_OLD_PW = 10,        
                             
    HAVE_NEW_PW = 11,        
                             
    OFFLINE_LOGIN = 12,      
    DEMO_LOGIN = 13,         
    ONLINE_LOGIN = 14,       
    UNLOCK = 15,             
    ONLINE_FAILED = 16,      
                             
    GUEST_LOGIN = 17,        
    PUBLIC_ACCOUNT_LOGIN = 18,        
    LOCALLY_MANAGED_USER_LOGIN = 19,  
    LOGIN_FAILED = 20,       
    OWNER_REQUIRED = 21,     
    FAILED_USERNAME_HASH = 22,        
    KIOSK_ACCOUNT_LOGIN = 23,         
    REMOVED_DATA_AFTER_FAILURE = 24,  
                                      
  };

  explicit ParallelAuthenticator(LoginStatusConsumer* consumer);

  
  virtual void CompleteLogin(Profile* profile,
                             const UserContext& user_context) OVERRIDE;

  
  
  
  
  
  
  
  
  virtual void AuthenticateToLogin(Profile* profile,
                                   const UserContext& user_context) OVERRIDE;

  
  
  
  
  virtual void AuthenticateToUnlock(const UserContext& user_context) OVERRIDE;

  
  
  
  virtual void LoginAsLocallyManagedUser(
      const UserContext& user_context) OVERRIDE;

  
  
  virtual void LoginRetailMode() OVERRIDE;

  
  
  virtual void LoginOffTheRecord() OVERRIDE;

  
  
  
  virtual void LoginAsPublicAccount(const std::string& username) OVERRIDE;

  
  
  
  virtual void LoginAsKioskAccount(const std::string& app_user_id) OVERRIDE;

  
  
  virtual void OnRetailModeLoginSuccess() OVERRIDE;
  virtual void OnLoginSuccess() OVERRIDE;
  virtual void OnLoginFailure(const LoginFailure& error) OVERRIDE;
  virtual void RecoverEncryptedData(
      const std::string& old_password) OVERRIDE;
  virtual void ResyncEncryptedData() OVERRIDE;

  
  
  
  
  
  
  
  virtual void Resolve() OVERRIDE;

  
  static std::string HashPassword(const std::string& password,
                                  const std::string& ascii_salt);

  void OnOffTheRecordLoginSuccess();
  void OnPasswordChangeDetected();

 protected:
  virtual ~ParallelAuthenticator();

 private:
  friend class ParallelAuthenticatorTest;
  FRIEND_TEST_ALL_PREFIXES(ParallelAuthenticatorTest,
                           ResolveOwnerNeededDirectFailedMount);
  FRIEND_TEST_ALL_PREFIXES(ParallelAuthenticatorTest, ResolveOwnerNeededMount);
  FRIEND_TEST_ALL_PREFIXES(ParallelAuthenticatorTest,
                           ResolveOwnerNeededFailedMount);

  
  void RemoveEncryptedData();

  
  
  
  AuthState ResolveState();

  
  
  
  
  AuthState ResolveCryptohomeFailureState();

  
  
  
  
  AuthState ResolveCryptohomeSuccessState();

  
  
  
  
  AuthState ResolveOnlineSuccessState(AuthState offline_state);

  
  void set_attempt_state(TestAttemptState* new_state) {  
    current_state_.reset(new_state);
  }

  
  void SetOwnerState(bool owner_check_finished, bool check_result);

  
  
  
  bool VerifyOwner();

  
  void OnOwnershipChecked(bool is_owner);

  
  
  void ResolveLoginCompletionStatus();

  scoped_ptr<AuthAttemptState> current_state_;
  bool migrate_attempted_;
  bool remove_attempted_;
  bool resync_attempted_;
  bool ephemeral_mount_attempted_;
  bool check_key_attempted_;

  
  
  
  
  bool already_reported_success_;
  base::Lock success_lock_;  

  
  
  bool owner_is_verified_;
  bool user_can_login_;

  
  bool remove_user_data_on_failure_;

  
  
  const LoginFailure* delayed_login_failure_;

  DISALLOW_COPY_AND_ASSIGN(ParallelAuthenticator);
};

}  

#endif  
