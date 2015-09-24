// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_MANAGED_LOCALLY_MANAGED_USER_CREATION_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_MANAGED_LOCALLY_MANAGED_USER_CREATION_CONTROLLER_H_

#include <string>

#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "base/timer/timer.h"
#include "chrome/browser/chromeos/login/managed/managed_user_authenticator.h"
#include "chrome/browser/managed_mode/managed_user_registration_utility.h"

class Profile;

namespace chromeos {


class LocallyManagedUserCreationController
    : public ManagedUserAuthenticator::AuthStatusConsumer {
 public:
  
  
  
  static const int kDummyAvatarIndex;

  enum ErrorCode {
    NO_ERROR,
    CRYPTOHOME_NO_MOUNT,
    CRYPTOHOME_FAILED_MOUNT,
    CRYPTOHOME_FAILED_TPM,
    CLOUD_SERVER_ERROR,
    TOKEN_WRITE_FAILED,
  };

  class StatusConsumer {
   public:
    virtual ~StatusConsumer();

    virtual void OnCreationError(ErrorCode code) = 0;
    virtual void OnLongCreationWarning() = 0;
    virtual void OnCreationTimeout() = 0;
    virtual void OnCreationSuccess() = 0;
  };

  
  
  
  LocallyManagedUserCreationController(StatusConsumer* consumer,
                                       const std::string& manager_id);
  virtual ~LocallyManagedUserCreationController();

  
  static LocallyManagedUserCreationController* current_controller() {
    return current_controller_;
  }

  
  
  
  void SetUpCreation(const base::string16& display_name,
                     const std::string& password,
                     int avatar_index);

  
  
  
  void StartImport(const base::string16& display_name,
                   const std::string& password,
                   int avatar_index,
                   const std::string& sync_id,
                   const std::string& master_key);
  void SetManagerProfile(Profile* manager_profile);
  void StartCreation();
  void CancelCreation();
  void FinishCreation();
  std::string GetManagedUserId();

 private:
  
  enum CreationType {
    NEW_USER,
    USER_IMPORT,
  };

  
  struct UserCreationContext {
    UserCreationContext();
    ~UserCreationContext();

    base::string16 display_name;
    int avatar_index;
    std::string manager_id;
    std::string local_user_id; 
    std::string sync_user_id;  
    std::string password;
    std::string mount_hash;
    std::string master_key;
    bool token_acquired;
    std::string token;
    bool token_succesfully_written;
    CreationType creation_type;
    Profile* manager_profile;
    scoped_ptr<ManagedUserRegistrationUtility> registration_utility;
  };

  
  virtual void OnAuthenticationFailure(
      ManagedUserAuthenticator::AuthState error) OVERRIDE;
  virtual void OnMountSuccess(const std::string& mount_hash) OVERRIDE;
  virtual void OnAddKeySuccess() OVERRIDE;

  void CreationTimedOut();
  void RegistrationCallback(const GoogleServiceAuthError& error,
                            const std::string& token);

  void TokenFetched(const std::string& token);

  
  
  void OnManagedUserFilesStored(bool success);

  
  
  static LocallyManagedUserCreationController* current_controller_;

  StatusConsumer* consumer_;

  scoped_refptr<ManagedUserAuthenticator> authenticator_;

  
  scoped_ptr<UserCreationContext> creation_context_;

  
  base::OneShotTimer<LocallyManagedUserCreationController> timeout_timer_;

  
  base::WeakPtrFactory<LocallyManagedUserCreationController> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(LocallyManagedUserCreationController);
};

}  

#endif  
