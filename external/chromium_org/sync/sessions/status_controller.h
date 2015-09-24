// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef SYNC_SESSIONS_STATUS_CONTROLLER_H_
#define SYNC_SESSIONS_STATUS_CONTROLLER_H_

#include <map>
#include <vector>

#include "base/logging.h"
#include "base/stl_util.h"
#include "base/time/time.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/engine/model_safe_worker.h"
#include "sync/internal_api/public/sessions/model_neutral_state.h"

namespace syncer {
namespace sessions {

class SYNC_EXPORT_PRIVATE StatusController {
 public:
  explicit StatusController();
  ~StatusController();

  
  const ModelTypeSet commit_request_types() const {
    return model_neutral_.commit_request_types;
  }
  void set_commit_request_types(ModelTypeSet value) {
    model_neutral_.commit_request_types = value;
  }

  
  int64 num_server_changes_remaining() const {
    return model_neutral_.num_server_changes_remaining;
  }

  
  int num_encryption_conflicts() const;
  int num_hierarchy_conflicts() const;
  int num_server_conflicts() const;

  
  int TotalNumConflictingItems() const;

  
  int num_updates_applied() const;

  int num_server_overwrites() const;

  base::Time sync_start_time() const {
    
    return sync_start_time_;
  }

  const ModelNeutralState& model_neutral_state() const {
    return model_neutral_;
  }

  SyncerError last_get_key_result() const;

  
  void set_num_server_changes_remaining(int64 changes_remaining);
  void increment_num_updates_downloaded_by(int value);
  void increment_num_tombstone_updates_downloaded_by(int value);
  void increment_num_reflected_updates_downloaded_by(int value);

  
  void increment_num_updates_applied_by(int value);
  void increment_num_encryption_conflicts_by(int value);
  void increment_num_hierarchy_conflicts_by(int value);
  void increment_num_server_conflicts();
  void increment_num_local_overwrites();
  void increment_num_server_overwrites();

  
  void increment_num_successful_commits();
  void increment_num_successful_bookmark_commits();
  void set_num_successful_bookmark_commits(int value);

  
  void set_sync_protocol_error(const SyncProtocolError& error);
  void set_last_get_key_result(const SyncerError result);
  void set_last_download_updates_result(const SyncerError result);
  void set_commit_result(const SyncerError result);

  
  void set_types_needing_local_migration(ModelTypeSet types);

  void UpdateStartTime();

 private:
  ModelNeutralState model_neutral_;

  base::Time sync_start_time_;

  DISALLOW_COPY_AND_ASSIGN(StatusController);
};

}  
}  

#endif  
