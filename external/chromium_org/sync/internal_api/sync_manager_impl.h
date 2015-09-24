// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_SYNC_MANAGER_H_
#define SYNC_INTERNAL_API_SYNC_MANAGER_H_

#include <string>
#include <vector>

#include "net/base/network_change_notifier.h"
#include "sync/base/sync_export.h"
#include "sync/engine/all_status.h"
#include "sync/engine/net/server_connection_manager.h"
#include "sync/engine/sync_engine_event.h"
#include "sync/engine/traffic_recorder.h"
#include "sync/internal_api/change_reorder_buffer.h"
#include "sync/internal_api/debug_info_event_listener.h"
#include "sync/internal_api/js_mutation_event_observer.h"
#include "sync/internal_api/js_sync_encryption_handler_observer.h"
#include "sync/internal_api/js_sync_manager_observer.h"
#include "sync/internal_api/public/sync_manager.h"
#include "sync/internal_api/public/user_share.h"
#include "sync/internal_api/sync_encryption_handler_impl.h"
#include "sync/js/js_backend.h"
#include "sync/notifier/invalidation_handler.h"
#include "sync/notifier/invalidator_state.h"
#include "sync/syncable/directory_change_delegate.h"
#include "sync/util/cryptographer.h"
#include "sync/util/time.h"

namespace syncer {

class SyncAPIServerConnectionManager;
class WriteNode;
class WriteTransaction;

namespace sessions {
class SyncSessionContext;
}

class SYNC_EXPORT_PRIVATE SyncManagerImpl :
    public SyncManager,
    public net::NetworkChangeNotifier::IPAddressObserver,
    public net::NetworkChangeNotifier::ConnectionTypeObserver,
    public JsBackend,
    public SyncEngineEventListener,
    public ServerConnectionEventListener,
    public syncable::DirectoryChangeDelegate,
    public SyncEncryptionHandler::Observer {
 public:
  
  explicit SyncManagerImpl(const std::string& name);
  virtual ~SyncManagerImpl();

  
  virtual void Init(
      const base::FilePath& database_location,
      const WeakHandle<JsEventHandler>& event_handler,
      const std::string& sync_server_and_path,
      int sync_server_port,
      bool use_ssl,
      scoped_ptr<HttpPostProviderFactory> post_factory,
      const std::vector<ModelSafeWorker*>& workers,
      ExtensionsActivity* extensions_activity,
      SyncManager::ChangeDelegate* change_delegate,
      const SyncCredentials& credentials,
      const std::string& invalidator_client_id,
      const std::string& restored_key_for_bootstrapping,
      const std::string& restored_keystore_key_for_bootstrapping,
      InternalComponentsFactory* internal_components_factory,
      Encryptor* encryptor,
      scoped_ptr<UnrecoverableErrorHandler> unrecoverable_error_handler,
      ReportUnrecoverableErrorFunction
          report_unrecoverable_error_function,
      CancelationSignal* cancelation_signal) OVERRIDE;
  virtual void ThrowUnrecoverableError() OVERRIDE;
  virtual ModelTypeSet InitialSyncEndedTypes() OVERRIDE;
  virtual ModelTypeSet GetTypesWithEmptyProgressMarkerToken(
      ModelTypeSet types) OVERRIDE;
  virtual bool PurgePartiallySyncedTypes() OVERRIDE;
  virtual void UpdateCredentials(const SyncCredentials& credentials) OVERRIDE;
  virtual void StartSyncingNormally(
      const ModelSafeRoutingInfo& routing_info) OVERRIDE;
  virtual void ConfigureSyncer(
      ConfigureReason reason,
      ModelTypeSet to_download,
      ModelTypeSet to_purge,
      ModelTypeSet to_journal,
      ModelTypeSet to_unapply,
      const ModelSafeRoutingInfo& new_routing_info,
      const base::Closure& ready_task,
      const base::Closure& retry_task) OVERRIDE;
  virtual void OnInvalidatorStateChange(InvalidatorState state) OVERRIDE;
  virtual void OnIncomingInvalidation(
      const ObjectIdInvalidationMap& invalidation_map) OVERRIDE;
  virtual void AddObserver(SyncManager::Observer* observer) OVERRIDE;
  virtual void RemoveObserver(SyncManager::Observer* observer) OVERRIDE;
  virtual SyncStatus GetDetailedStatus() const OVERRIDE;
  virtual void SaveChanges() OVERRIDE;
  virtual void ShutdownOnSyncThread() OVERRIDE;
  virtual UserShare* GetUserShare() OVERRIDE;
  virtual const std::string cache_guid() OVERRIDE;
  virtual bool ReceivedExperiment(Experiments* experiments) OVERRIDE;
  virtual bool HasUnsyncedItems() OVERRIDE;
  virtual SyncEncryptionHandler* GetEncryptionHandler() OVERRIDE;

  
  virtual void OnPassphraseRequired(
      PassphraseRequiredReason reason,
      const sync_pb::EncryptedData& pending_keys) OVERRIDE;
  virtual void OnPassphraseAccepted() OVERRIDE;
  virtual void OnBootstrapTokenUpdated(
      const std::string& bootstrap_token,
      BootstrapTokenType type) OVERRIDE;
  virtual void OnEncryptedTypesChanged(
      ModelTypeSet encrypted_types,
      bool encrypt_everything) OVERRIDE;
  virtual void OnEncryptionComplete() OVERRIDE;
  virtual void OnCryptographerStateChanged(
      Cryptographer* cryptographer) OVERRIDE;
  virtual void OnPassphraseTypeChanged(
      PassphraseType type,
      base::Time explicit_passphrase_time) OVERRIDE;

  static int GetDefaultNudgeDelay();
  static int GetPreferencesNudgeDelay();

  
  virtual void OnSyncEngineEvent(const SyncEngineEvent& event) OVERRIDE;

  
  virtual void OnServerConnectionEvent(
      const ServerConnectionEvent& event) OVERRIDE;

  
  virtual void SetJsEventHandler(
      const WeakHandle<JsEventHandler>& event_handler) OVERRIDE;
  virtual void ProcessJsMessage(
      const std::string& name, const JsArgList& args,
      const WeakHandle<JsReplyHandler>& reply_handler) OVERRIDE;

  
  
  
  
  virtual void HandleTransactionCompleteChangeEvent(
      ModelTypeSet models_with_changes) OVERRIDE;
  virtual ModelTypeSet HandleTransactionEndingChangeEvent(
      const syncable::ImmutableWriteTransactionInfo& write_transaction_info,
      syncable::BaseTransaction* trans) OVERRIDE;
  virtual void HandleCalculateChangesChangeEventFromSyncApi(
      const syncable::ImmutableWriteTransactionInfo& write_transaction_info,
      syncable::BaseTransaction* trans,
      std::vector<int64>* entries_changed) OVERRIDE;
  virtual void HandleCalculateChangesChangeEventFromSyncer(
      const syncable::ImmutableWriteTransactionInfo& write_transaction_info,
      syncable::BaseTransaction* trans,
      std::vector<int64>* entries_changed) OVERRIDE;

  
  virtual void RefreshTypes(ModelTypeSet types) OVERRIDE;

  
  
  virtual void OnIPAddressChanged() OVERRIDE;
  
  virtual void OnConnectionTypeChanged(
      net::NetworkChangeNotifier::ConnectionType) OVERRIDE;

  const SyncScheduler* scheduler() const;

  bool GetHasInvalidAuthTokenForTest() const;

 protected:
  
  virtual void NotifyInitializationSuccess();
  virtual void NotifyInitializationFailure();

 private:
  friend class SyncManagerTest;
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, NudgeDelayTest);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, OnNotificationStateChange);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, OnIncomingNotification);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, PurgeDisabledTypes);
  FRIEND_TEST_ALL_PREFIXES(SyncManagerTest, PurgeUnappliedTypes);

  struct NotificationInfo {
    NotificationInfo();
    ~NotificationInfo();

    int total_count;
    std::string payload;

    
    base::DictionaryValue* ToValue() const;
  };

  base::TimeDelta GetNudgeDelayTimeDelta(const ModelType& model_type);

  typedef std::map<ModelType, NotificationInfo> NotificationInfoMap;
  typedef JsArgList (SyncManagerImpl::*UnboundJsMessageHandler)(
      const JsArgList&);
  typedef base::Callback<JsArgList(const JsArgList&)> JsMessageHandler;
  typedef std::map<std::string, JsMessageHandler> JsMessageHandlerMap;

  
  
  
  
  
  bool VisiblePositionsDiffer(
      const syncable::EntryKernelMutation& mutation) const;

  
  
  
  bool VisiblePropertiesDiffer(
      const syncable::EntryKernelMutation& mutation,
      Cryptographer* cryptographer) const;

  
  bool OpenDirectory(const std::string& username);

  
  
  
  
  bool PurgeDisabledTypes(ModelTypeSet to_purge,
                          ModelTypeSet to_journal,
                          ModelTypeSet to_unapply);

  void RequestNudgeForDataTypes(
      const tracked_objects::Location& nudge_location,
      ModelTypeSet type);

  
  
  
  void SetExtraChangeRecordData(int64 id,
                                ModelType type,
                                ChangeReorderBuffer* buffer,
                                Cryptographer* cryptographer,
                                const syncable::EntryKernel& original,
                                bool existed_before,
                                bool exists_now);

  
  
  void UpdateNotificationInfo(const ObjectIdInvalidationMap& invalidation_map);

  
  void OnNetworkConnectivityChangedImpl();

  
  void BindJsMessageHandler(
    const std::string& name, UnboundJsMessageHandler unbound_message_handler);

  
  static base::DictionaryValue* NotificationInfoToValue(
      const NotificationInfoMap& notification_info);

  static std::string NotificationInfoToString(
      const NotificationInfoMap& notification_info);

  
  JsArgList GetNotificationState(const JsArgList& args);
  JsArgList GetNotificationInfo(const JsArgList& args);
  JsArgList GetRootNodeDetails(const JsArgList& args);
  JsArgList GetAllNodes(const JsArgList& args);
  JsArgList GetNodeSummariesById(const JsArgList& args);
  JsArgList GetNodeDetailsById(const JsArgList& args);
  JsArgList GetChildNodeIds(const JsArgList& args);
  JsArgList GetClientServerTraffic(const JsArgList& args);

  syncable::Directory* directory();

  base::FilePath database_path_;

  const std::string name_;

  base::ThreadChecker thread_checker_;

  
  
  
  
  
  
  
  
  
  WeakHandle<SyncManagerImpl> weak_handle_this_;

  
  
  UserShare share_;

  
  
  WeakHandle<SyncManager::ChangeObserver> change_observer_;

  ObserverList<SyncManager::Observer> observers_;

  
  
  scoped_ptr<SyncAPIServerConnectionManager> connection_manager_;

  
  
  scoped_ptr<sessions::SyncSessionContext> session_context_;

  
  
  scoped_ptr<SyncScheduler> scheduler_;

  
  
  AllStatus allstatus_;

  
  
  
  
  
  
  typedef std::map<int, ImmutableChangeRecordList> ChangeRecordMap;
  ChangeRecordMap change_records_;

  SyncManager::ChangeDelegate* change_delegate_;

  
  bool initialized_;

  bool observing_network_connectivity_changes_;

  InvalidatorState invalidator_state_;

  
  
  NotificationInfoMap notification_info_map_;

  
  JsMessageHandlerMap js_message_handlers_;
  WeakHandle<JsEventHandler> js_event_handler_;
  JsSyncManagerObserver js_sync_manager_observer_;
  JsMutationEventObserver js_mutation_event_observer_;
  JsSyncEncryptionHandlerObserver js_sync_encryption_handler_observer_;

  
  DebugInfoEventListener debug_info_event_listener_;

  TrafficRecorder traffic_recorder_;

  Encryptor* encryptor_;
  scoped_ptr<UnrecoverableErrorHandler> unrecoverable_error_handler_;
  ReportUnrecoverableErrorFunction report_unrecoverable_error_function_;

  
  
  
  scoped_ptr<SyncEncryptionHandlerImpl> sync_encryption_handler_;

  base::WeakPtrFactory<SyncManagerImpl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(SyncManagerImpl);
};

}  

#endif  
