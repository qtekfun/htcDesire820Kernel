// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_PROFILE_SYNC_SERVICE_HARNESS_H_
#define CHROME_BROWSER_SYNC_PROFILE_SYNC_SERVICE_HARNESS_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/sync/profile_sync_service.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "chrome/browser/sync/syncable/model_type.h"

class Profile;

namespace browser_sync {
  namespace sessions {
    struct SyncSessionSnapshot;
  }
}

class ProfileSyncServiceHarness : public ProfileSyncServiceObserver {
 public:
  ProfileSyncServiceHarness(Profile* profile,
                            const std::string& username,
                            const std::string& password,
                            int id);

  virtual ~ProfileSyncServiceHarness() {}

  
  
  
  static ProfileSyncServiceHarness* CreateAndAttach(Profile* profile);

  
  void SetCredentials(const std::string& username, const std::string& password);

  
  bool IsSyncAlreadySetup();

  
  
  
  
  bool SetupSync();

  
  
  bool SetupSync(const syncable::ModelTypeSet& synced_datatypes);

  
  virtual void OnStateChanged();

  
  
  bool AwaitSyncCycleCompletion(const std::string& reason);

  
  
  bool WaitUntilTimestampMatches(
      ProfileSyncServiceHarness* partner, const std::string& reason);

  
  
  
  
  
  
  
  bool AwaitMutualSyncCycleCompletion(ProfileSyncServiceHarness* partner);

  
  
  
  
  bool AwaitGroupSyncCycleCompletion(
      std::vector<ProfileSyncServiceHarness*>& partners);

  
  
  
  
  static bool AwaitQuiescence(
      std::vector<ProfileSyncServiceHarness*>& clients);

  
  
  bool AwaitPassphraseAccepted();

  
  ProfileSyncService* service() { return service_; }

  
  ProfileSyncService::Status GetStatus();

  
  bool ServiceIsPushingChanges() { return service_->ShouldPushChanges(); }

  
  void EnableSyncForDatatype(syncable::ModelType datatype);

  
  void DisableSyncForDatatype(syncable::ModelType datatype);

  
  void EnableSyncForAllDatatypes();

  
  void DisableSyncForAllDatatypes();

  
  const browser_sync::sessions::SyncSessionSnapshot*
      GetLastSessionSnapshot() const;

  
  
  
  
  
  
  bool EnableEncryptionForType(syncable::ModelType type);

  
  bool IsTypeEncrypted(syncable::ModelType type);

 private:
  friend class StateChangeTimeoutEvent;

  enum WaitState {
    
    INITIAL_WAIT_STATE = 0,

    
    WAITING_FOR_ON_BACKEND_INITIALIZED,

    
    WAITING_FOR_INITIAL_SYNC,

    
    WAITING_FOR_SYNC_TO_FINISH,

    
    WAITING_FOR_UPDATES,

    
    
    WAITING_FOR_PASSPHRASE_ACCEPTED,

    
    WAITING_FOR_ENCRYPTION,

    
    SERVER_UNREACHABLE,

    
    FULLY_SYNCED,

    
    SYNC_DISABLED,

    NUMBER_OF_STATES,
  };

  
  void SignalStateCompleteWithNextState(WaitState next_state);

  
  void SignalStateComplete();

  
  
  bool RunStateChangeMachine();

  
  bool AwaitStatusChangeWithTimeout(int timeout_milliseconds,
                                    const std::string& reason);

  
  bool IsSynced();

  
  
  bool MatchesOtherClient(ProfileSyncServiceHarness* partner);

  
  void LogClientInfo(const std::string& message);

  
  
  std::string GetUpdatedTimestamp(syncable::ModelType model_type);

  
  
  syncable::ModelType waiting_for_encryption_type_;

  WaitState wait_state_;

  Profile* profile_;
  ProfileSyncService* service_;

  
  
  ProfileSyncServiceHarness* timestamp_match_partner_;

  
  std::string username_;
  std::string password_;

  
  int id_;

  DISALLOW_COPY_AND_ASSIGN(ProfileSyncServiceHarness);
};

#endif  
