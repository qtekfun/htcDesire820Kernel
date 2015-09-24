// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_PREF_VALUE_MAP_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_PREF_VALUE_MAP_H_

#include <map>
#include <set>
#include <string>

#include "base/observer_list.h"
#include "base/prefs/pref_value_map.h"
#include "base/time/time.h"
#include "base/values.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "extensions/browser/extension_prefs_scope.h"

class ExtensionPrefValueMap : public BrowserContextKeyedService {
 public:
  
  class Observer {
   public:
    
    
    
    virtual void OnPrefValueChanged(const std::string& key) = 0;
    
    virtual void OnInitializationCompleted() = 0;
    
    
    virtual void OnExtensionPrefValueMapDestruction() = 0;

   protected:
    virtual ~Observer() {}
  };

  ExtensionPrefValueMap();
  virtual ~ExtensionPrefValueMap();

  
  virtual void Shutdown() OVERRIDE;

  
  
  
  
  
  void SetExtensionPref(const std::string& ext_id,
                        const std::string& key,
                        extensions::ExtensionPrefsScope scope,
                        base::Value* value);

  
  
  void RemoveExtensionPref(const std::string& ext_id,
                           const std::string& key,
                           extensions::ExtensionPrefsScope scope);

  
  
  
  
  
  bool CanExtensionControlPref(const std::string& extension_id,
                               const std::string& pref_key,
                               bool incognito) const;

  
  void ClearAllIncognitoSessionOnlyPreferences();

  
  
  
  
  
  
  
  
  
  bool DoesExtensionControlPref(const std::string& extension_id,
                                const std::string& pref_key,
                                bool* from_incognito) const;

  
  
  
  std::string GetExtensionControllingPref(const std::string& pref_key) const;

  
  void NotifyInitializationCompleted();

  
  void RegisterExtension(const std::string& ext_id,
                         const base::Time& install_time,
                         bool is_enabled);

  
  void UnregisterExtension(const std::string& ext_id);

  
  
  void SetExtensionState(const std::string& ext_id, bool is_enabled);

  
  void AddObserver(Observer* observer);

  void RemoveObserver(Observer* observer);

  const base::Value* GetEffectivePrefValue(const std::string& key,
                                           bool incognito,
                                           bool* from_incognito) const;

 private:
  struct ExtensionEntry;

  typedef std::map<std::string, ExtensionEntry*> ExtensionEntryMap;

  const PrefValueMap* GetExtensionPrefValueMap(
      const std::string& ext_id,
      extensions::ExtensionPrefsScope scope) const;

  PrefValueMap* GetExtensionPrefValueMap(
      const std::string& ext_id,
      extensions::ExtensionPrefsScope scope);

  
  
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

  
  
  
  bool destroyed_;

  ObserverList<Observer, true> observers_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionPrefValueMap);
};

#endif  
