// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_MANAGED_MANAGED_USER_AUTHENTICATOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_MANAGED_MANAGED_USER_AUTHENTICATOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "third_party/cros_system_api/dbus/service_constants.h"

namespace chromeos {

class ManagedUserAuthenticator
    : public base::RefCountedThreadSafe<ManagedUserAuthenticator> {
 public:
  enum AuthState {
    CONTINUE,      
    NO_MOUNT,      
    FAILED_MOUNT,  
    FAILED_TPM,    
    SUCCESS,       
  };

  class AuthAttempt {
   public:
    AuthAttempt(const std::string& username,
                const std::string& password,
                bool add_key_attempt);
    ~AuthAttempt();

    
    
    
    void RecordCryptohomeStatus(bool cryptohome_outcome,
                                cryptohome::MountError cryptohome_code);

    
    
    
    void RecordHash(const std::string& hash);

    bool cryptohome_complete();
    bool cryptohome_outcome();
    bool hash_obtained();
    std::string hash();
    cryptohome::MountError cryptohome_code();

    const std::string username;
    const std::string password;
    const bool add_key;

   private:
    bool cryptohome_complete_;
    bool cryptohome_outcome_;
    bool hash_obtained_;
    std::string hash_;

    cryptohome::MountError cryptohome_code_;
    DISALLOW_COPY_AND_ASSIGN(AuthAttempt);
  };

  class AuthStatusConsumer {
   public:
    virtual ~AuthStatusConsumer() {}
    
    virtual void OnAuthenticationFailure(AuthState state) = 0;
    
    virtual void OnMountSuccess(const std::string& mount_hash) = 0;
    
    virtual void OnAddKeySuccess() = 0;
  };

  explicit ManagedUserAuthenticator(AuthStatusConsumer* consumer);

  void AuthenticateToMount(const std::string& username,
                           const std::string& password);

  void AuthenticateToCreate(const std::string& username,
                            const std::string& password);

  void AddMasterKey(const std::string& username,
                    const std::string& password,
                    const std::string& master_key);
  void Resolve();

 private:
  friend class base::RefCountedThreadSafe<ManagedUserAuthenticator>;

  ~ManagedUserAuthenticator();

  AuthState ResolveState();
  AuthState ResolveCryptohomeFailureState();
  AuthState ResolveCryptohomeSuccessState();
  void OnAuthenticationSuccess(const std::string& mount_hash, bool add_key);
  void OnAuthenticationFailure(AuthState state);

  scoped_ptr<AuthAttempt> current_state_;
  AuthStatusConsumer* consumer_;

  DISALLOW_COPY_AND_ASSIGN(ManagedUserAuthenticator);
};

}  

#endif  
