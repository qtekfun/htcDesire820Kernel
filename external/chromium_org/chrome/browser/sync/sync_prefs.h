// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNC_PREFS_H_
#define CHROME_BROWSER_SYNC_SYNC_PREFS_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_member.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/notifier/invalidation_state_tracker.h"

class PrefService;
class ProfileIOData;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace browser_sync {

class SyncPrefObserver {
 public:
  
  
  virtual void OnSyncManagedPrefChange(bool is_sync_managed) = 0;

 protected:
  virtual ~SyncPrefObserver();
};

class SyncPrefs : NON_EXPORTED_BASE(public base::NonThreadSafe),
                  public base::SupportsWeakPtr<SyncPrefs> {
 public:
  
  
  explicit SyncPrefs(PrefService* pref_service);

  virtual ~SyncPrefs();

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  void AddSyncPrefObserver(SyncPrefObserver* sync_pref_observer);
  void RemoveSyncPrefObserver(SyncPrefObserver* sync_pref_observer);

  
  void ClearPreferences();

  

  bool HasSyncSetupCompleted() const;
  void SetSyncSetupCompleted();

  bool SyncHasAuthError() const;
  void SetSyncAuthError(bool error);

  bool IsStartSuppressed() const;
  void SetStartSuppressed(bool is_suppressed);

  std::string GetGoogleServicesUsername() const;

  base::Time GetLastSyncedTime() const;
  void SetLastSyncedTime(base::Time time);

  bool HasKeepEverythingSynced() const;
  void SetKeepEverythingSynced(bool keep_everything_synced);

  
  
  
  syncer::ModelTypeSet GetPreferredDataTypes(
      syncer::ModelTypeSet registered_types) const;
  
  
  
  
  
  
  void SetPreferredDataTypes(
    syncer::ModelTypeSet registered_types,
    syncer::ModelTypeSet preferred_types);

  
  bool IsManaged() const;

  
  std::string GetEncryptionBootstrapToken() const;
  void SetEncryptionBootstrapToken(const std::string& token);

  
  
  std::string GetKeystoreEncryptionBootstrapToken() const;
  void SetKeystoreEncryptionBootstrapToken(const std::string& token);

  
  std::string GetSyncSessionsGUID() const;
  void SetSyncSessionsGUID(const std::string& guid);

  
  static const char* GetPrefNameForDataType(syncer::ModelType data_type);

#if defined(OS_CHROMEOS)
  
  
  std::string GetSpareBootstrapToken() const;
  void SetSpareBootstrapToken(const std::string& token);
#endif

  
  void AcknowledgeSyncedTypes(syncer::ModelTypeSet types);

  

  void SetManagedForTest(bool is_managed);
  syncer::ModelTypeSet GetAcknowledgeSyncedTypesForTest() const;

 private:
  void RegisterPrefGroups();

  static void RegisterDataTypePreferredPref(
      user_prefs::PrefRegistrySyncable* prefs,
      syncer::ModelType type,
      bool is_preferred);
  bool GetDataTypePreferred(syncer::ModelType type) const;
  void SetDataTypePreferred(syncer::ModelType type, bool is_preferred);

  
  
  syncer::ModelTypeSet ResolvePrefGroups(
      syncer::ModelTypeSet registered_types,
      syncer::ModelTypeSet types) const;

  void OnSyncManagedPrefChanged();

  
  PrefService* const pref_service_;

  ObserverList<SyncPrefObserver> sync_pref_observers_;

  
  
  BooleanPrefMember pref_sync_managed_;

  
  
  
  
  
  
  
  typedef std::map<syncer::ModelType, syncer::ModelTypeSet> PrefGroupsMap;
  PrefGroupsMap pref_groups_;

  DISALLOW_COPY_AND_ASSIGN(SyncPrefs);
};

}  

#endif  
