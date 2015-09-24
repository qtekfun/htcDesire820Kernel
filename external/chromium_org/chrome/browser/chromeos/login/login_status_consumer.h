// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_STATUS_CONSUMER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_STATUS_CONSUMER_H_

#include <string>

#include "base/logging.h"
#include "google_apis/gaia/gaia_auth_consumer.h"
#include "google_apis/gaia/google_service_auth_error.h"
#include "net/base/net_errors.h"

namespace chromeos {

struct UserContext;

class LoginFailure {
 public:
  enum FailureReason {
    NONE,
    COULD_NOT_MOUNT_CRYPTOHOME,
    COULD_NOT_MOUNT_TMPFS,
    COULD_NOT_UNMOUNT_CRYPTOHOME,
    DATA_REMOVAL_FAILED,    
    LOGIN_TIMED_OUT,
    UNLOCK_FAILED,
    NETWORK_AUTH_FAILED,    
    OWNER_REQUIRED,         
    WHITELIST_CHECK_FAILED, 
                            
                            
                            
                            
    TPM_ERROR,              
    USERNAME_HASH_FAILED,   
    NUM_FAILURE_REASONS,    
  };

  explicit LoginFailure(FailureReason reason)
      : reason_(reason),
        error_(GoogleServiceAuthError::NONE) {
    DCHECK(reason != NETWORK_AUTH_FAILED);
  }

  inline bool operator==(const LoginFailure &b) const {
    if (reason_ != b.reason_) {
      return false;
    }
    if (reason_ == NETWORK_AUTH_FAILED) {
      return error_ == b.error_;
    }
    return true;
  }

  static LoginFailure FromNetworkAuthFailure(
      const GoogleServiceAuthError& error) {
    return LoginFailure(NETWORK_AUTH_FAILED, error);
  }

  static LoginFailure LoginFailureNone() {
    return LoginFailure(NONE);
  }

  const std::string GetErrorString() const {
    switch (reason_) {
      case DATA_REMOVAL_FAILED:
        return "Could not destroy your old data.";
      case COULD_NOT_MOUNT_CRYPTOHOME:
        return "Could not mount cryptohome.";
      case COULD_NOT_UNMOUNT_CRYPTOHOME:
        return "Could not unmount cryptohome.";
      case COULD_NOT_MOUNT_TMPFS:
        return "Could not mount tmpfs.";
      case LOGIN_TIMED_OUT:
        return "Login timed out. Please try again.";
      case UNLOCK_FAILED:
        return "Unlock failed.";
      case NETWORK_AUTH_FAILED:
        if (error_.state() == GoogleServiceAuthError::CONNECTION_FAILED) {
          return net::ErrorToString(error_.network_error());
        }
        return "Google authentication failed.";
      case OWNER_REQUIRED:
        return "Login is restricted to the owner's account only.";
      case WHITELIST_CHECK_FAILED:
        return "Login attempt blocked by whitelist.";
      default:
        NOTREACHED();
        return std::string();
    }
  }

  const GoogleServiceAuthError& error() const { return error_; }
  const FailureReason& reason() const { return reason_; }

 private:
  LoginFailure(FailureReason reason, GoogleServiceAuthError error)
      : reason_(reason),
        error_(error) {
  }

  FailureReason reason_;
  GoogleServiceAuthError error_;
};

class LoginStatusConsumer {
 public:
  virtual ~LoginStatusConsumer() {}
  
  virtual void OnLoginFailure(const LoginFailure& error) = 0;

  
  
  
  virtual void OnRetailModeLoginSuccess(const UserContext& user_context);
  
  virtual void OnLoginSuccess(const UserContext& user_context) = 0;
  
  virtual void OnOffTheRecordLoginSuccess() {}
  
  virtual void OnPasswordChangeDetected();
};

}  

#endif  
