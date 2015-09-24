// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_SYNCER_TYPES_H_
#define CHROME_BROWSER_SYNC_ENGINE_SYNCER_TYPES_H_
#pragma once

#include <map>
#include <vector>

#include "base/observer_list.h"
#include "chrome/browser/sync/syncable/model_type.h"

namespace syncable {
class BaseTransaction;
class Id;
}

namespace browser_sync {

namespace sessions {
struct SyncSessionSnapshot;
}
class Syncer;

enum UpdateAttemptResponse {
  
  SUCCESS,

  
  
  CONFLICT,
};

enum ServerUpdateProcessingResult {
  
  
  SUCCESS_PROCESSED,

  
  SUCCESS_STORED,

  
  
  FAILED_INCONSISTENT,

  
  FAILED_CORRUPT,

  
  
  SUCCESS_VALID = SUCCESS_STORED
};

enum VerifyResult {
  VERIFY_FAIL,
  VERIFY_SUCCESS,
  VERIFY_UNDELETE,
  VERIFY_SKIP,
  VERIFY_UNDECIDED
};

enum VerifyCommitResult {
  VERIFY_UNSYNCABLE,
  VERIFY_OK,
};

struct SyncEngineEvent {
  enum EventCause {
    
    
    STATUS_CHANGED,

    
    
    
    SYNC_CYCLE_ENDED,

    
    

    
    UPDATED_TOKEN,

    
    
    
    
    STOP_SYNCING_PERMANENTLY,

    
    
    CLEAR_SERVER_DATA_SUCCEEDED,
    CLEAR_SERVER_DATA_FAILED,
  };

  explicit SyncEngineEvent(EventCause cause);
  ~SyncEngineEvent();

  EventCause what_happened;

  
  const sessions::SyncSessionSnapshot* snapshot;

  
  std::string updated_token;
};

class SyncEngineEventListener {
 public:
  
  
  virtual void OnSyncEngineEvent(const SyncEngineEvent& event) = 0;
 protected:
  virtual ~SyncEngineEventListener() {}
};

typedef std::vector<syncable::Id> ConflictSet;

typedef std::map<syncable::Id, ConflictSet*> IdToConflictSetMap;

}  

#endif  
