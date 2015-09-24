// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_PREF_VALUE_STORE_H_
#define CHROME_BROWSER_PREFS_PREF_VALUE_STORE_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/values.h"
#include "chrome/common/pref_store.h"
#include "content/browser/browser_thread.h"

class FilePath;
class PrefNotifier;
class PrefStore;

class PrefValueStore {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  PrefValueStore(PrefStore* managed_platform_prefs,
                 PrefStore* managed_cloud_prefs,
                 PrefStore* extension_prefs,
                 PrefStore* command_line_prefs,
                 PrefStore* user_prefs,
                 PrefStore* recommended_platform_prefs,
                 PrefStore* recommended_cloud_prefs,
                 PrefStore* default_prefs,
                 PrefNotifier* pref_notifier);
  virtual ~PrefValueStore();

  // Creates a clone of this PrefValueStore with PrefStores overwritten
  
  PrefValueStore* CloneAndSpecialize(PrefStore* managed_platform_prefs,
                                     PrefStore* managed_cloud_prefs,
                                     PrefStore* extension_prefs,
                                     PrefStore* command_line_prefs,
                                     PrefStore* user_prefs,
                                     PrefStore* recommended_platform_prefs,
                                     PrefStore* recommended_cloud_prefs,
                                     PrefStore* default_prefs,
                                     PrefNotifier* pref_notifier);

  
  
  
  
  
  bool GetValue(const std::string& name,
                Value::ValueType type,
                const Value** out_value) const;

  
  
  
  bool PrefValueInManagedStore(const char* name) const;
  bool PrefValueInExtensionStore(const char* name) const;
  bool PrefValueInUserStore(const char* name) const;

  
  
  
  bool PrefValueFromExtensionStore(const char* name) const;
  bool PrefValueFromUserStore(const char* name) const;
  bool PrefValueFromDefaultStore(const char* name) const;

  
  
  bool PrefValueUserModifiable(const char* name) const;

  
  
  bool PrefValueExtensionModifiable(const char* name) const;

 private:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  enum PrefStoreType {
    
    
    INVALID_STORE = -1,
    MANAGED_PLATFORM_STORE = 0,
    MANAGED_CLOUD_STORE,
    EXTENSION_STORE,
    COMMAND_LINE_STORE,
    USER_STORE,
    RECOMMENDED_PLATFORM_STORE,
    RECOMMENDED_CLOUD_STORE,
    DEFAULT_STORE,
    PREF_STORE_TYPE_MAX = DEFAULT_STORE
  };

  
  
  
  
  class PrefStoreKeeper : public PrefStore::Observer {
   public:
    PrefStoreKeeper();
    virtual ~PrefStoreKeeper();

    
    void Initialize(PrefValueStore* store,
                    PrefStore* pref_store,
                    PrefStoreType type);

    PrefStore* store() { return pref_store_.get(); }
    const PrefStore* store() const { return pref_store_.get(); }

   private:
    
    virtual void OnPrefValueChanged(const std::string& key);
    virtual void OnInitializationCompleted();

    
    PrefValueStore* pref_value_store_;

    
    scoped_refptr<PrefStore> pref_store_;

    
    PrefStoreType type_;

    DISALLOW_COPY_AND_ASSIGN(PrefStoreKeeper);
  };

  typedef std::map<std::string, Value::ValueType> PrefTypeMap;

  friend class PrefValueStorePolicyRefreshTest;
  FRIEND_TEST_ALL_PREFIXES(PrefValueStorePolicyRefreshTest, TestPolicyRefresh);
  FRIEND_TEST_ALL_PREFIXES(PrefValueStorePolicyRefreshTest,
                           TestRefreshPolicyPrefsCompletion);
  FRIEND_TEST_ALL_PREFIXES(PrefValueStorePolicyRefreshTest,
                           TestConcurrentPolicyRefresh);

  
  
  
  bool PrefValueInStore(const char* name, PrefStoreType store) const;

  
  
  
  
  bool PrefValueInStoreRange(const char* name,
                             PrefStoreType first_checked_store,
                             PrefStoreType last_checked_store) const;

  
  
  
  
  
  PrefStoreType ControllingPrefStoreForPref(const char* name) const;

  
  bool GetValueFromStore(const char* name,
                         PrefStoreType store,
                         const Value** out_value) const;

  
  
  
  
  void NotifyPrefChanged(const char* path, PrefStoreType new_store);

  
  
  void OnPrefValueChanged(PrefStoreType type, const std::string& key);

  
  void OnInitializationCompleted(PrefStoreType type);

  
  
  void InitPrefStore(PrefStoreType type, PrefStore* pref_store);

  
  
  void CheckInitializationCompleted();

  
  
  PrefStore* GetPrefStore(PrefStoreType type) {
    return pref_stores_[type].store();
  }
  const PrefStore* GetPrefStore(PrefStoreType type) const {
    return pref_stores_[type].store();
  }

  
  PrefStoreKeeper pref_stores_[PREF_STORE_TYPE_MAX + 1];

  
  
  
  PrefNotifier* pref_notifier_;

  
  PrefTypeMap pref_types_;

  DISALLOW_COPY_AND_ASSIGN(PrefValueStore);
};

#endif  
