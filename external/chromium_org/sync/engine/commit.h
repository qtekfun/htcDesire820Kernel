// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_COMMIT_H_
#define SYNC_ENGINE_COMMIT_H_

#include <map>

#include "base/stl_util.h"
#include "sync/base/sync_export.h"
#include "sync/engine/sync_directory_commit_contribution.h"
#include "sync/engine/sync_directory_commit_contributor.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/engine/model_safe_worker.h"
#include "sync/internal_api/public/util/syncer_error.h"
#include "sync/protocol/sync.pb.h"
#include "sync/util/extensions_activity.h"

namespace syncer {

namespace sessions {
class StatusController;
class SyncSession;
}

class Syncer;

class SYNC_EXPORT_PRIVATE Commit {
 public:
  Commit(
      const std::map<ModelType, SyncDirectoryCommitContribution*>&
          contributions,
      const sync_pb::ClientToServerMessage& message,
      ExtensionsActivity::Records extensions_activity_buffer);

  
  ~Commit();

  static Commit* Init(
      ModelTypeSet requested_types,
      size_t max_entries,
      const std::string& account_name,
      const std::string& cache_guid,
      CommitContributorMap* contributor_map,
      ExtensionsActivity* extensions_activity);

  SyncerError PostAndProcessResponse(
      sessions::SyncSession* session,
      sessions::StatusController* status,
      ExtensionsActivity* extensions_activity);

  
  
  void CleanUp();

 private:
  typedef std::map<ModelType, SyncDirectoryCommitContribution*> ContributionMap;

  ContributionMap contributions_;
  STLValueDeleter<ContributionMap> deleter_;

  sync_pb::ClientToServerMessage message_;
  sync_pb::ClientToServerResponse response_;
  ExtensionsActivity::Records extensions_activity_buffer_;

  
  bool cleaned_up_;
};

}  

#endif  
