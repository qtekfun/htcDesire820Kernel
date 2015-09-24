// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_PREF_MODEL_ASSOCIATOR_H_
#define CHROME_BROWSER_PREFS_PREF_MODEL_ASSOCIATOR_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "base/observer_list.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/prefs/synced_pref_observer.h"
#include "sync/api/sync_data.h"
#include "sync/api/syncable_service.h"

class PrefRegistrySyncable;
class PrefServiceSyncable;

namespace sync_pb {
class PreferenceSpecifics;
}

namespace base {
class Value;
}

class PrefModelAssociator
    : public syncer::SyncableService,
      public base::NonThreadSafe {
 public:
  explicit PrefModelAssociator(syncer::ModelType type);
  virtual ~PrefModelAssociator();

  
  bool models_associated() const { return models_associated_; }

  
  virtual syncer::SyncDataList GetAllSyncData(
      syncer::ModelType type) const OVERRIDE;
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list) OVERRIDE;
  virtual syncer::SyncMergeResult MergeDataAndStartSyncing(
      syncer::ModelType type,
      const syncer::SyncDataList& initial_sync_data,
      scoped_ptr<syncer::SyncChangeProcessor> sync_processor,
      scoped_ptr<syncer::SyncErrorFactory> sync_error_factory) OVERRIDE;
  virtual void StopSyncing(syncer::ModelType type) OVERRIDE;

  
  
  std::set<std::string> registered_preferences() const;

  
  
  
  
  
  virtual void RegisterPref(const char* name);

  
  virtual bool IsPrefRegistered(const char* name);

  
  
  virtual void ProcessPrefChange(const std::string& name);

  void SetPrefService(PrefServiceSyncable* pref_service);

  
  
  
  
  
  static scoped_ptr<base::Value> MergePreference(
      const std::string& name,
      const base::Value& local_value,
      const base::Value& server_value);

  
  
  bool CreatePrefSyncData(const std::string& name,
                          const base::Value& value,
                          syncer::SyncData* sync_data) const;

  
  base::Value* ReadPreferenceSpecifics(
      const sync_pb::PreferenceSpecifics& specifics,
      std::string* name);

  
  
  bool IsPrefSynced(const std::string& name) const;

  
  void AddSyncedPrefObserver(const std::string& name,
                             SyncedPrefObserver* observer);

  
  void RemoveSyncedPrefObserver(const std::string& name,
                                SyncedPrefObserver* observer);

 protected:
  friend class ProfileSyncServicePreferenceTest;

  typedef std::map<std::string, syncer::SyncData> SyncDataMap;

  
  
  
  
  
  
  
  
  
  
  void InitPrefAndAssociate(const syncer::SyncData& sync_pref,
                            const std::string& pref_name,
                            syncer::SyncChangeList* sync_changes,
                            SyncDataMap* migrated_preference_list);

  static base::Value* MergeListValues(
      const base::Value& from_value, const base::Value& to_value);
  static base::Value* MergeDictionaryValues(const base::Value& from_value,
                                            const base::Value& to_value);

  
  
  static bool IsMigratedPreference(const char* preference_name);
  static bool IsOldMigratedPreference(const char* old_preference_name);

  
  
  
  
  bool models_associated_;

  
  
  bool processing_syncer_changes_;

  
  typedef std::set<std::string> PreferenceSet;

  
  PreferenceSet registered_preferences_;

  
  
  
  
  
  
  
  PreferenceSet synced_preferences_;

  
  PrefServiceSyncable* pref_service_;

  
  scoped_ptr<syncer::SyncChangeProcessor> sync_processor_;

  
  scoped_ptr<syncer::SyncErrorFactory> sync_error_factory_;

  
  
  syncer::ModelType type_;

 private:
  
  
  
  typedef ObserverList<SyncedPrefObserver> SyncedPrefObserverList;
  typedef base::hash_map<std::string, SyncedPrefObserverList*>
      SyncedPrefObserverMap;

  void NotifySyncedPrefObservers(const std::string& path, bool from_sync) const;

  SyncedPrefObserverMap synced_pref_observers_;

  DISALLOW_COPY_AND_ASSIGN(PrefModelAssociator);
};

#endif  
