// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_STORAGE_SETTINGS_FRONTEND_H_
#define CHROME_BROWSER_EXTENSIONS_API_STORAGE_SETTINGS_FRONTEND_H_

#include <map>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/api/storage/settings_namespace.h"
#include "chrome/browser/extensions/api/storage/settings_observer.h"
#include "chrome/browser/extensions/api/storage/settings_storage_factory.h"
#include "chrome/browser/extensions/api/storage/settings_storage_quota_enforcer.h"
#include "chrome/browser/extensions/api/storage/value_store_cache.h"
#include "sync/api/syncable_service.h"

class Profile;

namespace extensions {

class SettingsFrontend {
 public:
  
  static SettingsFrontend* Create(Profile* profile);

  
  static SettingsFrontend* Create(
      const scoped_refptr<SettingsStorageFactory>& storage_factory,
      Profile* profile);

  virtual ~SettingsFrontend();

  
  
  syncer::SyncableService* GetBackendForSync(syncer::ModelType type) const;

  
  bool IsStorageEnabled(settings_namespace::Namespace settings_namespace) const;

  
  
  void RunWithStorage(
      const std::string& extension_id,
      settings_namespace::Namespace settings_namespace,
      const ValueStoreCache::StorageCallback& callback);

  
  void DeleteStorageSoon(const std::string& extension_id);

  
  scoped_refptr<SettingsObserverList> GetObservers();

  void DisableStorageForTesting(
      settings_namespace::Namespace settings_namespace);

 private:
  typedef std::map<settings_namespace::Namespace, ValueStoreCache*> CacheMap;

  SettingsFrontend(
      const scoped_refptr<SettingsStorageFactory>& storage_factory,
      Profile* profile);

  
  
  const SettingsStorageQuotaEnforcer::Limits local_quota_limit_;
  const SettingsStorageQuotaEnforcer::Limits sync_quota_limit_;

  
  Profile* const profile_;

  
  scoped_refptr<SettingsObserverList> observers_;

  
  scoped_ptr<SettingsObserver> profile_observer_;

  
  
  CacheMap caches_;

  DISALLOW_COPY_AND_ASSIGN(SettingsFrontend);
};

}  

#endif  
