// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef SYNC_SESSIONS_SYNC_SESSION_CONTEXT_H_
#define SYNC_SESSIONS_SYNC_SESSION_CONTEXT_H_

#include <map>
#include <string>
#include <vector>

#include "base/stl_util.h"
#include "sync/base/sync_export.h"
#include "sync/engine/sync_directory_commit_contributor.h"
#include "sync/engine/sync_directory_update_handler.h"
#include "sync/engine/sync_engine_event.h"
#include "sync/engine/syncer_types.h"
#include "sync/engine/traffic_recorder.h"
#include "sync/internal_api/public/engine/model_safe_worker.h"
#include "sync/protocol/sync.pb.h"
#include "sync/sessions/debug_info_getter.h"

namespace syncer {

class ExtensionsActivity;
class ServerConnectionManager;

namespace syncable {
class Directory;
}

static const int kDefaultMaxCommitBatchSize = 25;

namespace sessions {
class TestScopedSessionEventListener;

class SYNC_EXPORT_PRIVATE SyncSessionContext {
 public:
  SyncSessionContext(ServerConnectionManager* connection_manager,
                     syncable::Directory* directory,
                     const std::vector<ModelSafeWorker*>& workers,
                     ExtensionsActivity* extensions_activity,
                     const std::vector<SyncEngineEventListener*>& listeners,
                     DebugInfoGetter* debug_info_getter,
                     TrafficRecorder* traffic_recorder,
                     bool keystore_encryption_enabled,
                     bool client_enabled_pre_commit_update_avoidance,
                     const std::string& invalidator_client_id);

  ~SyncSessionContext();

  ServerConnectionManager* connection_manager() {
    return connection_manager_;
  }
  syncable::Directory* directory() {
    return directory_;
  }

  ModelTypeSet enabled_types() const {
    return enabled_types_;
  }

  void set_routing_info(const ModelSafeRoutingInfo& routing_info);

  UpdateHandlerMap* update_handler_map() {
    return &update_handler_map_;
  }

  CommitContributorMap* commit_contributor_map() {
    return &commit_contributor_map_;
  }

  ExtensionsActivity* extensions_activity() {
    return extensions_activity_.get();
  }

  DebugInfoGetter* debug_info_getter() {
    return debug_info_getter_;
  }

  
  void set_notifications_enabled(bool enabled) {
    notifications_enabled_ = enabled;
  }
  bool notifications_enabled() { return notifications_enabled_; }

  
  void set_account_name(const std::string& name) {
    DCHECK(account_name_.empty());
    account_name_ = name;
  }
  const std::string& account_name() const { return account_name_; }

  void set_max_commit_batch_size(int batch_size) {
    max_commit_batch_size_ = batch_size;
  }
  int32 max_commit_batch_size() const { return max_commit_batch_size_; }

  void NotifyListeners(const SyncEngineEvent& event) {
    FOR_EACH_OBSERVER(SyncEngineEventListener, listeners_,
                      OnSyncEngineEvent(event));
  }

  TrafficRecorder* traffic_recorder() {
    return traffic_recorder_;
  }

  bool keystore_encryption_enabled() const {
    return keystore_encryption_enabled_;
  }

  void set_hierarchy_conflict_detected(bool value) {
    client_status_.set_hierarchy_conflict_detected(value);
  }

  const sync_pb::ClientStatus& client_status() const {
    return client_status_;
  }

  const std::string& invalidator_client_id() const {
    return invalidator_client_id_;
  }

  bool ShouldFetchUpdatesBeforeCommit() const {
    return !(server_enabled_pre_commit_update_avoidance_ ||
             client_enabled_pre_commit_update_avoidance_);
  }

  void set_server_enabled_pre_commit_update_avoidance(bool value) {
    server_enabled_pre_commit_update_avoidance_ = value;
  }

 private:
  
  
  
  friend class TestScopedSessionEventListener;

  ObserverList<SyncEngineEventListener> listeners_;

  ServerConnectionManager* const connection_manager_;
  syncable::Directory* const directory_;

  
  
  ModelTypeSet enabled_types_;

  
  
  
  UpdateHandlerMap update_handler_map_;

  
  STLValueDeleter<UpdateHandlerMap> update_handler_deleter_;

  
  
  
  CommitContributorMap commit_contributor_map_;

  
  STLValueDeleter<CommitContributorMap> commit_contributor_deleter_;

  
  std::map<ModelSafeGroup, scoped_refptr<ModelSafeWorker> > workers_;

  
  
  scoped_refptr<ExtensionsActivity> extensions_activity_;

  
  
  bool notifications_enabled_;

  
  std::string account_name_;

  
  int max_commit_batch_size_;

  
  
  DebugInfoGetter* const debug_info_getter_;

  TrafficRecorder* traffic_recorder_;

  
  sync_pb::ClientStatus client_status_;

  
  
  
  bool keystore_encryption_enabled_;

  
  
  
  
  const std::string invalidator_client_id_;

  
  
  
  bool server_enabled_pre_commit_update_avoidance_;

  
  
  const bool client_enabled_pre_commit_update_avoidance_;

  DISALLOW_COPY_AND_ASSIGN(SyncSessionContext);
};

}  
}  

#endif  
