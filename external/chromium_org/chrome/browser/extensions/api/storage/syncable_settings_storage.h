// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_STORAGE_SYNCABLE_SETTINGS_STORAGE_H_
#define CHROME_BROWSER_EXTENSIONS_API_STORAGE_SYNCABLE_SETTINGS_STORAGE_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list_threadsafe.h"
#include "base/values.h"
#include "chrome/browser/extensions/api/storage/setting_sync_data.h"
#include "chrome/browser/extensions/api/storage/settings_observer.h"
#include "chrome/browser/value_store/value_store.h"
#include "sync/api/sync_change.h"
#include "sync/api/syncable_service.h"

namespace extensions {

class SettingsSyncProcessor;

class SyncableSettingsStorage : public ValueStore {
 public:
  SyncableSettingsStorage(
      const scoped_refptr<SettingsObserverList>& observers,
      const std::string& extension_id,
      
      ValueStore* delegate);

  virtual ~SyncableSettingsStorage();

  
  virtual size_t GetBytesInUse(const std::string& key) OVERRIDE;
  virtual size_t GetBytesInUse(const std::vector<std::string>& keys) OVERRIDE;
  virtual size_t GetBytesInUse() OVERRIDE;
  virtual ReadResult Get(const std::string& key) OVERRIDE;
  virtual ReadResult Get(const std::vector<std::string>& keys) OVERRIDE;
  virtual ReadResult Get() OVERRIDE;
  virtual WriteResult Set(
      WriteOptions options,
      const std::string& key,
      const Value& value) OVERRIDE;
  virtual WriteResult Set(
      WriteOptions options, const base::DictionaryValue& values) OVERRIDE;
  virtual WriteResult Remove(const std::string& key) OVERRIDE;
  virtual WriteResult Remove(const std::vector<std::string>& keys) OVERRIDE;
  virtual WriteResult Clear() OVERRIDE;

  
  
  

  
  syncer::SyncError StartSyncing(
      const base::DictionaryValue& sync_state,
      scoped_ptr<SettingsSyncProcessor> sync_processor);

  
  void StopSyncing();

  
  syncer::SyncError ProcessSyncChanges(const SettingSyncDataList& sync_changes);

 private:
  
  void SyncResultIfEnabled(const ValueStore::WriteResult& result);

  
  syncer::SyncError SendLocalSettingsToSync(
      const base::DictionaryValue& settings);

  
  syncer::SyncError OverwriteLocalSettingsWithSync(
      const base::DictionaryValue& sync_state,
      const base::DictionaryValue& settings);

  
  
  syncer::SyncError OnSyncAdd(
      const std::string& key,
      Value* new_value,
      ValueStoreChangeList* changes);
  syncer::SyncError OnSyncUpdate(
      const std::string& key,
      Value* old_value,
      Value* new_value,
      ValueStoreChangeList* changes);
  syncer::SyncError OnSyncDelete(
      const std::string& key,
      Value* old_value,
      ValueStoreChangeList* changes);

  
  const scoped_refptr<SettingsObserverList> observers_;

  
  std::string const extension_id_;

  
  const scoped_ptr<ValueStore> delegate_;

  
  scoped_ptr<SettingsSyncProcessor> sync_processor_;

  DISALLOW_COPY_AND_ASSIGN(SyncableSettingsStorage);
};

}  

#endif  
