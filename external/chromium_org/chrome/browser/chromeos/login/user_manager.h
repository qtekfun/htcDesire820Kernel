// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_MANAGER_H_

#include <string>

#include "chrome/browser/chromeos/base/locale_util.h"
#include "chrome/browser/chromeos/login/user.h"
#include "chrome/browser/chromeos/login/user_flow.h"

class PrefRegistrySimple;

namespace chromeos {

class RemoveUserDelegate;
class UserImageManager;
class SupervisedUserManager;

class UserManager {
 public:
  
  
  class Observer {
   public:
    
    virtual void LocalStateChanged(UserManager* user_manager);

   protected:
    virtual ~Observer();
  };

  
  
  
  class UserSessionStateObserver {
   public:
    
    virtual void ActiveUserChanged(const User* active_user);

    
    virtual void UserAddedToSession(const User* added_user);

    
    
    virtual void ActiveUserHashChanged(const std::string& hash);

    
    virtual void PendingUserSessionsRestoreFinished();

   protected:
    virtual ~UserSessionStateObserver();
  };

  
  class UserAccountData {
   public:
    UserAccountData(const base::string16& display_name,
                    const base::string16& given_name,
                    const std::string& locale);
    ~UserAccountData();
    const base::string16& display_name() const { return display_name_; }
    const base::string16& given_name() const { return given_name_; }
    const std::string& locale() const { return locale_; }

   private:
    const base::string16 display_name_;
    const base::string16 given_name_;
    const std::string locale_;

    DISALLOW_COPY_AND_ASSIGN(UserAccountData);
  };

  
  static const char kStubUser[];

  
  
  
  
  
  static const char kGuestUserName[];

  
  static const char kLocallyManagedUserDomain[];

  
  static const char kRetailModeUserName[];

  
  
  static void Initialize();

  
  
  static bool IsInitialized();

  
  
  
  
  virtual void Shutdown() = 0;

  
  
  static void Destroy();

  
  
  
  static UserManager* Get();

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  static bool IsMultipleProfilesAllowed();

  virtual ~UserManager();

  virtual UserImageManager* GetUserImageManager() = 0;
  virtual SupervisedUserManager* GetSupervisedUserManager() = 0;

  
  
  virtual const UserList& GetUsers() const = 0;

  
  virtual UserList GetUsersAdmittedForMultiProfile() const = 0;

  
  virtual const UserList& GetLoggedInUsers() const = 0;

  
  
  
  virtual const UserList& GetLRULoggedInUsers() = 0;

  
  virtual UserList GetUnlockUsers() const = 0;

  
  
  virtual const std::string& GetOwnerEmail() = 0;

  
  
  
  
  
  virtual void UserLoggedIn(const std::string& user_id,
                            const std::string& username_hash,
                            bool browser_restart) = 0;

  
  virtual void SwitchActiveUser(const std::string& user_id) = 0;

  
  
  
  
  
  
  
  virtual void SessionStarted() = 0;

  
  
  
  
  
  virtual void RestoreActiveSessions() = 0;

  
  
  
  virtual void RemoveUser(const std::string& user_id,
                          RemoveUserDelegate* delegate) = 0;

  
  
  virtual void RemoveUserFromList(const std::string& user_id) = 0;

  
  
  virtual bool IsKnownUser(const std::string& user_id) const = 0;

  
  
  virtual const User* FindUser(const std::string& user_id) const = 0;

  
  
  
  virtual User* FindUserAndModify(const std::string& user_id) = 0;

  
  
  
  virtual const User* GetLoggedInUser() const = 0;
  virtual User* GetLoggedInUser() = 0;

  
  
  
  virtual const User* GetActiveUser() const = 0;
  virtual User* GetActiveUser() = 0;

  
  
  virtual const User* GetPrimaryUser() const = 0;

  
  virtual User* GetUserByProfile(Profile* profile) const = 0;

  
  virtual Profile* GetProfileByUser(const User* user) const = 0;

  
  virtual void SaveUserOAuthStatus(
      const std::string& user_id,
      User::OAuthTokenStatus oauth_token_status) = 0;

  
  
  virtual void SaveUserDisplayName(const std::string& user_id,
                                   const base::string16& display_name) = 0;

  
  virtual void UpdateUserAccountData(const std::string& user_id,
                                     const UserAccountData& account_data) = 0;

  
  
  
  virtual base::string16 GetUserDisplayName(
      const std::string& user_id) const = 0;

  
  
  virtual void SaveUserDisplayEmail(const std::string& user_id,
                                    const std::string& display_email) = 0;

  
  
  
  virtual std::string GetUserDisplayEmail(
      const std::string& user_id) const = 0;

  
  virtual bool IsCurrentUserOwner() const = 0;

  
  virtual bool IsCurrentUserNew() const = 0;

  
  
  
  virtual bool IsCurrentUserNonCryptohomeDataEphemeral() const = 0;

  
  
  virtual bool CanCurrentUserLock() const = 0;

  
  virtual bool IsUserLoggedIn() const = 0;

  
  virtual bool IsLoggedInAsRegularUser() const = 0;

  
  virtual bool IsLoggedInAsDemoUser() const = 0;

  
  virtual bool IsLoggedInAsPublicAccount() const = 0;

  
  virtual bool IsLoggedInAsGuest() const = 0;

  
  virtual bool IsLoggedInAsLocallyManagedUser() const = 0;

  
  virtual bool IsLoggedInAsKioskApp() const = 0;

  
  virtual bool IsLoggedInAsStub() const = 0;

  
  
  
  virtual bool IsSessionStarted() const = 0;

  
  
  virtual bool UserSessionsRestored() const = 0;

  
  
  virtual bool HasBrowserRestarted() const = 0;

  
  
  
  virtual bool IsUserNonCryptohomeDataEphemeral(
      const std::string& user_id) const = 0;

  
  
  
  
  virtual void SetUserFlow(const std::string& user_id, UserFlow* flow) = 0;

  
  
  
  virtual UserFlow* GetCurrentUserFlow() const = 0;

  
  
  
  virtual UserFlow* GetUserFlow(const std::string& user_id) const = 0;

  
  virtual void ResetUserFlow(const std::string& user_id) = 0;

  
  
  virtual bool GetAppModeChromeClientOAuthInfo(
      std::string* chrome_client_id,
      std::string* chrome_client_secret) = 0;
  virtual void SetAppModeChromeClientOAuthInfo(
      const std::string& chrome_client_id,
      const std::string& chrome_client_secret) = 0;

  virtual void AddObserver(Observer* obs) = 0;
  virtual void RemoveObserver(Observer* obs) = 0;

  virtual void AddSessionStateObserver(UserSessionStateObserver* obs) = 0;
  virtual void RemoveSessionStateObserver(UserSessionStateObserver* obs) = 0;

  virtual void NotifyLocalStateChanged() = 0;

  
  virtual bool AreLocallyManagedUsersAllowed() const = 0;

  
  virtual base::FilePath GetUserProfileDir(const std::string& user_id)
      const = 0;

  
  
  virtual bool RespectLocalePreference(
      Profile* profile,
      const User* user,
      scoped_ptr<locale_util::SwitchLanguageCallback> callback) const = 0;

 private:
  friend class ScopedUserManagerEnabler;

  
  
  static UserManager* SetForTesting(UserManager* user_manager);
};

class ScopedUserManagerEnabler {
 public:
  
  explicit ScopedUserManagerEnabler(UserManager* user_manager);
  ~ScopedUserManagerEnabler();

 private:
  UserManager* previous_user_manager_;

  DISALLOW_COPY_AND_ASSIGN(ScopedUserManagerEnabler);
};

class ScopedTestUserManager {
 public:
  ScopedTestUserManager();
  ~ScopedTestUserManager();

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedTestUserManager);
};

}  

#endif  
