// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_INTERNAL_COMPONENTS_FACTORY_H_
#define SYNC_INTERNAL_API_PUBLIC_INTERNAL_COMPONENTS_FACTORY_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/engine/model_safe_worker.h"

namespace syncer {

class ExtensionsActivity;
class ServerConnectionManager;
class SyncEngineEventListener;
class CancelationSignal;
class SyncScheduler;
class TrafficRecorder;

namespace sessions {
class DebugInfoGetter;
class SyncSessionContext;
}

namespace syncable {
class Directory;
class DirectoryBackingStore;
}

class SYNC_EXPORT InternalComponentsFactory {
 public:
  enum EncryptionMethod {
    ENCRYPTION_LEGACY,
    
    ENCRYPTION_KEYSTORE
  };

  enum BackoffOverride {
    BACKOFF_NORMAL,
    
    
    
    BACKOFF_SHORT_INITIAL_RETRY_OVERRIDE
  };

  enum PreCommitUpdatesPolicy {
    
    
    SERVER_CONTROLLED_PRE_COMMIT_UPDATE_AVOIANCE,

    
    
    FORCE_ENABLE_PRE_COMMIT_UPDATE_AVOIDANCE,
  };

  
  
  
  
  
  struct Switches {
    EncryptionMethod encryption_method;
    BackoffOverride backoff_override;
    PreCommitUpdatesPolicy pre_commit_updates_policy;
  };

  virtual ~InternalComponentsFactory() {}

  virtual scoped_ptr<SyncScheduler> BuildScheduler(
      const std::string& name,
      sessions::SyncSessionContext* context,
      CancelationSignal* cancelation_signal) = 0;

  virtual scoped_ptr<sessions::SyncSessionContext> BuildContext(
      ServerConnectionManager* connection_manager,
      syncable::Directory* directory,
      const std::vector<ModelSafeWorker*>& workers,
      ExtensionsActivity* extensions_activity,
      const std::vector<SyncEngineEventListener*>& listeners,
      sessions::DebugInfoGetter* debug_info_getter,
      TrafficRecorder* traffic_recorder,
      const std::string& invalidator_client_id) = 0;

  virtual scoped_ptr<syncable::DirectoryBackingStore>
  BuildDirectoryBackingStore(
      const std::string& dir_name,
      const base::FilePath& backing_filepath) = 0;

  
  
  virtual Switches GetSwitches() const = 0;
};

}  

#endif  
