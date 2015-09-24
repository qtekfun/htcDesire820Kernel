// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_SYNC_DIRECTORY_COMMIT_CONTRIBUTION_H_
#define SYNC_ENGINE_SYNC_DIRECTORY_COMMIT_CONTRIBUTION_H_

#include <vector>

#include "base/gtest_prod_util.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/util/syncer_error.h"
#include "sync/protocol/sync.pb.h"
#include "sync/sessions/status_controller.h"

namespace syncer {

namespace sessions {
class StatusController;
}  

namespace syncable {
class Directory;
}  

class SYNC_EXPORT_PRIVATE SyncDirectoryCommitContribution {
 public:
  
  ~SyncDirectoryCommitContribution();

  
  
  
  
  
  
  static SyncDirectoryCommitContribution* Build(
      syncable::Directory* dir,
      ModelType type,
      size_t max_items);

  
  
  
  
  
  void AddToCommitMessage(sync_pb::ClientToServerMessage* msg);

  
  
  
  
  
  
  SyncerError ProcessCommitResponse(
      const sync_pb::ClientToServerResponse& response,
      sessions::StatusController* status);

  
  
  void CleanUp();

  
  size_t GetNumEntries() const;

 private:
  class SyncDirectoryCommitContributionTest;
  FRIEND_TEST_ALL_PREFIXES(SyncDirectoryCommitContributionTest, GatherByTypes);
  FRIEND_TEST_ALL_PREFIXES(SyncDirectoryCommitContributionTest,
                           GatherAndTruncate);

  SyncDirectoryCommitContribution(
      const std::vector<int64>& metahandles,
      const google::protobuf::RepeatedPtrField<sync_pb::SyncEntity>& entities,
      syncable::Directory* directory);

  void UnsetSyncingBits();

  syncable::Directory* dir_;
  const std::vector<int64> metahandles_;
  const google::protobuf::RepeatedPtrField<sync_pb::SyncEntity> entities_;
  size_t entries_start_index_;

  
  
  
  
  bool syncing_bits_set_;

  DISALLOW_COPY_AND_ASSIGN(SyncDirectoryCommitContribution);
};

}  

#endif  
