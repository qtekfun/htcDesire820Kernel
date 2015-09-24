// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_PROFILE_SYNC_SERVICE_H_
#define CHROME_BROWSER_SYNC_PROFILE_SYNC_SERVICE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/string16.h"
#include "base/task.h"
#include "base/time.h"
#include "base/timer.h"
#include "base/tracked.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/sync/engine/model_safe_worker.h"
#include "chrome/browser/sync/glue/data_type_controller.h"
#include "chrome/browser/sync/glue/sync_backend_host.h"
#include "chrome/browser/sync/js_event_handler_list.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "chrome/browser/sync/sync_setup_wizard.h"
#include "chrome/browser/sync/syncable/autofill_migration.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/browser/sync/unrecoverable_error_handler.h"
#include "chrome/common/net/gaia/google_service_auth_error.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/notification_type.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/native_widget_types.h"

class NotificationDetails;
class NotificationSource;
class Profile;
class ProfileSyncFactory;
class SigninManager;

namespace browser_sync {
class BackendMigrator;
class ChangeProcessor;
class DataTypeManager;
class JsFrontend;
class SessionModelAssociator;
namespace sessions { struct SyncSessionSnapshot; }
}

namespace sync_api {
class BaseTransaction;
struct SyncCredentials;
struct UserShare;
}

class ProfileSyncService : public browser_sync::SyncFrontend,
                           public browser_sync::UnrecoverableErrorHandler,
                           public NotificationObserver {
 public:
  typedef ProfileSyncServiceObserver Observer;
  typedef browser_sync::SyncBackendHost::Status Status;

  enum SyncEventCodes  {
    MIN_SYNC_EVENT_CODE = 0,

    
    START_FROM_NTP = 1,      
    START_FROM_WRENCH = 2,   
    START_FROM_OPTIONS = 3,  
    START_FROM_BOOKMARK_MANAGER = 4,  

    
    CANCEL_FROM_SIGNON_WITHOUT_AUTH = 10,   
                                            
    CANCEL_DURING_SIGNON = 11,              
    CANCEL_DURING_CONFIGURE = 12,           
                                            
    
    STOP_FROM_OPTIONS = 20,  

    

    MAX_SYNC_EVENT_CODE
  };

  
  enum ClearServerDataState {
    CLEAR_NOT_STARTED = 1,
    CLEAR_CLEARING = 2,
    CLEAR_FAILED = 3,
    CLEAR_SUCCEEDED = 4,
  };

  
  static const char* kSyncServerUrl;
  
  static const char* kDevServerUrl;

  ProfileSyncService(ProfileSyncFactory* factory_,
                     Profile* profile,
                     const std::string& cros_user);
  virtual ~ProfileSyncService();

  
  
  void Initialize();

  void RegisterAuthNotifications();

  
  
  bool AreCredentialsAvailable();

  
  
  
  
  void RegisterDataTypeController(
      browser_sync::DataTypeController* data_type_controller);

  
  
  
  browser_sync::SessionModelAssociator* GetSessionModelAssociator();

  
  void ResetClearServerDataState();
  ClearServerDataState GetClearServerDataState();

  
  
  void GetDataTypeControllerStates(
    browser_sync::DataTypeController::StateMap* state_map) const;

  
  virtual void DisableForUser();

  
  void ClearServerData();

  
  virtual bool HasSyncSetupCompleted() const;
  virtual void SetSyncSetupCompleted();

  
  virtual void OnBackendInitialized();
  virtual void OnSyncCycleCompleted();
  virtual void OnAuthError();
  virtual void OnStopSyncingPermanently();
  virtual void OnClearServerDataFailed();
  virtual void OnClearServerDataTimeout();
  virtual void OnClearServerDataSucceeded();
  virtual void OnPassphraseRequired(bool for_decryption);
  virtual void OnPassphraseAccepted();
  virtual void OnEncryptionComplete(
      const syncable::ModelTypeSet& encrypted_types);
  virtual void OnMigrationNeededForTypes(
      const syncable::ModelTypeSet& types);

  
  virtual void OnUserSubmittedAuth(const std::string& username,
                                   const std::string& password,
                                   const std::string& captcha,
                                   const std::string& access_code);

  
  void UpdateAuthErrorState(const GoogleServiceAuthError& error);

  
  
  
  
  
  virtual void OnUserChoseDatatypes(bool sync_everything,
      const syncable::ModelTypeSet& chosen_types);

  
  virtual void OnUserCancelledDialog();

  
  browser_sync::SyncBackendHost::StatusSummary QuerySyncStatusSummary();
  virtual browser_sync::SyncBackendHost::Status QueryDetailedSyncStatus();

  const GoogleServiceAuthError& GetAuthError() const {
    return last_auth_error_;
  }

  
  
  
  
  
  
  virtual bool SetupInProgress() const;
  bool WizardIsVisible() const {
    return wizard_.IsVisible();
  }
  virtual void ShowLoginDialog(gfx::NativeWindow parent_window);
  SyncSetupWizard& get_wizard() { return wizard_; }

  
  
  virtual void ShowErrorUI(gfx::NativeWindow parent_window);

  
  
  
  
  void ShowConfigure(gfx::NativeWindow parent_window, bool sync_everything);

  void PromptForExistingPassphrase(gfx::NativeWindow parent_window);
  void SigninForPassphraseMigration(gfx::NativeWindow parent_window);

  
  static std::string BuildSyncStatusSummaryText(
      const browser_sync::SyncBackendHost::StatusSummary& summary);

  
  
  
  
  
  
  
  
  bool sync_initialized() const { return backend_initialized_; }
  virtual bool unrecoverable_error_detected() const;
  const std::string& unrecoverable_error_message() {
    return unrecoverable_error_message_;
  }
  tracked_objects::Location unrecoverable_error_location() {
    return unrecoverable_error_location_.get() ?
        *unrecoverable_error_location_.get() : tracked_objects::Location();
  }

  bool UIShouldDepictAuthInProgress() const {
    return is_auth_in_progress_;
  }

  bool observed_passphrase_required() const {
    return observed_passphrase_required_;
  }

  bool passphrase_required_for_decryption() const {
    return passphrase_required_for_decryption_;
  }

  
  virtual string16 GetLastSyncedTimeString() const;

  
  
  
  virtual string16 GetAuthenticatedUsername() const;

  const std::string& last_attempted_user_email() const {
    return last_attempted_user_email_;
  }

  
  Profile* profile() const { return profile_; }

  
  
  virtual void AddObserver(Observer* observer);
  virtual void RemoveObserver(Observer* observer);

  
  bool HasObserver(Observer* observer) const;

  
  
  
  virtual browser_sync::JsFrontend* GetJsFrontend();

  
  static void SyncEvent(SyncEventCodes code);

  
  
  
  static bool IsSyncEnabled();

  
  
  bool IsManaged();

  
  virtual void OnUnrecoverableError(
      const tracked_objects::Location& from_here,
      const std::string& message);

  
  

  
  
  
  
  sync_api::UserShare* GetUserShare() const;

  
  
  

  virtual const browser_sync::sessions::SyncSessionSnapshot*
      GetLastSessionSnapshot() const;

  
  
  
  bool HasUnsyncedItems() const;

  
  
  
  
  
  
  
  
  void GetModelSafeRoutingInfo(browser_sync::ModelSafeRoutingInfo* out);

  
  

  syncable::AutofillMigrationState
      GetAutofillMigrationState();

  void SetAutofillMigrationState(
      syncable::AutofillMigrationState state);

  syncable::AutofillMigrationDebugInfo
      GetAutofillMigrationDebugInfo();

  void SetAutofillMigrationDebugInfo(
      syncable::AutofillMigrationDebugInfo::PropertyToSet property_to_set,
      const syncable::AutofillMigrationDebugInfo& info);

  virtual void ActivateDataType(
      browser_sync::DataTypeController* data_type_controller,
      browser_sync::ChangeProcessor* change_processor);
  virtual void DeactivateDataType(
      browser_sync::DataTypeController* data_type_controller,
      browser_sync::ChangeProcessor* change_processor);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  
  virtual void ChangePreferredDataTypes(
      const syncable::ModelTypeSet& preferred_types);

  
  
  
  virtual void GetPreferredDataTypes(
      syncable::ModelTypeSet* preferred_types) const;

  
  
  
  
  virtual void GetRegisteredDataTypes(
      syncable::ModelTypeSet* registered_types) const;

  
  
  
  virtual bool IsCryptographerReady(
      const sync_api::BaseTransaction* trans) const;

  
  virtual bool IsUsingSecondaryPassphrase() const;

  
  
  
  
  
  
  
  
  
  
  
  virtual void SetPassphrase(const std::string& passphrase,
                             bool is_explicit,
                             bool is_creation);

  
  
  
  
  
  
  virtual void EncryptDataTypes(
      const syncable::ModelTypeSet& encrypted_types);

  
  virtual void GetEncryptedDataTypes(
      syncable::ModelTypeSet* encrypted_types) const;

  
  
  bool ShouldPushChanges();

  const GURL& sync_service_url() const { return sync_service_url_; }
  SigninManager* signin() { return signin_.get(); }
  const std::string& cros_user() const { return cros_user_; }

 protected:
  
  virtual browser_sync::SyncBackendHost* GetBackendForTest();

  
  void ConfigureDataTypeManager();

  
  void StartUp();
  
  
  void Shutdown(bool sync_disabled);

  
  void RegisterPreferences();
  void ClearPreferences();

  
  sync_api::SyncCredentials GetCredentials();

  
  
  
  
  
  
  virtual void CreateBackend();

  const browser_sync::DataTypeController::TypeMap& data_type_controllers() {
    return data_type_controllers_;
  }

  
  bool IsEncryptedDatatypeEnabled() const;

  
  
  GoogleServiceAuthError last_auth_error_;

  
  
  scoped_ptr<browser_sync::SyncBackendHost> backend_;

  
  std::string last_attempted_user_email_;

  
  
  bool observed_passphrase_required_;

  
  
  bool passphrase_required_for_decryption_;

  
  bool passphrase_migration_in_progress_;

 private:
  friend class ProfileSyncServicePasswordTest;
  friend class TestProfileSyncService;
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceTest, InitialState);

  
  
  void InitializeBackend(bool delete_sync_data_folder);

  
  void InitSettings();

  
  void UpdateLastSyncedTime();

  void NotifyObservers();

  static const char* GetPrefNameForDataType(syncable::ModelType data_type);

  
  base::TimeTicks auth_start_time_;

  
  base::TimeTicks auth_error_time_;

  
  ProfileSyncFactory* factory_;

  
  Profile* profile_;

  
  std::string cros_user_;

  
  
  GURL sync_service_url_;

  
  
  base::Time last_synced_time_;

  
  browser_sync::DataTypeController::TypeMap data_type_controllers_;

  
  bool backend_initialized_;

  
  
  
  
  bool is_auth_in_progress_;

  SyncSetupWizard wizard_;

  
  scoped_ptr<SigninManager> signin_;

  
  
  
  bool unrecoverable_error_detected_;

  
  std::string unrecoverable_error_message_;
  scoped_ptr<tracked_objects::Location> unrecoverable_error_location_;

  
  scoped_ptr<browser_sync::DataTypeManager> data_type_manager_;

  ObserverList<Observer> observers_;

  browser_sync::JsEventHandlerList js_event_handlers_;

  NotificationRegistrar registrar_;

  ScopedRunnableMethodFactory<ProfileSyncService>
      scoped_runnable_method_factory_;

  
  
  BooleanPrefMember pref_sync_managed_;

  
  
  
  bool expect_sync_configuration_aborted_;

  
  
  
  
  struct CachedPassphrase {
    std::string value;
    bool is_explicit;
    bool is_creation;
    CachedPassphrase() : is_explicit(false), is_creation(false) {}
  };
  CachedPassphrase cached_passphrase_;

  
  std::string gaia_password_;

  
  
  
  
  
  
  
  bool tried_implicit_gaia_remove_when_bug_62103_fixed_;

  
  ClearServerDataState clear_server_data_state_;

  
  
  
  
  
  base::OneShotTimer<ProfileSyncService> clear_server_data_timer_;

  
  
  syncable::ModelTypeSet encrypted_types_;

  scoped_ptr<browser_sync::BackendMigrator> migrator_;

  DISALLOW_COPY_AND_ASSIGN(ProfileSyncService);
};

#endif  
