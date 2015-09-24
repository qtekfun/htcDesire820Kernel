// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_PROCESS_COMMIT_RESPONSE_COMMAND_H_
#define CHROME_BROWSER_SYNC_ENGINE_PROCESS_COMMIT_RESPONSE_COMMAND_H_
#pragma once

#include <set>
#include <string>

#include "base/basictypes.h"
#include "chrome/browser/sync/engine/model_changing_syncer_command.h"
#include "chrome/browser/sync/engine/syncproto.h"

namespace syncable {
class Id;
class WriteTransaction;
class MutableEntry;
}

namespace browser_sync {

class ProcessCommitResponseCommand : public ModelChangingSyncerCommand {
 public:

  ProcessCommitResponseCommand();
  virtual ~ProcessCommitResponseCommand();

  
  virtual bool ModelNeutralExecuteImpl(sessions::SyncSession* session);
  virtual void ModelChangingExecuteImpl(sessions::SyncSession* session);

 private:
  CommitResponse::ResponseType ProcessSingleCommitResponse(
      syncable::WriteTransaction* trans,
      const sync_pb::CommitResponse_EntryResponse& pb_commit_response,
      const sync_pb::SyncEntity& pb_committed_entry,
      const syncable::Id& pre_commit_id,
      std::set<syncable::Id>* conflicting_new_directory_ids,
      std::set<syncable::Id>* deleted_folders);

  
  void ProcessCommitResponse(sessions::SyncSession* session);

  void ProcessSuccessfulCommitResponse(
      const sync_pb::SyncEntity& committed_entry,
      const CommitResponse_EntryResponse& entry_response,
      const syncable::Id& pre_commit_id, syncable::MutableEntry* local_entry,
      bool syncing_was_set, std::set<syncable::Id>* deleted_folders);

  
  
  bool UpdateVersionAfterCommit(
      const sync_pb::SyncEntity& committed_entry,
      const CommitResponse_EntryResponse& entry_response,
      const syncable::Id& pre_commit_id,
      syncable::MutableEntry* local_entry);

  
  
  bool ChangeIdAfterCommit(
      const CommitResponse_EntryResponse& entry_response,
      const syncable::Id& pre_commit_id,
      syncable::MutableEntry* local_entry);

  
  
  void UpdateServerFieldsAfterCommit(
      const sync_pb::SyncEntity& committed_entry,
      const CommitResponse_EntryResponse& entry_response,
      syncable::MutableEntry* local_entry);

  
  
  
  
  
  
  void OverrideClientFieldsAfterCommit(
      const sync_pb::SyncEntity& committed_entry,
      const CommitResponse_EntryResponse& entry_response,
      syncable::MutableEntry* local_entry);

  
  const std::string& GetResultingPostCommitName(
      const sync_pb::SyncEntity& committed_entry,
      const CommitResponse_EntryResponse& entry_response);

  DISALLOW_COPY_AND_ASSIGN(ProcessCommitResponseCommand);
};

}  

#endif  
