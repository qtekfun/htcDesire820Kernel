// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_USER_PREFS_PREF_REGISTRY_SYNCABLE_H_
#define COMPONENTS_USER_PREFS_PREF_REGISTRY_SYNCABLE_H_

#include <set>
#include <string>

#include "base/callback.h"
#include "base/prefs/pref_registry.h"
#include "components/user_prefs/user_prefs_export.h"

namespace base {
class DictionaryValue;
class FilePath;
class ListValue;
class Value;
}

namespace user_prefs {

class USER_PREFS_EXPORT PrefRegistrySyncable : public PrefRegistry {
 public:
  
  
  
  
  
  enum PrefSyncStatus {
    UNSYNCABLE_PREF,
    SYNCABLE_PREF,
    SYNCABLE_PRIORITY_PREF,
  };

  typedef
      base::Callback<void(const char* path, const PrefSyncStatus sync_status)>
          SyncableRegistrationCallback;

  PrefRegistrySyncable();

  typedef std::map<std::string, PrefSyncStatus> PrefToStatus;

  
  const PrefToStatus& syncable_preferences() const;

  
  
  
  
  
  
  
  void SetSyncableRegistrationCallback(const SyncableRegistrationCallback& cb);

  void RegisterBooleanPref(const char* path,
                           bool default_value,
                           PrefSyncStatus sync_status);
  void RegisterIntegerPref(const char* path,
                           int default_value,
                           PrefSyncStatus sync_status);
  void RegisterDoublePref(const char* path,
                          double default_value,
                          PrefSyncStatus sync_status);
  void RegisterStringPref(const char* path,
                          const std::string& default_value,
                          PrefSyncStatus sync_status);
  void RegisterFilePathPref(const char* path,
                            const base::FilePath& default_value,
                            PrefSyncStatus sync_status);
  void RegisterListPref(const char* path,
                        PrefSyncStatus sync_status);
  void RegisterDictionaryPref(const char* path,
                              PrefSyncStatus sync_status);
  void RegisterListPref(const char* path,
                        base::ListValue* default_value,
                        PrefSyncStatus sync_status);
  void RegisterDictionaryPref(const char* path,
                              base::DictionaryValue* default_value,
                              PrefSyncStatus sync_status);
  void RegisterLocalizedBooleanPref(const char* path,
                                    int locale_default_message_id,
                                    PrefSyncStatus sync_status);
  void RegisterLocalizedIntegerPref(const char* path,
                                    int locale_default_message_id,
                                    PrefSyncStatus sync_status);
  void RegisterLocalizedDoublePref(const char* path,
                                   int locale_default_message_id,
                                   PrefSyncStatus sync_status);
  void RegisterLocalizedStringPref(const char* path,
                                   int locale_default_message_id,
                                   PrefSyncStatus sync_status);
  void RegisterInt64Pref(const char* path,
                         int64 default_value,
                         PrefSyncStatus sync_status);
  void RegisterUint64Pref(const char* path,
                          uint64 default_value,
                          PrefSyncStatus sync_status);

  
  
  scoped_refptr<PrefRegistrySyncable> ForkForIncognito();

 private:
  virtual ~PrefRegistrySyncable();

  void RegisterSyncablePreference(const char* path,
                                  base::Value* default_value,
                                  PrefSyncStatus sync_status);

  SyncableRegistrationCallback callback_;

  
  PrefToStatus syncable_preferences_;

  DISALLOW_COPY_AND_ASSIGN(PrefRegistrySyncable);
};

}  

#endif  
