// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_MANAGER_IMPL_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_MANAGER_IMPL_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/login/login_utils.h"
#include "chrome/browser/chromeos/login/multi_profile_user_controller_delegate.h"
#include "chrome/browser/chromeos/login/user.h"
#include "chrome/browser/chromeos/login/user_image_manager_impl.h"
#include "chrome/browser/chromeos/login/user_manager.h"
#include "chrome/browser/chromeos/login/wallpaper_manager.h"
#include "chrome/browser/chromeos/policy/device_local_account_policy_service.h"
#include "chrome/browser/chromeos/settings/cros_settings.h"
#include "chrome/browser/chromeos/settings/device_settings_service.h"
#include "chromeos/dbus/session_manager_client.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class PrefService;
class ProfileSyncService;

namespace policy {
struct DeviceLocalAccount;
}

namespace chromeos {

class MultiProfileFirstRunNotification;
class MultiProfileUserController;
class RemoveUserDelegate;
class SupervisedUserManagerImpl;
class SessionLengthLimiter;

class UserManagerImpl
    : public UserManager,
      public LoginUtils::Delegate,
      public content::NotificationObserver,
      public policy::DeviceLocalAccountPolicyService::Observer,
      public MultiProfileUserControllerDelegate {
 public:
  virtual ~UserManagerImpl();

  
  virtual void Shutdown() OVERRIDE;
  virtual UserImageManager* GetUserImageManager() OVERRIDE;
  virtual SupervisedUserManager* GetSupervisedUserManager() OVERRIDE;
  virtual const UserList& GetUsers() const OVERRIDE;
  virtual UserList GetUsersAdmittedForMultiProfile() const OVERRIDE;
  virtual const UserList& GetLoggedInUsers() const OVERRIDE;
  virtual const UserList& GetLRULoggedInUsers() OVERRIDE;
  virtual UserList GetUnlockUsers() const OVERRIDE;
  virtual const std::string& GetOwnerEmail() OVERRIDE;
  virtual void UserLoggedIn(const std::string& user_id,
                            const std::string& user_id_hash,
                            bool browser_restart) OVERRIDE;
  virtual void SwitchActiveUser(const std::string& user_id) OVERRIDE;
  virtual void RestoreActiveSessions() OVERRIDE;
  virtual void SessionStarted() OVERRIDE;
  virtual void RemoveUser(const std::string& user_id,
                          RemoveUserDelegate* delegate) OVERRIDE;
  virtual void RemoveUserFromList(const std::string& user_id) OVERRIDE;
  virtual bool IsKnownUser(const std::string& user_id) const OVERRIDE;
  virtual const User* FindUser(const std::string& user_id) const OVERRIDE;
  virtual User* FindUserAndModify(const std::string& user_id) OVERRIDE;
  virtual const User* GetLoggedInUser() const OVERRIDE;
  virtual User* GetLoggedInUser() OVERRIDE;
  virtual const User* GetActiveUser() const OVERRIDE;
  virtual User* GetActiveUser() OVERRIDE;
  virtual const User* GetPrimaryUser() const OVERRIDE;
  virtual User* GetUserByProfile(Profile* profile) const OVERRIDE;
  virtual Profile* GetProfileByUser(const User* user) const OVERRIDE;
  virtual void SaveUserOAuthStatus(
      const std::string& user_id,
      User::OAuthTokenStatus oauth_token_status) OVERRIDE;
  virtual void SaveUserDisplayName(const std::string& user_id,
                                   const base::string16& display_name) OVERRIDE;
  virtual base::string16 GetUserDisplayName(
      const std::string& user_id) const OVERRIDE;
  virtual void SaveUserDisplayEmail(const std::string& user_id,
                                    const std::string& display_email) OVERRIDE;
  virtual std::string GetUserDisplayEmail(
      const std::string& user_id) const OVERRIDE;
  virtual void UpdateUserAccountData(
      const std::string& user_id,
      const UserAccountData& account_data) OVERRIDE;
  virtual bool IsCurrentUserOwner() const OVERRIDE;
  virtual bool IsCurrentUserNew() const OVERRIDE;
  virtual bool IsCurrentUserNonCryptohomeDataEphemeral() const OVERRIDE;
  virtual bool CanCurrentUserLock() const OVERRIDE;
  virtual bool IsUserLoggedIn() const OVERRIDE;
  virtual bool IsLoggedInAsRegularUser() const OVERRIDE;
  virtual bool IsLoggedInAsDemoUser() const OVERRIDE;
  virtual bool IsLoggedInAsPublicAccount() const OVERRIDE;
  virtual bool IsLoggedInAsGuest() const OVERRIDE;
  virtual bool IsLoggedInAsLocallyManagedUser() const OVERRIDE;
  virtual bool IsLoggedInAsKioskApp() const OVERRIDE;
  virtual bool IsLoggedInAsStub() const OVERRIDE;
  virtual bool IsSessionStarted() const OVERRIDE;
  virtual bool UserSessionsRestored() const OVERRIDE;
  virtual bool HasBrowserRestarted() const OVERRIDE;
  virtual bool IsUserNonCryptohomeDataEphemeral(
      const std::string& user_id) const OVERRIDE;
  virtual void AddObserver(UserManager::Observer* obs) OVERRIDE;
  virtual void RemoveObserver(UserManager::Observer* obs) OVERRIDE;
  virtual void AddSessionStateObserver(
      UserManager::UserSessionStateObserver* obs) OVERRIDE;
  virtual void RemoveSessionStateObserver(
      UserManager::UserSessionStateObserver* obs) OVERRIDE;
  virtual void NotifyLocalStateChanged() OVERRIDE;

  virtual UserFlow* GetCurrentUserFlow() const OVERRIDE;
  virtual UserFlow* GetUserFlow(const std::string& user_id) const OVERRIDE;
  virtual void SetUserFlow(const std::string& user_id, UserFlow* flow) OVERRIDE;
  virtual void ResetUserFlow(const std::string& user_id) OVERRIDE;
  virtual bool GetAppModeChromeClientOAuthInfo(
      std::string* chrome_client_id,
      std::string* chrome_client_secret) OVERRIDE;
  virtual void SetAppModeChromeClientOAuthInfo(
      const std::string& chrome_client_id,
      const std::string& chrome_client_secret) OVERRIDE;
  virtual bool AreLocallyManagedUsersAllowed() const OVERRIDE;
  virtual base::FilePath GetUserProfileDir(
      const std::string& user_id) const OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnPolicyUpdated(const std::string& user_id) OVERRIDE;
  virtual void OnDeviceLocalAccountsChanged() OVERRIDE;

  virtual bool RespectLocalePreference(
      Profile* profile,
      const User* user,
      scoped_ptr<locale_util::SwitchLanguageCallback> callback) const OVERRIDE;

 private:
  friend class SupervisedUserManagerImpl;
  friend class UserManager;
  friend class WallpaperManager;
  friend class UserManagerTest;
  friend class WallpaperManagerTest;

  
  
  enum UserLoadStage {
    STAGE_NOT_LOADED = 0,
    STAGE_LOADING,
    STAGE_LOADED
  };

  UserManagerImpl();

  
  
  virtual void OnProfilePrepared(Profile* profile) OVERRIDE;

  
  
  void EnsureUsersLoaded();

  
  
  
  void RetrieveTrustedDevicePolicies();

  
  
  bool AreEphemeralUsersEnabled() const;

  
  
  UserList& GetUsersAndModify();

  
  
  const User* FindUserInList(const std::string& user_id) const;

  
  
  const bool UserExistsInList(const std::string& user_id) const;

  
  User* FindUserInListAndModify(const std::string& user_id);

  
  void GuestUserLoggedIn();

  
  void RegularUserLoggedIn(const std::string& user_id);

  
  void RegularUserLoggedInAsEphemeral(const std::string& user_id);

  
  void LocallyManagedUserLoggedIn(const std::string& user_id);

  
  void PublicAccountUserLoggedIn(User* user);

  
  void KioskAppLoggedIn(const std::string& app_id);

  
  void RetailModeUserLoggedIn();

  
  
  void NotifyOnLogin();

  
  User::OAuthTokenStatus LoadUserOAuthStatus(const std::string& user_id) const;

  void SetCurrentUserIsOwner(bool is_current_user_owner);

  
  void UpdateOwnership();

  
  
  void RemoveNonCryptohomeData(const std::string& user_id);

  
  
  
  User* RemoveRegularOrLocallyManagedUserFromList(const std::string& user_id);

  
  
  void CleanUpPublicAccountNonCryptohomeDataPendingRemoval();

  
  
  
  
  void CleanUpPublicAccountNonCryptohomeData(
      const std::vector<std::string>& old_public_accounts);

  
  
  
  
  
  bool UpdateAndCleanUpPublicAccounts(
      const std::vector<policy::DeviceLocalAccount>& device_local_accounts);

  
  
  void UpdatePublicAccountDisplayName(const std::string& user_id);

  
  void NotifyUserListChanged();

  
  void NotifyMergeSessionStateChanged();

  
  void NotifyActiveUserChanged(const User* active_user);

  
  void NotifyUserAddedToSession(const User* added_user);

  
  void NotifyActiveUserHashChanged(const std::string& hash);

  
  void NotifyPendingUserSessionsRestoreFinished();

  
  UserFlow* GetDefaultUserFlow() const;

  
  void UpdateLoginState();

  
  void SetLRUUser(User* user);

  
  
  void AddUserRecord(User* user);

  
  void OnRestoreActiveSessions(
      const SessionManagerClient::ActiveSessionsMap& sessions,
      bool success);

  
  
  
  
  void RestorePendingUserSessions();

  
  void SendRegularUserLoginMetrics(const std::string& user_id);

  
  
  
  void RemoveUserInternal(const std::string& user_email,
                          RemoveUserDelegate* delegate);

  
  
  void RemoveNonOwnerUserInternal(const std::string& user_email,
                                  RemoveUserDelegate* delegate);

  
  virtual void OnUserNotAllowed() OVERRIDE;

  
  virtual void UpdateUserAccountLocale(const std::string& user_id,
                                       const std::string& locale);

  
  void DoUpdateAccountLocale(const std::string& user_id,
                             const std::string& resolved_locale);

  
  CrosSettings* cros_settings_;

  
  policy::DeviceLocalAccountPolicyService* device_local_account_policy_service_;

  
  UserLoadStage user_loading_stage_;

  
  
  
  UserList users_;

  
  
  UserList logged_in_users_;

  
  
  
  UserList lru_logged_in_users_;

  
  UserList temp_single_logged_in_users_;

  
  
  
  
  User* active_user_;

  
  
  User* primary_user_;

  
  bool session_started_;

  
  
  bool user_sessions_restored_;

  
  
  bool is_current_user_owner_;
  mutable base::Lock is_current_user_owner_lock_;

  
  
  bool is_current_user_new_;

  
  
  
  
  
  bool is_current_user_ephemeral_regular_user_;

  
  
  
  bool ephemeral_users_enabled_;

  
  
  std::string owner_email_;

  
  std::string chrome_client_id_;
  std::string chrome_client_secret_;

  content::NotificationRegistrar registrar_;

  ObserverList<UserManager::Observer> observer_list_;

  
  ObserverList<UserManager::UserSessionStateObserver>
      session_state_observer_list_;

  
  scoped_ptr<UserImageManagerImpl> user_image_manager_;

  
  scoped_ptr<SupervisedUserManagerImpl> supervised_user_manager_;

  
  scoped_ptr<SessionLengthLimiter> session_length_limiter_;

  typedef std::map<std::string, UserFlow*> FlowMap;

  
  mutable scoped_ptr<UserFlow> default_flow_;

  
  
  FlowMap specific_flows_;

  
  
  SessionManagerClient::ActiveSessionsMap pending_user_sessions_;

  
  base::TimeTicks manager_creation_time_;

  scoped_ptr<CrosSettings::ObserverSubscription>
      local_accounts_subscription_;
  scoped_ptr<CrosSettings::ObserverSubscription>
      supervised_users_subscription_;

  scoped_ptr<MultiProfileUserController> multi_profile_user_controller_;
  scoped_ptr<MultiProfileFirstRunNotification>
      multi_profile_first_run_notification_;

  DISALLOW_COPY_AND_ASSIGN(UserManagerImpl);
};

}  

#endif  
