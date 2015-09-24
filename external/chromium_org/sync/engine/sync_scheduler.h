// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef SYNC_ENGINE_SYNC_SCHEDULER_H_
#define SYNC_ENGINE_SYNC_SCHEDULER_H_

#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/time/time.h"
#include "sync/base/sync_export.h"
#include "sync/engine/nudge_source.h"
#include "sync/sessions/sync_session.h"

namespace tracked_objects {
class Location;
}  

namespace syncer {

class ObjectIdInvalidationMap;
struct ServerConnectionEvent;

struct SYNC_EXPORT_PRIVATE ConfigurationParams {
  ConfigurationParams();
  ConfigurationParams(
      const sync_pb::GetUpdatesCallerInfo::GetUpdatesSource& source,
      ModelTypeSet types_to_download,
      const ModelSafeRoutingInfo& routing_info,
      const base::Closure& ready_task,
      const base::Closure& retry_task);
  ~ConfigurationParams();

  
  sync_pb::GetUpdatesCallerInfo::GetUpdatesSource source;
  
  ModelTypeSet types_to_download;
  
  ModelSafeRoutingInfo routing_info;
  
  base::Closure ready_task;
  
  base::Closure retry_task;
};

class SYNC_EXPORT_PRIVATE SyncScheduler
    : public sessions::SyncSession::Delegate {
 public:
  enum Mode {
    
    
    
    
    CONFIGURATION_MODE,
    
    
    NORMAL_MODE,
  };

  
  

  SyncScheduler();
  virtual ~SyncScheduler();

  
  
  
  virtual void Start(Mode mode) = 0;

  
  
  
  
  
  
  
  virtual void ScheduleConfiguration(const ConfigurationParams& params) = 0;

  
  
  virtual void Stop() = 0;

  
  
  
  
  
  
  
  

  
  
  virtual void ScheduleLocalNudge(
      const base::TimeDelta& desired_delay,
      ModelTypeSet types,
      const tracked_objects::Location& nudge_location) = 0;

  
  
  
  
  virtual void ScheduleLocalRefreshRequest(
      const base::TimeDelta& desired_delay,
      ModelTypeSet types,
      const tracked_objects::Location& nudge_location) = 0;

  
  
  
  
  virtual void ScheduleInvalidationNudge(
      const base::TimeDelta& desired_delay,
      const ObjectIdInvalidationMap& invalidations,
      const tracked_objects::Location& nudge_location) = 0;

  
  virtual void SetNotificationsEnabled(bool notifications_enabled) = 0;

  virtual base::TimeDelta GetSessionsCommitDelay() const = 0;

  
  virtual void OnCredentialsUpdated() = 0;

  
  virtual void OnConnectionStatusChange() = 0;
};

}  

#endif  
