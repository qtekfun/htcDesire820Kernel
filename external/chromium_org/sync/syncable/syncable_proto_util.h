// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNCABLE_PROTOCOL_PROTO_UTIL_H_
#define SYNCABLE_PROTOCOL_PROTO_UTIL_H_

#include <string>

#include "sync/base/sync_export.h"
#include "sync/syncable/syncable_id.h"

namespace sync_pb {
class SyncEntity;
}

namespace syncer {

syncable::Id SyncableIdFromProto(const std::string& proto_string);

SYNC_EXPORT_PRIVATE std::string SyncableIdToProto(
    const syncable::Id& syncable_id);

bool IsFolder(const sync_pb::SyncEntity& entity);

bool IsRoot(const sync_pb::SyncEntity& entity);

}  

#endif  
