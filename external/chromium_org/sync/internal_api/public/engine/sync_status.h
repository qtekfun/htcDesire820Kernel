// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_ENGINE_STATUS_SUMMARY_H_
#define SYNC_INTERNAL_API_PUBLIC_ENGINE_STATUS_SUMMARY_H_

#include <string>

#include "base/time/time.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/sync_encryption_handler.h"
#include "sync/protocol/sync_protocol_error.h"

namespace syncer {

struct SYNC_EXPORT SyncStatus {
  SyncStatus();
  ~SyncStatus();

  
  
  bool notifications_enabled;  

  
  int notifications_received;

  SyncProtocolError sync_protocol_error;

  
  int encryption_conflicts;

  
  int hierarchy_conflicts;

  
  
  int server_conflicts;

  
  int committed_count;

  bool syncing;

  
  int64 updates_available;
  
  int updates_received;
  
  int reflected_updates_received;
  
  int tombstone_updates_received;

  
  int num_commits_total;

  
  int num_local_overwrites_total;
  int num_server_overwrites_total;

  
  int nonempty_get_updates;
  int empty_get_updates;

  
  int sync_cycles_with_commits;
  int sync_cycles_without_commits;

  
  int useless_sync_cycles;
  int useful_sync_cycles;

  
  int nudge_source_notification;
  int nudge_source_local;
  int nudge_source_local_refresh;

  
  ModelTypeSet encrypted_types;
  bool cryptographer_ready;
  bool crypto_has_pending_keys;
  bool has_keystore_key;
  base::Time keystore_migration_time;
  PassphraseType passphrase_type;

  
  ModelTypeSet throttled_types;

  
  std::string sync_id;

  
  std::string invalidator_client_id;

  
  std::vector<int> num_entries_by_type;
  std::vector<int> num_to_delete_entries_by_type;

  
  base::Time retry_time;
};

}  

#endif  
