// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef SYNC_SESSIONS_DATA_TYPE_TRACKER_H_
#define SYNC_SESSIONS_DATA_TYPE_TRACKER_H_

#include <deque>
#include <string>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "sync/protocol/sync.pb.h"

namespace syncer {

class Invalidation;
class SingleObjectInvalidationSet;

namespace sessions {

typedef std::deque<std::string> PayloadList;

class DataTypeTracker {
 public:
  DataTypeTracker();
  ~DataTypeTracker();

  
  

  
  void RecordLocalChange();

  
  void RecordLocalRefreshRequest();

  
  void RecordRemoteInvalidations(
      const SingleObjectInvalidationSet& invalidations);

  
  
  
  
  void RecordSuccessfulSyncCycle();

  
  void UpdatePayloadBufferSize(size_t new_size);

  
  
  
  bool IsSyncRequired() const;

  
  
  bool IsGetUpdatesRequired() const;

  
  bool HasLocalChangePending() const;

  
  
  bool HasPendingInvalidation() const;

  
  std::string GetMostRecentInvalidationPayload() const;

  
  void SetLegacyNotificationHint(
      sync_pb::DataTypeProgressMarker* progress) const;

  
  
  
  void FillGetUpdatesTriggersMessage(sync_pb::GetUpdateTriggers* msg) const;

  
  bool IsThrottled() const;

  
  
  
  base::TimeDelta GetTimeUntilUnthrottle(base::TimeTicks now) const;

  
  void ThrottleType(base::TimeDelta duration, base::TimeTicks now);

  
  void UpdateThrottleState(base::TimeTicks now);

 private:
  
  
  int local_nudge_count_;

  
  
  int local_refresh_request_count_;

  
  
  
  PayloadList pending_payloads_;

  
  
  bool local_payload_overflow_;

  
  
  bool server_payload_overflow_;

  size_t payload_buffer_size_;

  
  
  base::TimeTicks unthrottle_time_;
};

}  
}  

#endif  
