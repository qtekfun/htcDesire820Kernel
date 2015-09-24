// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_SYNC_SESSIONS_SYNC_SESSION_CONTEXT_H_
#define CHROME_BROWSER_SYNC_SESSIONS_SYNC_SESSION_CONTEXT_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/sync/engine/model_safe_worker.h"
#include "chrome/browser/sync/engine/syncer_types.h"

namespace syncable {
class DirectoryManager;
}

namespace browser_sync {

class ConflictResolver;
class ExtensionsActivityMonitor;
class ModelSafeWorkerRegistrar;
class ServerConnectionManager;

static const int kDefaultMaxCommitBatchSize = 25;

namespace sessions {
class ScopedSessionContextConflictResolver;
struct SyncSessionSnapshot;
class TestScopedSessionEventListener;

class SyncSessionContext {
 public:
  SyncSessionContext(ServerConnectionManager* connection_manager,
                     syncable::DirectoryManager* directory_manager,
                     ModelSafeWorkerRegistrar* model_safe_worker_registrar,
                     const std::vector<SyncEngineEventListener*>& listeners);
  ~SyncSessionContext();

  ConflictResolver* resolver() { return resolver_; }
  ServerConnectionManager* connection_manager() {
    return connection_manager_;
  }
  syncable::DirectoryManager* directory_manager() {
    return directory_manager_;
  }
  ModelSafeWorkerRegistrar* registrar() {
    return registrar_;
  }
  ExtensionsActivityMonitor* extensions_monitor() {
    return extensions_activity_monitor_;
  }

  
  void set_notifications_enabled(bool enabled) {
    notifications_enabled_ = enabled;
  }
  bool notifications_enabled() { return notifications_enabled_; }

  
  void set_account_name(const std::string name) {
    DCHECK(account_name_.empty());
    account_name_ = name;
  }
  const std::string& account_name() { return account_name_; }

  void set_max_commit_batch_size(int batch_size) {
    max_commit_batch_size_ = batch_size;
  }
  int32 max_commit_batch_size() const { return max_commit_batch_size_; }

  const ModelSafeRoutingInfo& previous_session_routing_info() const {
    return previous_session_routing_info_;
  }

  void set_previous_session_routing_info(const ModelSafeRoutingInfo& info) {
    previous_session_routing_info_ = info;
  }

  void NotifyListeners(const SyncEngineEvent& event) {
    FOR_EACH_OBSERVER(SyncEngineEventListener, listeners_,
                      OnSyncEngineEvent(event));
  }

 private:
  
  
  
  friend class ScopedSessionContextConflictResolver;
  friend class TestScopedSessionEventListener;

  
  ConflictResolver* resolver_;

  ObserverList<SyncEngineEventListener> listeners_;

  ServerConnectionManager* const connection_manager_;
  syncable::DirectoryManager* const directory_manager_;

  
  
  ModelSafeWorkerRegistrar* registrar_;

  
  
  ExtensionsActivityMonitor* extensions_activity_monitor_;

  
  
  bool notifications_enabled_;

  
  std::string account_name_;

  
  int max_commit_batch_size_;

  
  
  ModelSafeRoutingInfo previous_session_routing_info_;

  
  scoped_ptr<sessions::SyncSessionSnapshot> previous_session_snapshot_;

  DISALLOW_COPY_AND_ASSIGN(SyncSessionContext);
};

class ScopedSessionContextConflictResolver {
 public:
  
  ScopedSessionContextConflictResolver(SyncSessionContext* context,
                                       ConflictResolver* resolver)
      : context_(context), resolver_(resolver) {
    DCHECK(NULL == context->resolver_);
    context->resolver_ = resolver;
  }
  ~ScopedSessionContextConflictResolver() {
    context_->resolver_ = NULL;
   }
 private:
  SyncSessionContext* context_;
  ConflictResolver* resolver_;
  DISALLOW_COPY_AND_ASSIGN(ScopedSessionContextConflictResolver);
};

}  
}  

#endif  
