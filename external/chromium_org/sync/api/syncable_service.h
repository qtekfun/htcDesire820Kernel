// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_API_SYNCABLE_SERVICE_H_
#define SYNC_API_SYNCABLE_SERVICE_H_

#include <vector>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "sync/api/sync_change_processor.h"
#include "sync/api/sync_data.h"
#include "sync/api/sync_error.h"
#include "sync/api/sync_merge_result.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"

namespace syncer {

class SyncErrorFactory;

class SYNC_EXPORT SyncableService
    : public SyncChangeProcessor,
      public base::SupportsWeakPtr<SyncableService> {
 public:
  
  
  
  
  
  
  
  typedef base::Callback<void(ModelType)> StartSyncFlare;

  
  
  
  
  
  
  
  
  
  virtual SyncMergeResult MergeDataAndStartSyncing(
      ModelType type,
      const SyncDataList& initial_sync_data,
      scoped_ptr<SyncChangeProcessor> sync_processor,
      scoped_ptr<SyncErrorFactory> error_handler) = 0;

  
  virtual void StopSyncing(ModelType type) = 0;

  
  
  
  
  
  virtual SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const SyncChangeList& change_list) OVERRIDE = 0;

 protected:
  virtual ~SyncableService();
};

}  

#endif  
