// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_API_SYNC_CHANGE_PROCESSOR_H_
#define SYNC_API_SYNC_CHANGE_PROCESSOR_H_

#include <vector>

#include "sync/api/sync_data.h"
#include "sync/api/sync_error.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"

namespace tracked_objects {
class Location;
}  

namespace syncer {

class SyncChange;

typedef std::vector<SyncChange> SyncChangeList;

class SYNC_EXPORT SyncChangeProcessor {
 public:
  SyncChangeProcessor();
  virtual ~SyncChangeProcessor();

  
  
  
  
  
  
  
  virtual SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const SyncChangeList& change_list) = 0;

  
  
  
  
  
  
  virtual SyncDataList GetAllSyncData(ModelType type) const = 0;
};

}  

#endif  
