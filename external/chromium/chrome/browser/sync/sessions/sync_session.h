// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_SYNC_SESSIONS_SYNC_SESSION_H_
#define CHROME_BROWSER_SYNC_SESSIONS_SYNC_SESSION_H_
#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/time.h"
#include "chrome/browser/sync/engine/model_safe_worker.h"
#include "chrome/browser/sync/sessions/ordered_commit_set.h"
#include "chrome/browser/sync/sessions/session_state.h"
#include "chrome/browser/sync/sessions/status_controller.h"
#include "chrome/browser/sync/sessions/sync_session_context.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/browser/sync/util/extensions_activity_monitor.h"

namespace syncable {
class WriteTransaction;
}

namespace browser_sync {
class ModelSafeWorker;

namespace sessions {

class SyncSession {
 public:
  
  
  
  class Delegate {
   public:
    
    
    virtual void OnSilencedUntil(const base::TimeTicks& silenced_until) = 0;

    
    
    
    
    
    
    
    
    
    virtual bool IsSyncingCurrentlySilenced() = 0;

    
    virtual void OnReceivedShortPollIntervalUpdate(
        const base::TimeDelta& new_interval) = 0;

    
    virtual void OnReceivedLongPollIntervalUpdate(
        const base::TimeDelta& new_interval) = 0;

    
    
    
    
    virtual void OnShouldStopSyncingPermanently() = 0;

   protected:
    virtual ~Delegate() {}
  };

  SyncSession(SyncSessionContext* context,
              Delegate* delegate,
              const SyncSourceInfo& source,
              const ModelSafeRoutingInfo& routing_info,
              const std::vector<ModelSafeWorker*>& workers);
  ~SyncSession();

  
  SyncSessionSnapshot TakeSnapshot() const;

  
  
  bool HasMoreToSync() const;

  
  
  
  
  
  void Coalesce(const SyncSession& session);

  
  
  void ResetTransientState();

  SyncSessionContext* context() const { return context_; }
  Delegate* delegate() const { return delegate_; }
  syncable::WriteTransaction* write_transaction() { return write_transaction_; }
  StatusController* status_controller() { return status_controller_.get(); }

  const ExtensionsActivityMonitor::Records& extensions_activity() const {
    return extensions_activity_;
  }
  ExtensionsActivityMonitor::Records* mutable_extensions_activity() {
    return &extensions_activity_;
  }

  
  
  
  SyncSourceInfo TestAndSetSource();

  const std::vector<ModelSafeWorker*>& workers() const { return workers_; }
  const ModelSafeRoutingInfo& routing_info() const { return routing_info_; }
  const SyncSourceInfo& source() const { return source_; }

 private:
  
  
  
  friend class ScopedSetSessionWriteTransaction;

  
  SyncSessionContext* const context_;

  
  SyncSourceInfo source_;

  
  ExtensionsActivityMonitor::Records extensions_activity_;

  
  syncable::WriteTransaction* write_transaction_;

  
  Delegate* delegate_;

  
  scoped_ptr<StatusController> status_controller_;

  
  
  std::vector<ModelSafeWorker*> workers_;

  
  
  
  ModelSafeRoutingInfo routing_info_;

  DISALLOW_COPY_AND_ASSIGN(SyncSession);
};

class ScopedSetSessionWriteTransaction {
 public:
  ScopedSetSessionWriteTransaction(SyncSession* session,
                                   syncable::WriteTransaction* trans)
      : session_(session) {
    DCHECK(!session_->write_transaction_);
    session_->write_transaction_ = trans;
  }
  ~ScopedSetSessionWriteTransaction() { session_->write_transaction_ = NULL; }

 private:
  SyncSession* session_;
  DISALLOW_COPY_AND_ASSIGN(ScopedSetSessionWriteTransaction);
};

}  
}  

#endif  
