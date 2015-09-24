// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MANAGED_MODE_MANAGED_USER_SYNC_SERVICE_H_
#define CHROME_BROWSER_MANAGED_MODE_MANAGED_USER_SYNC_SERVICE_H_

#include <vector>

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/managed_mode/managed_user_sync_service_observer.h"
#include "chrome/browser/managed_mode/managed_users.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "sync/api/syncable_service.h"

namespace base {
class DictionaryValue;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class PrefService;

class ManagedUserSyncService : public BrowserContextKeyedService,
                               public syncer::SyncableService {
 public:
  
  typedef base::Callback<void(const base::DictionaryValue*)>
      ManagedUsersCallback;

  
  static const char kAcknowledged[];
  static const char kChromeAvatar[];
  static const char kChromeOsAvatar[];
  static const char kMasterKey[];
  static const char kName[];

  
  static const int kNoAvatar;

  virtual ~ManagedUserSyncService();

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  
  
  
  
  
  
  static bool GetAvatarIndex(const std::string& avatar_str,
                             int* avatar_index);

  
  
  
  static std::string BuildAvatarString(int avatar_index);

  void AddObserver(ManagedUserSyncServiceObserver* observer);
  void RemoveObserver(ManagedUserSyncServiceObserver* observer);

  void AddManagedUser(const std::string& id,
                      const std::string& name,
                      const std::string& master_key,
                      int avatar_index);
  void DeleteManagedUser(const std::string& id);

  
  
  
  
  
  
  
  bool UpdateManagedUserAvatarIfNeeded(const std::string& id, int avatar_index);
  void ClearManagedUserAvatar(const std::string& id);

  
  
  
  
  
  const base::DictionaryValue* GetManagedUsers();

  
  
  void GetManagedUsersAsync(const ManagedUsersCallback& callback);

  
  virtual void Shutdown() OVERRIDE;

  
  virtual syncer::SyncMergeResult MergeDataAndStartSyncing(
      syncer::ModelType type,
      const syncer::SyncDataList& initial_sync_data,
      scoped_ptr<syncer::SyncChangeProcessor> sync_processor,
      scoped_ptr<syncer::SyncErrorFactory> error_handler) OVERRIDE;
  virtual void StopSyncing(syncer::ModelType type) OVERRIDE;
  virtual syncer::SyncDataList GetAllSyncData(syncer::ModelType type) const
      OVERRIDE;
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list) OVERRIDE;

 private:
  friend class ManagedUserSyncServiceFactory;

  
  
  explicit ManagedUserSyncService(PrefService* prefs);

  void OnLastSignedInUsernameChange();

  void NotifyManagedUserAcknowledged(const std::string& managed_user_id);
  void NotifyManagedUsersSyncingStopped();

  void DispatchCallbacks();

  PrefService* prefs_;
  PrefChangeRegistrar pref_change_registrar_;

  scoped_ptr<syncer::SyncChangeProcessor> sync_processor_;
  scoped_ptr<syncer::SyncErrorFactory> error_handler_;

  ObserverList<ManagedUserSyncServiceObserver> observers_;

  std::vector<ManagedUsersCallback> callbacks_;

  DISALLOW_COPY_AND_ASSIGN(ManagedUserSyncService);
};

#endif  
