// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_STORAGE_MANAGED_VALUE_STORE_CACHE_H_
#define CHROME_BROWSER_EXTENSIONS_API_STORAGE_MANAGED_VALUE_STORE_CACHE_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/api/storage/settings_observer.h"
#include "chrome/browser/extensions/api/storage/value_store_cache.h"
#include "components/policy/core/common/policy_service.h"

class Profile;

namespace policy {
class PolicyMap;
}

namespace extensions {

class PolicyValueStore;
class SettingsStorageFactory;

class ManagedValueStoreCache : public ValueStoreCache,
                               public policy::PolicyService::Observer {
 public:
  
  
  
  ManagedValueStoreCache(Profile* profile,
                         const scoped_refptr<SettingsStorageFactory>& factory,
                         const scoped_refptr<SettingsObserverList>& observers);
  virtual ~ManagedValueStoreCache();

 private:
  class ExtensionTracker;

  
  typedef std::map<std::string, linked_ptr<PolicyValueStore> >
      PolicyValueStoreMap;

  
  virtual void ShutdownOnUI() OVERRIDE;
  virtual void RunWithValueStoreForExtension(
      const StorageCallback& callback,
      scoped_refptr<const Extension> extension) OVERRIDE;
  virtual void DeleteStorageSoon(const std::string& extension_id) OVERRIDE;

  
  virtual void OnPolicyServiceInitialized(policy::PolicyDomain domain) OVERRIDE;
  virtual void OnPolicyUpdated(const policy::PolicyNamespace& ns,
                               const policy::PolicyMap& previous,
                               const policy::PolicyMap& current) OVERRIDE;

  
  
  void UpdatePolicyOnFILE(const std::string& extension_id,
                          scoped_ptr<policy::PolicyMap> current_policy);

  
  PolicyValueStore* GetStoreFor(const std::string& extension_id);

  
  bool HasStore(const std::string& extension_id) const;

  
  
  Profile* profile_;

  
  policy::PolicyService* policy_service_;

  
  
  scoped_ptr<ExtensionTracker> extension_tracker_;

  
  scoped_refptr<SettingsStorageFactory> storage_factory_;
  scoped_refptr<SettingsObserverList> observers_;
  base::FilePath base_path_;

  
  
  PolicyValueStoreMap store_map_;

  DISALLOW_COPY_AND_ASSIGN(ManagedValueStoreCache);
};

}  

#endif  
