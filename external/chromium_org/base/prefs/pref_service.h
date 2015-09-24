// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


// information and MUST not be extracted, overwritten or modified except

#ifndef BASE_PREFS_PREF_SERVICE_H_
#define BASE_PREFS_PREF_SERVICE_H_

#include <set>
#include <string>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/base_prefs_export.h"
#include "base/prefs/persistent_pref_store.h"
#include "base/threading/non_thread_safe.h"
#include "base/values.h"

class PrefNotifier;
class PrefNotifierImpl;
class PrefObserver;
class PrefRegistry;
class PrefValueStore;
class PrefStore;

namespace base {
class FilePath;
}

namespace subtle {
class PrefMemberBase;
class ScopedUserPrefUpdateBase;
}

// extracted, overwritten or modified except through the defined APIs.
class BASE_PREFS_EXPORT PrefService : public base::NonThreadSafe {
 public:
  enum PrefInitializationStatus {
    INITIALIZATION_STATUS_WAITING,
    INITIALIZATION_STATUS_SUCCESS,
    INITIALIZATION_STATUS_CREATED_NEW_PREF_STORE,
    INITIALIZATION_STATUS_ERROR
  };

  
  class BASE_PREFS_EXPORT Preference {
   public:
    
    
    
    
    Preference(const PrefService* service,
               const char* name,
               base::Value::Type type);
    ~Preference() {}

    
    
    const std::string name() const;

    
    base::Value::Type GetType() const;

    
    
    const base::Value* GetValue() const;

    
    const base::Value* GetRecommendedValue() const;

    
    
    
    bool IsManaged() const;

    
    
    bool IsRecommended() const;

    
    
    bool HasExtensionSetting() const;

    
    
    bool HasUserSetting() const;

    
    
    bool IsExtensionControlled() const;

    
    
    bool IsUserControlled() const;

    
    
    
    bool IsDefaultValue() const;

    
    
    
    bool IsUserModifiable() const;

    
    
    
    bool IsExtensionModifiable() const;

   private:
    friend class PrefService;

    PrefValueStore* pref_value_store() const {
      return pref_service_->pref_value_store_.get();
    }

    const std::string name_;

    const base::Value::Type type_;

    
    const PrefService* pref_service_;
  };

  
  
  PrefService(
      PrefNotifierImpl* pref_notifier,
      PrefValueStore* pref_value_store,
      PersistentPrefStore* user_prefs,
      PrefRegistry* pref_registry,
      base::Callback<void(PersistentPrefStore::PrefReadError)>
          read_error_callback,
      bool async);
  virtual ~PrefService();

  
  
  void CommitPendingWrite();

  
  
  bool IsManagedPreference(const char* pref_name) const;

  
  
  bool IsUserModifiablePreference(const char* pref_name) const;

  
  
  const PrefService::Preference* FindPreference(const char* path) const;

  
  
  
  bool GetBoolean(const char* path) const;
  int GetInteger(const char* path) const;
  double GetDouble(const char* path) const;
  std::string GetString(const char* path) const;
  base::FilePath GetFilePath(const char* path) const;

  
  
  
  const base::DictionaryValue* GetDictionary(
      const char* path) const;
  const base::ListValue* GetList(const char* path) const;

  
  void ClearPref(const char* path);

  
  
  
  
  
  void Set(const char* path, const base::Value& value);
  void SetBoolean(const char* path, bool value);
  void SetInteger(const char* path, int value);
  void SetDouble(const char* path, double value);
  void SetString(const char* path, const std::string& value);
  void SetFilePath(const char* path, const base::FilePath& value);

  
  
  
  void SetInt64(const char* path, int64 value);
  int64 GetInt64(const char* path) const;

  
  void SetUint64(const char* path, uint64 value);
  uint64 GetUint64(const char* path) const;

  
  
  const base::Value* GetUserPrefValue(const char* path) const;

  
  
  
  
  void SetDefaultPrefValue(const char* path, base::Value* value);

  
  
  const base::Value* GetDefaultPrefValue(const char* path) const;

  
  
  
  
  bool HasPrefPath(const char* path) const;

  
  
  scoped_ptr<base::DictionaryValue> GetPreferenceValues() const;

  
  
  
  
  
  
  
  
  scoped_ptr<base::DictionaryValue> GetPreferenceValuesWithoutPathExpansion()
      const;

  bool ReadOnly() const;

  PrefInitializationStatus GetInitializationStatus() const;

  
  
  virtual void UpdateCommandLinePrefStore(PrefStore* command_line_store);

  
  
  
  void AddPrefInitObserver(base::Callback<void(bool)> callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  PrefRegistry* DeprecatedGetPrefRegistry();

 protected:
  
  
  
  scoped_ptr<PrefNotifierImpl> pref_notifier_;

  
  
  scoped_ptr<PrefValueStore> pref_value_store_;

  scoped_refptr<PrefRegistry> pref_registry_;

  
  scoped_refptr<PersistentPrefStore> user_pref_store_;

  
  base::Callback<void(PersistentPrefStore::PrefReadError)> read_error_callback_;

 private:
  
  
  
  
  typedef base::hash_map<std::string, Preference> PreferenceMap;

  
  friend class subtle::ScopedUserPrefUpdateBase;

  
  
  
  
  
  
  friend class PrefChangeRegistrar;
  friend class subtle::PrefMemberBase;

  
  
  
  
  
  
  
  
  
  virtual void AddPrefObserver(const char* path, PrefObserver* obs);
  virtual void RemovePrefObserver(const char* path, PrefObserver* obs);

  
  
  void ReportUserPrefChanged(const std::string& key);

  
  
  void SetUserPrefValue(const char* path, base::Value* new_value);

  
  
  void InitFromStorage(bool async);

  
  
  
  
  
  
  
  base::Value* GetMutableUserPref(const char* path,
                                  base::Value::Type type);

  
  
  
  
  
  const base::Value* GetPreferenceValue(const std::string& path) const;

  
  
  
  mutable PreferenceMap prefs_map_;

  DISALLOW_COPY_AND_ASSIGN(PrefService);
};

#endif  
