// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_PROFILE_SYNC_SERVICE_H_
#define CHROME_BROWSER_SYNC_PROFILE_SYNC_SERVICE_H_

#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/location.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/sync/backend_unrecoverable_error_handler.h"
#include "chrome/browser/sync/glue/data_type_controller.h"
#include "chrome/browser/sync/glue/data_type_encryption_handler.h"
#include "chrome/browser/sync/glue/data_type_manager.h"
#include "chrome/browser/sync/glue/data_type_manager_observer.h"
#include "chrome/browser/sync/glue/failed_data_types_handler.h"
#include "chrome/browser/sync/glue/sync_backend_host.h"
#include "chrome/browser/sync/glue/sync_frontend.h"
#include "chrome/browser/sync/glue/synced_device_tracker.h"
#include "chrome/browser/sync/profile_sync_service_base.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "chrome/browser/sync/sessions2/sessions_sync_manager.h"
#include "chrome/browser/sync/sync_prefs.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_types.h"
#include "google_apis/gaia/google_service_auth_error.h"
#include "google_apis/gaia/oauth2_token_service.h"
#include "net/base/backoff_entry.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/engine/model_safe_worker.h"
#include "sync/internal_api/public/sync_manager_factory.h"
#include "sync/internal_api/public/util/experiments.h"
#include "sync/internal_api/public/util/unrecoverable_error_handler.h"
#include "sync/js/sync_js_controller.h"
#include "url/gurl.h"

class Profile;
class ProfileOAuth2TokenService;
class ProfileSyncComponentsFactory;
class SigninManagerBase;
class SyncGlobalError;

namespace browser_sync {
class BackendMigrator;
class ChangeProcessor;
class DataTypeManager;
class DeviceInfo;
class FaviconCache;
class JsController;
class OpenTabsUIDelegate;
class SessionModelAssociator;

namespace sessions {
class SyncSessionSnapshot;
}  
}  

namespace syncer {
class BaseTransaction;
class NetworkResources;
struct SyncCredentials;
struct UserShare;
}  

namespace sync_pb {
class EncryptedData;
}  

using browser_sync::SessionsSyncManager;

class ProfileSyncService
    : public ProfileSyncServiceBase,
      public browser_sync::SyncFrontend,
      public browser_sync::SyncPrefObserver,
      public browser_sync::DataTypeManagerObserver,
      public syncer::UnrecoverableErrorHandler,
      public content::NotificationObserver,
      public BrowserContextKeyedService,
      public browser_sync::DataTypeEncryptionHandler,
      public OAuth2TokenService::Consumer,
      public OAuth2TokenService::Observer,
      public SessionsSyncManager::SyncInternalApiDelegate {
 public:
  typedef browser_sync::SyncBackendHost::Status Status;

  
  struct SyncTokenStatus {
    SyncTokenStatus();
    ~SyncTokenStatus();

    
    base::Time connection_status_update_time;
    syncer::ConnectionStatus connection_status;

    
    base::Time token_request_time;
    base::Time token_receive_time;

    
    
    GoogleServiceAuthError last_get_token_error;
    base::Time next_token_request_time;
  };

  enum SyncEventCodes  {
    MIN_SYNC_EVENT_CODE = 0,

    
    START_FROM_NTP = 1,      
    START_FROM_WRENCH = 2,   
    START_FROM_OPTIONS = 3,  
    START_FROM_BOOKMARK_MANAGER = 4,  
    START_FROM_PROFILE_MENU = 5,  
    START_FROM_URL = 6,  

    
    CANCEL_FROM_SIGNON_WITHOUT_AUTH = 10,   
                                            
    CANCEL_DURING_SIGNON = 11,              
    CANCEL_DURING_CONFIGURE = 12,           
                                            
    
    STOP_FROM_OPTIONS = 20,  
    STOP_FROM_ADVANCED_DIALOG = 21,  

    

    MAX_SYNC_EVENT_CODE
  };

  
  
  
  
  
  
  
  enum StartBehavior {
    AUTO_START,
    MANUAL_START,
  };

  
  enum PassphraseType {
    IMPLICIT,  
               
    EXPLICIT,  
               
  };

  enum SyncStatusSummary {
    UNRECOVERABLE_ERROR,
    NOT_ENABLED,
    SETUP_INCOMPLETE,
    DATATYPES_NOT_INITIALIZED,
    INITIALIZED,
    UNKNOWN_ERROR,
  };

  
  static const char* kSyncServerUrl;
  
  static const char* kDevServerUrl;

  
  ProfileSyncService(ProfileSyncComponentsFactory* factory,
                     Profile* profile,
                     SigninManagerBase* signin,
                     ProfileOAuth2TokenService* oauth2_token_service,
                     StartBehavior start_behavior);
  virtual ~ProfileSyncService();

  
  
  void Initialize();

  virtual void SetSyncSetupCompleted();

  
  virtual bool HasSyncSetupCompleted() const OVERRIDE;
  virtual bool ShouldPushChanges() OVERRIDE;
  virtual syncer::ModelTypeSet GetActiveDataTypes() const OVERRIDE;
  virtual void AddObserver(ProfileSyncServiceBase::Observer* observer) OVERRIDE;
  virtual void RemoveObserver(
      ProfileSyncServiceBase::Observer* observer) OVERRIDE;
  virtual bool HasObserver(
      ProfileSyncServiceBase::Observer* observer) const OVERRIDE;

  void RegisterAuthNotifications();
  void UnregisterAuthNotifications();

  
  
  
  
  
  virtual bool IsSyncEnabledAndLoggedIn();

  
  
  virtual bool IsOAuthRefreshTokenAvailable();

  
  
  
  
  void RegisterDataTypeController(
      browser_sync::DataTypeController* data_type_controller);

  
  
  
  
  
  
  
  
  virtual browser_sync::SessionModelAssociator*
      GetSessionModelAssociatorDeprecated();

  
  
  virtual browser_sync::OpenTabsUIDelegate* GetOpenTabsUIDelegate();

  
  virtual syncer::SyncableService* GetSessionsSyncableService();

  
  
  
  
  virtual scoped_ptr<browser_sync::DeviceInfo> GetLocalDeviceInfo()
      const OVERRIDE;

  
  
  
  
  
  virtual std::string GetLocalSyncCacheGUID() const OVERRIDE;

  
  
  
  virtual scoped_ptr<browser_sync::DeviceInfo> GetDeviceInfo(
      const std::string& client_id) const;

  
  
  virtual ScopedVector<browser_sync::DeviceInfo> GetAllSignedInDevices() const;

  
  virtual void AddObserverForDeviceInfoChange(
      browser_sync::SyncedDeviceTracker::Observer* observer);

  
  virtual void RemoveObserverForDeviceInfoChange(
      browser_sync::SyncedDeviceTracker::Observer* observer);

  
  
  void GetDataTypeControllerStates(
    browser_sync::DataTypeController::StateMap* state_map) const;

  
  virtual void DisableForUser();

  
  virtual void OnBackendInitialized(
      const syncer::WeakHandle<syncer::JsBackend>& js_backend,
      const syncer::WeakHandle<syncer::DataTypeDebugInfoListener>&
          debug_info_listener,
      bool success) OVERRIDE;
  virtual void OnSyncCycleCompleted() OVERRIDE;
  virtual void OnSyncConfigureRetry() OVERRIDE;
  virtual void OnConnectionStatusChange(
      syncer::ConnectionStatus status) OVERRIDE;
  virtual void OnStopSyncingPermanently() OVERRIDE;
  virtual void OnPassphraseRequired(
      syncer::PassphraseRequiredReason reason,
      const sync_pb::EncryptedData& pending_keys) OVERRIDE;
  virtual void OnPassphraseAccepted() OVERRIDE;
  virtual void OnEncryptedTypesChanged(
      syncer::ModelTypeSet encrypted_types,
      bool encrypt_everything) OVERRIDE;
  virtual void OnEncryptionComplete() OVERRIDE;
  virtual void OnMigrationNeededForTypes(
      syncer::ModelTypeSet types) OVERRIDE;
  virtual void OnExperimentsChanged(
      const syncer::Experiments& experiments) OVERRIDE;
  virtual void OnActionableError(
      const syncer::SyncProtocolError& error) OVERRIDE;

  
  virtual void OnConfigureDone(
      const browser_sync::DataTypeManager::ConfigureResult& result) OVERRIDE;
  virtual void OnConfigureRetry() OVERRIDE;
  virtual void OnConfigureStart() OVERRIDE;

  
  virtual bool IsPassphraseRequired() const OVERRIDE;
  virtual syncer::ModelTypeSet GetEncryptedDataTypes() const OVERRIDE;

  
  
  
  
  
  virtual void OnUserChoseDatatypes(bool sync_everything,
      syncer::ModelTypeSet chosen_types);

  
  SyncStatusSummary QuerySyncStatusSummary();

  
  std::string QuerySyncStatusSummaryString();

  
  
  
  virtual bool QueryDetailedSyncStatus(
      browser_sync::SyncBackendHost::Status* result);

  virtual const GoogleServiceAuthError& GetAuthError() const;

  
  
  
  
  virtual bool FirstSetupInProgress() const;

  
  
  
  
  
  
  
  virtual void SetSetupInProgress(bool setup_in_progress);

  
  
  
  
  
  
  
  
  virtual bool sync_initialized() const;

  virtual bool HasUnrecoverableError() const;
  const std::string& unrecoverable_error_message() {
    return unrecoverable_error_message_;
  }
  tracked_objects::Location unrecoverable_error_location() {
    return unrecoverable_error_location_;
  }

  
  
  virtual bool IsPassphraseRequiredForDecryption() const;

  syncer::PassphraseRequiredReason passphrase_required_reason() const {
    return passphrase_required_reason_;
  }

  
  virtual base::string16 GetLastSyncedTimeString() const;

  
  std::string GetBackendInitializationStateString() const;

  
  
  virtual bool IsStartSuppressed() const;

  ProfileSyncComponentsFactory* factory() { return factory_.get(); }

  
  Profile* profile() const { return profile_; }

  
  
  virtual base::WeakPtr<syncer::JsController> GetJsController();

  
  static void SyncEvent(SyncEventCodes code);

  
  
  
  
  
  
  static bool IsSyncEnabled();

  
  
  virtual bool IsManaged() const;

  
  virtual void OnUnrecoverableError(
      const tracked_objects::Location& from_here,
      const std::string& message) OVERRIDE;

  
  
  virtual void DisableBrokenDatatype(
      syncer::ModelType type,
      const tracked_objects::Location& from_here,
      std::string message);

  
  

  
  
  
  
  virtual syncer::UserShare* GetUserShare() const;

  
  
  

  virtual syncer::sessions::SyncSessionSnapshot
      GetLastSessionSnapshot() const;

  
  
  
  bool HasUnsyncedItems() const;

  
  browser_sync::BackendMigrator* GetBackendMigratorForTest();

  
  bool IsRetryingAccessTokenFetchForTest() const;

  
  std::string GetAccessTokenForTest() const;

  
  void GetModelSafeRoutingInfo(syncer::ModelSafeRoutingInfo* out) const;

  
  
  
  
  
  
  
  
  
  
  
  Value* GetTypeStatusMap() const;

  
  
  virtual void ActivateDataType(
      syncer::ModelType type, syncer::ModelSafeGroup group,
      browser_sync::ChangeProcessor* change_processor);
  virtual void DeactivateDataType(syncer::ModelType type);

  
  virtual void OnSyncManagedPrefChange(bool is_sync_managed) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  
  virtual void ChangePreferredDataTypes(
      syncer::ModelTypeSet preferred_types);

  
  
  virtual syncer::ModelTypeSet GetPreferredDataTypes() const;

  
  
  
  
  virtual syncer::ModelTypeSet GetRegisteredDataTypes() const;

  
  
  
  virtual bool IsCryptographerReady(
      const syncer::BaseTransaction* trans) const;

  
  
  virtual bool IsUsingSecondaryPassphrase() const;

  
  virtual syncer::PassphraseType GetPassphraseType() const;

  
  
  
  virtual base::Time GetExplicitPassphraseTime() const;

  
  
  
  
  
  

  
  
  
  
  
  virtual void SetEncryptionPassphrase(const std::string& passphrase,
                                       PassphraseType type);

  
  
  
  virtual bool SetDecryptionPassphrase(const std::string& passphrase)
      WARN_UNUSED_RESULT;

  
  
  virtual void EnableEncryptEverything();

  
  
  
  
  virtual bool EncryptEverythingEnabled() const;

  
  virtual bool encryption_pending() const;

  const GURL& sync_service_url() const { return sync_service_url_; }
  bool auto_start_enabled() const { return auto_start_enabled_; }
  SigninManagerBase* signin() const { return signin_; }
  bool setup_in_progress() const { return setup_in_progress_; }

  
  void StopAndSuppress();

  
  virtual void UnsuppressAndStart();

  
  
  void AcknowledgeSyncedTypes();

  SyncGlobalError* sync_global_error() { return sync_global_error_.get(); }

  
  const browser_sync::FailedDataTypesHandler& failed_data_types_handler() const;

  browser_sync::DataTypeManager::ConfigureStatus configure_status() {
    return configure_status_;
  }

  
  
  
  
  
  virtual bool waiting_for_auth() const;

  
  const syncer::Experiments& current_experiments() const;

  
  virtual void OnGetTokenSuccess(
      const OAuth2TokenService::Request* request,
      const std::string& access_token,
      const base::Time& expiration_time) OVERRIDE;
  virtual void OnGetTokenFailure(
      const OAuth2TokenService::Request* request,
      const GoogleServiceAuthError& error) OVERRIDE;

  
  virtual void OnRefreshTokenAvailable(const std::string& account_id) OVERRIDE;
  virtual void OnRefreshTokenRevoked(const std::string& account_id) OVERRIDE;
  virtual void OnRefreshTokensLoaded() OVERRIDE;

  
  
  virtual void Shutdown() OVERRIDE;

  
  
  
  
  void OnDataTypeRequestsSyncStartup(syncer::ModelType type);

  
  SyncTokenStatus GetSyncTokenStatus() const;

  browser_sync::FaviconCache* GetFaviconCache();

  
  
  void OverrideNetworkResourcesForTest(
      scoped_ptr<syncer::NetworkResources> network_resources);

 protected:
  
  void ConfigurePriorityDataTypes();

  
  void ConfigureDataTypeManager();

  
  
  
  void ShutdownImpl(browser_sync::SyncBackendHost::ShutdownOption option);

  
  syncer::SyncCredentials GetCredentials();

  virtual syncer::WeakHandle<syncer::JsEventHandler> GetJsEventHandler();

  
  
  
  
  
  
  virtual void CreateBackend();

  const browser_sync::DataTypeController::TypeMap& data_type_controllers() {
    return data_type_controllers_;
  }

  
  bool IsEncryptedDatatypeEnabled() const;

  
  
  
  void OnUnrecoverableErrorImpl(
      const tracked_objects::Location& from_here,
      const std::string& message,
      bool delete_sync_database);

  
  
  
  GoogleServiceAuthError last_auth_error_;

  
  
  scoped_ptr<browser_sync::SyncBackendHost> backend_;

  
  
  
  syncer::PassphraseRequiredReason passphrase_required_reason_;

 private:
  enum UnrecoverableErrorReason {
    ERROR_REASON_UNSET,
    ERROR_REASON_SYNCER,
    ERROR_REASON_BACKEND_INIT_FAILURE,
    ERROR_REASON_CONFIGURATION_RETRY,
    ERROR_REASON_CONFIGURATION_FAILURE,
    ERROR_REASON_ACTIONABLE_ERROR,
    ERROR_REASON_LIMIT
  };

  enum AuthErrorMetric {
    AUTH_ERROR_ENCOUNTERED,
    AUTH_ERROR_FIXED,
    AUTH_ERROR_LIMIT
  };

  friend class ProfileSyncServicePasswordTest;
  friend class SyncTest;
  friend class TestProfileSyncService;
  FRIEND_TEST_ALL_PREFIXES(ProfileSyncServiceTest, InitialState);

  
  void UpdateAuthErrorState(const GoogleServiceAuthError& error);

  
  
  
  void TrySyncDatatypePrefRecovery();

  
  
  void TryStart();

  
  
  void StartSyncingWithServer();

  
  
  
  void ResolvePassphraseRequired();

  
  
  
  
  void ConsumeCachedPassphraseIfPossible();

  
  
  
  
  virtual void RequestAccessToken();

  
  
  void InitializeBackend(bool delete_sync_data_folder);

  
  void InitSettings();

  
  void UpdateLastSyncedTime();

  void NotifyObservers();
  void NotifySyncCycleCompleted();

  void ClearStaleErrors();

  void ClearUnrecoverableError();

  enum StartUpDeferredOption {
    STARTUP_BACKEND_DEFERRED,
    STARTUP_IMMEDIATE
  };
  void StartUp(StartUpDeferredOption deferred_option);

  
  void StartUpSlowBackendComponents();

  
  
  static std::string GetExperimentNameForDataType(
      syncer::ModelType data_type);

  
  void RegisterNewDataType(syncer::ModelType data_type);

  
  
  
  
  
  
  virtual void ReconfigureDatatypeManager();

  
  
  void UpdateSelectedTypesHistogram(
      bool sync_everything,
      const syncer::ModelTypeSet chosen_types) const;

#if defined(OS_CHROMEOS)
  
  
  void RefreshSpareBootstrapToken(const std::string& passphrase);
#endif

  
  
  void OnInternalUnrecoverableError(const tracked_objects::Location& from_here,
                                    const std::string& message,
                                    bool delete_sync_database,
                                    UnrecoverableErrorReason reason);

  bool IsSessionsDataTypeControllerRunning() const;

  
  
  std::string GetEffectiveUsername();

  
  std::string GetAccountIdToUse();

 
  scoped_ptr<ProfileSyncComponentsFactory> factory_;

  
  Profile* profile_;

  
  
  browser_sync::SyncPrefs sync_prefs_;

  
  
  GURL sync_service_url_;

  
  
  base::Time last_synced_time_;

  
  
  
  base::Time start_up_time_;

  
  
  
  
  bool data_type_requested_sync_startup_;

  
  
  
  base::Time sync_configure_start_time_;

  
  
  bool is_first_time_sync_configure_;

  
  browser_sync::DataTypeController::TypeMap data_type_controllers_;

  
  bool backend_initialized_;

  
  
  
  bool sync_disabled_by_admin_;

  
  
  bool is_auth_in_progress_;

  
  
  SigninManagerBase* signin_;

  
  UnrecoverableErrorReason unrecoverable_error_reason_;
  std::string unrecoverable_error_message_;
  tracked_objects::Location unrecoverable_error_location_;

  
  scoped_ptr<browser_sync::DataTypeManager> data_type_manager_;

  ObserverList<ProfileSyncServiceBase::Observer> observers_;

  syncer::SyncJsController sync_js_controller_;

  content::NotificationRegistrar registrar_;

  
  
  
  bool expect_sync_configuration_aborted_;

  
  
  
  
  std::string cached_passphrase_;

  
  
  syncer::ModelTypeSet encrypted_types_;

  
  bool encrypt_everything_;

  
  
  
  bool encryption_pending_;

  
  
  
  
  const bool auto_start_enabled_;

  scoped_ptr<browser_sync::BackendMigrator> migrator_;

  
  
  syncer::SyncProtocolError last_actionable_error_;

  
  scoped_ptr<SyncGlobalError> sync_global_error_;

  
  
  browser_sync::FailedDataTypesHandler failed_data_types_handler_;

  browser_sync::DataTypeManager::ConfigureStatus configure_status_;

  
  
  bool setup_in_progress_;

  
  syncer::Experiments current_experiments_;

  
  
  syncer::WeakHandle<syncer::DataTypeDebugInfoListener> debug_info_listener_;

  
  
  
  
  
  scoped_ptr<base::Thread> sync_thread_;

  
  ProfileOAuth2TokenService* oauth2_token_service_;

  
  
  std::string access_token_;

  
  
  scoped_ptr<OAuth2TokenService::Request> access_token_request_;

  
  
  base::OneShotTimer<ProfileSyncService> request_access_token_retry_timer_;
  net::BackoffEntry request_access_token_backoff_;

  base::WeakPtrFactory<ProfileSyncService> weak_factory_;

  
  base::Time connection_status_update_time_;
  syncer::ConnectionStatus connection_status_;
  base::Time token_request_time_;
  base::Time token_receive_time_;
  GoogleServiceAuthError last_get_token_error_;
  base::Time next_token_request_time_;

  scoped_ptr<SessionsSyncManager> sessions_sync_manager_;

  scoped_ptr<syncer::NetworkResources> network_resources_;

  DISALLOW_COPY_AND_ASSIGN(ProfileSyncService);
};

bool ShouldShowActionOnUI(
    const syncer::SyncProtocolError& error);


#endif  
