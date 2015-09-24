// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef SYNC_SESSIONS_SYNC_SESSION_H_
#define SYNC_SESSIONS_SYNC_SESSION_H_

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/engine/model_safe_worker.h"
#include "sync/internal_api/public/sessions/sync_session_snapshot.h"
#include "sync/sessions/status_controller.h"
#include "sync/sessions/sync_session_context.h"

namespace syncer {
class ModelSafeWorker;

namespace sessions {

class NudgeTracker;

class SYNC_EXPORT_PRIVATE SyncSession {
 public:
  
  
  
  class SYNC_EXPORT_PRIVATE Delegate {
   public:
    
    
    virtual void OnThrottled(const base::TimeDelta& throttle_duration) = 0;

    
    virtual void OnTypesThrottled(
        ModelTypeSet types,
        const base::TimeDelta& throttle_duration) = 0;

    
    
    
    
    
    
    
    
    
    virtual bool IsCurrentlyThrottled() = 0;

    
    virtual void OnReceivedShortPollIntervalUpdate(
        const base::TimeDelta& new_interval) = 0;

    
    virtual void OnReceivedLongPollIntervalUpdate(
        const base::TimeDelta& new_interval) = 0;

    
    
    virtual void OnReceivedSessionsCommitDelay(
        const base::TimeDelta& new_delay) = 0;

    
    virtual void OnSyncProtocolError(
        const sessions::SyncSessionSnapshot& snapshot) = 0;

    
    
    virtual void OnReceivedClientInvalidationHintBufferSize(int size) = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  static SyncSession* Build(SyncSessionContext* context,
                            Delegate* delegate);
  ~SyncSession();

  
  SyncSessionSnapshot TakeSnapshot() const;
  SyncSessionSnapshot TakeSnapshotWithSource(
      sync_pb::GetUpdatesCallerInfo::GetUpdatesSource legacy_updates_source)
      const;

  
  void SendSyncCycleEndEventNotification(
      sync_pb::GetUpdatesCallerInfo::GetUpdatesSource source);
  void SendEventNotification(SyncEngineEvent::EventCause cause);

  
  SyncSessionContext* context() const { return context_; }
  Delegate* delegate() const { return delegate_; }
  const StatusController& status_controller() const {
    return *status_controller_.get();
  }
  StatusController* mutable_status_controller() {
    return status_controller_.get();
  }

 private:
  SyncSession(SyncSessionContext* context, Delegate* delegate);

  
  SyncSessionContext* const context_;

  
  Delegate* const delegate_;

  
  scoped_ptr<StatusController> status_controller_;

  DISALLOW_COPY_AND_ASSIGN(SyncSession);
};

}  
}  

#endif  
