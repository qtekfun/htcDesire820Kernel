// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MANAGED_MODE_MANAGED_USER_SETTINGS_SERVICE_H_
#define CHROME_BROWSER_MANAGED_MODE_MANAGED_USER_SETTINGS_SERVICE_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_store.h"
#include "base/values.h"
#include "chrome/browser/managed_mode/managed_users.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "sync/api/syncable_service.h"

class PersistentPrefStore;
class Profile;

namespace base {
class FilePath;
class SequencedTaskRunner;
}

class ManagedUserSettingsService : public BrowserContextKeyedService,
                                   public syncer::SyncableService,
                                   public PrefStore::Observer {
 public:
  
  
  
  typedef base::Callback<void(const base::DictionaryValue*)> SettingsCallback;

  ManagedUserSettingsService();
  virtual ~ManagedUserSettingsService();

  
  
  
  
  void Init(base::FilePath profile_path,
            base::SequencedTaskRunner* sequenced_task_runner,
            bool load_synchronously);

  
  
  
  void Init(scoped_refptr<PersistentPrefStore> pref_store);

  
  
  void Subscribe(const SettingsCallback& callback);

  
  void Activate();

  
  bool IsReady();

  
  void Clear();

  
  
  static std::string MakeSplitSettingKey(const std::string& prefix,
                                         const std::string& key);

  
  
  
  
  
  void UploadItem(const std::string& key, scoped_ptr<base::Value> value);

  
  void SetLocalSettingForTesting(const std::string& key,
                                 scoped_ptr<base::Value> value);

  
  static syncer::SyncData CreateSyncDataForSetting(const std::string& name,
                                                   const base::Value& value);

  
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

  
  virtual void OnPrefValueChanged(const std::string& key) OVERRIDE;
  virtual void OnInitializationCompleted(bool success) OVERRIDE;

 private:
  base::DictionaryValue* GetOrCreateDictionary(const std::string& key) const;
  base::DictionaryValue* GetAtomicSettings() const;
  base::DictionaryValue* GetSplitSettings() const;
  base::DictionaryValue* GetQueuedItems() const;

  
  
  
  
  base::DictionaryValue* GetDictionaryAndSplitKey(std::string* key) const;

  
  
  scoped_ptr<base::DictionaryValue> GetSettings();

  
  
  void InformSubscribers();

  
  
  scoped_refptr<PersistentPrefStore> store_;

  bool active_;

  
  scoped_ptr<base::DictionaryValue> local_settings_;

  std::vector<SettingsCallback> subscribers_;

  scoped_ptr<syncer::SyncChangeProcessor> sync_processor_;
  scoped_ptr<syncer::SyncErrorFactory> error_handler_;

  DISALLOW_COPY_AND_ASSIGN(ManagedUserSettingsService);
};

#endif  
