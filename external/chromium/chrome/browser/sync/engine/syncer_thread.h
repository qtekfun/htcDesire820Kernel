// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_BROWSER_SYNC_ENGINE_SYNCER_THREAD_H_
#define CHROME_BROWSER_SYNC_ENGINE_SYNCER_THREAD_H_
#pragma once

#include "base/callback.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/task.h"
#include "base/threading/thread.h"
#include "base/time.h"
#include "base/timer.h"
#include "chrome/browser/sync/engine/nudge_source.h"
#include "chrome/browser/sync/engine/polling_constants.h"
#include "chrome/browser/sync/engine/syncer.h"
#include "chrome/browser/sync/syncable/model_type_payload_map.h"
#include "chrome/browser/sync/engine/net/server_connection_manager.h"
#include "chrome/browser/sync/sessions/sync_session.h"
#include "chrome/browser/sync/sessions/sync_session_context.h"

namespace browser_sync {

struct ServerConnectionEvent;

class SyncerThread : public sessions::SyncSession::Delegate,
                     public ServerConnectionEventListener {
 public:
  enum Mode {
    
    
    
    
    CONFIGURATION_MODE,
    
    
    NORMAL_MODE,
  };

  
  SyncerThread(sessions::SyncSessionContext* context, Syncer* syncer);
  virtual ~SyncerThread();

  typedef Callback0::Type ModeChangeCallback;

  
  
  
  
  
  
  
  
  
  
  
  void Start(Mode mode, ModeChangeCallback* callback);

  
  
  void Stop();

  
  void ScheduleNudge(const base::TimeDelta& delay, NudgeSource source,
                     const syncable::ModelTypeBitSet& types,
                     const tracked_objects::Location& nudge_location);
  void ScheduleNudgeWithPayloads(
      const base::TimeDelta& delay, NudgeSource source,
      const syncable::ModelTypePayloadMap& types_with_payloads,
      const tracked_objects::Location& nudge_location);
  void ScheduleConfig(const syncable::ModelTypeBitSet& types);
  void ScheduleClearUserData();

  
  void set_notifications_enabled(bool notifications_enabled);

  
  
  
  static base::TimeDelta GetRecommendedDelay(const base::TimeDelta& base_delay);

  
  virtual void OnSilencedUntil(const base::TimeTicks& silenced_until);
  virtual bool IsSyncingCurrentlySilenced();
  virtual void OnReceivedShortPollIntervalUpdate(
      const base::TimeDelta& new_interval);
  virtual void OnReceivedLongPollIntervalUpdate(
      const base::TimeDelta& new_interval);
  virtual void OnShouldStopSyncingPermanently();

  
  
  virtual void OnServerConnectionEvent(const ServerConnectionEvent2& event);

 private:
  enum JobProcessDecision {
    
    CONTINUE,
    
    SAVE,
    
    DROP,
  };

  struct SyncSessionJob {
    
    enum SyncSessionJobPurpose {
      
      
      POLL,
      
      
      NUDGE,
      
      
      CLEAR_USER_DATA,
      
      
      
      CONFIGURATION,
    };
    SyncSessionJob();
    SyncSessionJob(SyncSessionJobPurpose purpose, base::TimeTicks start,
        linked_ptr<sessions::SyncSession> session, bool is_canary_job,
        const tracked_objects::Location& nudge_location);
    ~SyncSessionJob();
    SyncSessionJobPurpose purpose;
    base::TimeTicks scheduled_start;
    linked_ptr<sessions::SyncSession> session;
    bool is_canary_job;

    
    
    
    tracked_objects::Location nudge_location;
  };
  friend class SyncerThread2Test;
  friend class SyncerThread2WhiteboxTest;

  FRIEND_TEST_ALL_PREFIXES(SyncerThread2WhiteboxTest,
      DropNudgeWhileExponentialBackOff);
  FRIEND_TEST_ALL_PREFIXES(SyncerThread2WhiteboxTest, SaveNudge);
  FRIEND_TEST_ALL_PREFIXES(SyncerThread2WhiteboxTest, ContinueNudge);
  FRIEND_TEST_ALL_PREFIXES(SyncerThread2WhiteboxTest, DropPoll);
  FRIEND_TEST_ALL_PREFIXES(SyncerThread2WhiteboxTest, ContinuePoll);
  FRIEND_TEST_ALL_PREFIXES(SyncerThread2WhiteboxTest, ContinueConfiguration);
  FRIEND_TEST_ALL_PREFIXES(SyncerThread2WhiteboxTest,
                           SaveConfigurationWhileThrottled);
  FRIEND_TEST_ALL_PREFIXES(SyncerThread2WhiteboxTest,
                           SaveNudgeWhileThrottled);
  FRIEND_TEST_ALL_PREFIXES(SyncerThread2WhiteboxTest,
                           ContinueClearUserDataUnderAllCircumstances);
  FRIEND_TEST_ALL_PREFIXES(SyncerThread2WhiteboxTest,
                           ContinueCanaryJobConfig);
  FRIEND_TEST_ALL_PREFIXES(SyncerThread2WhiteboxTest,
      ContinueNudgeWhileExponentialBackOff);

  
  
  class DelayProvider {
   public:
    DelayProvider();
    virtual base::TimeDelta GetDelay(const base::TimeDelta& last_delay);
    virtual ~DelayProvider();
   private:
    DISALLOW_COPY_AND_ASSIGN(DelayProvider);
  };

  struct WaitInterval {
    enum Mode {
      
      
      
      EXPONENTIAL_BACKOFF,
      
      
      THROTTLED,
    };
    WaitInterval();
    ~WaitInterval();

    Mode mode;

    
    
    bool had_nudge;
    base::TimeDelta length;
    base::OneShotTimer<SyncerThread> timer;

    
    
    scoped_ptr<SyncSessionJob> pending_configure_job;
    WaitInterval(Mode mode, base::TimeDelta length);
  };

  
  void ScheduleSyncSessionJob(
      const base::TimeDelta& delay,
      SyncSessionJob::SyncSessionJobPurpose purpose,
      sessions::SyncSession* session,
      const tracked_objects::Location& nudge_location);

  
  void DoSyncSessionJob(const SyncSessionJob& job);

  
  
  void FinishSyncSessionJob(const SyncSessionJob& job);

  
  
  void UpdateCarryoverSessionState(const SyncSessionJob& old_job);

  
  void ScheduleNextSync(const SyncSessionJob& old_job);

  
  void AdjustPolling(const SyncSessionJob* old_job);

  
  void HandleConsecutiveContinuationError(const SyncSessionJob& old_job);

  
  
  
  bool ShouldRunJob(const SyncSessionJob& job);

  
  JobProcessDecision DecideOnJob(const SyncSessionJob& job);

  
  
  JobProcessDecision DecideWhileInWaitInterval(const SyncSessionJob& job);

  
  void SaveJob(const SyncSessionJob& job);

  
  void InitOrCoalescePendingJob(const SyncSessionJob& job);

  
  
  void StartImpl(Mode mode, ModeChangeCallback* callback);
  void ScheduleNudgeImpl(
      const base::TimeDelta& delay,
      sync_pb::GetUpdatesCallerInfo::GetUpdatesSource source,
      const syncable::ModelTypePayloadMap& types_with_payloads,
      bool is_canary_job, const tracked_objects::Location& nudge_location);
  void ScheduleConfigImpl(const ModelSafeRoutingInfo& routing_info,
      const std::vector<ModelSafeWorker*>& workers,
      const sync_pb::GetUpdatesCallerInfo::GetUpdatesSource source);
  void ScheduleClearUserDataImpl();

  
  bool IsBackingOff() const;

  
  void Notify(SyncEngineEvent::EventCause cause);

  
  void DoCanaryJob();
  void Unthrottle();

  
  
  
  void DoPendingJobIfPossible(bool is_canary_job);

  
  browser_sync::sessions::SyncSession* CreateSyncSession(
      const browser_sync::sessions::SyncSourceInfo& info);

  
  void PollTimerCallback();

  
  
  void SetSyncerStepsForPurpose(SyncSessionJob::SyncSessionJobPurpose purpose,
                                SyncerStep* start,
                                SyncerStep* end);

  
  void WatchConnectionManager();

  
  void CheckServerConnectionManagerStatus(
      HttpResponse::ServerConnectionCode code);

  
  
  
  void SendInitialSnapshot();

  base::Thread thread_;

  
  
  base::TimeDelta syncer_short_poll_interval_seconds_;
  base::TimeDelta syncer_long_poll_interval_seconds_;

  
  base::RepeatingTimer<SyncerThread> poll_timer_;

  
  Mode mode_;

  
  
  base::TimeTicks last_sync_session_end_time_;

  
  bool server_connection_ok_;

  
  scoped_ptr<SyncSessionJob> pending_nudge_;

  
  scoped_ptr<WaitInterval> wait_interval_;

  scoped_ptr<DelayProvider> delay_provider_;

  
  scoped_ptr<Syncer> syncer_;

  scoped_ptr<sessions::SyncSessionContext> session_context_;

  DISALLOW_COPY_AND_ASSIGN(SyncerThread);
};


}  

DISABLE_RUNNABLE_METHOD_REFCOUNT(browser_sync::SyncerThread);

#endif  
