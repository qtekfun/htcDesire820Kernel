// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_SYNC_ENGINE_EVENT_H_
#define SYNC_ENGINE_SYNC_ENGINE_EVENT_H_

#include <string>

#include "base/observer_list.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/sessions/sync_session_snapshot.h"

namespace syncable {
class Id;
}

namespace syncer {

struct SYNC_EXPORT_PRIVATE SyncEngineEvent {
  enum EventCause {
    
    
    SYNC_CYCLE_BEGIN,

    
    STATUS_CHANGED,

    
    SYNC_CYCLE_ENDED,

    
    

    
    
    
    
    STOP_SYNCING_PERMANENTLY,

    
    
    ACTIONABLE_ERROR,

    
    
    
    RETRY_TIME_CHANGED,

    
    THROTTLED_TYPES_CHANGED,
  };

  explicit SyncEngineEvent(EventCause cause);
  ~SyncEngineEvent();

  EventCause what_happened;

  
  sessions::SyncSessionSnapshot snapshot;

  
  std::string updated_token;

  
  base::Time retry_time;

  
  ModelTypeSet throttled_types;
};

class SYNC_EXPORT_PRIVATE SyncEngineEventListener {
 public:
  
  
  virtual void OnSyncEngineEvent(const SyncEngineEvent& event) = 0;
 protected:
  virtual ~SyncEngineEventListener() {}
};

}  

#endif  
