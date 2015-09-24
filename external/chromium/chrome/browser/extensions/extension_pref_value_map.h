// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_PREF_VALUE_MAP_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_PREF_VALUE_MAP_H_
#pragma once

#include <map>
#include <set>
#include <string>

#include "base/time.h"
#include "chrome/browser/prefs/value_map_pref_store.h"

class ExtensionPrefValueMap {
 public:
  
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    
    virtual void OnPrefValueChanged(const std::string& key) = 0;
    
    virtual void OnInitializationCompleted() = 0;
    
    
    virtual void OnExtensionPrefValueMapDestruction() = 0;
  };

  ExtensionPrefValueMap();
  virtual ~ExtensionPrefValueMap();

  
  
  
  
  
  void SetExtensionPref(const std::string& ext_id,
                        const std::string& key,
                        bool incognito,
                        Value* value);

  
  
  void RemoveExtensionPref(const std::string& ext_id,
                           const std::string& key,
                           bool incognito);

  
  
  
  
  
  bool CanExtensionControlPref(const std::string& extension_id,
                               const std::string& pref_key,
                               bool incognito) const;

  
  
  
  
  
  
  bool DoesExtensionControlPref(const std::string& extension_id,
                                const std::string& pref_key,
                                bool incognito) const;

  
  void NotifyInitializationCompleted();

  
  void RegisterExtension(const std::string& ext_id,
                         const base::Time& install_time,
                         bool is_enabled);

  
  void UnregisterExtension(const std::string& ext_id);

  
  
  void SetExtensionState(const std::string& ext_id, bool is_enabled);

  
  void AddObserver(Observer* observer);

  void RemoveObserver(Observer* observer);

  const Value* GetEffectivePrefValue(const std::string& key,
                                     bool incognito,
                                     bool* from_incognito) const;

 private:
  struct ExtensionEntry;

  typedef std::map<std::string, ExtensionEntry*> ExtensionEntryMap;

  const PrefValueMap* GetExtensionPrefValueMap(const std::string& ext_id,
                                               bool incognito) const;

  PrefValueMap* GetExtensionPrefValueMap(const std::string& ext_id,
                                         bool incognito);

  
  
  void GetExtensionControlledKeys(const ExtensionEntry& entry,
                                  std::set<std::string>* out) const;

  
  
  
  
  
  ExtensionEntryMap::const_iterator GetEffectivePrefValueController(
      const std::string& key,
      bool incognito,
      bool* from_incognito) const;

  void NotifyOfDestruction();
  void NotifyPrefValueChanged(const std::string& key);
  void NotifyPrefValueChanged(const std::set<std::string>& keys);

  
  
  
  ExtensionEntryMap entries_;

  ObserverList<Observer, true> observers_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionPrefValueMap);
};

#endif  
