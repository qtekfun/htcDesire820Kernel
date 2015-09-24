// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_SYNC_DIRECTORY_UPDATE_HANDLER_H_
#define SYNC_ENGINE_SYNC_DIRECTORY_UPDATE_HANDLER_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "sync/base/sync_export.h"
#include "sync/engine/process_updates_util.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/util/syncer_error.h"

namespace sync_pb {
class DataTypeProgressMarker;
class GetUpdatesResponse;
}

namespace syncer {

namespace sessions {
class StatusController;
}

namespace syncable {
class Directory;
}

class ModelSafeWorker;

class SYNC_EXPORT_PRIVATE SyncDirectoryUpdateHandler {
 public:
  SyncDirectoryUpdateHandler(syncable::Directory* dir,
                             ModelType type,
                             scoped_refptr<ModelSafeWorker> worker);
  ~SyncDirectoryUpdateHandler();

  
  void GetDownloadProgress(
      sync_pb::DataTypeProgressMarker* progress_marker) const;

  
  
  
  
  
  
  void ProcessGetUpdatesResponse(
      const sync_pb::DataTypeProgressMarker& progress_marker,
      const SyncEntityList& applicable_updates,
      sessions::StatusController* status);

  
  
  void ApplyUpdates(sessions::StatusController* status);

 private:
  friend class SyncDirectoryUpdateHandlerApplyUpdateTest;
  friend class SyncDirectoryUpdateHandlerProcessUpdateTest;

  
  
  void UpdateSyncEntities(
      syncable::ModelNeutralWriteTransaction* trans,
      const SyncEntityList& applicable_updates,
      sessions::StatusController* status);

  
  
  void UpdateProgressMarker(
      const sync_pb::DataTypeProgressMarker& progress_marker);

  
  SyncerError ApplyUpdatesImpl(sessions::StatusController* status);

  syncable::Directory* dir_;
  ModelType type_;
  scoped_refptr<ModelSafeWorker> worker_;

  DISALLOW_COPY_AND_ASSIGN(SyncDirectoryUpdateHandler);
};

typedef std::map<ModelType, SyncDirectoryUpdateHandler*> UpdateHandlerMap;

}  

#endif  
