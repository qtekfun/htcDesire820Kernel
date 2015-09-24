// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_PROFILE_SYNC_SERVICE_HARNESS_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_PROFILE_SYNC_SERVICE_HARNESS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/sync/backend_migrator.h"
#include "chrome/browser/sync/profile_sync_service.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "chrome/browser/sync/test/integration/retry_verifier.h"
#include "sync/internal_api/public/base/model_type.h"

class Profile;
class StatusChangeChecker;

namespace invalidation {
class P2PInvalidationService;
}

namespace browser_sync {
namespace sessions {
class SyncSessionSnapshot;
}
}

class ProfileSyncServiceHarness
    : public ProfileSyncServiceObserver,
      public browser_sync::MigrationObserver {
 public:
  static ProfileSyncServiceHarness* Create(
      Profile* profile,
      const std::string& username,
      const std::string& password);

  static ProfileSyncServiceHarness* CreateForIntegrationTest(
      Profile* profile,
      const std::string& username,
      const std::string& password,
      invalidation::P2PInvalidationService* invalidation_service);

  virtual ~ProfileSyncServiceHarness();

  
  void SetCredentials(const std::string& username, const std::string& password);

  
  bool IsExponentialBackoffDone() const;

  
  bool IsSyncDisabled() const;

  
  bool HasAuthError() const;

  
  
  
  
  bool SetupSync();

  
  
  bool SetupSync(syncer::ModelTypeSet synced_datatypes);

  
  virtual void OnStateChanged() OVERRIDE;
  virtual void OnSyncCycleCompleted() OVERRIDE;

  
  virtual void OnMigrationStateChange() OVERRIDE;

  
  
  bool AwaitBackendInitialized();

  
  
  bool AwaitDataSyncCompletion();

  
  
  
  
  
  
  
  
  bool AwaitFullSyncCompletion();

  
  
  bool AwaitSyncDisabled();

  
  bool AwaitExponentialBackoffVerification();

  
  
  bool AwaitActionableError();

  
  bool AwaitMigration(syncer::ModelTypeSet expected_migrated_types);

  
  
  bool WaitUntilProgressMarkersMatch(ProfileSyncServiceHarness* partner);

  
  
  
  
  
  
  
  bool AwaitMutualSyncCycleCompletion(ProfileSyncServiceHarness* partner);

  
  
  
  
  bool AwaitGroupSyncCycleCompletion(
      std::vector<ProfileSyncServiceHarness*>& partners);

  
  
  
  
  static bool AwaitQuiescence(
      std::vector<ProfileSyncServiceHarness*>& clients);

  
  bool AwaitPassphraseRequired();

  
  
  bool AwaitPassphraseAccepted();

  
  ProfileSyncService* service() const { return service_; }

  
  ProfileSyncService::Status GetStatus() const;

  
  bool ServiceIsPushingChanges() const { return service_->ShouldPushChanges(); }

  
  bool EnableSyncForDatatype(syncer::ModelType datatype);

  
  bool DisableSyncForDatatype(syncer::ModelType datatype);

  
  bool EnableSyncForAllDatatypes();

  
  bool DisableSyncForAllDatatypes();

  
  syncer::sessions::SyncSessionSnapshot GetLastSessionSnapshot() const;

  
  
  
  bool EnableEncryption();

  
  
  bool WaitForEncryption();

  
  
  bool IsEncryptionComplete() const;

  
  bool IsTypeRunning(syncer::ModelType type);

  
  bool IsTypePreferred(syncer::ModelType type);

  
  bool IsDataSynced() const;

  
  
  
  
  
  
  
  bool IsFullySynced() const;

  
  
  size_t GetNumEntries() const;

  
  
  size_t GetNumDatatypes() const;

  
  bool AutoStartEnabled();

  
  
  
  
  
  bool AwaitStatusChange(StatusChangeChecker* checker,
                         const std::string& source);

  
  
  
  std::string GenerateFakeOAuth2RefreshTokenString();

  
  
  std::string GetClientInfoString(const std::string& message) const;

  
  
  bool MatchesPartnerClient() const;

  
  bool HasPendingBackendMigration() const;

 private:
  friend class StateChangeTimeoutEvent;

  ProfileSyncServiceHarness(
      Profile* profile,
      const std::string& username,
      const std::string& password,
      invalidation::P2PInvalidationService* invalidation_service);

  
  
  
  bool TryListeningToMigrationEvents();

  
  void SignalStateComplete();

  
  bool IsDataSyncedImpl() const;

  
  void FinishSyncSetup();

  
  
  
  std::string GetSerializedProgressMarker(syncer::ModelType model_type) const;

  
  std::string GetServiceStatus();

  
  Profile* profile_;

  
  ProfileSyncService* service_;

  
  invalidation::P2PInvalidationService* p2p_invalidation_service_;

  
  
  ProfileSyncServiceHarness* progress_marker_partner_;

  
  std::string username_;
  std::string password_;

  
  
  int oauth2_refesh_token_number_;

  
  
  syncer::ModelTypeSet pending_migration_types_;

  
  
  syncer::ModelTypeSet migrated_types_;

  
  const std::string profile_debug_name_;

  
  
  StatusChangeChecker* status_change_checker_;

  
  
  scoped_ptr<RetryVerifier> retry_verifier_;

  DISALLOW_COPY_AND_ASSIGN(ProfileSyncServiceHarness);
};

#endif  
