// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_PREF_SERVICE_SYNCABLE_H_
#define CHROME_BROWSER_PREFS_PREF_SERVICE_SYNCABLE_H_

#include "base/prefs/pref_service.h"
#include "chrome/browser/prefs/pref_model_associator.h"
#include "chrome/browser/prefs/synced_pref_observer.h"
#include "components/user_prefs/pref_registry_syncable.h"

class PrefServiceSyncableObserver;
class Profile;

namespace syncer {
class SyncableService;
}

class PrefServiceSyncable : public PrefService {
 public:
  
  
  
  
  
  
  
  
  
  static PrefServiceSyncable* FromProfile(Profile* profile);
  static PrefServiceSyncable* IncognitoFromProfile(Profile* profile);

  
  
  PrefServiceSyncable(
      PrefNotifierImpl* pref_notifier,
      PrefValueStore* pref_value_store,
      PersistentPrefStore* user_prefs,
      user_prefs::PrefRegistrySyncable* pref_registry,
      base::Callback<void(PersistentPrefStore::PrefReadError)>
          read_error_callback,
      bool async);
  virtual ~PrefServiceSyncable();

  
  
  
  
  PrefServiceSyncable* CreateIncognitoPrefService(
      PrefStore* incognito_extension_prefs);

  
  
  
  
  
  
  
  
  bool IsSyncing();

  
  
  bool IsPrioritySyncing();

  
  
  bool IsPrefSynced(const std::string& name) const;

  void AddObserver(PrefServiceSyncableObserver* observer);
  void RemoveObserver(PrefServiceSyncableObserver* observer);

  
  
  syncer::SyncableService* GetSyncableService(const syncer::ModelType& type);

  
  virtual void UpdateCommandLinePrefStore(PrefStore* cmd_line_store) OVERRIDE;

  void AddSyncedPrefObserver(const std::string& name,
                             SyncedPrefObserver* observer);
  void RemoveSyncedPrefObserver(const std::string& name,
                                SyncedPrefObserver* observer);

 private:
  friend class PrefModelAssociator;

  void AddRegisteredSyncablePreference(
      const char* path,
      const user_prefs::PrefRegistrySyncable::PrefSyncStatus sync_status);

  
  void OnIsSyncingChanged();

  
  
  void ProcessPrefChange(const std::string& name);

  
  
  bool pref_service_forked_;

  PrefModelAssociator pref_sync_associator_;
  PrefModelAssociator priority_pref_sync_associator_;

  ObserverList<PrefServiceSyncableObserver> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(PrefServiceSyncable);
};

#endif  
