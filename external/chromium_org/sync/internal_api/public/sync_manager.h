// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_SYNC_MANAGER_H_
#define SYNC_INTERNAL_API_PUBLIC_SYNC_MANAGER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/task_runner.h"
#include "base/threading/thread_checker.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/change_record.h"
#include "sync/internal_api/public/configure_reason.h"
#include "sync/internal_api/public/engine/model_safe_worker.h"
#include "sync/internal_api/public/engine/sync_status.h"
#include "sync/internal_api/public/sync_encryption_handler.h"
#include "sync/internal_api/public/util/report_unrecoverable_error_function.h"
#include "sync/internal_api/public/util/unrecoverable_error_handler.h"
#include "sync/internal_api/public/util/weak_handle.h"
#include "sync/notifier/invalidation_handler.h"
#include "sync/protocol/sync_protocol_error.h"

namespace sync_pb {
class EncryptedData;
}  

namespace syncer {

class BaseTransaction;
class DataTypeDebugInfoListener;
class Encryptor;
struct Experiments;
class ExtensionsActivity;
class HttpPostProviderFactory;
class InternalComponentsFactory;
class JsBackend;
class JsEventHandler;
class SyncEncryptionHandler;
class SyncScheduler;
struct UserShare;
class CancelationSignal;

namespace sessions {
class SyncSessionSnapshot;
}  

enum ConnectionStatus {
  CONNECTION_NOT_ATTEMPTED,
  CONNECTION_OK,
  CONNECTION_AUTH_ERROR,
  CONNECTION_SERVER_ERROR
};

struct SyncCredentials {
  
  std::string email;
  
  std::string sync_token;
};

class SYNC_EXPORT SyncManager : public syncer::InvalidationHandler {
 public:
  
  
  
  class SYNC_EXPORT ChangeDelegate {
   public:
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual void OnChangesApplied(
        ModelType model_type,
        int64 model_version,
        const BaseTransaction* trans,
        const ImmutableChangeRecordList& changes) = 0;

    
    
    
    
    
    
    
    
    
    
    
    virtual void OnChangesComplete(ModelType model_type) = 0;

   protected:
    virtual ~ChangeDelegate();
  };

  
  
  
  class SYNC_EXPORT_PRIVATE ChangeObserver {
   public:
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual void OnChangesApplied(
        ModelType model_type,
        int64 write_transaction_id,
        const ImmutableChangeRecordList& changes) = 0;

    virtual void OnChangesComplete(ModelType model_type) = 0;

   protected:
    virtual ~ChangeObserver();
  };

  
  
  
  
  class SYNC_EXPORT Observer {
   public:
    
    
    virtual void OnSyncCycleCompleted(
        const sessions::SyncSessionSnapshot& snapshot) = 0;

    
    
    virtual void OnConnectionStatusChange(ConnectionStatus status) = 0;

    
    
    
    
    
    
    
    
    
    

    

    

    
    

    

    
    
    

    

    

    

    virtual void OnInitializationComplete(
        const WeakHandle<JsBackend>& js_backend,
        const WeakHandle<DataTypeDebugInfoListener>& debug_info_listener,
        bool success,
        ModelTypeSet restored_types) = 0;

    
    
    
    
    virtual void OnStopSyncingPermanently() = 0;

    virtual void OnActionableError(
        const SyncProtocolError& sync_protocol_error) = 0;

   protected:
    virtual ~Observer();
  };

  SyncManager();
  virtual ~SyncManager();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Init(
      const base::FilePath& database_location,
      const WeakHandle<JsEventHandler>& event_handler,
      const std::string& sync_server_and_path,
      int sync_server_port,
      bool use_ssl,
      scoped_ptr<HttpPostProviderFactory> post_factory,
      const std::vector<ModelSafeWorker*>& workers,
      ExtensionsActivity* extensions_activity,
      ChangeDelegate* change_delegate,
      const SyncCredentials& credentials,
      const std::string& invalidator_client_id,
      const std::string& restored_key_for_bootstrapping,
      const std::string& restored_keystore_key_for_bootstrapping,
      InternalComponentsFactory* internal_components_factory,
      Encryptor* encryptor,
      scoped_ptr<UnrecoverableErrorHandler> unrecoverable_error_handler,
      ReportUnrecoverableErrorFunction report_unrecoverable_error_function,
      CancelationSignal* cancelation_signal) = 0;

  
  
  virtual void ThrowUnrecoverableError() = 0;

  virtual ModelTypeSet InitialSyncEndedTypes() = 0;

  
  
  virtual ModelTypeSet GetTypesWithEmptyProgressMarkerToken(
      ModelTypeSet types) = 0;

  
  
  
  virtual bool PurgePartiallySyncedTypes() = 0;

  
  virtual void UpdateCredentials(const SyncCredentials& credentials) = 0;

  
  virtual void StartSyncingNormally(
      const ModelSafeRoutingInfo& routing_info) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void ConfigureSyncer(
      ConfigureReason reason,
      ModelTypeSet to_download,
      ModelTypeSet to_purge,
      ModelTypeSet to_journal,
      ModelTypeSet to_unapply,
      const ModelSafeRoutingInfo& new_routing_info,
      const base::Closure& ready_task,
      const base::Closure& retry_task) = 0;

  
  virtual void OnInvalidatorStateChange(InvalidatorState state) = 0;

  
  virtual void OnIncomingInvalidation(
      const ObjectIdInvalidationMap& invalidation_map) = 0;

  
  
  
  virtual void AddObserver(Observer* observer) = 0;

  
  
  
  virtual void RemoveObserver(Observer* observer) = 0;

  
  virtual SyncStatus GetDetailedStatus() const = 0;

  
  
  virtual void SaveChanges() = 0;

  
  virtual void ShutdownOnSyncThread() = 0;

  
  virtual UserShare* GetUserShare() = 0;

  
  
  virtual const std::string cache_guid() = 0;

  
  
  
  virtual bool ReceivedExperiment(Experiments* experiments) = 0;

  
  
  virtual bool HasUnsyncedItems() = 0;

  
  virtual SyncEncryptionHandler* GetEncryptionHandler() = 0;

  
  virtual void RefreshTypes(ModelTypeSet types) = 0;
};

}  

#endif  
