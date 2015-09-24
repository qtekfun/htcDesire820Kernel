// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SETTINGS_CROS_SETTINGS_H_
#define CHROME_BROWSER_CHROMEOS_SETTINGS_CROS_SETTINGS_H_

#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/callback_list.h"
#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "chromeos/settings/cros_settings_names.h"
#include "chromeos/settings/cros_settings_provider.h"

namespace base {
class DictionaryValue;
class ListValue;
class Value;
}

namespace chromeos {

class DeviceSettingsService;

class CrosSettings : public base::NonThreadSafe {
 public:
  
  static void Initialize();
  static bool IsInitialized();
  static void Shutdown();
  static CrosSettings* Get();

  
  
  explicit CrosSettings(DeviceSettingsService* device_settings_service);
  virtual ~CrosSettings();

  
  static bool IsCrosSettings(const std::string& path);

  
  void Set(const std::string& path, const base::Value& in_value);

  
  const base::Value* GetPref(const std::string& path) const;

  
  
  
  
  
  virtual CrosSettingsProvider::TrustedStatus PrepareTrustedValues(
      const base::Closure& callback) const;

  
  
  void SetBoolean(const std::string& path, bool in_value);
  void SetInteger(const std::string& path, int in_value);
  void SetDouble(const std::string& path, double in_value);
  void SetString(const std::string& path, const std::string& in_value);

  
  
  
  
  
  void AppendToList(const std::string& path, const base::Value* value);
  void RemoveFromList(const std::string& path, const base::Value* value);

  
  
  
  bool GetBoolean(const std::string& path, bool* out_value) const;
  bool GetInteger(const std::string& path, int* out_value) const;
  bool GetDouble(const std::string& path, double* out_value) const;
  bool GetString(const std::string& path, std::string* out_value) const;
  bool GetList(const std::string& path,
               const base::ListValue** out_value) const;
  bool GetDictionary(const std::string& path,
                     const base::DictionaryValue** out_value) const;

  
  
  
  
  bool FindEmailInList(const std::string& path,
                       const std::string& email,
                       bool* wildcard_match) const;

  
  bool AddSettingsProvider(CrosSettingsProvider* provider);
  bool RemoveSettingsProvider(CrosSettingsProvider* provider);

  
  typedef base::CallbackList<void(void)>::Subscription ObserverSubscription;
  scoped_ptr<ObserverSubscription> AddSettingsObserver(
      const std::string& path,
      const base::Closure& callback);

  
  CrosSettingsProvider* GetProvider(const std::string& path) const;

 private:
  friend class CrosSettingsTest;

  
  void FireObservers(const std::string& path);

  
  std::vector<CrosSettingsProvider*> providers_;

  
  
  typedef base::hash_map<std::string, base::CallbackList<void(void)>*>
      SettingsObserverMap;
  SettingsObserverMap settings_observers_;

  DISALLOW_COPY_AND_ASSIGN(CrosSettings);
};

class ScopedTestCrosSettings {
 public:
  ScopedTestCrosSettings();
  ~ScopedTestCrosSettings();

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedTestCrosSettings);
};

}  

#endif  
