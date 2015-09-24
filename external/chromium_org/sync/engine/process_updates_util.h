// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_PROCESS_UPDATES_UTIL_H_
#define SYNC_ENGINE_PROCESS_UPDATES_UTIL_H_

#include <map>
#include <vector>

#include "base/compiler_specific.h"
#include "sync/base/sync_export.h"
#include "sync/engine/syncer_types.h"
#include "sync/internal_api/public/base/model_type.h"

namespace sync_pb {
class GetUpdatesResponse;
class SyncEntity;
}

namespace syncer {

namespace sessions {
class StatusController;
}

namespace syncable {
class ModelNeutralWriteTransaction;
class Directory;
}

class Cryptographer;

typedef std::vector<const sync_pb::SyncEntity*> SyncEntityList;
typedef std::map<ModelType, SyncEntityList> TypeSyncEntityMap;

void PartitionUpdatesByType(
    const sync_pb::GetUpdatesResponse& updates,
    ModelTypeSet requested_types,
    TypeSyncEntityMap* updates_by_type);

void ProcessDownloadedUpdates(
    syncable::Directory* dir,
    syncable::ModelNeutralWriteTransaction* trans,
    ModelType type,
    const SyncEntityList& applicable_updates,
    sessions::StatusController* status);

VerifyResult VerifyUpdate(
    syncable::ModelNeutralWriteTransaction* trans,
    const sync_pb::SyncEntity& entry,
    ModelType requested_type);

void ProcessUpdate(
    const sync_pb::SyncEntity& proto_update,
    const Cryptographer* cryptographer,
    syncable::ModelNeutralWriteTransaction* const trans);

}  

#endif  
