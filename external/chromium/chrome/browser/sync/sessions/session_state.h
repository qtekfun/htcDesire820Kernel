// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_SYNC_SESSIONS_SESSION_STATE_H_
#define CHROME_BROWSER_SYNC_SESSIONS_SESSION_STATE_H_
#pragma once

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/sync/engine/syncer_types.h"
#include "chrome/browser/sync/engine/syncproto.h"
#include "chrome/browser/sync/sessions/ordered_commit_set.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/browser/sync/syncable/model_type_payload_map.h"
#include "chrome/browser/sync/syncable/syncable.h"

class DictionaryValue;

namespace syncable {
class DirectoryManager;
}

namespace browser_sync {
namespace sessions {

class UpdateProgress;

struct SyncSourceInfo {
  SyncSourceInfo();
  explicit SyncSourceInfo(const syncable::ModelTypePayloadMap& t);
  SyncSourceInfo(
      const sync_pb::GetUpdatesCallerInfo::GetUpdatesSource& u,
      const syncable::ModelTypePayloadMap& t);
  ~SyncSourceInfo();

  
  DictionaryValue* ToValue() const;

  sync_pb::GetUpdatesCallerInfo::GetUpdatesSource updates_source;
  syncable::ModelTypePayloadMap types;
};

struct SyncerStatus {
  SyncerStatus();
  ~SyncerStatus();

  
  DictionaryValue* ToValue() const;

  
  bool invalid_store;
  
  bool syncer_stuck;
  bool syncing;
  int num_successful_commits;
  
  int num_successful_bookmark_commits;

  
  int num_updates_downloaded_total;
  int num_tombstone_updates_downloaded_total;

  
  
  syncable::ModelTypeSet types_needing_local_migration;
};

struct ErrorCounters {
  ErrorCounters();

  
  DictionaryValue* ToValue() const;

  int num_conflicting_commits;

  
  
  int consecutive_transient_error_commits;

  
  
  
  int consecutive_errors;
};

DictionaryValue* DownloadProgressMarkersToValue(
    const std::string
        (&download_progress_markers)[syncable::MODEL_TYPE_COUNT]);

struct SyncSessionSnapshot {
  SyncSessionSnapshot(
      const SyncerStatus& syncer_status,
      const ErrorCounters& errors,
      int64 num_server_changes_remaining,
      bool is_share_usable,
      const syncable::ModelTypeBitSet& initial_sync_ended,
      const std::string
          (&download_progress_markers)[syncable::MODEL_TYPE_COUNT],
      bool more_to_sync,
      bool is_silenced,
      int64 unsynced_count,
      int num_conflicting_updates,
      bool did_commit_items,
      const SyncSourceInfo& source);
  ~SyncSessionSnapshot();

  
  DictionaryValue* ToValue() const;

  const SyncerStatus syncer_status;
  const ErrorCounters errors;
  const int64 num_server_changes_remaining;
  const bool is_share_usable;
  const syncable::ModelTypeBitSet initial_sync_ended;
  const std::string download_progress_markers[syncable::MODEL_TYPE_COUNT];
  const bool has_more_to_sync;
  const bool is_silenced;
  const int64 unsynced_count;
  const int num_conflicting_updates;
  const bool did_commit_items;
  const SyncSourceInfo source;
};

class ConflictProgress {
 public:
  explicit ConflictProgress(bool* dirty_flag);
  ~ConflictProgress();
  
  IdToConflictSetMap::const_iterator IdToConflictSetBegin() const;
  IdToConflictSetMap::const_iterator IdToConflictSetEnd() const;
  IdToConflictSetMap::size_type IdToConflictSetSize() const;
  IdToConflictSetMap::const_iterator IdToConflictSetFind(
      const syncable::Id& the_id) const;
  const ConflictSet* IdToConflictSetGet(const syncable::Id& the_id);
  std::set<ConflictSet*>::const_iterator ConflictSetsBegin() const;
  std::set<ConflictSet*>::const_iterator ConflictSetsEnd() const;
  std::set<ConflictSet*>::size_type ConflictSetsSize() const;

  
  void AddConflictingItemById(const syncable::Id& the_id);
  void EraseConflictingItemById(const syncable::Id& the_id);
  int ConflictingItemsSize() const { return conflicting_item_ids_.size(); }
  std::set<syncable::Id>::iterator ConflictingItemsBegin();
  std::set<syncable::Id>::const_iterator ConflictingItemsBeginConst() const;
  std::set<syncable::Id>::const_iterator ConflictingItemsEnd() const;

  void MergeSets(const syncable::Id& set1, const syncable::Id& set2);
  void CleanupSets();

 private:
  
  std::set<syncable::Id> conflicting_item_ids_;
  std::map<syncable::Id, ConflictSet*> id_to_conflict_set_;
  std::set<ConflictSet*> conflict_sets_;

  
  
  
  bool* dirty_;
};

typedef std::pair<VerifyResult, sync_pb::SyncEntity> VerifiedUpdate;
typedef std::pair<UpdateAttemptResponse, syncable::Id> AppliedUpdate;

class UpdateProgress {
 public:
  UpdateProgress();
  ~UpdateProgress();

  void AddVerifyResult(const VerifyResult& verify_result,
                       const sync_pb::SyncEntity& entity);

  
  void AddAppliedUpdate(const UpdateAttemptResponse& response,
                        const syncable::Id& id);

  
  std::vector<AppliedUpdate>::iterator AppliedUpdatesBegin();
  std::vector<VerifiedUpdate>::const_iterator VerifiedUpdatesBegin() const;
  std::vector<AppliedUpdate>::const_iterator AppliedUpdatesEnd() const;
  std::vector<VerifiedUpdate>::const_iterator VerifiedUpdatesEnd() const;

  
  
  int AppliedUpdatesSize() const { return applied_updates_.size(); }
  int VerifiedUpdatesSize() const { return verified_updates_.size(); }
  bool HasVerifiedUpdates() const { return !verified_updates_.empty(); }
  bool HasAppliedUpdates() const { return !applied_updates_.empty(); }

  
  
  
  int SuccessfullyAppliedUpdateCount() const;

  
  
  bool HasConflictingUpdates() const;

 private:
  
  std::vector<VerifiedUpdate> verified_updates_;

  
  
  std::vector<AppliedUpdate> applied_updates_;
};

struct SyncCycleControlParameters {
  SyncCycleControlParameters() : conflict_sets_built(false),
                                 conflicts_resolved(false),
                                 items_committed(false) {}
  
  
  bool conflict_sets_built;

  
  bool conflicts_resolved;

  
  bool items_committed;
};

template <typename T>
class DirtyOnWrite {
 public:
  explicit DirtyOnWrite(bool* dirty) : dirty_(dirty) {}
  DirtyOnWrite(bool* dirty, const T& t) : t_(t), dirty_(dirty) {}
  T* mutate() {
    *dirty_ = true;
    return &t_;
  }
  const T& value() const { return t_; }
 private:
  T t_;
  bool* dirty_;
};


struct AllModelTypeState {
  explicit AllModelTypeState(bool* dirty_flag);
  ~AllModelTypeState();

  
  ClientToServerMessage commit_message;
  ClientToServerResponse commit_response;
  
  
  syncable::ModelTypeBitSet updates_request_types;
  ClientToServerResponse updates_response;
  
  DirtyOnWrite<std::vector<int64> > unsynced_handles;
  DirtyOnWrite<SyncerStatus> syncer_status;
  DirtyOnWrite<ErrorCounters> error_counters;
  SyncCycleControlParameters control_params;
  DirtyOnWrite<int64> num_server_changes_remaining;
  OrderedCommitSet commit_set;
};

struct PerModelSafeGroupState {
  explicit PerModelSafeGroupState(bool* dirty_flag);
  ~PerModelSafeGroupState();

  UpdateProgress update_progress;
  ConflictProgress conflict_progress;
};

}  
}  

#endif  
