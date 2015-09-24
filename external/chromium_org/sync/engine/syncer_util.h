// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_SYNCER_UTIL_H_
#define SYNC_ENGINE_SYNCER_UTIL_H_

#include <set>
#include <string>
#include <vector>

#include "sync/engine/syncer.h"
#include "sync/engine/syncer_types.h"
#include "sync/syncable/entry_kernel.h"
#include "sync/syncable/metahandle_set.h"
#include "sync/syncable/mutable_entry.h"
#include "sync/syncable/syncable_id.h"

namespace sync_pb {
class SyncEntity;
}  

namespace syncer {

namespace syncable {
class BaseTransaction;
class ModelNeutralWriteTransaction;
}  

class Cryptographer;

syncable::Id FindLocalIdToUpdate(
    syncable::BaseTransaction* trans,
    const sync_pb::SyncEntity& server_entry);

UpdateAttemptResponse AttemptToUpdateEntry(
    syncable::WriteTransaction* const trans,
    syncable::MutableEntry* const entry,
    Cryptographer* cryptographer);

UniquePosition GetUpdatePosition(const sync_pb::SyncEntity& update,
                                 const std::string& suffix);

std::string GetUniqueBookmarkTagFromUpdate(const sync_pb::SyncEntity& update);

void UpdateServerFieldsFromUpdate(
    syncable::ModelNeutralMutableEntry* local_entry,
    const sync_pb::SyncEntity& server_entry,
    const std::string& name);

void CreateNewEntry(syncable::ModelNeutralWriteTransaction *trans,
                    const syncable::Id& id);

void UpdateLocalDataFromServerData(syncable::WriteTransaction* trans,
                                   syncable::MutableEntry* entry);

VerifyCommitResult ValidateCommitEntry(syncable::Entry* entry);

VerifyResult VerifyNewEntry(const sync_pb::SyncEntity& update,
                            syncable::Entry* target,
                            const bool deleted);

VerifyResult VerifyUpdateConsistency(
    syncable::ModelNeutralWriteTransaction* trans,
    const sync_pb::SyncEntity& update,
    const bool deleted,
    const bool is_directory,
    ModelType model_type,
    syncable::ModelNeutralMutableEntry* target);

VerifyResult VerifyUndelete(syncable::ModelNeutralWriteTransaction* trans,
                            const sync_pb::SyncEntity& update,
                            syncable::ModelNeutralMutableEntry* target);

void MarkDeletedChildrenSynced(
    syncable::Directory* dir,
    syncable::BaseWriteTransaction* trans,
    std::set<syncable::Id>* deleted_folders);

}  

#endif  
