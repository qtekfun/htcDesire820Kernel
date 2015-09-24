// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_SETTINGS_H_
#define CHROME_BROWSER_CHROMEOS_CROS_SETTINGS_H_
#pragma once

#include <string>
#include <vector>

#include "base/hash_tables.h"
#include "base/memory/singleton.h"
#include "base/observer_list.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/chromeos/cros_settings_names.h"
#include "content/common/notification_observer.h"

namespace base {
template <typename T> struct DefaultLazyInstanceTraits;
}

class Value;

namespace chromeos {

class CrosSettingsProvider;

class CrosSettings : public base::NonThreadSafe {
 public:
  
  static CrosSettings* Get();

  
  static bool IsCrosSettings(const std::string& path);

  
  
  void Set(const std::string& path, Value* in_value);

  
  void FireObservers(const char* path);

  
  
  bool Get(const std::string& path, Value** out_value) const;

  
  
  void SetBoolean(const std::string& path, bool in_value);
  void SetInteger(const std::string& path, int in_value);
  void SetDouble(const std::string& path, double in_value);
  void SetString(const std::string& path, const std::string& in_value);

  
  
  
  bool GetBoolean(const std::string& path, bool* out_value) const;
  bool GetInteger(const std::string& path, int* out_value) const;
  bool GetDouble(const std::string& path, double* out_value) const;
  bool GetString(const std::string& path, std::string* out_value) const;

  
  bool AddSettingsProvider(CrosSettingsProvider* provider);
  bool RemoveSettingsProvider(CrosSettingsProvider* provider);

  
  
  void AddSettingsObserver(const char* path, NotificationObserver* obs);
  void RemoveSettingsObserver(const char* path, NotificationObserver* obs);

 private:
  
  std::vector<CrosSettingsProvider*> providers_;

  
  
  typedef ObserverList<NotificationObserver> NotificationObserverList;
  typedef base::hash_map<std::string, NotificationObserverList*>
      SettingsObserverMap;
  SettingsObserverMap settings_observers_;

  CrosSettings();
  ~CrosSettings();
  CrosSettingsProvider* GetProvider(const std::string& path) const;
  friend struct base::DefaultLazyInstanceTraits<CrosSettings>;

  DISALLOW_COPY_AND_ASSIGN(CrosSettings);
};

}  

#endif  
