// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_SYNC_SESSIONS_STATUS_CONTROLLER_H_
#define CHROME_BROWSER_SYNC_SESSIONS_STATUS_CONTROLLER_H_
#pragma once

#include <map>

#include "base/stl_util-inl.h"
#include "chrome/browser/sync/sessions/ordered_commit_set.h"
#include "chrome/browser/sync/sessions/session_state.h"

namespace browser_sync {
namespace sessions {

class StatusController {
 public:
  explicit StatusController(const ModelSafeRoutingInfo& routes);
  ~StatusController();

  
  
  bool TestAndClearIsDirty();

  
  const ConflictProgress& conflict_progress() {
    return GetOrCreateModelSafeGroupState(true,
        group_restriction_)->conflict_progress;
  }
  ConflictProgress* mutable_conflict_progress() {
    return &GetOrCreateModelSafeGroupState(true,
        group_restriction_)->conflict_progress;
  }
  const UpdateProgress& update_progress() {
    return GetOrCreateModelSafeGroupState(true,
        group_restriction_)->update_progress;
  }
  UpdateProgress* mutable_update_progress() {
    return &GetOrCreateModelSafeGroupState(true,
        group_restriction_)->update_progress;
  }
  
  
  UpdateProgress* GetUnrestrictedUpdateProgress(ModelSafeGroup group) {
    return &GetOrCreateModelSafeGroupState(false, group)->update_progress;
  }

  
  const ClientToServerMessage& commit_message() {
    return shared_.commit_message;
  }
  ClientToServerMessage* mutable_commit_message() {
    return &shared_.commit_message;
  }
  const ClientToServerResponse& commit_response() const {
    return shared_.commit_response;
  }
  ClientToServerResponse* mutable_commit_response() {
    return &shared_.commit_response;
  }
  const syncable::ModelTypeBitSet& updates_request_types() const {
    return shared_.updates_request_types;
  }
  void set_updates_request_types(const syncable::ModelTypeBitSet& value) {
    shared_.updates_request_types = value;
  }
  const ClientToServerResponse& updates_response() const {
    return shared_.updates_response;
  }
  ClientToServerResponse* mutable_updates_response() {
    return &shared_.updates_response;
  }

  
  const ErrorCounters& error_counters() const {
    return shared_.error_counters.value();
  }
  const SyncerStatus& syncer_status() const {
    return shared_.syncer_status.value();
  }

  
  int64 num_server_changes_remaining() const {
    return shared_.num_server_changes_remaining.value();
  }

  
  const std::vector<syncable::Id>& commit_ids() const {
    DCHECK(!group_restriction_in_effect_) << "Group restriction in effect!";
    return shared_.commit_set.GetAllCommitIds();
  }
  const OrderedCommitSet::Projection& commit_id_projection() {
    DCHECK(group_restriction_in_effect_)
        << "No group restriction for projection.";
    return shared_.commit_set.GetCommitIdProjection(group_restriction_);
  }
  const syncable::Id& GetCommitIdAt(size_t index) {
    DCHECK(CurrentCommitIdProjectionHasIndex(index));
    return shared_.commit_set.GetCommitIdAt(index);
  }
  syncable::ModelType GetCommitIdModelTypeAt(size_t index) {
    DCHECK(CurrentCommitIdProjectionHasIndex(index));
    return shared_.commit_set.GetModelTypeAt(index);
  }
  const std::vector<int64>& unsynced_handles() const {
    DCHECK(!group_restriction_in_effect_)
        << "unsynced_handles is unrestricted.";
    return shared_.unsynced_handles.value();
  }

  
  bool conflict_sets_built() const {
    return shared_.control_params.conflict_sets_built;
  }
  bool conflicts_resolved() const {
    return shared_.control_params.conflicts_resolved;
  }
  bool did_commit_items() const {
    return shared_.control_params.items_committed;
  }

  
  
  bool HasConflictingUpdates() const;

  
  
  int TotalNumConflictingItems() const;

  
  int64 CountUpdates() const;

  
  
  bool HasBookmarkCommitActivity() const {
    return ActiveGroupRestrictionIncludesModel(syncable::BOOKMARKS) &&
        shared_.commit_set.HasBookmarkCommitId();
  }

  
  
  bool download_updates_succeeded() const {
    return updates_response().has_get_updates();
  }

  
  
  
  
  
  
  bool ServerSaysNothingMoreToDownload() const;

  ModelSafeGroup group_restriction() const {
    return group_restriction_;
  }

  
  
  bool ActiveGroupRestrictionIncludesModel(syncable::ModelType model) const {
    if (!group_restriction_in_effect_)
      return true;
    ModelSafeRoutingInfo::const_iterator it = routing_info_.find(model);
    if (it == routing_info_.end())
      return false;
    return group_restriction() == it->second;
  }

  
  void increment_num_conflicting_commits_by(int value);
  void reset_num_conflicting_commits();
  void set_num_consecutive_transient_error_commits(int value);
  void increment_num_consecutive_transient_error_commits_by(int value);
  void set_num_consecutive_errors(int value);
  void increment_num_consecutive_errors();
  void increment_num_consecutive_errors_by(int value);
  void set_num_server_changes_remaining(int64 changes_remaining);
  void set_invalid_store(bool invalid_store);
  void set_syncer_stuck(bool syncer_stuck);
  void set_syncing(bool syncing);
  void set_num_successful_bookmark_commits(int value);
  void increment_num_successful_commits();
  void increment_num_successful_bookmark_commits();
  void increment_num_updates_downloaded_by(int value);
  void increment_num_tombstone_updates_downloaded_by(int value);
  void set_types_needing_local_migration(const syncable::ModelTypeSet& types);
  void set_unsynced_handles(const std::vector<int64>& unsynced_handles);

  void set_commit_set(const OrderedCommitSet& commit_set);
  void update_conflict_sets_built(bool built);
  void update_conflicts_resolved(bool resolved);
  void reset_conflicts_resolved();
  void set_items_committed();

 private:
  friend class ScopedModelSafeGroupRestriction;

  
  
  bool CurrentCommitIdProjectionHasIndex(size_t index);

  
  PerModelSafeGroupState* GetOrCreateModelSafeGroupState(bool restrict,
                                                         ModelSafeGroup group);

  AllModelTypeState shared_;
  std::map<ModelSafeGroup, PerModelSafeGroupState*> per_model_group_;

  STLValueDeleter<std::map<ModelSafeGroup, PerModelSafeGroupState*> >
      per_model_group_deleter_;

  
  
  bool is_dirty_;

  
  
  bool group_restriction_in_effect_;
  ModelSafeGroup group_restriction_;

  const ModelSafeRoutingInfo routing_info_;

  DISALLOW_COPY_AND_ASSIGN(StatusController);
};

class ScopedModelSafeGroupRestriction {
 public:
  ScopedModelSafeGroupRestriction(StatusController* to_restrict,
                                  ModelSafeGroup restriction)
      : status_(to_restrict) {
    DCHECK(!status_->group_restriction_in_effect_);
    status_->group_restriction_ = restriction;
    status_->group_restriction_in_effect_ = true;
  }
  ~ScopedModelSafeGroupRestriction() {
    DCHECK(status_->group_restriction_in_effect_);
    status_->group_restriction_in_effect_ = false;
  }
 private:
  StatusController* status_;
  DISALLOW_COPY_AND_ASSIGN(ScopedModelSafeGroupRestriction);
};

}
}

#endif  
