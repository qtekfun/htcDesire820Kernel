// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SYNC_BACKEND_HOST_H_
#define CHROME_BROWSER_SYNC_GLUE_SYNC_BACKEND_HOST_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread.h"
#include "base/timer.h"
#include "base/utf_string_conversions.h"
#include "chrome/browser/sync/engine/syncapi.h"
#include "chrome/browser/sync/engine/model_safe_worker.h"
#include "chrome/browser/sync/js_backend.h"
#include "chrome/browser/sync/js_sync_manager_observer.h"
#include "chrome/browser/sync/glue/data_type_controller.h"
#include "chrome/browser/sync/glue/ui_model_worker.h"
#include "chrome/browser/sync/js_event_router.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/common/net/gaia/google_service_auth_error.h"
#include "googleurl/src/gurl.h"
#include "net/url_request/url_request_context_getter.h"

class CancelableTask;
class Profile;

namespace net {
class URLRequestContextGetter;
}

namespace sync_notifier {
class SyncNotifier;
}  

namespace browser_sync {

namespace sessions {
struct SyncSessionSnapshot;
}

class ChangeProcessor;
class DataTypeController;
class JsArgList;

class SyncFrontend {
 public:
  SyncFrontend() {}

  
  
  virtual void OnBackendInitialized() = 0;

  
  virtual void OnSyncCycleCompleted() = 0;

  
  
  virtual void OnAuthError() = 0;

  
  
  virtual void OnStopSyncingPermanently() = 0;

  
  virtual void OnClearServerDataSucceeded() = 0;
  virtual void OnClearServerDataFailed() = 0;

  
  
  
  
  
  
  virtual void OnPassphraseRequired(bool for_decryption) = 0;

  
  
  
  virtual void OnPassphraseAccepted() = 0;

  virtual void OnEncryptionComplete(
      const syncable::ModelTypeSet& encrypted_types) = 0;

  
  virtual void OnMigrationNeededForTypes(
      const syncable::ModelTypeSet& types) = 0;

 protected:
  
  virtual ~SyncFrontend() {
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(SyncFrontend);
};

class SyncBackendHost : public browser_sync::ModelSafeWorkerRegistrar {
 public:
  typedef sync_api::SyncManager::Status::Summary StatusSummary;
  typedef sync_api::SyncManager::Status Status;
  typedef std::map<ModelSafeGroup,
                   scoped_refptr<browser_sync::ModelSafeWorker> > WorkerMap;

  
  
  
  explicit SyncBackendHost(Profile* profile);
  
  
  SyncBackendHost();
  virtual ~SyncBackendHost();

  
  
  
  
  void Initialize(SyncFrontend* frontend,
                  const GURL& service_url,
                  const syncable::ModelTypeSet& types,
                  net::URLRequestContextGetter* baseline_context_getter,
                  const sync_api::SyncCredentials& credentials,
                  bool delete_sync_data_folder);

  
  void UpdateCredentials(const sync_api::SyncCredentials& credentials);

  
  
  
  
  virtual void StartSyncingWithServer();

  
  
  
  
  
  void SetPassphrase(const std::string& passphrase, bool is_explicit);

  
  
  
  void Shutdown(bool sync_disabled);

  
  
  
  
  virtual void ConfigureDataTypes(
      const DataTypeController::TypeMap& data_type_controllers,
      const syncable::ModelTypeSet& types,
      CancelableTask* ready_task);

  
  
  virtual void StartConfiguration(Callback0::Type* callback);

  
  
  
  
  virtual void EncryptDataTypes(
      const syncable::ModelTypeSet& encrypted_types);

  syncable::AutofillMigrationState
      GetAutofillMigrationState();

  void SetAutofillMigrationState(
      syncable::AutofillMigrationState state);

  syncable::AutofillMigrationDebugInfo
      GetAutofillMigrationDebugInfo();

  void SetAutofillMigrationDebugInfo(
      syncable::AutofillMigrationDebugInfo::PropertyToSet property_to_set,
      const syncable::AutofillMigrationDebugInfo& info);

  
  
  
  
  void ActivateDataType(DataTypeController* data_type_controller,
                        ChangeProcessor* change_processor);

  
  void DeactivateDataType(DataTypeController* data_type_controller,
                          ChangeProcessor* change_processor);

  
  virtual bool RequestClearServerData();

  
  
  sync_api::UserShare* GetUserShare() const;

  
  
  Status GetDetailedStatus();
  StatusSummary GetStatusSummary();
  const GoogleServiceAuthError& GetAuthError() const;
  const sessions::SyncSessionSnapshot* GetLastSessionSnapshot() const;

  const FilePath& sync_data_folder_path() const {
    return sync_data_folder_path_;
  }

  
  
  
  string16 GetAuthenticatedUsername() const;

  
  virtual void GetWorkers(std::vector<browser_sync::ModelSafeWorker*>* out);
  virtual void GetModelSafeRoutingInfo(ModelSafeRoutingInfo* out);

  
  
  
  bool HasUnsyncedItems() const;

  
  bool IsNigoriEnabled() const;

  
  bool IsUsingExplicitPassphrase();

  
  
  
  bool IsCryptographerReady(const sync_api::BaseTransaction* trans) const;

  
  
  
  
  virtual JsBackend* GetJsBackend();

  
  

 protected:
  
  class Core : public base::RefCountedThreadSafe<SyncBackendHost::Core>,
               public sync_api::SyncManager::Observer,
               public JsBackend,
               public JsEventRouter {
   public:
    explicit Core(SyncBackendHost* backend);

    
    
    
    virtual void OnChangesApplied(
        syncable::ModelType model_type,
        const sync_api::BaseTransaction* trans,
        const sync_api::SyncManager::ChangeRecord* changes,
        int change_count);
    virtual void OnChangesComplete(syncable::ModelType model_type);
    virtual void OnSyncCycleCompleted(
        const sessions::SyncSessionSnapshot* snapshot);
    virtual void OnInitializationComplete();
    virtual void OnAuthError(const GoogleServiceAuthError& auth_error);
    virtual void OnPassphraseRequired(bool for_decryption);
    virtual void OnPassphraseFailed();
    virtual void OnPassphraseAccepted(const std::string& bootstrap_token);
    virtual void OnStopSyncingPermanently();
    virtual void OnUpdatedToken(const std::string& token);
    virtual void OnClearServerDataFailed();
    virtual void OnClearServerDataSucceeded();
    virtual void OnEncryptionComplete(
        const syncable::ModelTypeSet& encrypted_types);

    
    virtual void SetParentJsEventRouter(JsEventRouter* router);
    virtual void RemoveParentJsEventRouter();
    virtual const JsEventRouter* GetParentJsEventRouter() const;
    virtual void ProcessMessage(const std::string& name, const JsArgList& args,
                                const JsEventHandler* sender);

    
    virtual void RouteJsEvent(const std::string& event_name,
                              const JsArgList& args,
                              const JsEventHandler* dst);

    struct DoInitializeOptions {
      DoInitializeOptions(
          const GURL& service_url,
          sync_api::HttpPostProviderFactory* http_bridge_factory,
          const sync_api::SyncCredentials& credentials,
          bool delete_sync_data_folder,
          const std::string& restored_key_for_bootstrapping,
          bool setup_for_test_mode);
      ~DoInitializeOptions();

      GURL service_url;
      sync_api::HttpPostProviderFactory* http_bridge_factory;
      sync_api::SyncCredentials credentials;
      std::string lsid;
      bool delete_sync_data_folder;
      std::string restored_key_for_bootstrapping;
      bool setup_for_test_mode;
    };

    
    void CreateSyncNotifier(const scoped_refptr<net::URLRequestContextGetter>&
        request_context_getter);

    
    
    
    
    
    
    
    
    void DoInitialize(const DoInitializeOptions& options);

    
    
    void DoUpdateCredentials(const sync_api::SyncCredentials& credentials);

    
    void DoUpdateEnabledTypes();

    
    
    void DoStartSyncing();

    
    
    void DoRequestNudge(const tracked_objects::Location& location);
    void DoRequestClearServerData();

    
    void DeferNudgeForCleanup();

    
    
    void DoSetPassphrase(const std::string& passphrase, bool is_explicit);

    
    
    
    bool processing_passphrase() const;
    void set_processing_passphrase();

    
    
    void DoEncryptDataTypes(const syncable::ModelTypeSet& encrypted_types);

    
    
    
    
    
    
    
    
    
    void DoShutdown(bool stopping_sync);

    
    virtual void DoRequestConfig(const syncable::ModelTypeBitSet& added_types);

    
    virtual void DoStartConfiguration(Callback0::Type* callback);

    
    
    

    sync_api::SyncManager* syncapi() { return syncapi_.get(); }

    
    
    
    void DeleteSyncDataFolder();

    void ConnectChildJsEventRouter();

    void DisconnectChildJsEventRouter();

    void DoProcessMessage(
        const std::string& name, const JsArgList& args,
        const JsEventHandler* sender);

    
    void FinishConfigureDataTypes();

#if defined(UNIT_TEST)
    
    
    
    void DoInitializeForTest(const std::wstring& test_user,
                             sync_api::HttpPostProviderFactory* factory,
                             bool delete_sync_data_folder) {
      
      sync_api::SyncCredentials credentials;
      credentials.email = WideToUTF8(test_user);
      credentials.sync_token = "token";
      DoInitialize(DoInitializeOptions(GURL(), factory, credentials,
                                       delete_sync_data_folder,
                                       "", true));
    }
#endif

   private:
    friend class base::RefCountedThreadSafe<SyncBackendHost::Core>;
    friend class SyncBackendHostForProfileSyncTest;

    virtual ~Core();

    
    ChangeProcessor* GetProcessor(syncable::ModelType modeltype);

    
    
    
    
    void StartSavingChanges();

    
    
    
    
    
    
    void SaveChanges();

    
    
    void HandleAuthErrorEventOnFrontendLoop(
        const GoogleServiceAuthError& new_auth_error);

    
    
    
    void NotifyPassphraseRequired(bool for_decryption);

    
    
    
    void NotifyPassphraseFailed();

    
    void NotifyPassphraseAccepted(const std::string& bootstrap_token);

    
    void NotifyUpdatedToken(const std::string& token);

    
    
    void NotifyEncryptionComplete(const syncable::ModelTypeSet&
                                      encrypted_types);

    
    
    void HandleSyncCycleCompletedOnFrontendLoop(
        sessions::SyncSessionSnapshot* snapshot);

    void HandleStopSyncingPermanentlyOnFrontendLoop();

    
    void HandleClearServerDataSucceededOnFrontendLoop();
    void HandleClearServerDataFailedOnFrontendLoop();

    
    
    void HandleInitalizationCompletedOnFrontendLoop();

    void RouteJsEventOnFrontendLoop(
        const std::string& name, const JsArgList& args,
        const JsEventHandler* dst);

    void FinishConfigureDataTypesOnFrontendLoop();

    
    bool IsCurrentThreadSafeForModel(syncable::ModelType model_type);

    
    SyncBackendHost* host_;

    
    base::RepeatingTimer<Core> save_changes_timer_;

    
    scoped_ptr<sync_api::SyncManager> syncapi_;

    scoped_ptr<sync_notifier::SyncNotifier> sync_notifier_;

    JsSyncManagerObserver sync_manager_observer_;

    JsEventRouter* parent_router_;

    
    
    
    
    bool processing_passphrase_;

    
    
    bool deferred_nudge_for_cleanup_requested_;

    DISALLOW_COPY_AND_ASSIGN(Core);
  };

  
  
  
  virtual void HandleInitializationCompletedOnFrontendLoop();

  
  virtual void RequestNudge(const tracked_objects::Location& location);

  
  
  void FinishConfigureDataTypes();
  void FinishConfigureDataTypesOnFrontendLoop();

  
  virtual void InitCore(const Core::DoInitializeOptions& options);

  
  virtual sync_api::HttpPostProviderFactory* MakeHttpBridgeFactory(
      net::URLRequestContextGetter* getter);

  MessageLoop* core_loop() { return core_thread_.message_loop(); }

  void set_syncapi_initialized() { syncapi_initialized_ = true; }

  
  
  
  
  void PersistEncryptionBootstrapToken(const std::string& token);
  std::string RestoreEncryptionBootstrapToken();

  
  scoped_refptr<Core> core_;

 private:
  FRIEND_TEST_ALL_PREFIXES(SyncBackendHostTest, MakePendingConfigModeState);

  struct PendingConfigureDataTypesState {
    PendingConfigureDataTypesState();
    ~PendingConfigureDataTypesState();

    
    
    scoped_ptr<CancelableTask> ready_task;

    
    
    syncable::ModelTypeSet initial_types;

    
    bool deleted_type;
    syncable::ModelTypeBitSet added_types;
  };

  UIModelWorker* ui_worker();

  void ConfigureAutofillMigration();

  
  
  static PendingConfigureDataTypesState* MakePendingConfigModeState(
      const DataTypeController::TypeMap& data_type_controllers,
      const syncable::ModelTypeSet& types,
      CancelableTask* ready_task,
      ModelSafeRoutingInfo* routing_info);

  
  
  
  base::Thread core_thread_;

  
  
  MessageLoop* const frontend_loop_;

  Profile* profile_;

  
  struct {
    
    
    
    
    
    
    
    
    
    WorkerMap workers;
    browser_sync::ModelSafeRoutingInfo routing_info;
  } registrar_;

  
  
  
  
  
  
  
  
  
  mutable base::Lock registrar_lock_;

  
  SyncFrontend* frontend_;

  
  std::map<syncable::ModelType, ChangeProcessor*> processors_;

  
  FilePath sync_data_folder_path_;

  scoped_ptr<PendingConfigureDataTypesState> pending_download_state_;
  scoped_ptr<PendingConfigureDataTypesState> pending_config_mode_state_;

  
  GoogleServiceAuthError last_auth_error_;

  
  scoped_ptr<sessions::SyncSessionSnapshot> last_snapshot_;

  
  bool syncapi_initialized_;

  DISALLOW_COPY_AND_ASSIGN(SyncBackendHost);
};

}  

#endif  
