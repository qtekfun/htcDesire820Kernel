// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SESSIONS_MODEL_NEUTRAL_STATE_H
#define SYNC_SESSIONS_MODEL_NEUTRAL_STATE_H

#include "base/basictypes.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/util/syncer_error.h"
#include "sync/protocol/sync.pb.h"
#include "sync/protocol/sync_protocol_error.h"

namespace syncer {
namespace sessions {

struct SYNC_EXPORT ModelNeutralState {
  ModelNeutralState();
  ~ModelNeutralState();

  
  ModelTypeSet commit_request_types;

  int num_successful_commits;

  
  int num_successful_bookmark_commits;

  
  int num_updates_downloaded_total;
  int num_tombstone_updates_downloaded_total;
  int num_reflected_updates_downloaded_total;

  
  
  ModelTypeSet types_needing_local_migration;

  
  int num_updates_applied;
  int num_encryption_conflicts;
  int num_server_conflicts;
  int num_hierarchy_conflicts;

  
  int num_local_overwrites;
  int num_server_overwrites;

  
  SyncProtocolError sync_protocol_error;

  
  
  SyncerError last_get_key_result;
  SyncerError last_download_updates_result;
  SyncerError commit_result;

  
  bool items_committed;

  
  
  
  int64 num_server_changes_remaining;
};

bool HasSyncerError(const ModelNeutralState& state);

}  
}  

#endif  
