// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_SYNC_SCHEDULER_IMPL_H_
#define SYNC_ENGINE_SYNC_SCHEDULER_IMPL_H_

#include <map>
#include <string>

#include "base/callback.h"
#include "base/cancelable_callback.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "sync/base/sync_export.h"
#include "sync/engine/net/server_connection_manager.h"
#include "sync/engine/nudge_source.h"
#include "sync/engine/sync_scheduler.h"
#include "sync/engine/syncer.h"
#include "sync/internal_api/public/engine/polling_constants.h"
#include "sync/internal_api/public/util/weak_handle.h"
#include "sync/sessions/nudge_tracker.h"
#include "sync/sessions/sync_session.h"
#include "sync/sessions/sync_session_context.h"

namespace syncer {

class BackoffDelayProvider;

namespace sessions {
struct ModelNeutralState;
}

class SYNC_EXPORT_PRIVATE SyncSchedulerImpl
    : public SyncScheduler,
      public base::NonThreadSafe {
 public:
  
  
  SyncSchedulerImpl(const std::string& name,
                    BackoffDelayProvider* delay_provider,
                    sessions::SyncSessionContext* context,
                    Syncer* syncer);

  
  virtual ~SyncSchedulerImpl();

  virtual void Start(Mode mode) OVERRIDE;
  virtual void ScheduleConfiguration(
      const ConfigurationParams& params) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void ScheduleLocalNudge(
      const base::TimeDelta& desired_delay,
      ModelTypeSet types,
      const tracked_objects::Location& nudge_location) OVERRIDE;
  virtual void ScheduleLocalRefreshRequest(
      const base::TimeDelta& desired_delay,
      ModelTypeSet types,
      const tracked_objects::Location& nudge_location) OVERRIDE;
  virtual void ScheduleInvalidationNudge(
      const base::TimeDelta& desired_delay,
      const ObjectIdInvalidationMap& invalidation_map,
      const tracked_objects::Location& nudge_location) OVERRIDE;
  virtual void SetNotificationsEnabled(bool notifications_enabled) OVERRIDE;

  virtual base::TimeDelta GetSessionsCommitDelay() const OVERRIDE;

  virtual void OnCredentialsUpdated() OVERRIDE;
  virtual void OnConnectionStatusChange() OVERRIDE;

  
  virtual void OnThrottled(const base::TimeDelta& throttle_duration) OVERRIDE;
  virtual void OnTypesThrottled(
      ModelTypeSet types,
      const base::TimeDelta& throttle_duration) OVERRIDE;
  virtual bool IsCurrentlyThrottled() OVERRIDE;
  virtual void OnReceivedShortPollIntervalUpdate(
      const base::TimeDelta& new_interval) OVERRIDE;
  virtual void OnReceivedLongPollIntervalUpdate(
      const base::TimeDelta& new_interval) OVERRIDE;
  virtual void OnReceivedSessionsCommitDelay(
      const base::TimeDelta& new_delay) OVERRIDE;
  virtual void OnReceivedClientInvalidationHintBufferSize(int size) OVERRIDE;
  virtual void OnSyncProtocolError(
      const sessions::SyncSessionSnapshot& snapshot) OVERRIDE;

 private:
  enum JobPriority {
    
    NORMAL_PRIORITY,
    
    CANARY_PRIORITY
  };

  enum PollAdjustType {
    
    FORCE_RESET,
    
    UPDATE_INTERVAL,
  };

  friend class SyncSchedulerTest;
  friend class SyncSchedulerWhiteboxTest;
  friend class SyncerTest;

  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerWhiteboxTest, NoNudgesInConfigureMode);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerWhiteboxTest,
      DropNudgeWhileExponentialBackOff);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerWhiteboxTest, SaveNudge);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerWhiteboxTest,
                           SaveNudgeWhileTypeThrottled);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerWhiteboxTest, ContinueNudge);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerWhiteboxTest, ContinueConfiguration);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerWhiteboxTest,
                           SaveConfigurationWhileThrottled);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerWhiteboxTest,
                           SaveNudgeWhileThrottled);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerWhiteboxTest,
                           ContinueCanaryJobConfig);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerTest, TransientPollFailure);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerTest,
                           ServerConnectionChangeDuringBackoff);
  FRIEND_TEST_ALL_PREFIXES(SyncSchedulerTest,
                           ConnectionChangeCanaryPreemptedByNudge);
  FRIEND_TEST_ALL_PREFIXES(BackoffTriggersSyncSchedulerTest,
                           FailGetEncryptionKey);

  struct SYNC_EXPORT_PRIVATE WaitInterval {
    enum Mode {
      
      UNKNOWN = -1,
      
      
      EXPONENTIAL_BACKOFF,
      
      
      THROTTLED,
    };
    WaitInterval();
    ~WaitInterval();
    WaitInterval(Mode mode, base::TimeDelta length);

    static const char* GetModeString(Mode mode);

    Mode mode;
    base::TimeDelta length;
  };

  static const char* GetModeString(Mode mode);

  
  void DoNudgeSyncSessionJob(JobPriority priority);

  
  void DoConfigurationSyncSessionJob(JobPriority priority);

  
  void HandleFailure(
      const sessions::ModelNeutralState& model_neutral_state);

  
  void DoPollSyncSessionJob();

  
  base::TimeDelta GetPollInterval();

  
  
  void AdjustPolling(PollAdjustType type);

  
  void RestartWaiting();

  
  bool CanRunJobNow(JobPriority priority);

  
  bool CanRunNudgeJobNow(JobPriority priority);

  
  
  
  
  void ScheduleNudgeImpl(
      const base::TimeDelta& delay,
      const tracked_objects::Location& nudge_location);

  
  bool IsBackingOff() const;

  
  void Notify(SyncEngineEvent::EventCause cause);

  
  void NotifyRetryTime(base::Time retry_time);

  
  void NotifyThrottledTypesChanged(ModelTypeSet types);

  
  
  void TryCanaryJob();

  
  
  void TrySyncSessionJob(JobPriority priority);
  void TrySyncSessionJobImpl(JobPriority priority);

  
  void Unthrottle();

  
  void TypeUnthrottle(base::TimeTicks unthrottle_time);

  
  void PerformDelayedNudge();

  
  void ExponentialBackoffRetry();

  
  void OnServerConnectionErrorFixed();

  
  void PollTimerCallback();

  
  ModelTypeSet GetEnabledAndUnthrottledTypes();

  
  
  
  void SendInitialSnapshot();

  
  
  
  
  void UpdateNudgeTimeRecords(ModelTypeSet types);

  virtual void OnActionableError(const sessions::SyncSessionSnapshot& snapshot);

  
  WeakHandle<SyncSchedulerImpl> weak_handle_this_;

  
  const std::string name_;

  
  bool started_;

  
  
  base::TimeDelta syncer_short_poll_interval_seconds_;
  base::TimeDelta syncer_long_poll_interval_seconds_;

  
  base::TimeDelta sessions_commit_delay_;

  
  base::RepeatingTimer<SyncSchedulerImpl> poll_timer_;

  
  Mode mode_;

  
  scoped_ptr<WaitInterval> wait_interval_;

  scoped_ptr<BackoffDelayProvider> delay_provider_;

  
  base::OneShotTimer<SyncSchedulerImpl> pending_wakeup_timer_;

  
  base::OneShotTimer<SyncSchedulerImpl> type_unthrottle_timer_;

  
  
  scoped_ptr<ConfigurationParams> pending_configure_params_;

  
  base::TimeTicks scheduled_nudge_time_;

  
  sessions::NudgeTracker nudge_tracker_;

  
  scoped_ptr<Syncer> syncer_;

  sessions::SyncSessionContext* session_context_;

  
  
  
  typedef std::map<ModelType, base::TimeTicks> ModelTypeTimeMap;
  ModelTypeTimeMap last_local_nudges_by_model_type_;

  
  
  
  
  
  bool no_scheduling_allowed_;

  
  
  
  
  
  
  
  bool do_poll_after_credentials_updated_;

  
  
  
  base::TimeTicks last_poll_reset_;

  base::WeakPtrFactory<SyncSchedulerImpl> weak_ptr_factory_;

  
  
  base::WeakPtrFactory<SyncSchedulerImpl> weak_ptr_factory_for_weak_handle_;

  DISALLOW_COPY_AND_ASSIGN(SyncSchedulerImpl);
};

}  

#endif  
