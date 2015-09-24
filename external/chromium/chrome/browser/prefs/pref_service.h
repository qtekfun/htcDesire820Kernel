// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_PREFS_PREF_SERVICE_H_
#define CHROME_BROWSER_PREFS_PREF_SERVICE_H_
#pragma once

#include <set>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/values.h"
#include "chrome/common/json_pref_store.h"

class DefaultPrefStore;
class FilePath;
class NotificationObserver;
class PersistentPrefStore;
class PrefChangeObserver;
class PrefNotifier;
class PrefNotifierImpl;
class PrefStore;
class PrefValueStore;
class Profile;

namespace subtle {
class PrefMemberBase;
class ScopedUserPrefUpdateBase;
};

class PrefService : public base::NonThreadSafe,
                    public JsonPrefStore::Delegate {
 public:
  
  class Preference {
   public:

    
    
    
    
    Preference(const PrefService* service,
               const char* name,
               Value::ValueType type);
    ~Preference() {}

    
    
    const std::string name() const { return name_; }

    
    Value::ValueType GetType() const;

    
    
    const Value* GetValue() const;

    
    
    
    bool IsManaged() const;

    
    
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

    std::string name_;

    Value::ValueType type_;

    
    const PrefService* pref_service_;

    DISALLOW_COPY_AND_ASSIGN(Preference);
  };

  class Delegate {
   public:
    virtual void OnPrefsLoaded(PrefService* prefs, bool success) = 0;
#ifdef ANDROID
    virtual ~Delegate() {};
#endif
  };

  
  virtual void OnPrefsRead(PersistentPrefStore::PrefReadError error,
                           bool no_dir);

  
  
  
  
  
  
  
  static PrefService* CreatePrefService(const FilePath& pref_filename,
                                        PrefStore* extension_pref_store,
                                        Profile* profile);

  
  static PrefService* CreatePrefServiceAsync(const FilePath& pref_filename,
                                             PrefStore* extension_pref_store,
                                             Profile* profile,
                                             Delegate* delegate);

  
  
  
  
  PrefService* CreateIncognitoPrefService(PrefStore* incognito_extension_prefs);

  virtual ~PrefService();

  
  
  
  bool ReloadPersistentPrefs();

  
  
  bool IsManagedPreference(const char* pref_name) const;

  
  
  
  
  
  bool SavePersistentPrefs();

  
  void ScheduleSavePersistentPrefs();

  
  void CommitPendingWrite();

  
  void RegisterBooleanPref(const char* path, bool default_value);
  void RegisterIntegerPref(const char* path, int default_value);
  void RegisterDoublePref(const char* path, double default_value);
  void RegisterStringPref(const char* path, const std::string& default_value);
  void RegisterFilePathPref(const char* path, const FilePath& default_value);
  void RegisterListPref(const char* path);
  void RegisterDictionaryPref(const char* path);
  
  void RegisterListPref(const char* path, ListValue* default_value);
  void RegisterDictionaryPref(const char* path, DictionaryValue* default_value);

  
  void RegisterLocalizedBooleanPref(const char* path,
                                    int locale_default_message_id);
  void RegisterLocalizedIntegerPref(const char* path,
                                    int locale_default_message_id);
  void RegisterLocalizedDoublePref(const char* path,
                                   int locale_default_message_id);
  void RegisterLocalizedStringPref(const char* path,
                                   int locale_default_message_id);

  
  
  
  bool GetBoolean(const char* path) const;
  int GetInteger(const char* path) const;
  double GetDouble(const char* path) const;
  std::string GetString(const char* path) const;
  FilePath GetFilePath(const char* path) const;

  
  
  
  const DictionaryValue* GetDictionary(const char* path) const;
  const ListValue* GetList(const char* path) const;

  
  void ClearPref(const char* path);

  
  
  
  
  
  void Set(const char* path, const Value& value);
  void SetBoolean(const char* path, bool value);
  void SetInteger(const char* path, int value);
  void SetDouble(const char* path, double value);
  void SetString(const char* path, const std::string& value);
  void SetFilePath(const char* path, const FilePath& value);
  
  
  void SetList(const char* path, ListValue* value);

  
  
  
  void SetInt64(const char* path, int64 value);
  int64 GetInt64(const char* path) const;
  void RegisterInt64Pref(const char* path, int64 default_value);

  
  
  
  
  bool HasPrefPath(const char* path) const;

  
  
  DictionaryValue* GetPreferenceValues() const;

  
  
  const Preference* FindPreference(const char* pref_name) const;

  bool ReadOnly() const;

 protected:
  
  
  
  PrefService(PrefStore* managed_platform_prefs,
              PrefStore* managed_cloud_prefs,
              PrefStore* extension_prefs,
              PrefStore* command_line_prefs,
              PersistentPrefStore* user_prefs,
              PrefStore* recommended_platform_prefs,
              PrefStore* recommended_cloud_prefs,
              DefaultPrefStore* default_store,
              Delegate* delegate);

  
  
  
  scoped_ptr<PrefNotifierImpl> pref_notifier_;

 private:
  class PreferencePathComparator {
   public:
    bool operator() (Preference* lhs, Preference* rhs) const {
      return lhs->name() < rhs->name();
    }
  };
  typedef std::set<Preference*, PreferencePathComparator> PreferenceSet;

  friend class PrefServiceMockBuilder;

  
  
  
  
  
  
  friend class PrefChangeRegistrar;
  friend class subtle::PrefMemberBase;

  
  friend class subtle::ScopedUserPrefUpdateBase;

  
  
  PrefService(const PrefService& original,
              PrefStore* incognito_extension_prefs);

  
  
  void ReportUserPrefChanged(const std::string& key);

  
  
  
  
  virtual void AddPrefObserver(const char* path, NotificationObserver* obs);
  virtual void RemovePrefObserver(const char* path, NotificationObserver* obs);

  
  
  
  
  void RegisterPreference(const char* path, Value* default_value);

  
  
  void SetUserPrefValue(const char* path, Value* new_value);

  
  
  void InitFromStorage();

  
  
  
  
  
  
  
  Value* GetMutableUserPref(const char* path, Value::ValueType type);

  
  
  scoped_ptr<PrefValueStore> pref_value_store_;

  
  scoped_refptr<PersistentPrefStore> user_pref_store_;
  scoped_refptr<DefaultPrefStore> default_store_;

  
  
  
  mutable PreferenceSet prefs_;

  
  
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(PrefService);
};

#endif  
