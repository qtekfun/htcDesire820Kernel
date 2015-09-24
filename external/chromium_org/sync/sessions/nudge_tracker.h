// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef SYNC_SESSIONS_NUDGE_TRACKER_H_
#define SYNC_SESSIONS_NUDGE_TRACKER_H_

#include <list>
#include <map>

#include "base/compiler_specific.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/protocol/sync.pb.h"
#include "sync/sessions/data_type_tracker.h"

namespace syncer {

class ObjectIdInvalidationMap;

namespace sessions {

class SYNC_EXPORT_PRIVATE NudgeTracker {
 public:
  static size_t kDefaultMaxPayloadsPerType;

  NudgeTracker();
  ~NudgeTracker();

  
  
  
  bool IsSyncRequired() const;

  
  
  bool IsGetUpdatesRequired() const;

  
  
  void RecordSuccessfulSyncCycle();

  
  void RecordLocalChange(ModelTypeSet types);

  
  void RecordLocalRefreshRequest(ModelTypeSet types);

  
  void RecordRemoteInvalidation(
      const ObjectIdInvalidationMap& invalidation_map);

  
  
  void OnInvalidationsEnabled();
  void OnInvalidationsDisabled();

  
  void SetTypesThrottledUntil(ModelTypeSet types,
                              base::TimeDelta length,
                              base::TimeTicks now);

  
  void UpdateTypeThrottlingState(base::TimeTicks now);

  
  
  base::TimeDelta GetTimeUntilNextUnthrottle(base::TimeTicks now) const;

  
  bool IsAnyTypeThrottled() const;

  
  bool IsTypeThrottled(ModelType type) const;

  
  ModelTypeSet GetThrottledTypes() const;

  
  
  
  
  
  
  
  
  sync_pb::GetUpdatesCallerInfo::GetUpdatesSource updates_source() const;

  
  
  
  void FillProtoMessage(
      ModelType type,
      sync_pb::GetUpdateTriggers* msg) const;

  
  
  
  void SetLegacyNotificationHint(
      ModelType type,
      sync_pb::DataTypeProgressMarker* progress) const;

  
  void SetHintBufferSize(size_t size);

 private:
  typedef std::map<ModelType, DataTypeTracker> TypeTrackerMap;

  TypeTrackerMap type_trackers_;

  
  
  sync_pb::GetUpdatesCallerInfo::GetUpdatesSource updates_source_;

  
  bool invalidations_enabled_;

  
  
  
  
  
  
  
  
  
  bool invalidations_out_of_sync_;

  size_t num_payloads_per_type_;

  DISALLOW_COPY_AND_ASSIGN(NudgeTracker);
};

}  
}  

#endif  
