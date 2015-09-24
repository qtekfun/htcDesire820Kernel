// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SYNC_BACKEND_HOST_H_
#define CHROME_BROWSER_SYNC_GLUE_SYNC_BACKEND_HOST_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/thread.h"
#include "chrome/browser/sync/glue/backend_data_type_configurer.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/configure_reason.h"
#include "sync/internal_api/public/sessions/sync_session_snapshot.h"
#include "sync/internal_api/public/sync_manager.h"
#include "sync/internal_api/public/sync_manager_factory.h"
#include "sync/internal_api/public/util/report_unrecoverable_error_function.h"
#include "sync/internal_api/public/util/unrecoverable_error_handler.h"
#include "sync/internal_api/public/util/weak_handle.h"

class GURL;

namespace base {
class MessageLoop;
}

namespace syncer {
class NetworkResources;
class SyncManagerFactory;
}

namespace browser_sync {

class ChangeProcessor;
class SyncFrontend;
class SyncedDeviceTracker;

class SyncBackendHost : public BackendDataTypeConfigurer {
 public:
  typedef syncer::SyncStatus Status;

  
  SyncBackendHost();
  virtual ~SyncBackendHost();

  
  
  
  
  
  
  virtual void Initialize(
      SyncFrontend* frontend,
      scoped_ptr<base::Thread> sync_thread,
      const syncer::WeakHandle<syncer::JsEventHandler>& event_handler,
      const GURL& service_url,
      const syncer::SyncCredentials& credentials,
      bool delete_sync_data_folder,
      scoped_ptr<syncer::SyncManagerFactory> sync_manager_factory,
      scoped_ptr<syncer::UnrecoverableErrorHandler> unrecoverable_error_handler,
      syncer::ReportUnrecoverableErrorFunction
          report_unrecoverable_error_function,
      syncer::NetworkResources* network_resources) = 0;

  
  virtual void UpdateCredentials(
      const syncer::SyncCredentials& credentials) = 0;

  
  
  
  
  virtual void StartSyncingWithServer() = 0;

  
  
  
  
  
  
  
  
  
  
  
  virtual void SetEncryptionPassphrase(
      const std::string& passphrase,
      bool is_explicit) = 0;

  
  
  
  
  
  
  virtual bool SetDecryptionPassphrase(const std::string& passphrase)
      WARN_UNUSED_RESULT = 0;

  
  
  
  virtual void StopSyncingForShutdown() = 0;

  
  
  
  
  
  
  
  
  enum ShutdownOption {
    STOP,                      
    STOP_AND_CLAIM_THREAD,     
    DISABLE_AND_CLAIM_THREAD,  
  };
  virtual scoped_ptr<base::Thread> Shutdown(ShutdownOption option) = 0;

  
  
  virtual void UnregisterInvalidationIds() = 0;

  
  
  
  
  virtual void ConfigureDataTypes(
      syncer::ConfigureReason reason,
      const DataTypeConfigStateMap& config_state_map,
      const base::Callback<void(syncer::ModelTypeSet,
                                syncer::ModelTypeSet)>& ready_task,
      const base::Callback<void()>& retry_callback) OVERRIDE = 0;

  
  virtual void EnableEncryptEverything() = 0;

  
  
  
  
  virtual void ActivateDataType(
      syncer::ModelType type, syncer::ModelSafeGroup group,
      ChangeProcessor* change_processor) = 0;

  
  virtual void DeactivateDataType(syncer::ModelType type) = 0;

  
  
  
  virtual syncer::UserShare* GetUserShare() const = 0;

  
  
  virtual Status GetDetailedStatus() = 0;
  virtual syncer::sessions::SyncSessionSnapshot
      GetLastSessionSnapshot() const = 0;

  
  
  
  virtual bool HasUnsyncedItems() const = 0;

  
  virtual bool IsNigoriEnabled() const = 0;

  
  
  virtual syncer::PassphraseType GetPassphraseType() const = 0;

  
  
  virtual base::Time GetExplicitPassphraseTime() const = 0;

  
  
  
  virtual bool IsCryptographerReady(
      const syncer::BaseTransaction* trans) const = 0;

  virtual void GetModelSafeRoutingInfo(
      syncer::ModelSafeRoutingInfo* out) const = 0;

  
  virtual SyncedDeviceTracker* GetSyncedDeviceTracker() const = 0;

  virtual base::MessageLoop* GetSyncLoopForTesting() = 0;

  DISALLOW_COPY_AND_ASSIGN(SyncBackendHost);
};

}  

#endif  
