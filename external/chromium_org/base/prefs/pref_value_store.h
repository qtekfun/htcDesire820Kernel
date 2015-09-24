// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PREFS_PREF_VALUE_STORE_H_
#define BASE_PREFS_PREF_VALUE_STORE_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/prefs/base_prefs_export.h"
#include "base/prefs/pref_store.h"
#include "base/values.h"

class PrefNotifier;
class PrefStore;

class BASE_PREFS_EXPORT PrefValueStore {
 public:
  typedef base::Callback<void(const std::string&)> PrefChangedCallback;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  PrefValueStore(PrefStore* managed_prefs,
                 PrefStore* supervised_user_prefs,
                 PrefStore* extension_prefs,
                 PrefStore* command_line_prefs,
                 PrefStore* user_prefs,
                 PrefStore* recommended_prefs,
                 PrefStore* default_prefs,
                 PrefNotifier* pref_notifier);
  virtual ~PrefValueStore();

  // Creates a clone of this PrefValueStore with PrefStores overwritten
  
  PrefValueStore* CloneAndSpecialize(PrefStore* managed_prefs,
                                     PrefStore* supervised_user_prefs,
                                     PrefStore* extension_prefs,
                                     PrefStore* command_line_prefs,
                                     PrefStore* user_prefs,
                                     PrefStore* recommended_prefs,
                                     PrefStore* default_prefs,
                                     PrefNotifier* pref_notifier);

  
  
  
  
  void set_callback(const PrefChangedCallback& callback);

  
  
  
  
  
  bool GetValue(const std::string& name,
                base::Value::Type type,
                const base::Value** out_value) const;

  
  
  
  
  
  bool GetRecommendedValue(const std::string& name,
                           base::Value::Type type,
                           const base::Value** out_value) const;

  
  
  
  bool PrefValueInManagedStore(const char* name) const;
  bool PrefValueInExtensionStore(const char* name) const;
  bool PrefValueInUserStore(const char* name) const;

  
  
  
  bool PrefValueFromExtensionStore(const char* name) const;
  bool PrefValueFromUserStore(const char* name) const;
  bool PrefValueFromRecommendedStore(const char* name) const;
  bool PrefValueFromDefaultStore(const char* name) const;

  
  
  bool PrefValueUserModifiable(const char* name) const;

  
  
  bool PrefValueExtensionModifiable(const char* name) const;

  
  void UpdateCommandLinePrefStore(PrefStore* command_line_prefs);

 private:
  
  
  
  
  
  
  
  
  
  
  enum PrefStoreType {
    
    
    INVALID_STORE = -1,
    MANAGED_STORE = 0,
    SUPERVISED_USER_STORE,
    EXTENSION_STORE,
    COMMAND_LINE_STORE,
    USER_STORE,
    RECOMMENDED_STORE,
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
    
    virtual void OnPrefValueChanged(const std::string& key) OVERRIDE;
    virtual void OnInitializationCompleted(bool succeeded) OVERRIDE;

    
    PrefValueStore* pref_value_store_;

    
    scoped_refptr<PrefStore> pref_store_;

    
    PrefStoreType type_;

    DISALLOW_COPY_AND_ASSIGN(PrefStoreKeeper);
  };

  typedef std::map<std::string, base::Value::Type> PrefTypeMap;

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
                         const base::Value** out_value) const;

  
  bool GetValueFromStoreWithType(const char* name,
                                 base::Value::Type type,
                                 PrefStoreType store,
                                 const base::Value** out_value) const;

  
  
  
  
  void NotifyPrefChanged(const char* path, PrefStoreType new_store);

  
  
  void OnPrefValueChanged(PrefStoreType type, const std::string& key);

  
  void OnInitializationCompleted(PrefStoreType type, bool succeeded);

  
  
  void InitPrefStore(PrefStoreType type, PrefStore* pref_store);

  
  
  void CheckInitializationCompleted();

  
  
  PrefStore* GetPrefStore(PrefStoreType type) {
    return pref_stores_[type].store();
  }
  const PrefStore* GetPrefStore(PrefStoreType type) const {
    return pref_stores_[type].store();
  }

  
  PrefStoreKeeper pref_stores_[PREF_STORE_TYPE_MAX + 1];

  PrefChangedCallback pref_changed_callback_;

  
  
  PrefNotifier* pref_notifier_;

  
  PrefTypeMap pref_types_;

  
  bool initialization_failed_;

  DISALLOW_COPY_AND_ASSIGN(PrefValueStore);
};

#endif  
